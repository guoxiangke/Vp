<?php
// $Id: baidu_login.info,v 0.1 20110527 Felix $

function baidu_login_admin_settings(){
  $form['baidu_login_is_login'] = array(
    '#type' => 'radios',
    '#title' => '允许使用百度hao123帐号登录',
    '#default_value' => variable_get('baidu_login_is_login', 1),
    '#options' => array('不允许', '允许'),
    '#description' => '将与现有账户绑定或创建一个新用户',
    '#required' => TRUE,
  );
  $form['baidu_login_app_id'] = array(
    '#type' => 'textfield',
    '#title' => 'APP_ID',
    '#default_value' => variable_get('baidu_login_app_id', '122261'),
    '#description' => '输入你申请应用的id',
    '#required' => TRUE,
  );
  $form['baidu_login_app_key'] = array(
    '#type' => 'textfield',
    '#title' => 'APP_KEY',
    '#default_value' => variable_get('baidu_login_app_key', 'iPZPpkg2zbZMtpHcBGBw4fCm'),
    '#description' => '输入你申请到的key',
    '#required' => TRUE,
  );
  $form['baidu_login_app_secret'] = array(
    '#type' => 'textfield',
    '#title' => 'APP_SECRET',
    '#default_value' => variable_get('baidu_login_app_secret', 'pC24EMtIy4z86VPVAaBaHv6PPjoOVYGA'),
    '#description' => '输入你申请到的Secret',
    '#required' => TRUE,
  );
  return system_settings_form($form);
}