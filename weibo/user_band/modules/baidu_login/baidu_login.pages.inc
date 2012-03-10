<?php
// $Id: baidu_login.info,v 0.1 20110527 Felix $

function baidu_login_page_login(){
	require_once("lib/BaiduOpenAPI.inc.php");

	if (!$baidu->get_loggedin_user()) {
		$baidu->require_authorization();
	} else {
		if (!empty($baidu->uname)) {
			
			$ouid = $baidu->user;
			$name = $baidu->uname;
			$token = md5($baidu->api_client->session_key . $baidu->secret); 
			
			//执行用户绑定接口程序，参数 $ouser 结构见 user_band_bandapi 函数定义
			$ouser = array(
				'ouid'=> $ouid,
				'name'=> $name,
				//'email' => $email,
				'token' => $token,
				'from'=>'baidu',
				);
			user_band_bandapi($ouser);
		} else {
			try {
				$user = $baidu->api_client->users_getLoggedInUser();
				$uid = $user['user'];
				$name = $user['uname'];
				$ouid = $baidu->uid;
				$name = $baidu->uname;
				$token = md5($baidu->api_client->session_key . $baidu->secret); 
				
				//执行用户绑定接口程序，参数 $ouser 结构见 user_band_bandapi 函数定义
				$ouser = array(
					'ouid'=> $ouid,
					'name'=> $name,
					//'email' => $email,
					'token' => $token,
					'from'=>'baidu',
					);
				user_band_bandapi($ouser);
			} catch (Exception $e) {
				//肯定是cookie中session失效了，重新授权
				$baidu->require_authorization();
			}
		}
	}
}
