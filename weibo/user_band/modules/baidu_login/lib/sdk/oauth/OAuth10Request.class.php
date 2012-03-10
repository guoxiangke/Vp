<?php
/***************************************************************************
 *
 * Copyright (c)2009 Baidu.com, Inc. All Rights Reserved
 *
 **************************************************************************/

/**
 * OAuth 1.0 Request
 *
 * @package	OAuth
 * @author	zhujt(zhujianting@baidu.com)
 * @version	$Revision: 1.0 $
 **/
class OAuth10Request
{
	/**
     * OAuth Parameters
     *
     * @var string $oauthParams OAuth parameters
     */
    protected static $oauthParams = array(
        'oauth_consumer_key',
        'oauth_token',
        'oauth_token_secret',
        'oauth_signature_method',
        'oauth_signature',
        'oauth_timestamp',
        'oauth_nonce',
        'oauth_verifier',
        'oauth_version',
        'oauth_callback',
    );
    
    protected static $httpConfig = array(
    	'connect_timeout'   => 1000,	
    	'timeout'           => 3000,
        'buffer_size'       => 16384,
        'ssl_verify_peer'   => false,
    	'user-agent'		=> 'OAuth Consumer',
    );
    
	/**
     *  Auth type constants
     */
    const AUTH_HEADER = 1;
    const AUTH_POST   = 2;
    const AUTH_GET    = 3;
    
    /**
     * Http request config
     * @var array
     */
    protected $config = array();
    
    /**
     * Authorization type
     * @var int
     */
    protected $authType = self::AUTH_HEADER;
    
    /**
     * Http Method
     * @var string
     */
    protected $method = 'POST';
    
    /**
     * Parameters
     *
     * @var array
     */
    protected $parameters = array();
    
    /**
     * Array of consumer and token secret
     * @var array
     */
    protected $secrets = array('', '');
    
    /**
     * Current Request Url
     * @var string
     */
    protected $url = '';
    
    /**
     * Realm for oauth authorization header
     * @var string
     */
    private $realm = '';
    
    /**
     * Base string uri for signature
     * @var string
     */
    private $baseStringUri = '';
    
    /**
     * Port for request url
     * @var int
     */
    private $port;
    
	/**
     * Construct
     *
     * Sets url, secrets, and http method
     *
     * @param string $url     Url to be requested
     * @param array  $secrets Array of consumer and token secret
     *
     * @return void
     */
    public function __construct($url = null, array $secrets = array())
    {
    	$this->url = $url;
    	$this->parseUrl();
    	
    	$this->secrets = $secrets;
    	$this->config = self::$httpConfig;	
    }
    
	/**
     * Set http request config
     * @param array $config
     */
    public function setConfig(array $config)
    {
    	$this->config = array_merge($this->config, $config);
    }
    
    /**
     * Get http request config
     * @return array
     */
    public function getConfig()
    {
    	return $this->config;
    }
    
	/**
     * Sets authentication type. Valid auth types are self::AUTH_HEADER,
     * self::AUTH_POST, and self::AUTH_GET
     *
     * @param int $type Auth type defined by this class constants
     * @return void
     */
    public function setAuthType($type)
    {
		static $valid = array(
			self::AUTH_HEADER, self::AUTH_POST, self::AUTH_GET
		);
		
		if (!in_array($type, $valid)) {
			$type = self::AUTH_HEADER;
		}
		$this->authType = $type;
    }

    /**
     * Gets authentication type
     * @return int
     */
    public function getAuthType()
    {
        return $this->authType;
    }

    /**
     * Set http method
     * @param string $method
     */
    public function setMethod($method)
    {
		static $valid = array(
			'POST', 'GET'
		);
		
		$method = strtoupper($method);
		if (!in_array($method, $valid)) {
			$method = 'POST';
		}
		$this->method = $method;
    }
    
    /**
     * Get Http method
     * @return string
     */
    public function getMethod()
    {
    	return $this->method;
    }
    
    /**
     * Set oauth signature method
     * @param string $signature_method
     */
    public function setSignatureMethod($signature_method)
    {
		static $valid = array(
			'HMAC-SHA1', 'MD5'
		);
		
		$signature_method = strtoupper($signature_method);
		if (!in_array($signature_method, $valid)) {
			$signature_method = 'HMAC-SHA1';
		}
		$this->parameters['oauth_signature_method'] = $signature_method;
    }
    
	/**
     * Get oauth signature method
     * @return string
     */
    public function getSignatureMethod()
    {
		if (isset($this->parameters['oauth_signature_method'])) {
			return $this->parameters['oauth_signature_method'];
		}
		
		return 'HMAC-SHA1';
    }
    
 	/**
     * Sets consumer/token secrets array
     *
     * @param array $secrets Array of secrets to set
     * @return void
     */
    public function setSecrets(array $secrets = array())
    {
        if (count($secrets) == 1) {
            $secrets[1] = '';
        }

        $this->secrets = $secrets;
    }

    /**
     * Get parameters
     * @return array
     */
    public function getParameters()
    {
        return $this->parameters;
    }

	/**
	 * Set parameters
	 *
	 * @param array $params Name => value pair array of parameters
	 * @return void
	 */
	public function setParameters(array $params)
	{
		$this->parameters = array_merge($this->parameters, $params);
	}

	/**
	 * Get OAuth specific parameters
	 *
	 * @return array OAuth specific parameters
	 */
	public function getOAuthParameters()
	{
		$params = array();
		foreach (self::$oauthParams as $param) {
			if (isset($this->parameters[$param])) {
				$params[$param] = $this->parameters[$param];
			}
		}
		ksort($params);
		return $params;
	}
    
	/**
     * Sends oauth request
     *
     * @return OAuth10Response
     * @throws OAuthException
     */
    public function send()
    {
    	$this->parameters['oauth_timestamp'] = time();
    	$this->parameters['oauth_nonce'] = self::generateRandomKey(8);
    	$this->parameters['oauth_version'] = '1.0';
        $this->parameters['oauth_signature'] = $this->generateSignature();
        
        $headers = array();
        $post_fields = array();
        $query_fields = array();
        $this->initRequestParams($headers, $post_fields, $query_fields);
        
        $response = $this->sendHttpRequest($headers, $post_fields, $query_fields);
        
        return new OAuth10Response($response);
    }

	/**
	 * Build HTTP Query
	 *
	 * @param array $params Name => value array of parameters
	 *
	 * @return string HTTP query
	 */
	public static function buildHttpQuery(array $params)
	{
		if (empty($params)) {
			return '';
		}
		
		$keys = self::urlencode(array_keys($params));
		$values = self::urlencode(array_values($params));
		$params = array_combine($keys, $values);
		
		ksort($params);
		
		$pairs = array();
		foreach ($params as $key => $value) {
			if (is_array($value)) {
				sort($value);
				foreach ($value as $v) {
					$pairs[] = $key . '=' . $v;
				}
			} else {
				$pairs[] = $key . '=' . $value;
			}
		}
		
		return implode('&', $pairs);
	}

	/**
	 * URL Encode
	 *
	 * @param mixed $item string or array of items to url encode
	 * @return mixed url encoded string or array of strings
	 */
	public static function urlencode($item)
	{
		if (is_array($item)) {
			return array_map(array('OAuth10Request', 'urlencode'), $item);
		} elseif (is_object($item)) {
			$rvar = null;
			foreach ($item as $key => $val) {
				$rvar->{$key} = self::urlencode($val);
			}
			return $rvar;
		} else {
			return str_replace('%7E', '~', rawurlencode($item));
		}
	}

	/**
	 * URL Decode
	 *
	 * @param mixed $item Item to url decode
	 * @return string URL decoded string
	 */
	public static function urldecode($item)
	{
		if (is_array($item)) {
			return array_map(array('OAuth10Request', 'urldecode'), $item);
		} elseif (is_object($item)) {
			$rvar = null;
			foreach ($item as $key => $val) {
				$rvar->{$key} = self::urldecode($val);
			}
			return $rvar;
		} else {
			return rawurldecode($item);
		}
	}
    
	/**
	 * Generate a random string of specifified length
	 * @param int $len
	 * @param string $seed
	 * @return string
	 */
	public static function generateRandomKey($len, $seed = '')
	{
		if (empty($seed)) {
			$seed = 'abcdefghijklmnopqrstuvwxyz' .
					'ABCDEFGHIJKLMNOPQRSTUVWXYZ' .
					'0123456789~-_%&#';
		}
		
		$seed_len = strlen($seed);
		$word = '';
		
		for ($i = 0; $i < $len; ++$i) {
			$word .= $seed{mt_rand() % $seed_len};
		}
		
		return $word;
	}

	/**
	 * Get realm value for OAuth authorization header
	 * @return string
	 */
	public function getRealm()
	{
		return $this->realm;
	}
	
	/**
	 * Get uri for signature base string
	 * @return string
	 */
	public function getBaseStringUri()
    {
    	return $this->baseStringUri;
    }
    
	/**
	 * Creates OAuth header
	 *
	 * Given the passed in OAuth parameters, put them together
	 * in a formated string for a Authorization header.
	 *
	 * @param array $params OAuth parameters
	 * @return string
	 */
	protected function getAuthForHeader(array $params)
	{
		$header = 'OAuth realm="' . $this->getRealm() . '"';
		foreach ($params as $name => $value) {
			$name = self::urlencode($name);
			$value = self::urlencode($value);
			$header .= ', ' . $name . '="' . $value . '"';
		}
		
		return $header;
	}
	
	/**
	 * Generate signature for oauth params
	 * 
	 * @return string
	 * @throws OAuthException
	 */
	protected function generateSignature()
    {
    	if (isset($this->parameters['oauth_signature'])) {
    		unset($this->parameters['oauth_signature']);
    	}
    	//Get Signature base string
    	$method	= $this->method;
    	$uri	= $this->getBaseStringUri();
    	$params	= self::buildHttpQuery($this->parameters);
    	
    	$parts = array($method, $uri, $params);
        $base  = implode('&', self::urlencode($parts));
        
        $sig = OAuthSignature::factory($this->getSignatureMethod());
        return $sig->signature($base, $this->secrets[0], $this->secrets[1]);
    }
    
	/**
	 * Parse url to extract the realm and base string uri
	 */
	private function parseUrl()
	{
		$info = parse_url($this->url);
		
		$scheme = strtolower($info['scheme']);
    	$host = strtolower($info['host']);
    	
    	$default_port = ($info['scheme'] == 'https') ? 443 : 80;
    	
    	$port = '';
		if (isset($info['port'])) {
			if ($info['port'] != $default_port) {
				$port = $info['port'];
			}
		}
    	
    	$this->realm = $scheme . '://' . $host . ($port ? ":$port" : '') . '/';
		$this->baseStringUri = $scheme . '://' . $host . ($port ? ":$port" : '') . $info['path'];
		$this->port = $port ? $port : $default_port;
	}
	
	/**
	 * Init params for http request
	 * 
	 * @param array $headers
	 * @param array $postFields
	 * @param array $queryFields
	 */
	public function initRequestParams(array & $headers, array & $postFields, array & $queryFields)
	{
		$headers = array();
        $postFields = array();
        $queryFields = array();
		
		$params = $this->getOAuthParameters();
        switch ($this->authType) {
	        case self::AUTH_HEADER:
	            $auth = $this->getAuthForHeader($params);
	            $headers[] = 'Authorization: ' . $auth;
	            break;
	            
	        case self::AUTH_POST:
	        	$postFields = $params;
	            break;
	            
	        case self::AUTH_GET:
	        	$queryFields = $params;
	            break;
        }
        
		switch ($this->method) {
			case 'POST' :
				$headers[] = 'Content-Type: application/x-www-form-urlencoded';
				foreach ($this->parameters as $name => $value) {
					if (substr($name, 0, 6) == 'oauth_') {
						continue;
					}
					$postFields[$name] = $value;
				}
				break;
				
			case 'GET' :
				foreach ($this->parameters as $name => $value) {
					if (substr($name, 0, 6) == 'oauth_') {
						continue;
					}
					$queryFields[$name] = $value;
				}
				break;
				
			default :
				break;
		}
	}
	
	protected function sendHttpRequest(array $headers, array $postFields, array $queryFields)
	{
		$ch = curl_init();
		
		$curl_opts = array(
			CURLOPT_USERAGENT => $this->config['user_agent'], 
			CURLOPT_HTTP_VERSION => CURL_HTTP_VERSION_1_1, 
			CURLOPT_RETURNTRANSFER => true,
			CURLOPT_HEADER => true, 
			CURLOPT_FOLLOWLOCATION => false,
			CURLOPT_BUFFERSIZE => $this->config['buffer_size'],
		);
		
		// setup connection timeout
		if (defined('CURLOPT_CONNECTTIMEOUT_MS')) {
			$curl_opts[CURLOPT_CONNECTTIMEOUT_MS] = $this->config['connect_timeout'];
		} else {
			$curl_opts[CURLOPT_CONNECTTIMEOUT] = ceil($this->config['connect_timeout'] / 1000);
		}
		
        // setup request timeout
		if (defined('CURLOPT_TIMEOUT_MS')) {
			$curl_opts[CURLOPT_TIMEOUT_MS] = $this->config['timeout'];
		} else {
			$curl_opts[CURLOPT_TIMEOUT] = ceil($this->config['timeout'] / 1000);
		}
		
		if (!$this->config['ssl_verify_peer']) {
			$curl_opts[CURLOPT_SSL_VERIFYPEER] = false;
		}
		
		//setup http header
		if ($headers) {
			$curl_opts[CURLOPT_HTTPHEADER] = $headers;
		}
		
		//setup http post body, if it use POST method
		if ($this->method == 'POST') {
			$curl_opts[CURLOPT_POST] = true;
			$curl_opts[CURLOPT_POSTFIELDS] = self::buildHttpQuery($postFields);
		} else {
			if ($postFields) {
				$queryFields = array_merge($postFields, $queryFields);
			}
			$curl_opts[CURLOPT_POST] = false;
		}
		
		if ($queryFields) {
			$url = $this->baseStringUri . '?' . self::buildHttpQuery($queryFields);
		} else {
			$url = $this->baseStringUri;
		}
		$curl_opts[CURLOPT_URL] = $url;

		curl_setopt_array($ch, $curl_opts);

		$result = curl_exec($ch);

		curl_close($ch);

		return $result;
	}
}

/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */