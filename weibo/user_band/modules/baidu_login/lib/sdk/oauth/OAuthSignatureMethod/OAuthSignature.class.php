<?php
/***************************************************************************
 *
 * Copyright (c)2009 Baidu.com, Inc. All Rights Reserved
 *
 **************************************************************************/

/**
 * Baidu OAuth Signature
 *
 * @package	OAuth
 * @author	zhujt(zhujianting@baidu.com)
 * @version	$Revision: 1.0 $
 **/
abstract class OAuthSignature
{
	/**
	 * Factory
	 *
	 * @param string $method Signature method
	 * @return OAuthSignatureMethod
	 */
	public static function factory($method)
	{
		$method = preg_replace('/[^A-Z0-9]/', '_', strtoupper($method));
		$class = 'OAuthSignatureMethod_' . $method;
		
		require_once (dirname(__FILE__) . '/' . $class . '.class.php');
		if (class_exists($class) === false) {
			throw new OAuthException('Unsupported signature method:' . $method);
		}
		
		$instance = new $class();
		if (!$instance instanceof OAuthSignatureMethod) {
			throw new OAuthException('Signature class does not extend OAuthSignatureMethod');
		}
		
		return $instance;
    }
}


/* vi:set ts=4 sts=4 sw=4 binary noeol: */