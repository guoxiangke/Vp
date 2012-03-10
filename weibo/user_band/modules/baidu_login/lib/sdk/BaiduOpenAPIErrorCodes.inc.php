<?php
/***************************************************************************
 *
 * Copyright (c) 2009 Baidu.com, Inc. All Rights Reserved
 *
 **************************************************************************/

/**
 * Error codes and descriptions for the Baidu Open API.
 * If the developer is going to add his own error codes, to retain compatibility
 * with Baidu Open API, you may wish to begin your error codes at 10000 and above
 * 
 * @package	BaiduOpenAPI
 * @author	zhujt(zhujianting@baidu.com)
 * @version $Revision: 1.3 $
 **/

define('BDAPI_EC_SUCCESS', 0);

/**
 * general errors
 **/
define('BDAPI_EC_UNKNOWN', 1);
define('BDAPI_EC_SERVICE', 2);
define('BDAPI_EC_METHOD', 3);
define('BDAPI_EC_TOO_MANY_CALLS', 4);
define('BDAPI_EC_BAD_IP', 5);
define('BDAPI_EC_PERMISSION', 6);
define('BDAPI_EC_BAD_REFERER', 7);

/**
 * param errors
 **/
define('BDAPI_EC_PARAM', 100);
define('BDAPI_EC_PARAM_API_KEY', 101);
define('BDAPI_EC_PARAM_SESSION_KEY', 102);
define('BDAPI_EC_PARAM_CALL_ID', 103);
define('BDAPI_EC_PARAM_SIGNATURE', 104);
define('BDAPI_EC_PARAM_TOO_MANY', 105);
define('BDAPI_EC_PARAM_SIGMETHOD', 106);
define('BDAPI_EC_PARAM_TIMESTAMP', 107);
define('BDAPI_EC_PARAM_USER_ID', 108);
define('BDAPI_EC_PARAM_USER_FIELD', 109);

/**
 * user permission errors
 **/
define('BDAPI_EC_PERMISSION_USER', 210);
define('BDAPI_EC_PERMISSION_INVALID_PERM', 211);

/**
 * Pay API errors
 **/
define('BDAPI_EC_PAY', 300);
define('BDAPI_EC_PAY_ORDER_INVALID', 301);
define('BDAPI_EC_PAY_ORDER_NOT_EXISTS', 302);
define('BDAPI_EC_PAY_NOT_AUTHORIZED', 303);
define('BDAPI_EC_PAY_STOPPED', 304);

/**
 * data store API errors
 **/
define('BDAPI_EC_DATA_UNKNOWN_ERROR', 800); // should never happen
define('BDAPI_EC_DATA_INVALID_OPERATION', 801);
define('BDAPI_EC_DATA_QUOTA_EXCEEDED', 802);
define('BDAPI_EC_DATA_OBJECT_NOT_FOUND', 803);
define('BDAPI_EC_DATA_OBJECT_ALREADY_EXISTS', 804);
define('BDAPI_EC_DATA_DATABASE_ERROR', 805);

/**
 * application info errors
 **/
define('BDAPI_EC_NO_SUCH_APP', 900);

/**
 * batch API errors
 **/
define('BDAPI_EC_BATCH_ALREADY_STARTED', 950);
define('BDAPI_EC_BATCH_NOT_STARTED', 951);
define('BDAPI_EC_BATCH_TOO_MANY_ITEMS', 952);
define('BDAPI_EC_BATCH_METHOD_NOT_ALLOWED_IN_BATCH_MODE', 953);


class BaiduOpenAPIErrorDescs
{
	protected static $arrOpenAPIErrDescs = array(
		BDAPI_EC_SUCCESS			=> 'Success',
		BDAPI_EC_UNKNOWN			=> 'Unknown error',
		BDAPI_EC_SERVICE			=> 'Service temporarily unavailable',
		BDAPI_EC_METHOD				=> 'Unsupported openapi method',
		BDAPI_EC_TOO_MANY_CALLS		=> 'Open api request limit reached',
		BDAPI_EC_BAD_IP				=> 'Unauthorized client IP address:%s',
		BDAPI_EC_PERMISSION			=> 'No permission to access user data',
		BDAPI_EC_BAD_REFERER		=> 'No permission to access data for this referer',

		BDAPI_EC_PARAM				=> 'Invalid parameter',
		BDAPI_EC_PARAM_API_KEY		=> 'Invalid API key',
		BDAPI_EC_PARAM_SESSION_KEY	=> 'Session key invalid or no longer valid',
		BDAPI_EC_PARAM_CALL_ID		=> 'Invalid/Used call_id parameter',
		BDAPI_EC_PARAM_SIGNATURE	=> 'Incorrect signature',
		BDAPI_EC_PARAM_TOO_MANY		=> 'Too many parameters',
		BDAPI_EC_PARAM_SIGMETHOD	=> 'Unsupported signature method',
		BDAPI_EC_PARAM_TIMESTAMP	=> 'Invalid/Used timestamp parameter',
		BDAPI_EC_PARAM_USER_ID		=> 'Invalid user id',
		BDAPI_EC_PARAM_USER_FIELD	=> 'Invalid user info field',
		
		BDAPI_EC_PERMISSION_USER	=> 'User not visible',
		BDAPI_EC_PERMISSION_INVALID_PERM => 'Unsupported permission:%s',
		
		BDAPI_EC_PAY					=> 'Unknown pay API error',
		BDAPI_EC_PAY_ORDER_INVALID		=> 'Order or amount format not match',
		BDAPI_EC_PAY_ORDER_NOT_EXISTS	=> 'Order not exist',
		BDAPI_EC_PAY_NOT_AUTHORIZED		=> 'App has not apply for the payment services',
		BDAPI_EC_PAY_STOPPED			=> 'Payment service for this app has been stopped',
		
		BDAPI_EC_DATA_UNKNOWN_ERROR	=> 'Unknown data store API error',
		BDAPI_EC_DATA_INVALID_OPERATION	=> 'Invalid operation',
		BDAPI_EC_DATA_QUOTA_EXCEEDED	=> 'Data store allowable quota was exceeded',
		BDAPI_EC_DATA_OBJECT_NOT_FOUND	=> 'Specified object cannot be found',
		BDAPI_EC_DATA_OBJECT_ALREADY_EXISTS => 'Specified object already exists',
		BDAPI_EC_DATA_DATABASE_ERROR	=> 'A database error occurred. Please try again',
		
		BDAPI_EC_NO_SUCH_APP			=> 'No such application exists',
		
		BDAPI_EC_BATCH_ALREADY_STARTED	=> 'begin_batch already called, please make sure to call end_batch first',
		BDAPI_EC_BATCH_NOT_STARTED		=> 'end_batch called before start_batch',
		BDAPI_EC_BATCH_TOO_MANY_ITEMS	=> 'Each batch API can not contain more than 20 items',
		BDAPI_EC_BATCH_METHOD_NOT_ALLOWED_IN_BATCH_MODE => 'This method is not allowed in batch mode',
	);

	public static function errmsg($errcode)
	{
		if (isset(self::$arrOpenAPIErrDescs)) {
			return self::$arrOpenAPIErrDescs[$errcode];
		} else {
			return self::$arrOpenAPIErrDescs[BDAPI_EC_UNKNOWN];
		}
	}
}

class BaiduOpenAPIException extends Exception
{

	public function __construct($errcode, $errmsg = null)
	{
		if (empty($errmsg)) {
			$errmsg = BaiduOpenAPIErrorDescs::errmsg($errcode);
		}
		parent::__construct($errmsg, $errcode);
	}
}


/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */
?>
