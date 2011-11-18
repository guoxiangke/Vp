<?php
/**
 * Menu callback; Generate a listing of promoted nodes.
 * copy form node_page_default() in node.module
 */
//module_load_include('inc', 'sina_vp', 'sina_vp.pages');
function sina_vp_center_page_default($tid = NULL) {
	$sql='SELECT n.nid, n.sticky, n.created FROM {node} n WHERE n.type in ("weibo") AND n.promote = 1 AND n.status = 1 ORDER BY n.sticky DESC, n.created DESC';
	if($tid){
  	//SELECT n.nid, n.sticky, n.created FROM {node} n WHERE n.type in ("weibo") AND n.promote = 1 AND n.status = 1 ORDER BY n.sticky DESC, n.created DESC	
  	$sql='SELECT n.nid, n.sticky, n.created
				 FROM {node} n 
				 INNER JOIN term_node term_node ON n.vid = term_node.vid
				 WHERE (n.type in ("weibo") AND n.promote = 1 AND n.status = 1) AND (term_node.tid = '.$tid.')';
  }
  $result = pager_query(db_rewrite_sql($sql), variable_get('default_nodes_main', 10));

  $output = '';
  $num_rows = FALSE;
  while ($node = db_fetch_object($result)) {
    $output .= node_view(node_load($node->nid), 1);
    $num_rows = TRUE;
  }

  if ($num_rows) {
    $feed_url = url('rss.xml', array('absolute' => TRUE));
    drupal_add_feed($feed_url, variable_get('site_name', 'Drupal') .' '. t('RSS'));
    $output .= theme('pager', NULL, variable_get('default_nodes_main', 10));
  }
  else {
    $default_message = t('<h1 class="title">Welcome to your new Drupal website!</h1><p>Please follow these steps to set up and start using your website:</p>');
    $default_message .= '<ol>';

    $default_message .= '<li>'. t('<strong>Configure your website</strong> Once logged in, visit the <a href="@admin">administration section</a>, where you can <a href="@config">customize and configure</a> all aspects of your website.', array('@admin' => url('admin'), '@config' => url('admin/settings'))) .'</li>';
    $default_message .= '<li>'. t('<strong>Enable additional functionality</strong> Next, visit the <a href="@modules">module list</a> and enable features which suit your specific needs. You can find additional modules in the <a href="@download_modules">Drupal modules download section</a>.', array('@modules' => url('admin/build/modules'), '@download_modules' => 'http://drupal.org/project/modules')) .'</li>';
    $default_message .= '<li>'. t('<strong>Customize your website design</strong> To change the "look and feel" of your website, visit the <a href="@themes">themes section</a>. You may choose from one of the included themes or download additional themes from the <a href="@download_themes">Drupal themes download section</a>.', array('@themes' => url('admin/build/themes'), '@download_themes' => 'http://drupal.org/project/themes')) .'</li>';
    $default_message .= '<li>'. t('<strong>Start posting content</strong> Finally, you can <a href="@content">create content</a> for your website. This message will disappear once you have promoted a post to the front page.', array('@content' => url('node/add'))) .'</li>';
    $default_message .= '</ol>';
    $default_message .= '<p>'. t('For more information, please refer to the <a href="@help">help section</a>, or the <a href="@handbook">online Drupal handbooks</a>. You may also post at the <a href="@forum">Drupal forum</a>, or view the wide range of <a href="@support">other support options</a> available.', array('@help' => url('admin/help'), '@handbook' => 'http://drupal.org/handbooks', '@forum' => 'http://drupal.org/forum', '@support' => 'http://drupal.org/support')) .'</p>';

    $output = '<div id="first-time">'. $default_message .'</div>';
  }
  if(isset($sina_vp_type)) drupal_set_title(t($sina_vp_type));
	if($tid=='Transfer' || $tid=='Show' ||is_null($tid)){
		 return $output;		  		
	}
	if(user_access("add new ".strtolower($sina_vp_type))){
		$output ="<p>点击<a href='?q=vp/add/$sina_vp_type'>这里</a>发布".t($sina_vp_type)."博文</p>".$output;
	}
	/*
	 * 	if(user_access("add new ".strtolower($sina_vp_type))){
		popups_add_popups();
		$output_add = '<ol id="test-list">';
	  $output_add .= '<li>点击'. l("这里", "vp/add/$sina_vp_type",
   									array( 'attributes' => array('class' => 'popups-form'))).'发#'.t($sina_vp_type).'#信息';
		$output .= "</li></ol>";
		$output=$output_add .$output;		
	 * */
  return $output;
}
/**
 * 买家或卖家中心，根据角色Seller Buyer而定.
 * copy form sina_vp_center_page_default()
 */
function sina_vp_UCenter_page($sina_vp_type = NULL) {
	global $user;
	$array=user_relationships_load(array("user" => $user->uid),array("sort" => 'requestee_id'));
	//$follows_uid ="'1'"; 
	foreach($array as $key=>$value){
		$follows_uid[] =$key; //取得粉丝用户uid
	}
	if(empty($array)){
		$follows_uid[] ='1';//没有任何订阅时，默认推送管理员的信息 more....
	}
	$follows_uid = implode(",",$follows_uid);
	if(in_array('Buyer',$user->roles)){		
		$sql = 'SELECT n.nid, n.sticky, n.created FROM {node} n ';
		$sql_where = 'WHERE n.type in ("weibo") AND n.promote = 1 AND n.status = 1 ';
		$sql_where_add = 'AND n.uid in('.$follows_uid.') ';
		$sql_order = 'ORDER BY n.sticky DESC, n.created DESC ';
		$sql =$sql.$sql_where.$sql_where_add .$sql_order;
	}elseif(in_array('Seller',$user->roles)){		
		$sql = 'SELECT n.nid, n.sticky, n.created FROM {node} n ';
		$sql_where = 'WHERE n.type in ("weibo") AND n.promote = 1 AND n.status = 1 ';
		$sql_where_add = 'AND n.uid in('.$user->uid.') ';
		$sql_order = 'ORDER BY n.sticky DESC, n.created DESC ';
		$sql =$sql.$sql_where.$sql_where_add .$sql_order;
	}else{
		return t('Access deny!请申请角色后再访问！');
	}
	$sina_vp_type_tid =sina_vp_get_type_by_string($sina_vp_type);
  if($sina_vp_type){
  	//SELECT n.nid, n.sticky, n.created FROM {node} n WHERE n.type in ("weibo") AND n.promote = 1 AND n.status = 1 ORDER BY n.sticky DESC, n.created DESC	
  	$sql='SELECT n.nid, n.sticky, n.created
				 FROM {node} n 
				 INNER JOIN term_node term_node ON n.vid = term_node.vid
				 WHERE (n.type in ("weibo") AND n.promote = 1 AND n.status = 1) AND (term_node.tid = '.$sina_vp_type_tid.')';
		$sql =$sql.$sql_where_add.$sql_order;;
  }
  $result = pager_query(db_rewrite_sql($sql), variable_get('default_nodes_main', 10));

  $output = '';
  $num_rows = FALSE;
  while ($node = db_fetch_object($result)) {
    $output .= node_view(node_load($node->nid), 1);
    $num_rows = TRUE;
  }

  if ($num_rows) {
    $feed_url = url('rss.xml', array('absolute' => TRUE));
    drupal_add_feed($feed_url, variable_get('site_name', 'Drupal') .' '. t('RSS'));
    $output .= theme('pager', NULL, variable_get('default_nodes_main', 10));
  }
  else {
  	if(in_array('Seller',$user->roles)){		
	    $default_message = t('<h1 class="title">作为一个有身份的卖家是不会偷懒的！</h1><p>Please follow these steps to set up and start using your website:</p>');
	    $weibo_type_array=array('Product','Activity','Special','Sale');//,'Transfer','Show'
			$default_message .= '<ol>';	
			foreach($weibo_type_array as $type){
		    $default_message .= '<li>'. t('<strong>发布<a href="@VpCenter">'.t($type).'</a>信息</strong>', array('@VpCenter' => url($type))) .'</li>';
		    }
			$default_message .= '</ol>';  			
		}elseif(in_array('Buyer',$user->roles)){			
	    $default_message = t('<h1 class="title">您订阅的用户信息少的可怜，我们推荐您关注更多街友！</h1><p>Please follow these steps to set up and start using your website:</p>');
	    $default_message .= '<ol>';	
	    $default_message .= '<li>'. t('<strong>关注更多卖家</strong>请移步<a href="@VpCenter">微铺大厅</a>查看更多卖家。', array('@VpCenter' => url('VpCenter'))) .'</li>';
	    $default_message .= '<li>'. t('<strong>添加更多粉丝</strong> 请移步<a href="@Show">真人秀</a>或<a href="@Transfer">转让潮</a>关注更多街友。', array('@Show' => url('Show'),'@Transfer' => url('Transfer'))) .'</li>';
	    $default_message .= '</ol>';
	    $default_message .= '<p>'. t('For more information, please refer to the <a href="@help">help section</a>, or the <a href="@handbook">online Drupal handbooks</a>. You may also post at the <a href="@forum">Drupal forum</a>, or view the wide range of <a href="@support">other support options</a> available.', array('@help' => url('admin/help'), '@handbook' => 'http://drupal.org/handbooks', '@forum' => 'http://drupal.org/forum', '@support' => 'http://drupal.org/support')) .'</p>';
				
		}else{
			return t('Access deny!请申请角色后再访问！');
		}
    $output = '<div id="first-time">'. $default_message .'</div>';
  }
  drupal_set_title('');
	if($sina_vp_type=='Transfer' || $sina_vp_type=='Show' ||is_null($sina_vp_type)){
		 return $output;		  		
	}
	if(user_access('add new '.strtolower($sina_vp_type))){
		$output ="<h3><a href='?q=vp/add/$sina_vp_type'>点击发".$sina_vp_type."博文</a></h3>".$output;
	}
  return $output;
}

//del.....
function user_timeline_page_form_sina($delete=true){
	drupal_set_title('user_timeline');
	global $user;
	$path = drupal_get_path('module', 'sina');
	$http_url="http://api.t.sina.com.cn/statuses/user_timeline.json";
	$w_user_id=sina_open_t_get_info($user)->id;
	$values=array(
								'user_id' => $w_user_id,
								'count'=>3,
								'base_app'=>1
	);
	$result  = sina_open_t_set_request($http_url, $values, $headers = array(), $method = 'POST');
	 if ($result->code == 200) {
    //drupal_set_message('信息获取成功');
		if(function_exists(dpm)) dpm(__FILE__.__LINE__,'信息获取成功!');
		//_sina_weibo2node($result->data);
  } else {
    drupal_set_message('信息获取失败', 'error');
    return false;
  }
	$ms  = json_decode($result->data);
	if(function_exists(dpm)) dpm($ms,"ms");
	 //Final output string
  return output_weibo($ms);
	
}
/**
 * 不要了，最后2行有用。
 */
function vp_center_page($delete=true){
	/*global $user;
	$path = drupal_get_path('module', 'sina');

	$http_url="http://api.t.sina.com.cn/statuses/public_timeline.json";//3360192113599959
	$w_user_id=sina_open_t_get_info($user)->id;
	$values=array(
								'count'=>200,
								'base_app'=>1
	);
	$result  = sina_open_t_set_request($http_url, $values, $headers = array(), $method = 'POST');
	 if ($result->code == 200) {
    //drupal_set_message('信息获取成功');
		if(function_exists(dpm)) dpm(__FILE__.__LINE__,'信息获取成功vp_center_page!');
		//_sina_weibo2node($result->data);
  } else {
    drupal_set_message('信息获取失败', 'error');
    return false;
  }
	$ms  = json_decode($result->data);
	if(function_exists(dpm)) dpm($ms,"ms");*/
	 //Final output string
	//drupal_get_form(weibo_page_default);
	 //return  page_weibo_add().public_page();
	 
}