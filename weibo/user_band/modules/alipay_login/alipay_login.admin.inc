<?php
// $Id: alipay_login.info,v 0.1 20110504 Felix $

/**
 * 管理设置
 *
 * 提示：如何获取安全校验码和合作身份者id
 * 1.用您的签约支付宝账号登录支付宝网站(www.alipay.com)
 * 2.点击“商家服务”(https://b.alipay.com/order/myorder.htm)
 * 3.点击“查询合作者身份(pid)”、“查询安全校验码(key)”
 *
 * 安全校验码查看时，输入支付密码后，页面呈灰色的现象，怎么办？
 * 解决方法：
 * 1、检查浏览器配置，不让浏览器做弹框屏蔽设置
 * 2、更换浏览器或电脑，重新登录查询。
 */
function alipay_login_admin_settings(){
  $form['alipay_login_is_login'] = array(
    '#type' => 'radios',
    '#title' => '允许使用支付宝帐号登录',
    '#default_value' => variable_get('alipay_login_is_login', 1),
    '#options' => array('不允许', '允许'),
    '#description' => '将与现有账户绑定或创建一个新用户',
    '#required' => TRUE,
  );
  $form['alipay_login_partner'] = array(
    '#type' => 'textfield',
    '#title' => '合作身份者id',
    '#default_value' => variable_get('alipay_login_partner', NULL),
    '#description' => '合作身份者id，以2088开头的16位纯数字',
    '#required' => TRUE,
  );
  $form['alipay_login_key'] = array(
    '#type' => 'textfield',
    '#title' => '安全检验码',
    '#default_value' => variable_get('alipay_login_key', NULL),
    '#description' => '安全检验码，以数字和字母组成的32位字符',
    '#required' => TRUE,
  );
  $form['alipay_login_sign_type'] = array(
    '#type' => 'textfield',
    '#title' => '签名方式',
    '#default_value' => variable_get('alipay_login_sign_type', 'MD5'),
    '#description' => '签名方式 不需修改',
    '#required' => TRUE,
  );
  $form['alipay_login_input_charset'] = array(
    '#type' => 'textfield',
    '#title' => '字符编码格式',
    '#default_value' => variable_get('alipay_login_input_charset', 'utf-8'),
    '#description' => '字符编码格式 目前支持 gbk 或 utf-8',
    '#required' => TRUE,
  );
  $form['alipay_login_transport'] = array(
    '#type' => 'textfield',
    '#title' => '访问模式',
    '#default_value' => variable_get('alipay_login_transport', 'http'),
    '#description' => '访问模式,根据自己的服务器是否支持ssl访问，若支持请选择https；若不支持请选择http',
    '#required' => TRUE,
  );

  return system_settings_form($form);
}