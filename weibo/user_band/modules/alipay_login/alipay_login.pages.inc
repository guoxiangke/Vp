<?php
// $Id: alipay_login.info,v 0.1 20110504 Felix $

function alipay_login_page_login(){
	// TO DO
	require_once("lib/alipay_service.class.php");
	$alipay_config = alipay_login_get_config();
	
	//扩展功能参数——防钓鱼//

	//防钓鱼时间戳
	$anti_phishing_key  = '';
	//获取客户端的IP地址，建议：编写获取客户端IP地址的程序
	$exter_invoke_ip = '';
	//注意：
	//1.请慎重选择是否开启防钓鱼功能
	//2.exter_invoke_ip、anti_phishing_key一旦被使用过，那么它们就会成为必填参数
	//3.开启防钓鱼功能后，服务器、本机电脑必须支持SSL，请配置好该环境。
	//示例：
	//$exter_invoke_ip = '202.1.1.1';
	//$ali_service_timestamp = new AlipayService($aliapy_config);
	//$anti_phishing_key = $ali_service_timestamp->query_timestamp();//获取防钓鱼时间戳函数
	
	//构造要请求的参数数组，无需改动
	$parameter = array(
	        //扩展功能参数——防钓鱼
	        "anti_phishing_key"	=> $anti_phishing_key,
			"exter_invoke_ip"	=> $exter_invoke_ip,
	);
	
	$alipayService = new AlipayService($alipay_config);
	$html_text = $alipayService->alipay_auth_authorize($parameter);
	
	return $html_text;
}

function alipay_login_page_callback(){
	// TO DO
	
	require_once("lib/alipay_notify.class.php");
	$alipay_config = alipay_login_get_config();
	//计算得出通知验
	$alipayNotify = new AlipayNotify($alipay_config);
	$verify_result = $alipayNotify->verifyReturn();
	if($verify_result) {//验证成功
		$ouid = $_GET['user_id'];  //支付宝用户id
		$name = $_GET['real_name'];
		//'email'=> $_GET[''], //如接口返回数据中有Email，则设置此项
		$token = $_GET['token'];  //授权令牌
	
		//执行用户绑定接口程序，参数 $ouser 结构见 user_band_bandapi 函数定义
		$ouser = array(
			'ouid'=> $ouid,  //支付宝用户id
			'name'=> $name,
			'token' => $token,  //授权令牌
			'from'=>'alipay',
			);
		$_SESSION['alipay_login_token'] = $token;
		//$url = 'alipay/address/'.base64_encode(serialize($token));
		$url = 'alipay/address';
		drupal_goto($url);
		//user_band_bandapi($ouser);
		
	}
	else {
	    //验证失败
	    //如要调试，请看alipay_notify.php页面的return_verify函数，比对sign和mysign的值是否相等，或者检查$veryfy_result有没有返回true
	    drupal_set_message('支付宝帐号验证失败', 'error');
	    drupal_goto('user');
	}
	
}

/*
 * 支付宝收货地址接口，获取用户配送信息
 */
function alipay_login_user_address(){
	// TO DO
	require_once("lib/alipay_service.class.php");
	$alipay_config = alipay_login_get_config('address');
	
	//必填参数//
	//授权令牌，该参数的值由快捷登录接口(alipay.auth.authorize)的页面跳转同步通知参数中获取
	$token = $_SESSION['alipay_login_token'];
	//注意：
	//token的有效时间为30分钟，过期后需重新执行快捷登录接口(alipay.auth.authorize)获得新的token
	
	//构造要请求的参数数组，无需改动
	$parameter = array(
	        "token"	=> $token,
	);
	
	$alipayService = new AlipayService($alipay_config);
	$html_text = $alipayService->user_logistics_address_query($parameter);
	
	return $html_text;
}

function alipay_login_page_address_callback(){
	// TO DO
	require_once("lib/alipay_notify.class.php");
	$alipay_config = alipay_login_get_config('address');
	
	//计算得出通知验
	$alipayNotify = new AlipayNotify($alipay_config);
	$verify_result = $alipayNotify->verifyNotify();
	if($verify_result) {//验证成功
		$ouid = $_POST['user_id'];  //支付宝用户id
		$name = $_POST['real_name'];
		$email = $_POST['email']; //如接口返回数据中有Email，则设置此项
		$token = $_SESSION['alipay_login_token'];  //授权令牌
	//var_dump($_POST);
		//执行用户绑定接口程序，参数 $ouser 结构见 user_band_bandapi 函数定义
		$ouser = array(
			'ouid'=> $ouid,  //支付宝用户id
			'name'=> $name,
			'email'=> $email, 
			'token' => $token,  //授权令牌
			'from'=>'alipay',
			);
		//var_dump($ouser);
		//drupal_goto();
		user_band_bandapi($ouser);
		
	}
	else {
	    //验证失败
	    //如要调试，请看alipay_notify.php页面的return_verify函数，比对sign和mysign的值是否相等，或者检查$veryfy_result有没有返回true
	    drupal_set_message('支付宝帐号验证失败', 'error');
	    drupal_goto('user');
	}
}