<?php
/**
 * //判断邀请吗与选择的角色是否一致。user/register/Buy|Sale
 */
function page_register_roles($role) {
	if($role=='Sale'){
		$role='Seller';
		$choose_role='Buyer';
	}elseif($role=='Buyer'){
		$choose_role='Sale';
	}else{
		//404 想干啥，自己写url,牛啊！}//对英语不足的教训。//
	}
	if($role!=$_SESSION['roles']){
		drupal_set_message('角色不对，您的邀请码只能选择'.l('我是'.t($_SESSION['roles']),'user/register/'.$choose_role),'error');
		drupal_goto('choose_a_role');
	}
	//unset($_SESSION['roles']);unset($_SESSION['reg_code']);
  return drupal_get_form('user_register');	  
}
/**
 * 卖家成功注册，提示信息。
 */
function page_register_success(){
	if($_SESSION['reg_code']){
		unset($_SESSION['reg_code']);
	}
	if($_SESSION['roles']){
		unset($_SESSION['roles']);
	}
	return '详见page-register_success.tpl.php';
}
/**
 * 输入邀请码
 */
function page_enter_regcode(){
	if($_SESSION['roles']&&$_SESSION['reg_code'])	drupal_goto('choose_a_role');
	return $output = '<div  id="regcode-voucher-show">
				<form action="choose_a_role" method="get">
				<input type="text" maxlength="20" name="regcode" id="regcode" size="15" value="" class="form-text required" placeholder=" 邀请码"/>
				<input  type="submit"  value="确定" />
				</form>
				</div>';
}
/**
 * 身份选择
 */
function page_choose_a_role(){
	//选择身份之前先输入邀请码。存入session，对邀请的判断。
	$output='
				<div class="sfxzvp-body-inner">
					<div class="sfxzbody-inner-head">
						<div class="sfxzselecthead">
							<div class="sfxzselectbu">1.身份确认</div>
							<div class="sfxzsltpic1"><img alt="" src="'.path_to_theme().'/images/'.'logo.gif"></div>
							<div class="sfxzselectbu">2.完善信息</div>
							<div class="sfxzselectbu">3.等待确认</div>
							<div class="sfxzselectbu2">4.成功注册</div>
							<div class="clear"></div>
						</div>
					</div>
					<div class="sfxzselectsf">
					    <div class="sfxzsftitle">欢迎进入微铺街，接下来请选择您的身份，开启您的微铺街旅程</div>
						<div class="sfxzsltbnt">
						<a href="user/register/Buyer" class="sfxzbuybtn">我是买家</a>
						<a href="user/register/Sale" class="sfxzsellbtn">我是卖家</a></div>
					</div>
			  </div>
				<div class="clear"></div>';
					
				//dpm($_REQUEST['regcode']);dvm($_REQUEST['reg_code']);
				
	if(is_null($_SESSION['reg_code'])){
		if(!$_REQUEST['regcode']){
			drupal_set_message('请输入邀请码,再选择角色！','error');
			drupal_goto('enter_regcode');
		}
		//检查邀请码，设置session
		$rid = db_result(db_query('SELECT rid FROM {regcode} WHERE code="'.$_REQUEST['regcode'].'" LIMIT 1'));
	  $regcode = regcode_load($rid);
		if($regcode->is_active){
			$_SESSION['reg_code'] = $_REQUEST['regcode'];
			foreach($regcode->tags as $term){
				if($term=='买家邀请码'){
						$_SESSION['roles'] = 'Buyer';
				}elseif($term=='卖家邀请码'){
						$_SESSION['roles'] = 'Seller';
				}
			}	
		}elseif($regcode->uid){
			drupal_set_message('邀请码已被使用，请重新输入！','error');
			unset($_SESSION['reg_code']);
			drupal_goto('enter_regcode');
		}else{
			drupal_set_message('邀请码不对，请重新输入！','error');
			drupal_goto('enter_regcode');
		}			
	}else{
		
		//有邀请码信息，判断下一步的角色是否一致
		$rid = db_result(db_query('SELECT rid FROM {regcode} WHERE code="'.$_SESSION['reg_code'].'" LIMIT 1'));
		//tid=53买家邀请码/54卖家邀请码
		if($regcode = regcode_load($rid))
		foreach($regcode->tags as $term){
			if($term=='买家邀请码'){
					$_SESSION['roles'] = 'Buyer';
			}elseif($term=='卖家邀请码'){
					$_SESSION['roles'] = 'Seller';
			}
		}
	}
	return $output;
}

