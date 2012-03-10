<?php
/***************************************************************************
 *
 * Copyright (c) 2008 Baidu.com, Inc. All Rights Reserved
 *
 **************************************************************************/

define('BAIDU_OPENAPI_CLIENT_LIB', dirname(__FILE__));
define('BAIDU_OAUTH_CLIENT_LIB', dirname(__FILE__) .'/oauth');

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
define('BD_APPS_DOMAIN', 'bb-passport-test00.bb01');
define('BD_OPENAPI_DOMAIN', 'bb-passport-test00.bb01');
define('BD_SPACE_OPENAPI_DOMAIN', 'http://tc-space-test00.tc.baidu.com:8000');

//百度OAuth的相关地址
define('BD_OAUTH_REQUEST_TOKEN_URL', 'http://bb-passport-test00.bb01.baidu.com:8008/oauth/1.0/request_token');
define('BD_OAUTH_AUTHORIZE_URL', 'http://bb-passport-test00.bb01.baidu.com:8008/oauth/1.0/authorize');
define('BD_OAUTH_ACCESS_TOKEN_URL', 'http://bb-passport-test00.bb01.baidu.com:8008/oauth/1.0/access_token');
define('BASE_DOMAIN', 'baidu.com');

//用户授权后回跳地址，配置为'oob'时平台将用注册应用时填写的oauth_callback_url作为回跳地址
if (isset($_SERVER['HTTP_X_FORWARDED_HOST'])) {
	$host = $_SERVER['HTTP_X_FORWARDED_HOST'];
} else {
	$host = $_SERVER['HTTP_HOST'];
}
define('CURRENT_DOMAIN', 'http://' . $host);


//测试环境特有的宏定义
define('IN_TEST_ENV', 0);
define('BD_TEST_ENV_PORT', 8008);
define('DEBUG_MODE' , 1);

if ($_REQUEST['bd_sig_app_id'] == 100010) {	//BDML demo
	$app_id = 100010;
	$api_key = 'dHHPRjeZECL42XA8s56EfGKY';
	$app_secret = 'uIRHExwHEgZGEVNtDclug9FhZRIMBpZE';
	//应用的callback地址
	define('APP_CALLBACK_URL', '/appdemo/demo/bdml');
} elseif ($_REQUEST['bd_sig_app_id'] == 100013) {
	$app_id = 100013;
	$api_key = 'OZXNrX17RtUHBWdC1Ftt6ImM';
	$app_secret = 'bd53pYEwZ27MBs1XQ2694dOBq2bsvf6E';
	//应用的callback地址
	define('APP_CALLBACK_URL', '/appdemo/demo/bdiframe');
} elseif ($_REQUEST['bd_sig_app_id'] == 100033) {
	$app_id = 100033;
	$api_key = '0iyxdyNgXBok1agAIeu8I2ea';
	$app_secret = 'uARPUtRA8d3lxmsVqYdMMZwvHdChMF9U';
} elseif ($_REQUEST['bd_sig_app_id'] == 100041) {
	$app_id = 100041;
	$api_key = 'X4CZP5Ij3YqMlcEZ9oGKdeD5';
	$app_secret = 'rGsrtoGMwS8mScwGnv63CzIi9mzdDvaa';
} elseif ($_REQUEST['bd_sig_app_id'] == 100233) {
	$app_id = 100233;
	$api_key = 'W9stqBD2k6U7LXvAmKMd9hNS';
	$app_secret = 'A8sahepzGobAEzMRPZ8FWAGye1rSvpwi';
} elseif ($_REQUEST['bd_sig_app_id'] == 100286) {
	$app_id = 100286;
	$api_key = 'M6mQfzzh6qaZScDXrYOw91el';
	$app_secret = 'GPImAQLP4OPdorH32LVxGSaQ3UkR4mte';
}

$baidu = new BaiduOpenApp($app_id, $api_key, $app_secret, BASE_DOMAIN);

if (isset($baidu->bd_params['mobile'])) {
	//暂不支持mobile版，后续会支持
	//$user = $baidu->require_wap_login();
} else {
	//$user = $baidu->require_login('', $baidu->current_url(), $baidu->get_app_homepage_url());
}




/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */
?>
