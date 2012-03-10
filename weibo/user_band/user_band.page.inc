<?php
// $Id: user_band.pages.inc,v 1.5 2011/04/22 13:52:44 Felix Exp $

/*
 * 
 */
function user_band_email($ouser){
	//var_dump($ouser);
	$output = drupal_get_form('user_band_email_form',$ouser);
    //echo $output;
    return $output;
    //exit();
}
 
function user_band_email_form(&$form_state, $ouser){
  $form['user_band_login_email'] = array(
    '#type' => 'textfield',
    '#title' => 'Email',
    '#description' => '请填写一个您常用的邮箱地址以方便您的使用，以后您可以直接用这个邮箱登陆团酷网，<br />请放心，我们和您一样讨厌垃圾邮件！',
    '#required' => 1
  );
  $form['ouser'] = array(
    '#type' => 'hidden',
    '#value' => $ouser,
  );
  $form['#validate'][] = 'user_band_email_form_validate';
  //$form['#redirect'] = "";
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => '确认'
  );
  return $form;
}

function user_band_email_form_validate($form, &$form_state) {
  $form_state['values']['user_band_login_email'] = trim($form_state['values']['user_band_login_email']);
  $email = $form_state['values']['user_band_login_email'];
  if (!valid_email_address($email)) {
    form_set_error('user_band_login_email', '请输入正确的Email格式');
  }
}

function user_band_email_form_submit($form, &$form_state){
	//-----log for test login times cost---------------------------------------
	$logtext = 'user_band_email form_submit';
	$filename = 'sinalog/'.time().'-18-'.$logtext.'.txt';
	$dest= file_directory_path() .'/'. $filename;
	file_save_data($logtext, $dest);
	//-----End log-------------------------------------------------------------
	$email = trim($form_state['values']['user_band_login_email']);
	$uid = user_band_get_uid_by_email($email);
	$ouser = unserialize(urldecode(base64_decode($form_state['values']['ouser'])));
	if($uid == 0){ //$ouser中的Email尚未注册
		// TO DO: 用Email进行注册
		$ouser['email'] = $email;
	//-----log for test login times cost---------------------------------------
	$logtext = 'user_band_email user_band_reg';
	$filename = 'sinalog/'.time().'-19-'.$logtext.'.txt';
	$dest= file_directory_path() .'/'. $filename;
	file_save_data($logtext, $dest);
	//-----End log-------------------------------------------------------------
		$user = user_band_reg($ouser);
		$uid = $user->uid;
	}
	//-----log for test login times cost---------------------------------------
	$logtext = 'user_band_email user_band_band';
	$filename = 'sinalog/'.time().'-20-'.$logtext.'.txt';
	$dest= file_directory_path() .'/'. $filename;
	file_save_data($logtext, $dest);
	//-----End log-------------------------------------------------------------
	user_band_band($uid,$ouser);
	$user_info = array('uid'=>$uid);
	//-----log for test login times cost---------------------------------------
	$logtext = 'user_band_email user_load';
	$filename = 'sinalog/'.time().'-21-'.$logtext.'.txt';
	$dest= file_directory_path() .'/'. $filename;
	file_save_data($logtext, $dest);
	//-----End log-------------------------------------------------------------
	$newuser = user_load($user_info);
	user_external_login($newuser); //为新用户登录
	//-----log for test login times cost---------------------------------------
	$logtext = 'user_band_email jump to front';
	$filename = 'sinalog/'.time().'-22-'.$logtext.'.txt';
	$dest= file_directory_path() .'/'. $filename;
	file_save_data($logtext, $dest);
	//-----End log-------------------------------------------------------------
	drupal_goto(); // jump to <front>
}
