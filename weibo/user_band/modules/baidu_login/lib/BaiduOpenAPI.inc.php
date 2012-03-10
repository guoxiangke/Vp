<?php
/***************************************************************************
 *
 * Copyright (c) 2008 Baidu.com, Inc. All Rights Reserved
 *
 **************************************************************************/

//define('BAIDU_OPENAPI_CLIENT_LIB', dirname(__FILE__) .'/../..');
define('BAIDU_OPENAPI_CLIENT_LIB', dirname(__FILE__) .'/sdk');
define('BAIDU_OAUTH_CLIENT_LIB', BAIDU_OPENAPI_CLIENT_LIB .'/oauth');

function RegisterMyClassName($className, $classPath)
{
	global $arrPublicClassName;
	$arrPublicClassName[$className] = $classPath;
}

function PublicLibAutoLoader($className)
{
	global $arrPublicClassName;
	if( array_key_exists($className, $arrPublicClassName) )
	{
		require_once($arrPublicClassName[$className]);
	}
}
$GLOBALS['arrPublicClassName'] = array(
	'BaiduOpenApp'			=> BAIDU_OPENAPI_CLIENT_LIB .'/BaiduOpenApp.class.php',
	'BaiduRestClient'		=> BAIDU_OPENAPI_CLIENT_LIB .'/BaiduRestClient.class.php',
	'BaiduSpaceRestClient'	=> BAIDU_OPENAPI_CLIENT_LIB .'/BaiduSpaceRestClient.class.php',
	'BaiduOpenAPIException'	=> BAIDU_OPENAPI_CLIENT_LIB .'/BaiduOpenAPIErrorCodes.inc.php',
	'BaiduOpenAPIErrorDescs'=> BAIDU_OPENAPI_CLIENT_LIB .'/BaiduOpenAPIErrorCodes.inc.php',
	'BaiduUtils'            => BAIDU_OPENAPI_CLIENT_LIB .'/BaiduUtils.class.php',
	'BaiduOAuthClient'		=> BAIDU_OPENAPI_CLIENT_LIB .'/BaiduOAuthClient.class.php',
	'OAuth10Consumer'		=> BAIDU_OAUTH_CLIENT_LIB .'/OAuth10Consumer.class.php',
	'OAuth10Request'		=> BAIDU_OAUTH_CLIENT_LIB .'/OAuth10Request.class.php',
	'OAuth10Response'		=> BAIDU_OAUTH_CLIENT_LIB .'/OAuth10Response.class.php',
	'OAuthException'		=> BAIDU_OAUTH_CLIENT_LIB .'/OAuthException.class.php',
	'OAuthSignature'		=> BAIDU_OAUTH_CLIENT_LIB .'/OAuthSignatureMethod/OAuthSignature.class.php',
);

spl_autoload_register('PublicLibAutoLoader');

define('OPEN_API_SDK_VERSION', '1.0');

//超时配置，单位s
define('POST_TIMEOUT', 21600);
define('GET_TIMEOUT', 21600);
define('COOKIE_TIMEOUT', 21600);

//http交互超时控制，单位: ms
define('CONNECT_TIMEOUT', 1000);
define('READ_TIMEOUT', 3000);

//apps域和openapi域的域名前缀
define('BD_APPS_DOMAIN', 'app');
define('BD_OPENAPI_DOMAIN', 'openapi');


//百度OAuth的相关地址
define('BD_OAUTH_REQUEST_TOKEN_URL', 'https://openapi.baidu.com/oauth/1.0/request_token');
define('BD_OAUTH_AUTHORIZE_URL', 'http://openapi.baidu.com/oauth/1.0/authorize');
define('BD_OAUTH_ACCESS_TOKEN_URL', 'https://openapi.baidu.com/oauth/1.0/access_token');

//用户授权后回跳地址，配置为'oob'时平台将用注册应用时填写的Canvas Callback URL（对于站内web应用）或
//授权回调地址（对于站外Connect应用）作为回跳地址，如果配置为'oob'但注册应用时未填写相应回调地址，则
//百度应用开放平台将会在用户同意授权后显示PIN码在页面上，需要用户将该PIN码输入到您的应用中作为
//oauth_verifier值继续完成oauth授权流程
define('BD_OAUTH_CALLBACK_URL', '');

//本应用的主域名，SDK中设置的cookie将保存在该域名下
define('BASE_DOMAIN', $_SERVER['HTTP_HOST']);

//当前页面所在具体域名
if (isset($_SERVER['HTTP_X_FORWARDED_HOST'])) {
	$host = $_SERVER['HTTP_X_FORWARDED_HOST'];
} else {
	$host = $_SERVER['HTTP_HOST'];
}
define('CURRENT_DOMAIN', 'http://' . $host);

//以下3个值是您注册应用时由百度应用开放平台分配的唯一标识您的应用的数据
$app_id = variable_get('baidu_login_app_id', '122261');
$api_key = variable_get('baidu_login_app_key', 'iPZPpkg2zbZMtpHcBGBw4fCm');
$app_secret = variable_get('baidu_login_app_secret', 'pC24EMtIy4z86VPVAaBaHv6PPjoOVYGA'); 

//应用的所有代码的公共父目录，该配置项仅在百度应用开放平台的站内Web应用中需要
//假设您的应用的Canvas Callback URL属性为http://www.example.com/baidu/appdemo/iframe/（即
//您的所有应用页面都在/baidu/appdemo/iframe/目录下，则APP_CALLBACK_URL就应该设置为'/baidu/appdemo/iframe'
define('APP_CALLBACK_URL', '');


$baidu = new BaiduOpenApp($app_id, $api_key, $app_secret, BASE_DOMAIN);


/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */