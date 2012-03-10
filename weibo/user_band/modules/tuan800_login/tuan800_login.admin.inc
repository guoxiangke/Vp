<?php
// $Id: tuan800_login.info,v 0.1 20110523 Felix $

function tuan800_login_admin_settings(){
  $form['tuan800_login_is_login'] = array(
    '#type' => 'radios',
    '#title' => '允许使用团800帐号登录',
    '#default_value' => variable_get('tuan800_login_is_login', 1),
    '#options' => array('不允许', '允许'),
    '#description' => '将与现有账户绑定或创建一个新用户',
    '#required' => TRUE,
  );
  $form['tuan800_login_app_key'] = array(
    '#type' => 'textfield',
    '#title' => 'APP_KEY',
    '#default_value' => variable_get('tuan800_login_app_key', '6379222889ec9088288aa8429ac20407'),
    '#description' => '输入你申请到的key',
    '#required' => TRUE,
  );
  $form['tuan800_login_app_secret'] = array(
    '#type' => 'textfield',
    '#title' => 'APP_SECRET',
    '#default_value' => variable_get('tuan800_login_app_secret', 'e88b91a7711a1fea105031e18b176be0'),
    '#description' => '输入你申请到的Secret',
    '#required' => TRUE,
  );
  $form['tuan800_login_auto_email'] = array(
    '#type' => 'radios',
    '#title' => '自动生成Email',
    '#default_value' => variable_get('tuan800_login_auto_email', 1),
    '#options' => array('不自动生成', '自动生成'),
    '#description' => '为团800用户自动生成一个形如 username@tuan800.com 的Email帐号，用于注册新用户并与团800帐号进行绑定，<br />不自动生成:将出现Email输入界面，提示用户输入一个常用的Email。<br />自动生成:将自动生成一个虚拟Email地址，不提示用户输入。',
    '#required' => TRUE,
  );
  return system_settings_form($form);
}