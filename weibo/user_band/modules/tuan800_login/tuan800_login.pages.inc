<?php
// $Id: tuan800_login.info,v 0.1 20110523 Felix $

function tuan800_login_page_login(){
	require_once("lib/TuanAuth.php");
	$tuan800_config = tuan800_login_get_config();
	//在此修改相应的回调地址
	$callback = $tuan800_config['return_url'];
	
	$authSvc      = new TuanAuth();
	$token        = $authSvc->getRequestToken();

	if($token["oauth_token"])
	{
	    $_SESSION['tuan800_request_token'] = $token;
	    
	    //输出相应的Token数据
	    $authorizeUrl = $authSvc->getAuthorizeURL( $token['oauth_token'] , $callback );
	    header("Location:$authorizeUrl");
	}
	else
	{
	    //错误了,可以定义跳转到对应错误页面
	    drupal_set_message('团800帐号验证失败', 'error');
	    drupal_goto('user');
	}
}

function tuan800_login_page_callback(){
	require_once("lib/TuanAuth.php");
	$tuan800_config = tuan800_login_get_config();
	//计算得出通知验
	$h = new TuanAuth($_SESSION['tuan800_request_token']['oauth_token'],$_SESSION['tuan800_request_token']['oauth_token_secret']);
	$accessToken   = $h->getAccessToken($_REQUEST['oauth_verifier']) ;

	if($accessToken["oauth_token"])
	{
		$oauth_token  = $accessToken["oauth_token"];
		$oauth_secret = $accessToken["oauth_token_secret"];
		$_SESSION["tuan800_access_token"] =array('oauth_token'=>$oauth_token,'oauth_token_secret'=>$oauth_secret);
		
		$h = new TuanAuth($oauth_token,$oauth_secret);
		$result = $h->get("http://api.tuan800.com/oauth/oauthapi/userinfo/userInfo.json",null) ;
	    
	    $userInfo = $result['userInfo'];
		$ouid = $userInfo['userName'];  //说明：团800官方不提供id号，username为唯一标识；绑定表中，第三方id支持字符串，故此处用username绑定
		$name = $userInfo['userName'];
		$token = $_SESSION['tuan800_access_token']; 
		//执行用户绑定接口程序，参数 $ouser 结构见 user_band_bandapi 函数定义
		$ouser = array(
			'ouid'=> $ouid,
			'name'=> $name,
			'token' => $token,
			'from'=>'tuan800',
			);
		if($tuan800_config['auto_email']){  //自动生成Email
			$ouser['email'] = $name.'@tuan800.com';
		}
		user_band_bandapi($ouser);
	}
	else
	{
	    //错误了,可以定义跳转到对应错误页面
	    drupal_set_message('团800帐号验证失败', 'error');
	    drupal_goto('user');
	}
	
}
