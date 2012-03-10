<?php
/***************************************************************************
 *
 * Copyright (c) 2010 Baidu.com, Inc. All Rights Reserved
 *
 **************************************************************************/

require_once 'oauth/OAuth10Consumer.class.php';

/**
 * Baidu OAuth Client
 * 
 * @package	BaiduOpenAPI
 * @author	zhujt(zhujianting@baidu.com)
 * @version $Revision: 1.10 $
 **/
class BaiduOAuthClient
{
	protected $consumer_key;
	protected $consumer_secret;
	protected $signature_method;

	protected $request_token_url;
	protected $user_authorize_url;
	protected $access_token_url;
	protected $auth_callback_url;

	protected $connect_timeout;
	protected $read_timeout;
	protected $user_agent;

	protected $auth_token_cookie;
	protected $base_domain;
	
	protected $debug_mode;

	protected $errmsg;
	
	protected $config = array();
 
	/**
	 * Construct
	 * 
	 * @param string $consumer_key
	 * @param string $consumer_secret
	 */
	public function __construct($consumer_key, $consumer_secret, $base_domain)
	{
		$this->consumer_key			= $consumer_key;
		$this->consumer_secret		= $consumer_secret;

		$this->request_token_url	= BD_OAUTH_REQUEST_TOKEN_URL;
		$this->user_authorize_url	= BD_OAUTH_AUTHORIZE_URL;
		$this->access_token_url		= BD_OAUTH_ACCESS_TOKEN_URL;
		$this->auth_callback_url	= BD_OAUTH_CALLBACK_URL;

		$this->auth_token_cookie	= 'bdoauth_' . $this->consumer_key;
		$this->base_domain			= $base_domain;
		
		$this->errmsg				= '';
		$this->debug_mode			= DEBUG_MODE;
		
		$this->config = array(
			'connect_timeout'   => CONNECT_TIMEOUT,	
	    	'timeout'           => READ_TIMEOUT,
	        'ssl_verify_peer'   => defined('DEBUG_MODE') && DEBUG_MODE ? false : true,
	    	'user-agent'		=> isset($_SERVER['HTTP_USER_AGENT']) ?
									$_SERVER['HTTP_USER_AGENT'] : 'OAuth Consumer',
		);
	}

	/**
	 * 启动OAuth请求，以获取用户对资源访问的授权
	 * @param string $callback_url	授权后跳转地址
	 * @param string $scope	请求的访问权限
	 */
	public function start_user_authorize($callback_url = '', $scope = '')
	{
		if (!empty($callback_url)) {
			$this->auth_callback_url = $callback_url;
		}
		
		$consumer = new OAuth10Consumer($this->consumer_key, $this->consumer_secret);
		$consumer->setConfig($this->config);
		$token = $consumer->getRequestToken($this->request_token_url, $this->getCallbackUrl());
		if (!$token) {
			//获取request token失败
			$this->failedToGetRequestToken();
			return false;
		} else {
			//保存request token信息，这里只是将其放到cookie中去，开发者也可以选择
			//将其放入到服务端的session中，或存入到数据库中，以保证更好的安全性，但
			//那样做的话，JS端想要调用百度Open API就比较难了，只能通过服务端来调用
			$this->save_token_info($token, false);
			//得到request token后的后续处理，这里是简单地跳转到用户授权页面
			return $this->onGetRequestToken($consumer, $scope);
		}
	}

	/**
	 * 用户授权操作完成后的处理，主要是获取access token
	 */
	public function finish_user_authorize()
	{
		$oauth_token = isset($_GET['oauth_token']) ? $_GET['oauth_token'] : null;
		$oauth_verifier = isset($_GET['oauth_verifier']) ? $_GET['oauth_verifier'] : null;
		if (!$oauth_token || !$oauth_verifier) {
			//用户授权后平台的回调请求中参数不全，以用户不同意授权处理
			$this->userDisagreeAuthorization();
			return false;
		}
		//取出request token对应的一次性密钥
		$request_token = $this->get_token_info();
		if (!$request_token) {
			//request token信息不存在或以过期
			$this->tokenHasExpired();
			return false;
		}
		//用户同意授权，则拿着授权后的request token换取access token
		$consumer = new OAuth10Consumer($this->consumer_key, $this->consumer_secret,
										$request_token['token'], $request_token['secret']);
		$consumer->setConfig($this->config);
		$access_token = $consumer->getAccessToken($this->access_token_url, $oauth_verifier);
		if (!$access_token) {
			//获取access token失败
			$this->failedToGetAccessToken();
			return false;
		} else {
			//保存access token信息，这里只是将其放到cookie中去，开发者也可以选择
			//将其放入到服务端的session中，或存入到数据库中，以保证更好的安全性，但
			//那样做的话，JS端想要调用百度Open API就比较难了，只能通过服务端来调用
			$allow_offline_access = true;	//如果有申请offline_access权限，否则置为false
			$this->save_token_info($access_token, true, $allow_offline_access);
			return true;
		}
	}

	/**
	 * 获取错误信息
	 * @return string
	 */
	public function errmsg()
	{
		return $this->errmsg;
	}

	/**
	 * 获取token信息
	 * @return array array('token' => xxx, 'secret' => xxx, 'authorized' => 1 or 0)
	 */
	public function get_token_info()
	{
		if (empty($_COOKIE[$this->auth_token_cookie])) {
			return false;
		}
		return json_decode($_COOKIE[$this->auth_token_cookie], true);
	}

	/**
	 * 保存request token信息，这里只是将其放到cookie中去，开发者也可以选择
	 * 将其放入到服务端的session中，或存入到数据库中，以保证更好的安全性，但
	 * 那样做的话，JS端想要调用百度Open API就比较难了，只能通过服务端来调用
	 * 
	 * @param string $token
	 * @param bool $authorized
	 * @param bool $allow_offline_access
	 * @todo
	 */
	protected function save_token_info($token, $authorized, $allow_offline_access = false)
	{
		$token['authorized'] = $authorized;
		if ($authorized) {
			if ($allow_offline_access) {
				//允许离线访问，cookie失效时间大些
				$expire = time() + 3650*3600*24;
			} else {
				//不允许离线访问，则最多使用1小时，1小时后需要重新获取access token
				$expire = time() + 3600;
			}
		} else {
			$expire = time() + 1200;	//request token的有效期只有20分钟
		}
		
		$value = json_encode($token);
		$domain = $this->base_domain ? '.' . $this->base_domain : null;
		setcookie($this->auth_token_cookie, $value, $expire, '/', $domain, false, false);
		$_COOKIE[$this->auth_token_cookie] = $value;
	}
	
	/**
	 * 开发者重载或重新实现该函数，用于处理请求request token失败的情况
	 * @todo
	 */
	protected function failedToGetRequestToken()
	{
		if ($this->debug_mode) {
			echo $this->errmsg . '</br>';
		}
	}
	
	/**
	 * 开发者重载或重新实现该函数，用于处理请求用户拒绝为应用授权的情况
	 * @todo
	 */
	protected function userDisagreeAuthorization()
	{
		$this->errmsg = 'authorization not grant by user';
		if ($this->debug_mode) {
			echo $this->errmsg . '</br>';
		}
	}
	
	/**
	 * 开发者重载或重新实现该函数，用于处理请求request token失效或不存在的情况
	 * 一般这种情况是由于用户停留在授权页面时间过长导致的
	 * @todo
	 */
	protected function tokenHasExpired()
	{
		$this->errmsg = 'request token not retrived yet or has been expired';
		if ($this->debug_mode) {
			echo $this->errmsg . '</br>';
		}
	}
	
	/**
	 * 开发者重载或重新实现该函数，用于处理请求access token失败的情况
	 * @todo
	 */
	protected function failedToGetAccessToken()
	{
		if ($this->debug_mode) {
			echo $this->errmsg . '</br>';
		}
	}
	
	/**
	 * 开发者重载或重新实现该函数，做获取到未授权的request token后的处理
	 * @param OAuth10Consumer $consumer
	 * @param string $scope
	 * @todo
	 */
	protected function onGetRequestToken(OAuth10Consumer $consumer, $scope = '')
	{
		//跳转到用户授权页面，开发者也可以将oauth_token, scope, display等参数输出
		//到返回页面中，由JS通过popup或dialog来加载用户授权页，以实现更好的用户体验
		$params = array('scope' => (string)$scope,
						'display' => 'popup');
		$authorize_url = $consumer->getAuthorizeUrl($this->user_authorize_url, $params);
		$this->redirect($authorize_url);
	}
	
	/**
	 * 获取本次OAuth请求的授权后回跳地址，开发者可以重载或重新实现该函数
	 * 
	 * 默认是以应用指定的特定页面作为回跳地址，一般用于通过新开一个dialog或popup窗口
	 * 来处理用户授权的情况。如果采用全页面跳转方式，则一般应该将当前页面的地址作为
	 * 授权回跳地址的一个参数进行传递，或直接以当前页面地址作为授权后回跳地址，以便
	 * 用户授权后能够回到原窗口
	 * 
	 * @return string
	 * @todo
	 */
	protected function getCallbackUrl()
	{
		/*
		if ($this->auth_callback_url != 'oob') {
			if (isset($_SERVER['HTTP_X_FORWARDED_HOST'])) {
				$host = $_SERVER['HTTP_X_FORWARDED_HOST'];
			} else {
				$host = $_SERVER['HTTP_HOST'];
			}
			$current_url = 'http://' . $host . $_SERVER['REQUEST_URI'];
			$info = parse_url($this->auth_callback_url);
			$callback_url = $this->auth_callback_url . (empty($info['query']) ? '?' : '&');
			$callback_url .= '__next=' . urlencode($current_url);
		} else {
			$callback_url = $this->auth_callback_url;
		}
		
		return $callback_url;
		*/
		return $this->auth_callback_url;
	}

	protected function redirect($url, $refresh_full_page = true)
	{
		if ($refresh_full_page) {
			// make sure baidu.com url's load in the full frame so that we don't
			// get a frame within a frame.
			echo '<script type="text/javascript"> top.location.href = "' . $url . '";</script>';
		} else {
			header('Location: ' . $url);
		}
		exit();
	}
}

/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */
?>