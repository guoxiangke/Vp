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
		$text=sina_vp_url_format($weibo_obj->text);
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
/*
 * 获取新浪转发数 counts
 * @param $nid
 *   $node->id.
 * @return
 * 	未登录时随机10-100（其实不会显示的。）
 * */
function weibo_rt_counts($nid){
	global $user;
	$uid=$user->uid;
	$wid = sina_vp_get_wid($nid);
	if($uid!=0){
		$counts = sina_open_t_set_request('http://api.t.sina.com.cn/statuses/counts.json', array('ids'=>$wid));
		$counts = json_decode($counts->data);
		//dpm($counts,'$counts');
		if(!is_array($counts)){drupal_set_message('貌似您还没绑定微博，尽力为您拉取转发数目失败！意思是说转发数目是不可靠的，嘿嘿^_^','status');return rand(10, 100);}
		$comments =$counts[0]->comments;
		return $rt =$counts[0]->rt;
	}
	return rand(10, 100);
}
/**
 * Menu callback; Generate a listing of weibo nodes.
 * LIKE node_page_default IN node.module
 */
function weibo_page_default(&$form_state, $type = NULL) {
	global $user;
	if ($type) {
		$form['page_title'] = array('#value' =>$type);
		switch ($type) {
			case 'public_timeline':
				$sql="select * from {sina_vp_weibo2node} WHERE cid=0 AND wid<>0 AND is_deleted=0 ORDER BY id DESC";
				$count_query="select count(*) from {sina_vp_weibo2node} WHERE cid=0 AND is_deleted=0 AND wid<>0";
				break;
			case 'user_timeline':
				$sql="select * from {sina_vp_weibo2node} WHERE cid=0 AND is_deleted=0 AND uid=$user->uid AND wid<>0 ORDER BY id DESC";
				$count_query="select count(*) from {sina_vp_weibo2node} WHERE cid=0 AND is_deleted=0 AND uid=$user->uid AND wid<>0";
				break;
			case 'friend_timeline':
				//1.从新浪微博获取 friends
				//2.系统自带friendship
				$sql="select * from {sina_vp_weibo2node} WHERE cid=0 AND is_deleted=0 AND uid=$user->uid AND wid<>0 ORDER BY id DESC";
				$count_query="select count(*) from {sina_vp_weibo2node} WHERE cid=0 AND is_deleted=0 AND uid=$user->uid AND wid<>0";
				break;			
			default://默认 = public_timeline
				$sql="select * from {sina_vp_weibo2node} WHERE cid=0 AND wid<>0 AND is_deleted=0 ORDER BY id DESC";
				$count_query="select count(*) from {sina_vp_weibo2node} WHERE cid=0 AND is_deleted=0 AND wid<>0";
				break;
		}
	}
  $num_rows = FALSE;	
	$result = pager_query(db_rewrite_sql($sql),1, $element = 0, $count_query);
	while ($record = db_fetch_object($result)) {
			$weibo_obj=json_decode($record->data);
			dpm($weibo_obj,'$weibo_obj');
			$nid = $record->nid;
			$node = node_load($nid);
			//form format weibo_obj
			$created_at=strtotime($weibo_obj->created_at);
			$time_string=sina_vp_time_format($weibo_obj->created_at);
			$form['nid'][$nid] = array('#value' =>$nid);
			$id=number_format($weibo_obj->id, 0, '', '');
			$mid=$weibo_obj->mid;
			$form['wid'][$nid] = array('#value' =>$id);	
   		$form['created_at'][$nid] = array('#value' =>$time_string);
			$text=sina_vp_url_format($weibo_obj->text);//
			$form['text'][$nid] = array('#value' =>$text);
			$form['source'][$nid] = array('#value' =>$weibo_obj->source);
			$w_user=$weibo_obj->user;//: 作者信息
			$w_uid=$w_user->id;
			$form['w_uid'][$nid] = array('#value' =>$w_uid);
			$domain=empty($w_user->domain)?$w_uid:$w_user->domain;
			$form['domain'][$nid] = array('#value' =>$domain);
			$profile_image_url = $w_user->profile_image_url.'.jpg';
			$form['profile_image_url'][$nid] = array('#value' =>$profile_image_url);
			$name = empty($w_user->screen_name)?$w_user->name:$w_user->screen_name;
			$form['screen_name'][$nid] = array('#value' =>$name);
			
			$favorited=$weibo_obj->favorited;//: 是否已收藏
			$form['favorited'][$nid] = array('#value' =>$favorited);
			$thumbnail_pic=$weibo_obj->thumbnail_pic;//: 缩略图
			$form['thumbnail_pic'][$nid] = array('#value' =>$thumbnail_pic);
			$bmiddle_pic=$weibo_obj->bmiddle_pic;//: 中型图片
			$form['bmiddle_pic'][$nid] = array('#value' =>$bmiddle_pic);
			$original_pic=$weibo_obj->original_pic;//：原始图片
			$form['original_pic'][$nid] = array('#value' =>$original_pic);		
			$retweeted_status=$weibo_obj->retweeted_status;//: 转发的博文，内容为status，如果不是转发，则没有此字段
			$form['retweeted_status'][$nid] = array('#value' =>$retweeted_status);
			//注释从这里开始。
			if (user_access('create weibo content')) {
				$form['comments'][$nid]['re_comment'] = array(
				  '#type' => 'checkbox',
				  '#title' => t('转发').'('.weibo_rt_counts($nid).')',
				  '#ahah' => array(
				    'path' => 'vp/weibo_page_default/comment_ahah_callback/'.$nid, //含参数的调用。
				    'wrapper' => 'textfield-wrapper-'.$nid,
				    'effect' => 'fade',
						)
				);
				$form['comments'][$nid]['favorite'] = array(
				  '#title' => t('收藏').'(N)',
				  '#type' => 'markup',
		      '#value' => flag_create_link('bookmarks', $nid),
				);
			}
			if (user_access('post comments')) {
				$form['comments'][$nid]['add_comment'] = array(
				  '#type' => 'checkbox',
				  '#title' => t('评论').'('.$node->comment_count.')',
				  '#ahah' => array(
				    'path' => 'vp/weibo_page_default/comment_ahah_callback/'.$nid, //含参数的调用。
				    'wrapper' => 'textfield-wrapper-'.$nid,
				    'effect' => 'fade',
						)
				);
				
			}$form['comments'][$nid]['textfields'] = array(//$form['textfields'][$nid]['textfields']
			    '#title' => t('评论'),
			    '#prefix' => '<div id="textfield-wrapper-'.$nid.'">',
			    '#suffix' => '</div>',
			    '#type' => 'fieldset',
			    '#description' => t('This is where we put automatically generated textfields'),
			  );
			//dpm($form_state,'001-->$form_state');//表单状态，点击后可以查看下与之前的不同。
			if (!empty($form_state['values']['add_comment'])) {				
				module_load_include('inc', 'sina_vp', 'comment.pages');
				$form['textfields'][$nid]['textfields']['makeup'] = array(
		      '#type' => 'markup',
		      '#value' => comment_reply($node, $pid = NULL),//drupal_get_form('comment_form',$form_state,$array=array('nid'=>$nid)),
		    );				
				//comment_form(&$form_state, $edit, $title = NULL);
				//drupal_retrieve_form($form_id='comment_form', &$form_state);
				//form_builder($form_id='comment_form', $form_comment, &$form_state);
				//	drupal_get_form('comment_form',$form_state,$array=array('nid'=>$nid));
				//comment_reply($node, $pid = NULL)//drupal_get_form('comment_form')
				//drupal_retrieve_form('comment_form',&$form_state);
		  }
			if (!empty($form_state['values']['re_comment'])) {				
				$form['textfields'][$nid]['textfields']['makeup'] = array(
		      '#type' => 'markup',
		      '#value' => page_weibo_add(),
		    );
			}
			//dpm($form,'$form000000');
	}
 	$form['pager'] = array('#value' => theme('pager', NULL, 1, 0));
	$form['#theme'] = 'weibo_page_default';
  return $form;
}
/**
 * 主题化 微博列表.
 */
function theme_weibo_page_default($form){
	drupal_set_title(drupal_render($form['page_title']));
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
		//if($nid==0) continue; >????
		$node = node_load($nid);
		$output .= '<p class="info W_linkb W_textb">
									<span>

									<i class="W_vline">|</i>';
		//$output .= flag_create_link('bookmarks', $node->nid);
		$output .= drupal_render($form['comments'][$node->nid]).'('.$node->comment_count.')';
		$output .= '<i class="W_vline">|</i>';
		$output .= '</span>
									<a node-type="feed_list_item_date" class="date" date="'.$form['created_at'][$key]['#value'].'"   href="?q=/node/'.$node->nid.'">'.$form['created_at'][$key]['#value'].'</a> 来自'.$form['source'][$key]['#value'].'
								</p>
								<div class="repeat" style="display:none;" node-type="feed_list_repeat"></div>';
								
		$output.='</dd>';
		$output.=drupal_render($form['textfields'][$node->nid]['textfields']['makeup']);
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
  return  $output;
}
/**
 * give info who is not login call from sina_open.
 */
function sina_vp_linked_info(){
	$linked_info= NULL;
	global $user;
	$token = sina_open_get_access_token($user->uid);
	if(empty($token)){
		module_load_include('inc', 'sina_open', 'sina_open.pages');
		$linked_info = sina_open_page_config(user_load($user->id));
	}
	return $linked_info;
}/**
 * vp_center_public not login page.
 */
function sina_vp_user_timeline(){
	$output =sina_vp_linked_info();
	if(!isset($output)){
		$output.= page_weibo_add();		
	}
	drupal_set_title(t('user_timeline'));
	//$output.= user_timeline_page();
	$type = 'user_timeline';
	$output.= drupal_get_form('weibo_page_default',$type);
	return $output;
}
/**
 * vp_public_timeline.
 */
function sina_vp_public_timeline(){
	$output =sina_vp_linked_info();
	if(!isset($output)){
		$output.= page_weibo_add();		
	}
	$type = 'public_timeline';
	$output.= drupal_get_form('weibo_page_default',$type);
	return $output;//
}
/**
 * sina_vp_friend_timeline.
 */
function sina_vp_friend_timeline(){
	$output =sina_vp_linked_info();
	if(!isset($output)){
		$output.= page_weibo_add();		
	}
	$type = 'friend_timeline';
	$output.= drupal_get_form('weibo_page_default',$type);
	return $output;
}



/**
 * sina_vp_new_content page.
 */
function sina_vp_new_content($type=NULL,$nid=NULL){
	$output =sina_vp_linked_info();
	if(!isset($output)){
		$output.= page_weibo_add();		
	}
	drupal_set_title('发布'.$type.'信息');
	if($type=='Transfer' || $type=='Show'){
		if(!node_load($nid)){
			return t('E11081810:链接请求错误，请到指定页面发布微博^_^');
		}
	}
	return $output;
}
/**
 * sina_vp_new_content page.
 */
function sina_vp_new($tid=NULL,$nid=NULL){
	$output =sina_vp_linked_info();
	if(!$term = taxonomy_get_term($tid)){
			drupal_set_message('链接请求错误，请到指定页面发布微博^_^','error');
			return t('链接请求错误，请到指定页面发布微博^_^！');
		}
	if(!isset($output)){
		$output.= page_weibo_add();		
	}
	if(taxonomy_get_term($tid))drupal_set_title('发布#'.$term->name.'#信息');
	//当Buyer 发布信息时，需要基于节点的
	$term_parent = taxonomy_get_parents($tid);
	foreach($term_parent as $key=>$term){
		if($term->name == 'Buyer'){
			if(!node_load($nid)){
				return t('E11081810:链接请求错误，请到指选定一个微博来发布信息^_^');
			}
		}
	}
	return $output;
}
/**
 * sina_vp_new_forward page.转发
 */
function sina_vp_new_forward($tid=NULL,$nid=NULL){
	if(!$node=node_load($nid)){
		return t('E11090955:链接请求错误(请选择一条微博)，请到指定页面转发微博^_^');
	}
	foreach($node->taxonomy as $node_tid=>$value){
		if($node_tid!=$tid){
			return t('E11090956:链接请求错误（转发的微博类型不对），请到指定页面转发微博^_^');
		}
	};
	$output =sina_vp_linked_info();
	if(!isset($output)){
		$output.= page_weibo_add();		
	}
	drupal_set_title('转发分享#'.$type.'#【'.$node->title.'】');
	return $output;
}
/**
 * Menu callback; Generate a listing of promoted nodes.
 * copy form node_page_default() in node.module
 */
function sina_VpCenter_page($tid = NULL) {
	$sql='SELECT n.nid, n.sticky, n.created FROM {node} n WHERE n.type in ("weibo") AND n.promote = 1 AND n.status = 1 ORDER BY n.sticky DESC, n.created DESC';
	if($tid){
		$sql='SELECT n.nid, n.sticky, n.created FROM {node} n 
				 INNER JOIN term_node term_node ON n.vid = term_node.vid
				 WHERE (n.type in ("weibo") AND n.promote = 1 AND n.status = 1) AND (term_node.tid = '.$tid.') ORDER BY n.sticky DESC, n.created DESC';
  }
  $result = pager_query(db_rewrite_sql($sql), variable_get('default_nodes_main', 10));
  while ($node = db_fetch_object($result)) {
  	$node = node_load($node->nid);
  	if($pre_node && $pre_node->uid == $node->uid){
			$hidden_array[]=$node->nid;			
			if(in_array($node->nid, $hidden_array)){							
					$show_more_nid[$pre_node->nid] = $pre_node->nid;
			}
  		$hidden[$pre_node->nid][] = array('nid'=>$node->nid);//该节点隐藏下的节点个数
  	}else{
  		$pre_node = $node;
  	}
  }
	//dpm($show_more_nid,'显示的节点');
	//dpm($hidden,'hidden的节点');  
	$result = pager_query(db_rewrite_sql($sql), variable_get('default_nodes_main', 10));
  $output = '';
  $num_rows = FALSE;
	while ($node = db_fetch_object($result)) {
    $node = node_load($node->nid);
  	if($pre_node && $pre_node->uid == $node->uid){			 
			$output .= '<div class="hidden" id="node-'.$node->nid.'"></div>';	
  	}else{
	    $output .= node_view($node,  $teaser = TRUE, $page = FALSE, $links = TRUE);
  		if(in_array($node->nid, $show_more_nid)){							
					$output .= '('.count($hidden[$node->nid]).')more...';
			}
  	}	
    $num_rows = TRUE;	 
		$pre_node = $node;	
  }
  if ($num_rows) {
    $feed_url = url('rss.xml', array('absolute' => TRUE));
    drupal_add_feed($feed_url, variable_get('site_name', 'Drupal') .' '. t('RSS'));
    $output .= theme('pager', NULL, variable_get('default_nodes_main', 10));
  }else{
		global $user;
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
		}else{
			drupal_set_message('Access deny1!请申请买家或卖家后才能访问！','error');
			return t('Access deny1!请申请买家或卖家后才能访问！');
		}
    $output = '<div id="first-time">'. $default_message .'</div>';
  }
  if(isset($tid)){
  	$term_name = taxonomy_get_term($tid)->name;
		drupal_set_title(t($term_name));
		if(user_access("add new Seller info") && $_GET['q']!='Show' && $_GET['q']!='Transfer'){
			$output ="<p>点击<a href='?q=new/$tid'>这里</a>发布#".t($term_name)."#信息</p>".$output;
		}
  }
	if(is_null($tid)|| $tid=='Show' ||$tid=='Transfer' ){
		 return $output;		  		
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
function sina_UCenter_page($tid = NULL) {
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
	if(in_array('Buyer',$user->roles)){ //对于买家显示其粉丝和他自己的信息
		$sql = 'SELECT n.nid, n.sticky, n.created FROM {node} n ';
		$sql_where = 'WHERE n.type in ("weibo") AND n.promote = 1 AND n.status = 1 ';
		$sql_where_add = 'AND n.uid in('.$follows_uid.','.$user->uid.') ';
		$sql_order = 'ORDER BY n.sticky DESC, n.created DESC ';
		$sql =$sql.$sql_where.$sql_where_add .$sql_order;
	}elseif(in_array('Seller',$user->roles)){	//对于卖家只显示他自己的
		$sql = 'SELECT n.nid, n.sticky, n.created FROM {node} n ';
		$sql_where = 'WHERE n.type in ("weibo") AND n.promote = 1 AND n.status = 1 ';
		$sql_where_add = 'AND n.uid in('.$user->uid.') ';
		$sql_order = 'ORDER BY n.sticky DESC, n.created DESC ';
		$sql =$sql.$sql_where.$sql_where_add .$sql_order;
	}else{
			drupal_set_message('Access deny!请申请买家或卖家后才能访问！','error');
			drupal_goto('user/'.$user->uid.'/apply_for_role');//.'&amp;destination='.urlencode($_GET['q'])
		}
  if($tid){
  	//SELECT n.nid, n.sticky, n.created FROM {node} n WHERE n.type in ("weibo") AND n.promote = 1 AND n.status = 1 ORDER BY n.sticky DESC, n.created DESC	
  	$sql='SELECT n.nid, n.sticky, n.created
				 FROM {node} n 
				 INNER JOIN term_node term_node ON n.vid = term_node.vid
				 WHERE (n.type in ("weibo") AND n.promote = 1 AND n.status = 1) AND (term_node.tid = '.$tid.')';
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
		}else{
			drupal_set_message('Access deny!请申请买家或卖家后才能访问本页！','error');
			drupal_goto('user/'.$user->uid.'/apply_for_role');
			//return t('Access deny!请申请买家或卖家后才能访问本页！');
		}
    $output = '<div id="first-time">'. $default_message .'</div>';
  }
  if(isset($tid)){
  	$term_name = taxonomy_get_term($tid)->name;
		drupal_set_title(t($term_name));
		if(user_access("add new Seller info")){
			$output ="<p>点击<a href='?q=new/$tid'>这里</a>发布#".t($term_name)."#信息</p>".$output;
		}
  }
	if(is_null($tid)|| $tid=='Show' ||$tid=='Transfer' ){
		 return $output;		  		
	}
  return $output;
}