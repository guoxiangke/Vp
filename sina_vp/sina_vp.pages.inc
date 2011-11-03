<?php

/**
 * @file
 * User page callback file for the sina module.
 */
/**
 * post a weibo in node.
 */
function page_weibo_add(){
	//module_load_include('inc', 'node', 'node.pages');//调用节点发布相关
	return node_add('weibo');
}
/**
 * page_user_timeline.
 */
module_load_include('inc', 'sina', 'sina.user.page');
function page_user_timeline() {
	drupal_set_title(t('个人中心'));
	return page_weibo_add().user_timeline_page();
}
/**
 * weibo list output function
 * @ weibo data objs .
 */
function output_weibo($ms){
	//Final output string
    $output = '<div class="feed_lists W_linka W_texta" node-type="feed_list" pagenum="1">';
    //first   
    $timezone = "Asia/Chongqing";
		if(function_exists('date_default_timezone_set')){ 
			date_default_timezone_set($timezone);
		}
    foreach($ms as $weibo_obj){
		$created_at=strtotime($weibo_obj->created_at);
		$time_string=sina_vp_time_format($weibo_obj->created_at);
		//$created_at=strtotime($weibo_obj->created_at); //$created_at = strtotime($json->created_at); 
		//if(now()-$created_at) {$created_at_str}
		$id=number_format($weibo_obj->id, 0, '', '');//==if(function_exists(dpm)) dpm($weibo_obj->mid);
		$mid=$weibo_obj->mid;
		$text=sina_vp_url_replace($weibo_obj->text);
		$source =$weibo_obj->source;
		$favorited=$weibo_obj->favorited;//: 是否已收藏
		$truncated=$weibo_obj->truncated;//: 是否被截断
		$in_reply_to_status_id=$weibo_obj->in_reply_to_status_id;//: 回复ID
		$in_reply_to_user_id=$weibo_obj->in_reply_to_user_id;//: 回复人UID
		$in_reply_to_screen_name=$weibo_obj->in_reply_to_screen_name;//: 回复人昵称
		$thumbnail_pic=$weibo_obj->thumbnail_pic;//: 缩略图
		$bmiddle_pic=$weibo_obj->bmiddle_pic;//: 中型图片
		$original_pic=$weibo_obj->original_pic;//：原始图片
		$w_user=$weibo_obj->user;//: 作者信息
		$retweeted_status=$weibo_obj->retweeted_status;//: 转发的博文，内容为status，如果不是转发，则没有此字段
		
		$w_uid=$w_user->id;
		$profile_image_url = $w_user->profile_image_url.'.jpg';
		$w_user_url='http://weibo.com/';
		$w_user_url  .=empty($w_user->domain)?$w_uid:$w_user->domain;
		$name = empty($w_user->screen_name)?$w_user->name:$w_user->screen_name;
		if(!empty($w_user->verified)){$verified='<img class="small_icon vip_blue" title="新浪机构认证" src="http://img.t.sinajs.cn/t35/style/images/common/transparent.gif">';}
		
		global $user;
		$uid=$user->uid;
		if($uid!=0){
			$counts = sina_open_t_set_request('http://api.t.sina.com.cn/statuses/counts.json', array('ids'=>$id));
			$counts = json_decode($counts->data);
			$comments =$counts[0]->comments;
			$rt =$counts[0]->rt;
		}
		$output .= '<dl class="feed_list " action-type="feed_list_item" mid="3360362947067368">';
  	$output .= '<dt class="face">
									<a href="'.$w_user_url.'" uid="'.$w_uid.'" namecard="true" action-type="namecard" action-data="????">
									<img src="'.$profile_image_url.'" imagetype="head" uid="'.$w_uid.'" title="'.$w_user->screen_name.'">
									</a>
								</dt>';
		$output .= '<dd class="content">
								<p node-type="feed_list_content">
									<a usercard="id='.$w_uid.'" href="'.$w_user_url.'" title="'.$name.'" nick-name="'.$w_user->screen_name.'">'.$name.'</a>
									：
									<em>'.$text.'</em>
								</p>';
		if($thumbnail_pic){
			$output .= '<ul node-type="feed_list_media_prev" class="piclist">
									<li>
										<img action-type="feed_list_media_img" alt="" src="'.$thumbnail_pic.'" class="bigcursor">
										<img class="loading_gif" src="http://img.t.sinajs.cn/t4/style/images/common/loading.gif" style="left: 39.5px; top: 52px; display: none;">
									</li>
									</ul>';
		
		}
		if(isset($retweeted_status)){
			$output .='<dl class="comment" node-type="feed_list_media_disp" style="display: none;">';
			$output .=output_comment($retweeted_status);
			$output .='</dl>';
		}else{
			$output .= '<dl class="comment" node-type="feed_list_media_disp" style="display: none;"></dl>';	
		}
		$nid = get_nid_by_wid($id);
		$node = node_load($nid);
		$output .= '<p class="info W_linkb W_textb">
									<span>
									<a href="javascript:void(0);">转发('.$rt.')</a>
									<i class="W_vline">|</i>';
		$output .= flag_create_link('bookmarks', $node->nid);
		$output .= '<i class="W_vline">|</i>
									<a href="?q=examples/ahah_example/autotextfields/callback">评论</a>
									</span>
									<a node-type="feed_list_item_date" class="date" date="'.$created_at.'" title="'.date('Y-m-d H:m:i',$created_at).'" href="node/'.$nid.'">'.$time_string.'</a> 来自'.$source.'
								</p>
								<div class="repeat" style="display:none;" node-type="feed_list_repeat"></div>';
								
		$output.='</dd>';
		$output.='<dd class="clear"></dd>';
		$output.='</dl>';
    }	
	$output .= '</div>';
  return $output;
}
/**
 * output_comment.
 */
function output_comment($comment){
	return '<dl class="comment">
					<dd class="arrow"><span>◆</span></dd>
					 		<dt node-type="feed_list_forwardContent">
					<a usercard="id=1762675215" title="爱物网" href="/myhers" nick-name="爱物网">
					@爱物网</a><a href="http://weibo.com/verify" target="_blank"><img class="approve_co" alt="新浪机构认证" title="新浪机构认证" src="http://img.t.sinajs.cn/t4/style/images/common/transparent.gif"></a>：<em>哈哈哈哈哈我不是故意的相信我！！via:慢绣雷墩</em>
					</dt>
					<dd node-type="feed_list_media_prev">
				<ul class="piclist">
					<li>
					<img alt="" action-type="feed_list_media_img" src="http://ww1.sinaimg.cn/thumbnail/69104a0fjw1dldcu62x5cj.jpg" class="bigcursor"><img class="loading_gif" src="http://img.t.sinajs.cn/t4/style/images/common/loading.gif" style="left: 39.5px; top: 52px; display: none;">
				</li>
				</ul>		</dd>
					<dd node-type="feed_list_media_disp" style="display: none;" class="expand"></dd>
					<dd class="info W_linkb W_textb"><span><a href="/1762675215/xpfDUj3Bb?type=repost">转发(187)</a><i class="W_vline">|</i><a href="/1762675215/xpfDUj3Bb">评论(32)</a></span><a href="/1762675215/xpfDUj3Bb" class="date">9月21日 22:00</a> 来自<a rel="nofollow" href="http://www.hers.com.cn" target="_blank">Hers爱物网</a></dd>		 		</dl>';
}

/**
 * page_user_timeline.
 */
function user_timeline_page(){
	drupal_set_title('user_timeline');
	global $user;
	$sql="select * from {sina_vp_weibo2node} WHERE cid=0 AND is_deleted=0 AND uid=$user->uid AND wid<>0 ORDER BY id DESC";
	$result = db_query($sql);
	while ($record = db_fetch_object($result)) {
		$ms[]  = json_decode($record->data);
		$flag = true;
	}
	if(function_exists(dpm)) dpm($ms['0'],"微博信息[0]");
	 //Final output string
	if($flag){
		return output_weibo($ms);	
	}  
}
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
 * 微铺大厅 page展示页 public_timeline
 */
function vp_center_page_with_post(){
	return page_update().vp_center_page();
}
/**
 * 不要了，最后2行有用。
 */
function vp_center_page(){
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
/**
 * Menu callback; Generate a listing of weibo nodes.
 * LIKE node_page_default IN node.module
 */
function weibo_page_default(&$form_state){
	drupal_set_title('public_timeline');
  $num_rows = FALSE;	
	$sql="select * from {sina_vp_weibo2node} WHERE cid=0 AND wid<>0 AND is_deleted=0 ORDER BY id DESC";
	$count_query="select count(*) from {sina_vp_weibo2node} WHERE cid=0 AND is_deleted=0 AND wid<>0";
	$result = pager_query(db_rewrite_sql($sql),2, $element = 0, $count_query);
	while ($record = db_fetch_object($result)) {
			$weibo_obj=json_decode($record->data);
			//form format weibo_obj
			$created_at=strtotime($weibo_obj->created_at);
			$time_string=sina_vp_time_format($weibo_obj->created_at);
			$form['nid'][$record->nid] = array('#value' =>$record->nid);
			$id=number_format($weibo_obj->id, 0, '', '');//==if(function_exists(dpm)) dpm($weibo_obj->mid);
			$mid=$weibo_obj->mid;
			$form['wid'][$record->nid] = array('#value' =>$id);	
   			$form['created_at'][$record->nid] = array('#value' =>$time_string);
			//$text=node_load($record->nid)->body;
			$text=sina_vp_url_replace($weibo_obj->text);//
			$form['text'][$record->nid] = array('#value' =>$text);
			$form['source'][$record->nid] = array('#value' =>$weibo_obj->source);
			$w_user=$weibo_obj->user;//: 作者信息
			$w_uid=$w_user->id;
			$form['w_uid'][$record->nid] = array('#value' =>$w_uid);
			$domain=empty($w_user->domain)?$w_uid:$w_user->domain;
			$form['domain'][$record->nid] = array('#value' =>$domain);
			$profile_image_url = $w_user->profile_image_url.'.jpg';
			$form['profile_image_url'][$record->nid] = array('#value' =>$profile_image_url);
			$name = empty($w_user->screen_name)?$w_user->name:$w_user->screen_name;
			$form['screen_name'][$record->nid] = array('#value' =>$name);
			
			$favorited=$weibo_obj->favorited;//: 是否已收藏
			$form['favorited'][$record->nid] = array('#value' =>$favorited);
			$thumbnail_pic=$weibo_obj->thumbnail_pic;//: 缩略图
			$form['thumbnail_pic'][$record->nid] = array('#value' =>$thumbnail_pic);
			$bmiddle_pic=$weibo_obj->bmiddle_pic;//: 中型图片
			$form['bmiddle_pic'][$record->nid] = array('#value' =>$bmiddle_pic);
			$original_pic=$weibo_obj->original_pic;//：原始图片
			$form['original_pic'][$record->nid] = array('#value' =>$original_pic);		
			$retweeted_status=$weibo_obj->retweeted_status;//: 转发的博文，内容为status，如果不是转发，则没有此字段
			$form['retweeted_status'][$record->nid] = array('#value' =>$retweeted_status);
			
			$default_first_name = !empty($form_state['values']['ask_first_name'][$record->nid]) ? $form_state['values']['ask_first_name'][$record->nid] : FALSE;
			$form['ask_first_name'][$record->nid]['ask_first_name'] = array(
			  '#type' => 'checkbox',
			  '#title' => t('Add new comment'),
			  '#default_value' => $default_first_name,
			  '#ahah' => array(
			    'path' => 'vp/public_timeline/weibo_page_default/callback/'.$record->nid,
			    'wrapper' => 'textfield-wrapper-'.$record->nid,
			    'effect' => 'fade',
					)
			);
			$form['textfields'][$record->nid]['textfields'] = array(
		    '#title' => t("Generated text fields for first and last name"),
		    '#prefix' => '<div id="textfield-wrapper-'.$record->nid.'">',
		    '#suffix' => '</div>',
		    '#type' => 'fieldset',
		    '#description' => t('This is where we put automatically generated textfields'),
		  );
			dpm($form_state,'001-->$form_state');
			if (!empty($form_state['values']['ask_first_name'])) {
		    $form['textfields'][$record->nid]['textfields']['first_name'] = array(
		      '#type' => 'textfield',
		      '#title' => t('First Name'),
		    );
		  }
	}
 	$form['pager'] = array('#value' => theme('pager', NULL, 1, 0));
	$form['#theme'] = 'weibo_page_default';
  return $form;
}
/**
 * 主题化 微博列表.
 */
function theme_weibo_page_default($form){
  $path = drupal_get_path('module', 'sina_vp');
  $has_posts = isset($form['nid']) && is_array($form['nid']);
  $select_header['width'] = '26px';
  $header = array();// t('nid'), t('wid'), t('text'), t('created_at'), t('Status2'), t('source'), t('wuid')
  $output = '';
  if ($has_posts) {
	$output = '<div class="feed_lists W_linka W_texta" node-type="feed_list" pagenum="1">';
    foreach (element_children($form['nid']) as $key) {
			$row_info['nid'] = drupal_render($form['nid'][$key]);
			$row_info['wid'] = drupal_render($form['wid'][$key]);
			$row_info['text'] = drupal_render($form['text'][$key]);
			$row_info['created_at'] = drupal_render($form['created_at'][$key]);
			$row_info['screen_name'] = drupal_render($form['screen_name'][$key]);
			$row_info['source'] = drupal_render($form['source'][$key]);
			$row_info['w_uid'] = drupal_render($form['w_uid'][$key]);    
			 
			$row_info['thumbnail_pic'] = drupal_render($form['thumbnail_pic']);
			$row_info['bmiddle_pic'] = drupal_render($form['bmiddle_pic']);
			$row_info['original_pic'] = drupal_render($form['original_pic']);
			$row_info['profile_image_url'] = drupal_render($form['profile_image_url']);
			$row_info['domain'] = drupal_render($form['domain']);
			$row_info['retweeted_status'] = drupal_render($form['retweeted_status']);
	  $row = array();
	  $row['nid'] =array('data' => '<img src="'.$row_info['profile_image_url'].'"/>', 'class' => 'profile_image');
      $row['wid'] =$row_info['wid'];
      $row['text'] = array('data' => $row_info['text'], 'class' => 'row-text-info');
      $row['created_at'] = $row_info['created_at'];
      $row['screen_name'] =$row_info['created_at'];
	  $row['source'] =$row_info['source'];
	  $row['w_uid'] = $row_info['w_uid'];  
	  	   
      $rows[] = $row;
			$output .= '<dl class="feed_list " action-type="feed_list_item" mid="3360362947067368">';
			$output .= '<dt class="face">
									<a href="'.$w_user_url.'" uid="'.$form['w_uid'][$key]['#value'].'" namecard="true" action-type="namecard" action-data="????">
									<img src="'.$form['profile_image_url'][$key]['#value'].'" imagetype="head" uid="'.$form['w_uid'][$key]['#value'].'" title="'.$form['screen_name'][$key]['#value'].'">
									</a>
								</dt>';
			$output .= '<dd class="content">
								<p node-type="feed_list_content">
									<a usercard="id='.$form['w_uid'][$key]['#value'].'" href="'.$w_user_url.'" title="'.$name.'" nick-name="'.$form['screen_name'][$key]['#value'].'">'.$form['screen_name'][$key]['#value'].'</a>
									：
									<em>'.$form['text'][$key]['#value'].'</em>
								</p>';
		if($thumbnail_pic){
		$output .= '<ul node-type="feed_list_media_prev" class="piclist">
									<li>
										<img action-type="feed_list_media_img" alt="" src="'.$thumbnail_pic.'" class="bigcursor">
										<img class="loading_gif" src="http://img.t.sinajs.cn/t4/style/images/common/loading.gif" style="left: 39.5px; top: 52px; display: none;">
									</li>
									</ul>';
		
		}
		if(isset($retweeted_status)){
			$output .='<dl class="comment" node-type="feed_list_media_disp" style="display: none;">';
			$output .=output_comment($retweeted_status);
			$output .='</dl>';
		}else{
			$output .= '<dl class="comment" node-type="feed_list_media_disp" style="display: none;"></dl>';	
		}
		$nid = get_nid_by_wid($form['wid'][$key]['#value']);
		if($nid==0) continue; //>????
		$node = node_load($nid);
		$output .= '<p class="info W_linkb W_textb">
									<span>
									<a href="javascript:void(0);">转发('.$rt.')</a>
									<i class="W_vline">|</i>';
		$output .= flag_create_link('bookmarks', $node->nid);
		$output .= '<i class="W_vline">|</i>';
		$output .= drupal_render($form['ask_first_name'][$node->nid]);
		$output .= '</span>
									<a node-type="feed_list_item_date" class="date" date="'.$form['created_at'][$key]['#value'].'"   href="/node/'.$node->nid.'">'.$form['created_at'][$key]['#value'].'</a> 来自'.$form['source'][$key]['#value'].'
								</p>
								<div class="repeat" style="display:none;" node-type="feed_list_repeat"></div>';
								
		$output.='</dd>';
		$output.=drupal_render($form['textfields'][$node->nid]);
		$output.='<dd class="clear"></dd>';
		$output.='</dl>';
   
    }
  }
  else {
    $rows[] = array(array('data' => t('No posts available.'), 'colspan' => '6'));
  }
	$output .= '</div>';
 
  if ($form['pager']['#value']) {
    $output .= drupal_render($form['pager']);
  }
 	$output .= theme('table', $header, $rows);
 
  $output .= drupal_render($form);
  return  $output;//theme_pager_link('第一回合',null,$element = 0);
}
/**
 * vp_center_public not login page.
 */
function sina_vp_linked_info(){
	$linked_info='';
	global $user;
	$token = sina_open_get_access_token($user->uid);
	if(empty($token)){
		module_load_include('inc', 'sina_open', 'sina_open.pages');
		$linked_info = sina_open_page_config(user_load($user->id));
	}
	return $linked_info;
}
/**
 * vp_center_public not login page.
 */
function public_page(){
	drupal_set_title('public_page');	
	$output = '';
	$output = sina_vp_linked_info();
	$output .= page_weibo_add();
	
	$sql="select * from {sina_vp_weibo2node} WHERE cid=0 AND is_deleted=0 AND wid<>0 ORDER BY id DESC limit 0,20";
	$result = db_query($sql);
	$flag=false;
	while ($record = db_fetch_object($result)) {
		$ms[]  = json_decode($record->data);
		$flag = true;
	}	
	if(function_exists(dpm)) dpm($ms[0],"ms[0]");
	if($flag){
		$output .= output_weibo($ms);
	}
	 //Final output string		
  return $output;
}
/**
 * vp_center_public not login page.
 */
function sina_vp_user_timeline(){
  return  $output = sina_vp_linked_info().page_weibo_add().user_timeline_page();
}
/**
 * vp_public_timeline.
 */
function sina_vp_public_timeline(){
	$output=drupal_get_form('weibo_page_default');
	return sina_vp_linked_info().$output;//.page_weibo_add()
}