<?php
/***************************************************************************
 * 
 * Copyright (c) 2008 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
include_once('BaiduOpenAPIErrorCodes.inc.php');

/**
 * Rest Client for Baidu Open API 
 * 
 * @package	BaiduOpenAPI
 * @author	zhujt(zhujianting@baidu.com)
 * @version $Revision: 1.20 $ 
 **/
class BaiduRestClient
{
	public $api_key;
	public $secret;
	public $using_session_secret = false;
	public $session_key;
	public $user;

	//to save making the friends.get api call, this will get prepopulated on canvas pages
	public $friends_list;
	//to save making the users.isAppUser api call
	public $is_user;

	public $last_call_id;
	public $batch_mode;
	protected $batch_queue;
	
	protected $http_method = 'POST';
	//result format for every api call
	protected $format = 'json';
	//Charactor encoding of current application
	protected $final_encode = 'UTF-8';

	const BATCH_MODE_DEFAULT = 0;
	const BATCH_MODE_SERVER_PARALLEL = 0;
	const BATCH_MODE_SERIAL_ONLY = 2;

	/**
	 * Create the client.
	 * @param string $api_key
	 * @param string $secret
	 * @param string $session_key if you haven't gotten a session key yet, leave
	 *                            this as null and then set it later by just
	 *                            directly accessing the $session_key member
	 *                            variable.
	**/
	public function __construct($api_key, $secret, $session_key = null)
	{
		$this->api_key		= $api_key;
		$this->secret		= $secret;
		$this->session_key	= $session_key;

		$this->batch_mode	= self::BATCH_MODE_DEFAULT;
		$this->last_call_id	= 0;
		
		//$this->server_addr = BaiduOpenApp::get_baidu_url('openapi') . '/restserver';
	}
	
	/**
	 * Classes extend from this class should override it to provide a corrent address
	 * 
	 * @return string
	 */
	public function get_restserver_url()
	{
		return BaiduOpenApp::get_baidu_url(BD_OPENAPI_DOMAIN) . '/1.0/restserver';
	}
	
	/**
	 * Return the openapi version of current baidu product
	 * 
	 * Extended classes may rewrite this interface as needed
	 *
	 * @return string
	**/
	public function openapi_version()
	{
		return '1.0';
	}
	
	/**
	 * Returns the http method to be used
	 * @return string 'POST' or 'GET'
	 */
	public function get_http_method()
	{
		return $this->http_method;
	}
	
	/**
	 * Set http method to use when call open api
	 * @param string $http_method	'POST' OR 'GET'
	 */
	public function set_http_method($http_method = 'POST')
	{
		$this->http_method = ($http_method == 'POST') ? 'POST' : 'GET';
	}
	
	/**
	 * Returns the response format for api call
	 * @return string 'json' or 'xml'
	 */
	public function get_format()
	{
		return $this->format;
	}
	
	/**
	 * Set the response format for api call
	 * @param string $format	'json' or 'xml'
	 */
	public function set_format($format)
	{
		$this->format = $format;
	}

	/**
	 * Returns the charactor encoding for current application
	 * @return string 'UTF-8' OR 'GBK'
	 */
	public function get_final_encode()
	{
		return $this->final_encode;
	}
	
	/**
	 * Set charactor encoding for current application
	 * @param string $final_encode	'UTF-8' OR 'GBK'
	 */
	public function set_final_encode($final_encode = 'GBK')
	{
		$this->final_encode = $final_encode;
	}
	
	/**
	 * Set the default user id for methods that allow the caller
	 * to pass an uid parameter to identify the target user
	 * instead of a session key. This currently applies to
	 * the user preferences methods.
	 * 
	 * @param $uid int the user id
	 */
	public function set_user($uid)
	{
		$this->user = $uid;
	}

	/**
	 * Switch to use the session secret instead of the app secret,
	 * for desktop and unsecured environment
	 */
	public function use_session_secret($session_secret)
	{
		if ($session_secret) {
			$this->secret = $session_secret;
			$this->using_session_secret = true;
		}
	}
	
	/**
	 * Switch to use the app secret instend of the session secret
	 * @param string $app_secret
	 */
	public function use_app_secret($app_secret)
	{
		$this->secret = $app_secret;
		$this->using_session_secret = false;
	}

	/**
	 * Start a batch operation.
	**/
	public function begin_batch()
	{
		if ($this->batch_queue !== null) {
			throw new BaiduRestClientException(BDAPI_EC_BATCH_ALREADY_STARTED);
		}
		$this->batch_queue = array();
	}

	/**
	 * End current batch operation
	**/
	public function end_batch()
	{
		if (!$this->batch_queue) {
			throw new BaiduRestClientException(BDAPI_EC_BATCH_NOT_STARTED);
		}
		$this->execute_server_side_batch();
		$this->batch_queue = null;
	}
  
	/**
	 * Execute batch api call
	 */
	private function execute_server_side_batch()
	{
		$num = count($this->batch_queue);
		$method_feed = array();
		foreach ($this->batch_queue as $batch_item) {
			list($get, $post) = $this->finalize_params($batch_item['m'], $batch_item['p']);
			$method_feed[] = $this->create_url_string(array_merge($post, $get));
		}
		
		$method_feed_json = json_encode($method_feed);

		$serial_only = ($this->batch_mode == self::BATCH_MODE_SERIAL_ONLY);
		$params = array('method_feed' => $method_feed_json,
						'serial_only' => $serial_only,
						'format' => $this->format);
		$result = $this->call_method('baidu.batch.run', $params);
		if (is_array($result) && isset($result['error_code'])) {
			throw new BaiduRestClientException($result['error_code'], $result['error_msg']);
		}
		
		for ($i = 0; $i < $num; $i++) {
			$item = $this->batch_queue[$i];
			$format = isset($item['p']['format']) ? $item['p']['format'] : null;
			if (strcasecmp($format, 'xml') === 0) {
				$item_result = $this->convert_xml_to_result($result[$i], $this->final_encode);
			} else {
				$item_result = $this->convert_json_to_result($result[$i], $this->final_encode);
			}
			
			if (is_array($item_result) && isset($item_result['error_code'])) {
				throw new BaiduRestClientException($item_result['error_code'], $item_result['error_msg']);
			}
			$item['r'] = $item_result;
		}
	}
		
	/**
	 * Expires the session that is currently being used.  If this call is
	 * successful, no further calls to the API (which require a session) can be
	 * made until a valid session is created.
	 *
	 * @return bool  true if session expiration was successful, false otherwise
	 */
	public function auth_expireSession()
	{
		return $this->call_method('baidu.auth.expireSession', array());
	}
	
	/**
	 * Revokes the given extended permissions that the user granted at some
	 * prior time (for instance, offline_access or email).  If no user is
	 * provided, it will be revoked for the user of the current session.
	 * 
	 * @param  string  $perms  The permissions to revoke, dot delimited string
	 * @param  int     $uid   The user for whom to revoke the permissions
	 */
	public function auth_revokeExtendedPermissions($perms, $uid = null)
	{
		$params = array('perms' => $perms, 'uid' => $uid);
		return $this->call_method('baidu.auth.revokeExtendedPermission', $params);
	}
	
	/**
	 * Revokes the user's agreement to the Baidu Terms of Service for your
	 * application.  If you call this method for one of your users, you will no
	 * longer be able to make API requests on their behalf until they again
	 * authorize your application.  Use with care.  Note that if this method is
	 * called without a user parameter, then it will revoke access for the
	 * current session's user.
	 * 
	 * @param int $uid  (Optional) User to revoke
	 * @return bool  true if revocation succeeds, false otherwise
	 */
	public function auth_revokeAuthorization($uid = null)
	{
		return $this->call_method('baidu.auth.revokeAuthorization', array('uid' => $uid));
	}

	/**
	 * Get current loggedin user's user name and uid
	 *
	 * @return array	array('uid' => int, 'uname' => string, 'portrait' => string)
	**/
	public function users_getLoggedInUser()
	{
		return $this->call_method('baidu.users.getLoggedInUser', array());
	}
	
	/**
	 * Returns the requested info fields for the requested set of users.
	 * 
	 * @param array|string $uids    An array of user ids
	 * @param array|string $fields  An array of info field names desired
	 * @return array  An array of user objects
	 */
	public function users_getInfo($uids = null, $fields = null)
	{
		$params = array('uids' => $uids, 'fields' => $fields);
		return $this->call_method('baidu.users.getInfo', $params);
	}
	
	/**
	 * Returns the requested info fields for the requested set of users.
	 * Note that if you want to get the email of the user, you should
	 * specify the 'secureemail' field and should get the authorization
	 * of email scope from the user first.
	 * 
	 * @param int $uid	The requested user's uid
	 * @param array|string $fields  An array of info field names desired
	 * @return array  User info
	 */
	public function users_getInfoEx($uid = null, $fields = null)
	{
		$params = array('uid' => $uid, 'fields' => $fields);
		return $this->call_method('baidu.users.getInfoEx', $params);
	}

	/**
	 * Returns whether or not the user corresponding to the current
	 * session object has the give the app basic authorization.
	 *
	 * @param string $uid
	 * @return bool
	**/
	public function users_isAppUser($uid = null)
	{
		if ($uid === null && isset($this->is_user)) {
			return array('result' => (int)$this->is_user);
		}
		
		return $this->call_method('baidu.users.isAppUser', array('uid' => $uid));
	}
	
	/**
	 * Returns 1 if the user has the specified permission, 0 otherwise.
	 * 
	 * @param string $ext_perm
	 * @param int $uid
	 * @return int  1 or 0
	 */
	public function users_hasAppPermission($ext_perm, $uid = null)
	{
		$params = array('ext_perm' => $ext_perm, 'uid' => $uid);
		return $this->call_method('baidu.users.hasAppPermission', $params);
	}

	/**
	 * Get the specified user's or current logged in user's (if $uid is null) friends info
	 *
	 * @param string $uid	the specified user, if null, current logged in user is requested
	 * @return array(0 => array('uid' => int, 'uname' => string, 'portrait' => string))
	**/
	public function friends_getFriends($uid = null)
	{
		if ($uid === null && isset($this->friends_list)) {
			return $this->friends_list;
		} 

		$params = array('uid' => $uid);
		return $this->call_method('baidu.friends.getFriends', $params);
	}

	/**
	 * Get the relationships between the specified user and his friends
	 *
	 * @param string $uid	the specified user, if null, current logged in user is requested
	 * @return array(0 => array('uid' => int, 'uname' => string, 'portrait' => string, 'are_friends' => 0 or 1))
	**/
	public function friends_getFriendRelations($uid = null)
	{
		$params = array('uid' => $uid);
		return $this->call_method('baidu.friends.getFriendRelations', $params);
	}

	/**
	 * Returns whether or not pairs of users are friends or reverse friends.
	 *
	 * @param array $uids1	array of ids (id_1, id_2,...) of some length X
	 * @param array $uids2	array of ids (id_A, id_B,...) of SAME length X
	 * @return array of uid pairs with bool, true if pair are friends, and true if pair are reverse friends, e.g.
	 *   array( 0 => array('uid1' => id_1, 'uid2' => id_A, 'are_friends' => 1, 'are_friends_reverse' => 1),
	 *          1 => array('uid1' => id_2, 'uid2' => id_B, 'are_friends' => 0, 'are_friends_reverse' => 0),
	 *         ...)
	**/
	public function friends_areFriends($uids1, $uids2)
	{
		$params = array('uids1' => $uids1, 'uids2' => $uids2);
		return $this->call_method('baidu.friends.areFriends', $params);
	}

	/**
	 * Returns the friends of the session user, who are also user of the calling application.
	 *
	 * @return array array(0 => array('uid' => int, 'uname' => string, 'portrait' => string), ...)
	**/
	public function friends_getAppUsers()
	{
		return $this->call_method('baidu.friends.getAppUsers', array());
	}

	/**
	 * Returns the friends of the session user, who are not the user of the calling application.
	 *
	 * @return array array(0 => array('uid' => int, 'uname' => string, 'portrait' => string), ...)
	**/
	public function friends_getNonAppUsers()
	{
		return $this->call_method('baidu.friends.getNonAppUsers', array());
	}

	/**
	 * query order status 
	 *
	 * @param int $order_id order id produced by APP
	 * @return  array( 'result' => 1(succeed) or 0(failedl)); 
	**/
	public function pay_isCompleted($order_id)
	{
		$params = array('order_id' => $order_id);
		return $this->call_method('baidu.pay.isCompleted', $params);
	}

	/**
	 * test api query order status 
	 *
	 * @param int $order_id order id produced by APP
	 * @return  array( 'result' => 1(succeed) or 0(failedl)); 
	**/
	public function payTest_isCompleted($order_id)
	{
		$params = array('order_id' => $order_id);
		return $this->call_method('baidu.payTest.isCompleted', $params);
	}
	
	/*************************************************************************
	 * 
	 * 						UTILITY FUNCTIONS
	 *
	 *************************************************************************/
	/**
	 * Implementation of the Call to the Open API
	 *
	 * @param string $method	name of the Open API, e.g. 'baidu.users.getInfo'
	 * @param array $params		parameters of the calling Open API, you can specify
	 *							the 'format' and 'callback' parameter here to control
	 *							the response data format and content
	 * @param string $http_method	http request mothod, use 'POST' or 'GET'
	 * @return array
	 * @throws BaiduRestClientException
	**/
	public function call_method($method, $params)
	{
		//Check if we are in batch mode
		if ($this->batch_queue === null) {
			$server_addr = $this->get_restserver_url();
			$result = $this->http_request($method, $params, $server_addr);
			if ($result === false) {
				throw new BaiduOpenAPIException(BDAPI_EC_SERVICE);
			}
			if (strcasecmp($this->format, 'xml') === 0) {
				$result = self::convert_xml_to_result($result, $this->final_encode);
			} else {
				$result = self::convert_json_to_result($result, $this->final_encode);
			}
			
			if (is_array($result) && isset($result['error_code'])) {
				throw new BaiduOpenAPIException($result['error_code'], $result['error_msg']);
			}
		} else {
			$result = null;
			$batch_item = array(
				'm' => $method, 'p' => $params, 'r' => & $result
			);
			$this->batch_queue[] = $batch_item;
		}

		return $result;
	}
	
	/**
	 * send http request to rest server and get the response
	 * 
	 * @param string $method		Method name
	 * @param array $params			Params in assoc array to be send
	 * @param string $server_addr	Rest server URL
	 * @return string	http response body
	 */
	protected function http_request($method, $params, $server_addr)
	{
		list($get, $post) = $this->finalize_params($method, $params);
		$post_string = $this->create_url_string($post);
		$get_string = $this->create_url_string($get);
		$url_with_get = $server_addr . '?' . $get_string;
		
		if (function_exists('curl_init')) {
			// Use CURL if installed...
			return $this->curl_http_request($url_with_get, $post_string, $this->http_method, CONNECT_TIMEOUT, READ_TIMEOUT);
		} else {
			// Non-CURL based version...
			return $this->socket_http_request($url_with_get, $post_string, $this->http_method, CONNECT_TIMEOUT, READ_TIMEOUT);
		}
	}
	
	/**
	 * Finalize the params
	 * @param string $method
	 * @param array $params
	 * @return array
	 */
	protected function finalize_params($method, $params)
	{
		$post = $params;
		$get = array();
		if ($this->using_session_secret) {
			$get['ss'] = '1';
		}
		if (isset($post['v'])) {
			$get['v'] = $post['v'];
			unset($post['v']);
		} else {
			$get['v'] = $this->openapi_version();
		}
		if (isset($post['format'])) {
			$get['format'] = $post['format'];
			unset($post['format']);
		} else {
			$get['format'] = $this->format;
		}
		$get['method'] = $method;
		$get['api_key'] = $this->api_key;
		$get['session_key'] = $this->session_key;
		$get['ie'] = $this->final_encode;
		$post['call_id'] = microtime(true);
		if ($post['call_id'] <= $this->last_call_id) {
			$post['call_id'] = $this->last_call_id + 0.001;
		}
		$this->last_call_id = $post['call_id'];
		
		foreach ($post as $key => & $val) {
			if (!$val) {
				continue;
			}
			if (is_array($val)) {
				$val = implode(',', $val);
				//$val = json_encode($val);
			}
		}
		
		$post['bd_sig'] = BaiduUtils::generate_sig(array_merge($get, $post), $this->secret);
		
		return array($get, $post);
	}

	private function create_url_string($params)
	{
		$post_params = array();
		foreach ($params as $key => &$val) {
			$post_params[] = $key . '=' . urlencode($val);
		}
		return implode('&', $post_params);
	}
	
	/**
	 * Do a http request by curl
	 *
	 * @param string $url 			the target uri of the http request
	 * @param string $param_string	query string for GET request or post string for POST request
	 * @param string $http_method	http request mothod, use 'POST' or 'GET'
	 * @param int $connect_timeout	timeout for connect
	 * @param int $read_timeout		timeout for reading http response
	 * @return string	http response body
	**/
	private function curl_http_request($url, $param_string, $http_method, $connect_timeout, $read_timeout)
	{
		$timeout = $connect_timeout + $read_timeout;
		$user_agent = sprintf('Baidu Open API PHP%s Client %s (curl)', phpversion(), $this->openapi_version());

		$ch = curl_init();
		$curl_opts = array( CURLOPT_USERAGENT => $user_agent,
							CURLOPT_HTTP_VERSION => CURL_HTTP_VERSION_1_1,
							CURLOPT_RETURNTRANSFER => true,
							CURLOPT_HEADER => false,
							CURLOPT_FOLLOWLOCATION => false,
							);
		if (defined('CURLOPT_CONNECTTIMEOUT_MS')) {
			$curl_opts[CURLOPT_CONNECTTIMEOUT_MS] = $connect_timeout;
			$curl_opts[CURLOPT_TIMEOUT_MS] = $timeout;
		} else {
			$curl_opts[CURLOPT_CONNECTTIMEOUT] = ceil($connect_timeout / 1000);
			$curl_opts[CURLOPT_TIMEOUT] = ceil($timeout / 1000);
		}
		if ($http_method == 'POST') {
			$curl_opts[CURLOPT_URL] = $url;
			$curl_opts[CURLOPT_POSTFIELDS] = $param_string;
		} else {
			$delimiter = strpos($url, '?') === false ? '?' : '&';
			$curl_opts[CURLOPT_URL] = $url . $delimiter . $param_string;
			$curl_opts[CURLOPT_POST] = false;
		}

		curl_setopt_array($ch, $curl_opts);

		$result = curl_exec($ch);

		curl_close($ch);

		return $result;
	}

	/**
	 * Do a http request by socket
	 *
	 * @param string $url			the target uri of the http request
	 * @param string $param_string	query string for GET request or post string for POST request
	 * @param string $http_method	http request mothod, use 'POST' or 'GET'
	 * @param int $connect_timeout	timeout for connect
	 * @param int $read_timeout		timeout for reading http response
	 * @return string	http response body
	**/
	private function socket_http_request($url, $param_string, $http_method, $connect_timeout, $read_timeout)
	{
		$info = parse_url($url);
		$info['path'] = ($info['path'] == '' ? '/' : $info['path']);
		$info['port'] = ($info['port'] == '' ? 80 : $info['port']);

		$host_ip = gethostbyname($info['host']);
		$user_agent = sprintf('Baidu Open API PHP%s Client %s (non-curl)', phpversion(), $this->openapi_version());

		$in = '';
		if ($http_method == 'POST') {
			$info['request'] =  $info['path'] . (empty($info['query']) ? '' : '?' . $info['query']);
			$in = 'POST ' . $info['request'] . " HTTP/1.0\r\n";
		} else {
			$info['request'] = $info['path'] . '?' . (empty($info['query']) ? $param_string : $info['query'] .
				 '&' . $param_string);
			$in = 'GET ' . $info['request'] . " HTTP/1.0\r\n";
		}
		if ($info['port'] == 80) {
			$in .= 'Host: ' . $info['host'] . "\r\n";
		} else {
			$in .= 'Host: ' . $info['host'] . ':' . $info['port'] . "\r\n";
		}
		$in .= "Accept: */*\r\n";
		$in .= "User-Agent: $user_agent\r\n";
		$in .= "Connection: Close\r\n\r\n";
		if ($http_method == 'POST') {
			$in .= "Content-type: application/x-www-form-urlencoded\r\n";
			$in .= 'Content-Length: ' . strlen($param_string) . "\r\n";
			$in .= "$param_string\r\n\r\n";
		}
		
		$fsock = fsockopen($host_ip, $info['port'], $errno, $errstr, (float)$connect_timeout / 1000.0);
		if (false === $fsock) {
			return false;
		}
		
		stream_set_timeout($fsock, 0, $read_timeout);
		if (!fwrite($fsock, $in, strlen($in))) {
			fclose($fsock);
			return false;
		}
		
		/* process response */
		$out = '';
		while (!feof($fsock)) {
			$buff = fgets($fsock, 4096);
			$out .= $buff;
		}
		fclose($fsock);
		
		$pos = strpos($out, "\r\n\r\n");
		$head = substr($out, 0, $pos);
		$status = substr($head, 0, strpos($head, "\r\n"));
		$body = substr($out, $pos + 4, strlen($out) - ($pos + 4));
		if (preg_match('/^HTTP\/\d\.\d\s([\d]+)\s.*$/', $status, $matches)) {
			if (intval($matches[1]) / 100 == 2) {
				return $body;
			}
		}
		return false;
	}

	/**
	 * Convert json string to associate array and convert the values to the specified encoding
	 *
	 * @param string $json			json string
	 * @param string $final_encode	the encoding of the application, use 'GBK' or 'UTF-8'
	 * @return array
	**/
	public static function convert_json_to_result($json, $final_encode)
	{
		$result = json_decode($json, true);
		if (strcasecmp($final_encode, 'UTF-8') !== 0) {
			$result = BaiduUtils::iconv_recursive($result, 'UTF-8', $final_encode);
		}

		return $result;
	}

	/**
	 * Convert xml to associate array and convert the values to the specified encoding
	 *
	 * @param string $xml			xml string
	 * @param string $final_encode	the encoding of the application, use 'GBK' or 'UTF-8'
	 * @return array
	**/
	public static function convert_xml_to_result($xml, $final_encode)
	{
		$sxml = simplexml_load_string($xml);
		$result = self::convert_simplexml_to_array($sxml, $final_encode);
		return $result;
	}

	/**
	 * Convert xml to associate array and convert the values to the specified encoding
	 *
	 * @param string $sxml			simplexml object
	 * @param string $final_encode	the encoding of the application, use 'GBK' or 'UTF-8'
	 * @return array
	**/
	public static function convert_simplexml_to_array($sxml, $final_encode)
	{
		$arr = array();
		if ($sxml) {
			foreach ($sxml as $k => $v) {
				if ($sxml['list']) {
					$arr[] = self::convert_simplexml_to_array($v, $final_encode);
				} else {
					$arr[$k] = self::convert_simplexml_to_array($v, $final_encode);
				}
			}
		}
		
		if (count($arr) > 0) {
			return $arr;
		} else {
			if (strcasecmp($final_encode, 'UTF-8') !== 0) {
				return iconv('UTF-8', $final_encode, $sxml);
			} else {
				return (string)$sxml;
			}
		}
	}
}

class BaiduRestClientException extends BaiduOpenAPIException
{
	    
}



// Supporting methods and values------


/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */
?>
