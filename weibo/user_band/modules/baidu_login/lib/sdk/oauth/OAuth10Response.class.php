<?php
/***************************************************************************
 *
 * Copyright (c)2009 Baidu.com, Inc. All Rights Reserved
 *
 **************************************************************************/

/**
 * OAuth 1.0 Response
 *
 * @package	OAuth
 * @author	zhujt(zhujianting@baidu.com)
 * @version	$Revision: 1.0 $
 **/
class OAuth10Response
{
	/**
	 * OAuth response datas
	 * @var array
	 */
	protected $params = array();
	
	/**
	 * Consturct
	 * @param string $response
	 */
	public function __construct($response)
	{
		$this->parseResponse($response);
	}
	
	/**
	 * Get response params
	 * @return array
	 */
	public function getResponseParams()
	{
		return $this->params;
	}
	
	/**
	 * Parse http response content to extract the response params
	 * @param string $response
	 * @return void
	 * @throws OAuthException
	 */
	protected function parseResponse($response)
	{
		$this->params = array();
		
		if (empty($response)) {
			throw new OAuthException('Some error occurred when do http request', -1);
		}
		
		$pos = strpos($response, "\r\n\r\n");
		$header = substr($response, 0, $pos);
		$body = trim(substr($response, $pos + 4));
		
		$status = substr($header, 0, strpos($header, "\r\n"));
		if (preg_match('/^HTTP\/\d\.\d\s([\d]+)\s(.*)$/', $status, $matches)) {
			$code = intval($matches[1]);
			$status = $matches[2];
			if ($code == 400 || $code == 401 || $code == 500) {
				throw new OAuthException($body, $code);
			} elseif ($code == 200) {
				parse_str($body, $this->params);
			} else {
				throw new OAuthException($status, $code);
			}
		} else {
			throw new OAuthException('Invalid http response', -1);
		}
	}
}

/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */