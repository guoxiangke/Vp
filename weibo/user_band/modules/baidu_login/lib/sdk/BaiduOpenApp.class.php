<?php
/***************************************************************************
 *
 * Copyright (c) 2008 Baidu.com, Inc. All Rights Reserved
 *
 **************************************************************************/

/**
 * Baidu Open App interface
 * 
 * Every inside web page should create this instance
 * 
 * @package	BaiduOpenAPI
 * @author	zhujt(zhujianting@baidu.com)
 * @version $Revision: 1.10 $
 **/
class BaiduOpenApp
{
	public $api_client;
	
	public $app_id;
	public $api_key;
	public $secret;
	public $use_session_secret;
	public $session_expires;

	public $bd_params;
	
	public $user;		//user id
	public $uname;		//user name
	/**
	 * substr of user portrait image url, developer can construct
	 * the final image url as following:
	 * small image: http://himg.bdimg.com/sys/portraitn/item/{$portrait}.jpg
	 * large image: http://himg.bdimg.com/sys/portrait/item/{$portrait}.jpg
	 */
	public $portrait;
	
	public $ext_perms = array();
	public $base_domain;
	public $canvas_pos = 'platform';
	public $keyword = '';

	public function __construct($app_id, $api_key, $secret, $base_domain, $use_session_secret = false)
	{
		$this->app_id = $app_id;
		$this->api_key = $api_key;
		$this->secret = $secret;
		$this->base_domain = $base_domain;
		$this->use_session_secret = $use_session_secret;
		
		$this->api_client = new BaiduRestClient($api_key, $secret, null);

		$this->validate_bd_params();
		
		//如果第三方应用采用GBK/GB2312编码，则可以将以下三行注释打开
		//以帮助应用解决编码转换问题，当然应用也可以自己在处理过程中决定对哪些参数进行转码以做到精确控制
		/*
		$this->api_client->set_final_encode();
		$_POST = BaiduUtils::iconv_recursive($_POST, 'UTF-8', 'GBK');
		$_GET = BaiduUtils::iconv_recursive($_GET, 'UTF-8', 'GBK');
		*/
	}

	public function validate_bd_params()
	{
		$this->bd_params = $this->get_valid_bd_params($_POST, POST_TIMEOUT);
		if (!$this->bd_params) {
			$this->bd_params = $this->get_valid_bd_params($_GET, GET_TIMEOUT);
		}
		// Okay, something came in via POST or GET
		if ($this->bd_params) {
			$user 				= isset($this->bd_params['user']) ?
									$this->bd_params['user'] : null;
			$uname				= isset($this->bd_params['uname']) ?
									$this->bd_params['uname'] : null;
			$portrait			= isset($this->bd_params['portrait']) ?
									$this->bd_params['portrait'] : null;
			$session_key 		= isset($this->bd_params['session_key']) ?
									$this->bd_params['session_key'] : null;
			$expires 			= isset($this->bd_params['expires']) ?
									$this->bd_params['expires'] : null;

			$this->ext_perms 	= isset($this->bd_params['ext_perms']) ?
									explode(',', $this->bd_params['ext_perms'])
									: array();
			$this->canvas_pos	= isset($this->bd_params['canvas_pos']) ?
									$this->bd_params['canvas_pos'] : 'platform';
			$this->keyword		= isset($this->bd_params['keyword']) ?
									$this->bd_params['keyword'] : '';
			
			if (!empty($this->bd_params['base_domain'])) {
				$this->base_domain = $this->bd_params['base_domain'];
			}
			
			$this->set_user($user, $uname, $portrait, $session_key, $expires, null, true);
		} elseif ($session = $this->get_session_from_cookie()) {
			// if no baidu parameters were found in the GET or POST variables,
			// then fall back to cookies, which may have cached user information,
			// Cookies are also used to receive session data via the Javascript API
			if (!empty($session['base_domain'])) {
				$this->base_domain = $session['base_domain'];
			}
			$this->set_user($session['user'],
							$session['uname'],
							$session['portrait'],
							$session['session_key'],
							isset($session['expires']) ? $session['expires'] : null,
							isset($session['secret']) ? $session['secret'] : null,
							false);
		} elseif (isset($_GET['oauth_token']) && $session = $this->do_get_session()) {
			// finally, if we received no parameters, but the 'oauth_token' GET var
			// is present, then we are in the middle of oauth handshake,
			// so go ahead and create the session
			if ($this->use_session_secret && !empty($session['secret'])) {
				$session_secret = $session['secret'];
			}
			if (!empty($session['base_domain'])) {
				$this->base_domain = $session['base_domain'];
			}
			$this->set_user($session['user'],
							$session['uname'],
							$session['portrait'],
							$session['session_key'],
							isset($session['expires']) ? $session['expires'] : null,
							isset($session_secret) ? $session_secret : null,
							true);
		}
	}

	/**
	 * Get the signed parameters that were sent from Baidu. Validates the set
	 * of parameters against the included signature.
	 * 
	 * Since Baidu sends data to your callback URL via unsecured means, the
	 * signature is the only way to make sure that the data actually came from
	 * Baidu. So if an app receives a request at the callback URL, it should
	 * always verify the signature that comes with against your own secret key.
	 * Otherwise, it's possible for someone to spoof a request by
	 * pretending to be someone else, i.e.:
	 *      www.your-callback-url.com/?bd_sig_user=10101
	 * 
	 * This is done automatically by verify_fb_params.
	 * @param assoc $params		a full array of external parameters.
	 *                          presumed $_GET, $_POST
	 * @param int $timeout		number of seconds that the args are good for.
	 * @param string $namespace	prefix string for the set of parameters we want
	 *                          to verify. i.e., bd_sig
	 * @return assoc	the subset of parameters containing the given prefix,
	 * 					and also matching the signature associated with them.
	 * 				OR  an empty array if the params do not validate
	 */
	public function get_valid_bd_params($params, $timeout = null, $namespace = 'bd_sig')
	{
		$prefix = $namespace . '_';
		$prefix_len = strlen($prefix);
		$bd_params = array();
		foreach ($params as $name => $val) {
			// pull out only those parameters that match the prefix
			// note that the signature itself ($params[$namespace]) is not in the list
			if (strpos($name, $prefix) === 0) {
				$bd_params[substr($name, $prefix_len)] = BaiduUtils::no_magic_quotes($val);
			}
		}
		/*
		// validate that the request hasn't expired. this is most likely
		// for params that come from $_COOKIE
		if ($timeout && (!isset($bd_params['time']) || time() - $bd_params['time'] > $timeout)) {
			return array();
		}*/
		// validate that the params match the signature
		if (!isset($params[$namespace]) || !$this->verify_signature($bd_params, $params[$namespace])) {
			return array();
		}

		return $bd_params;
	}
	
	/**
	 * Validates that a given set of parameters match their signature.
	 * Parameters all match a given input prefix, such as "bd_sig".
	 * 
	 * @param assoc $bd_params	an array of all Baidu-sent parameters,
	 * 							not including the signature itself
	 * @param string $expected_sig	the expected result to check against
	 * @return bool
	 */
	public function verify_signature($bd_params, $expected_sig)
	{
		return BaiduUtils::generate_sig($bd_params, $this->secret) == $expected_sig;
	}

	public function set_user($user, $uname, $portrait, $session_key,
							 $expires = null, $session_secret = null,
							 $write_cookie = true)
	{		
		if (!$this->in_bd_canvas() && $write_cookie) {
			$this->set_cookies($user, $uname, $portrait, $session_key,
				$expires, $session_secret);
		}

		$this->user = $user;
		$this->uname = $uname;
		$this->portrait = $portrait;
		$this->session_key = $session_key;
		$this->session_expires = $expires;
		
		$this->api_client->set_user($user);
    	$this->api_client->session_key = $session_key;
    	if ($this->use_session_secret && $session_secret) {
    		$this->api_client->use_session_secret($session_secret);
    	}
	}
	
	public function set_cookies($user, $uname, $portrait, $session_key,
								$expires = null, $session_secret = null)
	{
		$this->set_p3p_header();
		
		$key = $this->get_session_cookie_name();
		$domain = $this->base_domain ? '.' . $this->base_domain : null;
		if (!$user || !$session_key) {
			setcookie($key, 'delete', time() - 36000, '/', $domain);
			$_COOKIE[$key] = '';
			return;
		}
		
		$session = array();
		$session['user'] = $user;
		$session['uname'] = $uname;
		$session['portrait'] = $portrait;
		$session['session_key'] = $session_key;
		if ($expires != null) {
			$session['expires'] = $expires;
		}
		if ($session_secret != null) {
			$session['secret'] = $session_secret;
		}
		if ($this->base_domain != null) {
			$session['base_domain'] = $this->base_domain;
		}
		$session['bd_sig'] = BaiduUtils::generate_sig($session, $this->secret);
		
		$value = '"' . http_build_query($session, '', '&') . '"';
		setcookie($key, $value, (int)$expires, '/', $domain);
		$_COOKIE[$key] = $value;
	}
	
	public function get_session_from_cookie($namespace = 'bd_sig')
	{
		$key = $this->get_session_cookie_name();
		
		$session = array();
		if (isset($_COOKIE[$key])) {
			$value = BaiduUtils::no_magic_quotes($_COOKIE[$key]);
			parse_str(trim($value, '"'), $session);
			$sig = $session[$namespace];
			unset($session[$namespace]);
			// validate that the params match the signature
			if (!$this->verify_signature($session, $sig) ||
				!$session['user'] ||
				!$session['session_key']) {
				$session = array();
			}
		}
		return $session;
	}
	
	public function get_session_cookie_name()
	{
		return 'bds_' . $this->api_key;
	}
	
	/**
	 * Change oauth request token for access token to finish the oauth request
	 * handshake, and return the session if everything is ok.
	 * 
	 * @return array|false
	 */
	public function do_get_session()
	{
		$oauth_token = $_GET['oauth_token'];
		$oauth_verifier = $_GET['oauth_verifier'];
		if (!$oauth_token || !$oauth_verifier) {
			//oauth params not completed in authorization response,
			//treat it as an unauthorized token
			$this->onDisagreeAuthorization();
			return false;
		}

		$request_token = $this->get_request_token($oauth_token);

		if (!$request_token) {
			$this->onRequestTokenExpired();
			return false;
		}

		//now change the request token for access token
		$oauth = new OAuth10Consumer($this->api_key, $this->secret,
									 $request_token['token'],
									 $request_token['secret']);
		$access_token = $oauth->getAccessToken(BD_OAUTH_ACCESS_TOKEN_URL, $oauth_verifier);
		if (!$access_token) {
			$this->onGetAccessTokenFailed($oauth);
			return false;
		} else {
			//get access token success, now we could delete the request token and
			//should get user info to complete the session data
			$this->delete_request_token($request_token['token']);
			
			$user = $access_token['uid'];
			$uname = $access_token['uname'];
			$portrait = $access_token['portrait'];
			$session_key = $access_token['token'];
			$session_secret = $access_token['secret'];
			if (isset($access_token['expires']) && $access_token['expires'] > 0) {
				$expires = time() + $access_token['expires'];
			} else {
				$expires = time() + 315360000;	//ten years
			}
			//$this->api_client->session_key = $session_key;
			
			return array('user' => $user,
						 'uname' => $uname,
						 'portrait' => $portrait,
						 'session_key' => $session_key,
						 'secret' => $session_secret,
						 'expires' => $expires);
		}
	}
	
	/**
	 * Invalidate the session currently being used, and clear any state associated
	 * with it. Note that the user will still remain logged into Baidu.
	 */
	public function expire_session()
	{
		try {
			if ($this->api_client->auth_expireSession()) {
				$this->clear_cookie_state();
				return true;
			} else {
				return false;
			}
		} catch (Exception $e) {
			$this->clear_cookie_state();
		}
	}
	
	/**
	 * Clears any persistent state stored about the user, including
	 * cookies and information related to the current session in the client.
	 */
	public function clear_cookie_state()
	{	
		if (!$this->in_bd_canvas()) {
			$this->set_cookies(null, null, null, null);
		}
		
		// now, clear the rest of the stored state
		$this->user = 0;
		$this->api_client->set_user(0);
		$this->api_client->session_key = null;
		$this->api_client->use_app_secret($this->secret);
	}
	
	/**
	 * Logs the user out of all temporary application sessions as well as their
	 * Baidu session.  Note this will only work if the user has a valid current
	 * session with the application.
	 * 
	 * @param string  $next  URL to redirect to upon logging out
	 */
	public function logout($next = null)
	{
		$logout_url = $this->get_logout_url($next);
		
		//Developers should process the CSRF attack for his logout page,
		//or directly use baidu's logout url to process the logout request,
		//but in that case, developer should first check the bd_sig param in
		//the next url for logout request, and then clear the cookie,
		//to prevent CSRF attack
		$this->clear_cookie_state();
		$this->redirect($logout_url);
	}
	
	/**
	 * Check whether logout baidu success.
	 * 
	 * This function should be called in the next url page for logout
	 * to prevent CSRF attack.
	 * 
	 * @return bool
	 */
	public function is_logout_success()
	{
		return md5($this->api_client->session_key . $this->secret) == $_GET['bd_sig'];
	}
	
	/**
	 * Whether current page is inside BDML or BD Iframe page
	 * @return bool
	 */
	public function in_frame()
	{
		return isset($this->bd_params['in_canvas'])
			|| isset($this->bd_params['in_iframe']);
	}
	
	/**
	 * Whether current page is inside the BDML page
	 * @return bool
	 */
	public function in_bd_canvas()
	{
		return isset($this->bd_params['in_canvas']);
	}
	
	/**
	 * Check whether user has grant authorization to the app, and start an
	 * oauth request handshake if necessary.
	 * 
	 * This interface is suitable for all apps, but the caller page should be
	 * in a new window, e.x. use window.open() in JS or target="_blank" in HTML
	 * 
	 * @param string $req_perms	Required permissions, dot delimited string
	 * @param string $next	The url to go to if user grant the permissions
	 */
	public function require_authorization($req_perms = '', $next = null)
	{
		//ensure to clear session cookie
		$this->clear_cookie_state();
		
		if (!$next) {
			$next = $this->current_url(false);
		}
		$oauth = new OAuth10Consumer($this->api_key, $this->secret);
		$request_token = $oauth->getRequestToken(BD_OAUTH_REQUEST_TOKEN_URL, $next);
		if (!$request_token) {
			//get oauth request token failed!
			$this->onGetRequestTokenFailed($oauth);
			error_log($oauth->errmsg());
			return false;
		}
		
		//save the request token info, here we just set it into cookie,
		//developers could save it into cache(e.g. memcached) or DB(e.g. MySQL) themselves
		$this->save_request_token($request_token);
		
		//redirect to authorization page
		$params = array('scope' => $req_perms, 'display' => 'popup');
		$authorize_url = $oauth->getAuthorizeUrl(BD_OAUTH_AUTHORIZE_URL, $params);
		$this->redirect($authorize_url, true);
	}
	
	/**
	 * obtain access token
	 * 
	 * @param  string  $x_auth_username	username
	 * @param  string  $x_auth_password password
	 * @return array|false
	 */
	public function require_xauthorization($x_auth_username = '',
	                                       $x_auth_password = '')
	{
		//ensure to clear session cookie
		$this->clear_cookie_state();
		
		$additional = array('x_auth_username' => $x_auth_username,
		                    'x_auth_password' => $x_auth_password,
		                    'x_auth_mode' => 'client_auth',);
		
		$oauth = new OAuth10Consumer($this->api_key, $this->secret);
		$access_token = $oauth->getAccessToken(BD_OAUTH_ACCESS_TOKEN_URL, 
		                                       '', 
		                                       $additional,
		                                       'POST',
		                                       OAuth10Request::AUTH_HEADER,
		                                       true);
		if (!$access_token) {
			//get oauth access token failed!
			$this->onGetAccessTokenFailed($oauth);
			return false;
		}
		return $access_token;
	}
	
	/**
	 * Check whether user has loged in and authorized the app, and redirect
	 * the user to the authorize page if necessary.
	 * 
	 * This interface is only for apps developed by BDML or BD IFrame mode
	 * 
	 * @param string $req_perms	required permissions, dot delimited string
	 * @param string $next	The url to go to if user loged in
	 * @param string $cancel The url to go to if user do not want to loged in
	 * @return int	Uid if user has already loged in
	 */
	public function require_login($req_perms = '', $next = null, $cancel = null)
	{
		$user = $this->get_loggedin_user();
		$has_permissions = true;
		
		if ($req_perms) {
			$permissions = array_map('trim', explode(',', $req_perms));
			foreach ($permissions as $permission) {
				if (!in_array($permission, $this->ext_perms)) {
					$has_permissions = false;
					break;
				}
			}
		}
		
		if ($user && $has_permissions) {
			return $user;
		}
		
		if (!$next) {
			$next = $this->current_url();
		}
		if (!$cancel) {
			$cancel = $this->current_url();
		}
		
		$page = $this->get_auth_url($req_perms, $next, $cancel);
		$this->redirect($page);
	}
		
	public function get_loggedin_user()
	{
		return $this->user;
	}
	
	/**
	 * Get current page url
	 * @param bool $in_canvas	Whether current page is in inside web app
	 * @return string
	 */
	public function current_url($in_canvas = true)
	{
		$protocol = 'http';
		if (isset($_SERVER['HTTP_X_FORWARDED_PROTO'])) {
			$protocol = strtolower($_SERVER['HTTP_X_FORWARDED_PROTO']);
		} elseif (isset($_SERVER['HTTPS']) && $_SERVER['HTTPS'] == 'on') {
			$protocol = 'https';
		}
		$protocol .= '://';

		if ($in_canvas) {
			$req_uri = $_SERVER['REQUEST_URI'];
			if (stripos($req_uri, APP_CALLBACK_URL) !== 0) {
				return $protocol . $_SERVER['HTTP_HOST'] . $_SERVER['REQUEST_URI'];
			} else {
				$url_suffix = substr($req_uri, strlen(APP_CALLBACK_URL));
				if (strpos($url_suffix, '/') !== 0) {
					$url_suffix = '/' . $url_suffix;
				}
				$pos = strpos($url_suffix, '?');
				if ($pos) {
					$url_suffix = substr($url_suffix, 0, $pos);
				}
				$params = $_GET;
				foreach ($params as $key => $value) {
					if (stripos($key, 'bd_sig') === 0) {
						unset($params[$key]);
					}
				}
				if ($params) {
					$url_suffix .= '?' . http_build_query($params, '', '&');
				}
				return self::get_baidu_url(BD_APPS_DOMAIN) . '/' .
						$this->app_id . '' . $url_suffix;
			}
		} else {
			return $protocol . $_SERVER['HTTP_HOST'] . $_SERVER['REQUEST_URI'];
		}
	}
	
	/**
	 * Redirect user browser to a specified url
	 * @param string $url
	 */
	public function redirect($url, $refresh_top_location = false)
	{
		//for BDML
		if ($this->in_bd_canvas()) {
			echo '<bd:redirect url="' . $url . '"/>';
		}
		//for BD IFrame
		elseif (!$refresh_top_location && $this->in_frame() &&
				preg_match('/^https?:\/\/([^\/]*\.)?baidu\.com(:\d+)?/i', $url)) {
			$xd_url = self::get_baidu_url(BD_APPS_DOMAIN)
				. '/static/appstore/html/bdjs_callback.html#method=location&url='
				. urlencode($url);
			echo '<iframe src="' . $xd_url . '" height="0" style="border:0;"></iframe>';
		}
		//for others, e.x. Iframe or Connect website
		elseif ($refresh_top_location) {
			echo '<script type="text/javascript"> top.location.href = "' . $url . '";</script>';
		} else {
			header('Location: ' . $url);
		}
		
		exit();
	}
	
	public static function get_baidu_url($subdomain = 'www')
	{
		if (defined('IN_TEST_ENV') && defined('BD_TEST_ENV_PORT')) {
			return 'http://' . $subdomain . '.baidu.com:' . BD_TEST_ENV_PORT;
		} else {
			return 'http://' . $subdomain . '.baidu.com';
		}
	}
	
	public function get_apps_url($action, $params)
	{
		$page = self::get_baidu_url(BD_APPS_DOMAIN) . '/' . $action;
		foreach ($params as $key => $val) {
			if (!$val) {
				unset($params[$key]);
			}
		}
		$query_string = http_build_query($params, '', '&');
		return $page . ($query_string ? '?' . $query_string : '');
	}
	
	public function get_openapi_url($action, $params)
	{
		$page = self::get_baidu_url(BD_OPENAPI_DOMAIN) . '/' . $action;
		foreach ($params as $key => $val) {
			if (!$val) {
				unset($params[$key]);
			}
		}
		$query_string = http_build_query($params, '', '&');
		return $page . ($query_string ? '?' . $query_string : '');
	}
	
	public function get_app_homepage_url()
	{
		$params = array('appid' => $this->app_id);
		return $this->get_apps_url('app/introduce', $params);
	}
	
	/**
	 * This is only used for apps developed in BDML or BD IFrame mode
	 */
	public function get_auth_url($ext_perms, $next = null, $cancel = null)
	{
		$params = array('api_key'		=> $this->api_key,
						'ext_perms'		=> $ext_perms,
						'next'			=> $next ? $next : $this->current_url(),
						'cancel'		=> $cancel ? $cancel : $this->current_url(),
						'canvas_pos'	=> $this->canvas_pos,
						'display'		=> 'page');
		return $this->get_apps_url('app/authorize', $params);
	}
	
	/**
	 * This is used for apps developed in iframe or connect mode
	 * @param string $next
	 * @param string $cancel
	 * @param string $req_perms
	 * @param string $display
	 * @return string
	 */
	public function get_login_url($next, $cancel, $req_perms = '', $display = 'popup')
	{	
		$params = array('bdconnect' => 1,
						'api_key'	=> $this->api_key,
						'req_perms'	=> $req_perms,
						'next'		=> $next ? $next : $this->current_url(false),
						'cancel'	=> $cancel ? $cancel : $this->current_url(false),
						'display'	=> $display,
						'v'			=> 1.0,
						'asyn'		=> 1,
						'return_session' => 1);
		return $this->get_openapi_url('connect/login', $params);
	}
	
	public function get_logout_url($next = null)
	{
		$params = array('api_key'		=> $this->api_key,
						'session_key'	=> $this->api_client->session_key,
						'next'			=> $next ? $next : $this->current_url(false),
						'asyn'			=> 1);
		return $this->get_openapi_url('connect/logout', $params);
	}
	
	public function get_bdconnect_login_url()
	{
		$next = $cancel = $this->current_url(false);
		return $this->get_login_url($next, $cancel, '', 'popup');
	}
	
	public function get_bdconnect_logout_url()
	{
		return $this->get_logout_url($this->current_url(false));
	}
	
	public function get_prompt_permissions_url($req_perms, $next = null, $cancel = null)
	{
		$params = array('bdconnect' => 1,
						'api_key'	=> $this->api_key,
						'req_perms'	=> $req_perms,
						'next'		=> $next ? $next : $this->current_url(false),
						'cancel'	=> $cancel ? $cancel : $this->current_url(false),
						'display'	=> 'popup',
						'v'			=> 1.0,
						'asyn'		=> 1);
		return $this->get_apps_url('connect/prompt_permissions', $params);
	}
	
	/**
	 * Save the oauth request token.
	 * 
	 * Here we just save it into cookie, developers could rewrite
	 * this interface to save it into cache or db as you want.
	 * 
	 * @param array $request_token
	 */
	protected function save_request_token($request_token)
	{
		return $this->set_oauth_cookie($request_token);
	}
	
	/**
	 * Delete request token from where you saved it
	 * 
	 * Default implementation is delete the oauth cookie as we saved it
	 * in cookie. Developers should rewrite this if you save request token
	 * in cache or db.
	 * 
	 * @param string $oauth_token
	 */
	protected function delete_request_token($oauth_token)
	{
		$this->set_oauth_cookie(null);
	}
	
	/**
	 * Get request token which was saved when get temporary
	 * credential success in previouse oauth request.
	 * 
	 * Default implementation is get it from cookie, developer should
	 * rewrite this to get it from where you saved it in.
	 * 
	 * @param string $oauth_token
	 * @return array
	 */
	protected function get_request_token($oauth_token)
	{
		return $this->get_request_token_from_cookie();
	}
	
	/**
	 * Callback function, called when the client ask for request token failed
	 * 
	 * Default implementation is send an error message to error log.
	 * Developers could rewrite this to add your logic, e.x. popup a dialog to
	 * tell the user what's wrong with this
	 * 
	 * @param OAuth10Consumer $oauth	OAuth10Consumer instance
	 */
	protected function onGetRequestTokenFailed($oauth)
	{
		error_log($oauth->errmsg());
	}
	
	/**
	 * Callback function, called when the user diagree the oauth authorization
	 * 
	 * Default implementation is send an error message to error log.
	 * Developers could rewrite this to add your logic, e.x. popup a dialog to
	 * tell the user what's wrong with this
	 */
	protected function onDisagreeAuthorization()
	{
		$this->clear_oauth_cookie();
		error_log('authorization is not grant by user');
	}
	
	/**
	 * Callback function, called when the request token has expired
	 * 
	 * Default implementation is send an error message to error log.
	 * Developers could rewrite this to add your logic, e.x. popup a dialog to
	 * tell the user what's wrong with this, or restart the oauth request
	 */
	protected function onRequestTokenExpired()
	{
		$this->clear_oauth_cookie();
		error_log('the request token you saved has expired');
	}
	
	/**
	 * Callback function, called when the client change request token
	 * for access token failed.
	 * 
	 * Default implementation is send an error message to error log.
	 * Developers could rewrite this to add your logic, e.x. popup a dialog to
	 * tell the user what's wrong with this
	 * 
	 * @param OAuth10Consumer $oauth	OAuth10Consumer instance
	 */
	protected function onGetAccessTokenFailed($oauth)
	{
		$this->clear_oauth_cookie();
		error_log($oauth->errmsg());
	}
	
	/**
	 * Get oauth request token info from cookie
	 * @return array
	 */
	protected function get_request_token_from_cookie()
	{
		$key = $this->get_oauth_cookie_name();
		
		$request_token = array();
		if (isset($_COOKIE[$key])) {
			$value = BaiduUtils::no_magic_quotes($_COOKIE[$key]);
			parse_str(trim($value, '"'), $request_token);
			if (empty($request_token['token']) || empty($request_token['secret'])) {
				return array();
			}
		}
		return $request_token;
	}
	
	/**
	 * save request token to cookie
	 * @param array $request_token	Oauth request token info
	 */
	protected function set_oauth_cookie($request_token)
	{
		$this->set_p3p_header();
		
		$key = $this->get_oauth_cookie_name();
		$domain = $this->base_domain ? '.' . $this->base_domain : null;
		if (empty($request_token)) {
			setcookie($key, 'delete', time() - 36000, '/', $domain);
			$_COOKIE[$key] = '';
			return;
		}
		
		//request token cookie is valid for 20min in default
		$expires = 1200;
		if (isset($request_token['expires'])) {
			$expires = $request_token['expires'];
		}
		$expires += time();
		$values =  '';
		foreach($request_token as $k=>$val){
			$values .= $k.'='.$val.'&';
		}
		$values ='"'. substr($values, 0, -1).'"';
		setcookie($key, $values, (int)$expires, '/', $domain);
		$_COOKIE[$key] = $values;
	}
	
	/**
	 * Delete request token info from cookie
	 */
	protected function clear_oauth_cookie()
	{
		$this->set_oauth_cookie(null);
	}
	
	protected function get_oauth_cookie_name()
	{
		return 'bdo_' . $this->api_key;
	}
	
	private function set_p3p_header()
	{
		//通过P3P协议表明不同网站间COOKIE的信任关系
		header('P3P: CP="NOI ADM DEV PSAi COM NAV OUR OTR STP IND DEM"');
	}
}

/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */
?>
