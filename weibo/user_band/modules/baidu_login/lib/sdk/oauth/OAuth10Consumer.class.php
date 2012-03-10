<?php
/***************************************************************************
 *
 * Copyright (c)2009 Baidu.com, Inc. All Rights Reserved
 *
 **************************************************************************/

/**
 * OAuth 1.0 Client
 *
 * @package	OAuth
 * @author	zhujt(zhujianting@baidu.com)
 * @version	$Revision: 1.0 $
 **/
class OAuth10Consumer
{
	/**
     * Consumer key
     * @var string
     */
    protected $key = null;

    /**
     * secret
     * @var string
     */
    protected $secret = null;

    /**
     * Token
     * @var string
     */
    protected $token = null;

    /**
     * Token secret
     * @var string
     */
    protected $tokenSecret = null;

    /**
     * Signature method
     * @var string
     */
    protected $signatureMethod = 'HMAC-SHA1';
    
    /**
     * http request config
     * @var array
     */
    protected $config = array();
    
    /**
     * Error message
     * @var string
     */
    private $errmsg = '';
    
	/**
     * Construct
     *
     * @param string $key         Consumer key
     * @param string $secret      Consumer secret
     * @param string $token       Access/Reqest token
     * @param string $tokenSecret Access/Reqest token secret
     */
    public function __construct($key, $secret, $token = null, $tokenSecret = null)
    {
        $this->key			= $key;
        $this->secret		= $secret;
        $this->token		= $token;
        $this->tokenSecret	= $tokenSecret;
    }
    
	/**
     * Get request token
     *
     * @param string $url        Request token url
     * @param string $callback   Callback url
     * @param array  $additional Additional parameters to be in the request
     *                           recommended in the spec.
     * @param string $method     HTTP method to use for the request
     * @return array|false
     */
    public function getRequestToken($url, $callback = 'oob',
    								array $additional = array(),
    								$method = 'POST',
    								$authType = OAuth10Request::AUTH_HEADER)
    {
		$additional['oauth_callback'] = $callback;
		
		try {
			$response = $this->sendRequest($url, $additional, $method, $authType);
			$data = $response->getResponseParams();
			if (empty($data['oauth_token'])
				|| empty($data['oauth_token_secret'])
				|| $data['oauth_callback_confirmed'] != 'true') {
				$this->errmsg = 'Get request token failed';
				return false;
			}
			
			$this->token = $data['oauth_token'];
			$this->tokenSecret = $data['oauth_token_secret'];
			
			return array('token' => $this->token,
						 'secret' => $this->tokenSecret);
			
		} catch (OAuthException $e) {
			$this->errmsg = $e->getMessage();
			return false;
		}
    }

    /**
     * Get access token
     *
     * @param string $url        Access token url
     * @param string $verifier   OAuth verifier from the provider
     * @param array  $additional Additional parameters to be in the request
     *                           recommended in the spec.
     * @param string $method     HTTP method to use for the request
     * @param bool $use_xauth	Whether to use xauth or not, default is false
     *
     * @return array|false Token and token secret
     */
    public function getAccessToken($url, $verifier = '',
    								array $additional = array(),
    								$method = 'POST',
    								$authType = OAuth10Request::AUTH_HEADER,
    								$use_xauth = false)
	{
		try {
		    if (!$use_xauth) {
    			if ($this->token === null || $this->tokenSecret === null) {
    				$this->errmsg = 'No token or token_secret';
    				return false;
    			}
    			
    			$additional['oauth_verifier'] = $verifier;
		    }
			$response = $this->sendRequest($url, $additional, $method, $authType);
			$data = $response->getResponseParams();
			if (empty($data['oauth_token']) || empty($data['oauth_token_secret'])) {
				$this->errmsg = 'Get access token failed';
				return false;
			}
			
			$this->token = $data['oauth_token'];
			$this->tokenSecret = $data['oauth_token_secret'];
			$tmp_key = (!$use_xauth) ? 'expires' : 'x_auth_expires';
			$expires = isset($data[$tmp_key]) ? intval($data[$tmp_key]) : null;
			$uid = isset($data['uid']) ? intval($data['uid']) : 0;
			$uname = isset($data['uname']) ? $data['uname'] : '';
			$portrait = isset($data['portrait']) ? $data['portrait'] : '';
			
			return array('token' => $this->token,
						 'secret' => $this->tokenSecret,
						 'expires' => $expires,
						 'uid' => $uid,
						 'uname' => $uname,
						 'portrait' => $portrait,
			);
			
		} catch (OAuthException $e) {
			$this->errmsg = $e->getMessage();
			return false;
		}
    }

    /**
     * Get authorize url
     *
     * @param string $url        Authorize url
     * @param array  $additional Additional parameters for the auth url
     *
     * @return string Authorize url
     */
    public function getAuthorizeUrl($url, array $additional = array())
    {
        $params = array('oauth_token' => $this->token);
        $params = array_merge($additional, $params);

        return sprintf('%s?%s', $url, OAuth10Request::buildHTTPQuery($params));
    }
    
    /**
     * Get error message
     * @return string
     */
    public function errmsg()
    {
    	return $this->errmsg;
    }
    
    /**
     * Set config for http request
     * @param $config
     */
    public function setConfig(array $config)
    {
    	$this->config = $config;
    }

    /**
     * Set signature method
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

        $this->signatureMethod = $signature_method;
    }
    
    /**
     * Get signature method
     * @return string
     */
    public function getSignatureMethod()
    {
    	return $this->signatureMethod;
    }
    
	/**
     * Get secrets
     * @return array
     */
    public function getSecrets()
    {
        return array($this->secret, (string)$this->tokenSecret);
    }
    
	/**
     * Send oauth request
     *
     * @param string $url		URL of the protected resource
     * @param array $additional	Additional parameters
     * @param string $method	HTTP method to use
     * @param int $authType		Authorization type
     * @return OAuth10Response
     * @throws OAuthException
     */
    protected function sendRequest($url, array $additional, $method, $authType)
    {
        $params = array('oauth_consumer_key'     => $this->key,
        				'oauth_signature_method' => $this->getSignatureMethod());

        if ($this->token) {
			$params['oauth_token'] = $this->token;
		}
		
		$params = array_merge($additional, $params);
        
        $request = new OAuth10Request($url, $this->getSecrets());
        
        $request->setAuthType($authType);
        $request->setMethod($method);
        $request->setParameters($params);
        $request->setConfig($this->config);
        
        return $request->send();
    }
}

/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */