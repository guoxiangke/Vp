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
		drupal_add_js(drupal_get_path('module', 'sina_vp') .'/js/weibo-add.js');
		drupal_add_css(drupal_get_path('module', 'sina_vp') .'/css/weibo-add.css','module','screen');
		
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
								<p node-type="feed_list_">
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
	$result = pager_query(db_rewrite_sql($sql),20, $element = 0, $count_query);
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
		      //'#value' => flag_create_link('bookmarks', $nid),
		      '#value' =>'收藏',
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
			theme();
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
 * 微博发布页面 sina_vp_new content page. 
 * @param $tid
 * @param $nid  
 */
function sina_vp_new($tid=NULL,$nid=NULL){
	if(!$term = taxonomy_get_term($tid)){
			drupal_set_message('链接请求错误，请到指定页面发布微博^_^','error');
			return t('链接请求错误，请到指定页面发布微博^_^！');
		}
	$temp = sina_vp_linked_info();
	if(isset($temp)){
		$output = '<div id="blind_t_info">您还未绑定新浪微博帐号，暂不可同步到新浪。'.l('立即绑定','sina_open/t_login').'</div>';		
	}
	$output.= page_weibo_add();	
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
	$node = node_load($node->nid);    
 
	//$account = user_load($node->uid);//自己不可以转发自己的。
	$taxonomys = $node->taxonomy;
	foreach($taxonomys as $key=>$obj){
		if($obj->vid==2){//v2 == 微博类型
			$taxonomy_name=$obj->name;
			$taxonomy_id = 		$obj->tid;	
		}
	}
	if($taxonomy_id!=$tid){
		return t('E11090956:链接请求错误（转发的微博类型不对），请到指定页面转发微博^_^');
	};
	/*未绑定微博，不可以。。。
	 **/
	 $temp = sina_vp_linked_info();
	if(isset($temp)){
		drupal_set_message('您还未绑定微博帐号，暂不可同步！立即绑定'.l('新浪微博','sina_open/t_login'),'warning');		
	}
	$output = page_weibo_add();
	drupal_set_title('分享['.$taxonomy_name.']');
	return theme('status_messages').node_view($node,  $teaser = TRUE, $page = FALSE, $links = TRUE).$output;
}

/**
 * sina_vp_new_forward page.转发
 */
function sina_vp_new_forward_ajax($tid=NULL,$nid=NULL) {// node_add4_vp
	 if(!$node=node_load($nid)){
	 		drupal_set_message('链接请求错误(请选择一条微博)，请到指定页面转发微博^_^','warning');
			return drupal_json(array('status' => TRUE, 'data' =>theme('status_messages')));
		}
		$node = node_load($node->nid);    
	 
		//自己不可以转发自己的。
		$taxonomys = $node->taxonomy;
		foreach($taxonomys as $key=>$obj){
			if($obj->vid==2){//v2 == 微博类型
				$taxonomy_name=$obj->name;
				$taxonomy_id = 		$obj->tid;	
			}
		}
		if($taxonomy_id!=$tid){
			drupal_set_message('链接请求错误(转发的微博类型不对)，请到指定页面转发微博^_^','warning');
			return drupal_json(array('status' => TRUE, 'data' =>theme('status_messages')));
		};
		/*未绑定微博，不可以。。。
		 **/
		$temp = sina_vp_linked_info();
		if(isset($temp)){
			drupal_set_message('您还未绑定新浪微博帐号，暂不可同步到新浪。'.l('立即绑定','sina_open/t_login'),'warning');					
		}
		$output.= page_weibo_add();
   return drupal_json(array('status' => TRUE, 'data' =>theme('status_messages').page_weibo_add()));
}
/**
 * 折叠/隐藏ahah加载节点form
 */
function collapse_node_form(&$form_state, $hidden_array,$parent_nid){ 
	$form['show_node_checkbox']= array(
	  '#type' => 'checkbox',
	  '#title' => '<a onclick="">展开/折叠('.count($hidden_array).')条</a>',
		'#description' => '更多...[title余下的博文概览]去掉折叠js+css点击后隐藏',
	  '#ahah' => array(
	    'path' => 'ahah_callback/collapse_node_view', //./$parent_nid[0]含参数的调用。%node_load
	    'wrapper' => 'hidden-node-'.$parent_nid[0],
	    'effect' => 'fade',
		)
	);
	if($hidden_array){
		foreach($hidden_array as $array){
			$hidden_array_nid[] = $array['nid'];
		}		
	}//dpm(implode(',', $hidden_array_nid),'$hidden_array_nid');
	$form['send_ahah_array']=array(
          '#value' => implode(',', $hidden_array_nid),
          '#type'=>'hidden',
          );
	$form['show_node_area']= array(
          '#prefix' => '<div class="hidden_node" id="hidden-node-'.$parent_nid[0].'">',
          '#type'=>'item',
          '#suffix' => '</div>',
          );
	return $form;
}
/**
 * ahah 显示折叠/隐藏的节点 collapsible
 */
function  collapse_node_view(){
	$output = '';
	if($_POST['show_node_checkbox'] && !empty($_POST['send_ahah_array'])){
		$hidden_array_nid=explode($delimiter=',', $string=$_POST['send_ahah_array']);	
		foreach($hidden_array_nid as $key=>$nid){
			$output .= node_view(node_load($nid),1);
		}
	}
	drupal_json(array('status' => TRUE, 'data' =>theme('status_messages').$output));
}
/**
 * ahah页底加载更多微博form
 */
function show_more_form(&$form_state, $unoutputed){ 
	$form['show_more_checkbox']= array(
	  '#type' => 'checkbox',
	  '#title' => '<a onclick="">更多...余下('.count($unoutputed).')条</a>',
		'#description' => '更多...',
	  '#ahah' => array(
	    'path' => 'ahah_callback/show_more', //./$parent_nid[0]含参数的调用。%node_load
	    'wrapper' => 'show-more',
	    'effect' => 'fade',
		)
	);
	if($unoutputed){
		foreach($unoutputed as $nid){
			$show_more_nid[] = $nid;
		}		
	}//dpm(implode(',', $hidden_array_nid),'$hidden_array_nid');
	$form['send_ahah_array']=array(
          '#value' => implode(',', $show_more_nid),
          '#type'=>'hidden',
          );
	$form['show_node_area']= array(
          '#prefix' => '<div class="show-more" id="show-more">',
          '#type'=>'item',
          '#suffix' => '</div>',
          );
	return $form;
}
/**
 * ahah 显示show_more 页底加载更多的节点
 */
function show_more_node_view(){
	$output = '';
	if($_POST['show_more_checkbox'] && !empty($_POST['send_ahah_array'])){
		$hidden_array_nid=explode($delimiter=',', $string=$_POST['send_ahah_array']);	
		foreach($hidden_array_nid as $key=>$nid){
			$output .= node_view(node_load($nid),1);
		}
	}
	drupal_json(array('status' => TRUE, 'data' =>theme('status_messages').$output));
}
/**
 * 店铺列表
 * /VCenter
 */
function sina_VCenter_page($type = 'all') {
	//$items['VCenter/Sub_most'] = array(//Subscribe to a maximum of 订阅最多
	//$items['VCenter/New'] = array(//最新加入
	//$items['VCenter/Active'] = array(//最新更新
	switch ($type) {
		case 'Sub_most':	//订阅最多
			//订阅最多
			$sql='SELECT `requestee_id` FROM `user_relationships` where `rtid`=2  group by `requestee_id` order by count(1) desc limit 0,30';
			$results = db_query($sql);
			$count =30;
			$output = '<div class="dpbody-right-content VCenter-Sub_most">
					    <div class="dpright-content-top">
						    <div class="dpright-content-title">喜欢最多</div>
								<div class="dpright-content-right">
									<input type="checkbox" class="dpfxk" />
									<p>全选</p>
									<input type="button" class="dpdybtn"/>
									<div class="clear"></div>
								</div>
							<div class="clear"></div>
						  </div>
						  <div class="dpright-content-inner">';
			while($row = db_fetch_array($results)){
				$account = user_load($row['requestee_id']);		
				if($account->picture==''){$account->picture='sites/default/files/users/0.gif';}
				$output.='<div class="dpdianpu-content odd">
									<div class="dpdianpu-pic">'.
									l(theme('imagecache', '65x65', $account->picture, $account->name, $account->name, array('class'=>'vp-user-picture')),"UCenter/0/$account->uid",array('html'=>TRUE,'attributes'=>array('class'=>'Buyer-user-link')))
									.'</div>
									<div class="dpdianpu-xx">
										<p class="dpdpname">'.l($account->name,"UCenter/0/$account->uid").'</p>
										<p class="dpdpgg">'.truncate_utf8($account->signature, 55, $wordsafe = false, $dots = TRUE).'</p>
										<input type="checkbox" checked="checked"/>
									</div>
									<div class="dpdianpu-dy">
										<p>'.user_relationships_load(array("requestee_id" => $account->uid),array("count" => TRUE)).'</p>
										<p>已订阅</p>
									</div>						
								</div>';
				$count--;
			}
			while ($count--) {
				$output.='<div class="dpdianpu-content odd">
									<div class="dpdianpu-pic"><img src="/'.path_to_theme().'/images/3.jpg" alt="" width="65" height="65"/></div>
									<div class="dpdianpu-xx">
										<p class="dpdpname">小琳琳店铺</p>
										<p class="dpdpgg">主打针织袖点点广告</p>
										<input type="checkbox" checked="checked"/>
									</div>
									<div class="dpdianpu-dy">
										<p>232323</p>
										<p>已订阅</p>
									</div>
								</div>';
			}
			$output.='<div class="clear"></div></div>';
			$output.='<div class="clear"></div>					
					<div class="dpvp-more">'.l('订阅最多>>','VCenter/Sub_most').'</div>
				</div>';
			
			break;
		case 'New'://最新加入
			$sql='select u.uid uid from users u  INNER JOIN users_roles users_roles ON u.uid = users_roles.uid
	 			WHERE (users_roles.rid = 3) AND (u.status <> 0) order by created desc limit 0,30';
			$results = db_query($sql);
			$count =30;
			$output.='<div class="dpbody-right-content VCenter-New">
					    <div class="dpright-content-top">
						    <div class="dpright-content-title">最新加入</div>
								<div class="dpright-content-right">
									<input type="checkbox" class="dpfxk" />
									<p>全选</p>
									<input type="button" class="dpdybtn"/>
									<div class="clear"></div>
								</div>
							<div class="clear"></div>
						  </div>
						  <div class="dpright-content-inner">';
			while($row = db_fetch_array($results)){
				$account = user_load($row['requestee_id']);		
				if($account->picture==''){$account->picture='sites/default/files/users/0.gif';}
				$output.='<div class="dpdianpu-content odd">
									<div class="dpdianpu-pic">'.
									l(theme('imagecache', '65x65', $account->picture, $account->name, $account->name, array('class'=>'vp-user-picture')),"UCenter/0/$account->uid",array('html'=>TRUE,'attributes'=>array('class'=>'Buyer-user-link')))
									.'</div>
									<div class="dpdianpu-xx">
										<p class="dpdpname">'.l($account->name,"UCenter/0/$account->uid").'</p>
										<p class="dpdpgg">'.truncate_utf8($account->signature, 55, $wordsafe = false, $dots = TRUE).'</p>
										<input type="checkbox" checked="checked"/>
									</div>
									<div class="dpdianpu-dy">
										<p>'.user_relationships_load(array("requestee_id" => $account->uid),array("count" => TRUE)).'</p>
										<p>已订阅</p>
									</div>						
								</div>';
				$count--;
			}
			while ($count--) {
				$output.='<div class="dpdianpu-content odd">
									<div class="dpdianpu-pic"><img src="/'.path_to_theme().'/images/3.jpg" alt="" width="65" height="65"/></div>
									<div class="dpdianpu-xx">
										<p class="dpdpname">小琳琳店铺</p>
										<p class="dpdpgg">主打针织袖点点广告</p>
										<input type="checkbox" checked="checked"/>
									</div>
									<div class="dpdianpu-dy">
										<p>232323</p>
										<p>已订阅</p>
									</div>
								</div>';
			}
			$output.='<div class="clear"></div></div>';
			$output.='<div class="clear"></div>
					<div class="dpvp-more">'.l('最近更新>>','VCenter/Active').'</div>
				</div>';
			break;
		case 'Active'://最新更新
			$sql='select n.uid uid from node n  INNER JOIN users_roles users_roles ON n.uid = users_roles.uid
 WHERE (users_roles.rid = 3)  group by n.uid order by count(1) desc limit 0,30';
			$results = db_query($sql);
			$count =30;
			$output.='<div class="dpbody-right-content VCenter-Active">
					    <div class="dpright-content-top">
						    <div class="dpright-content-title">最近更新</div>
								<div class="dpright-content-right">
									<input type="checkbox" class="dpfxk" />
									<p>全选</p>
									<input type="button" class="dpdybtn"/>
									<div class="clear"></div>
								</div>
							<div class="clear"></div>
						  </div>
						  <div class="dpright-content-inner">';
			while($row = db_fetch_array($results)){
				$account = user_load($row['requestee_id']);		
				if($account->picture==''){$account->picture='sites/default/files/users/0.gif';}
				$output.='<div class="dpdianpu-content '.($count%2?'odd':'even').'">
									<div class="dpdianpu-pic">'.
									l(theme('imagecache', '65x65', $account->picture, $account->name, $account->name, array('class'=>'vp-user-picture')),"UCenter/0/$account->uid",array('html'=>TRUE,'attributes'=>array('class'=>'Buyer-user-link')))
									.'</div>
									<div class="dpdianpu-xx">
										<p class="dpdpname">'.l($account->name,"UCenter/0/$account->uid").'</p>
										<p class="dpdpgg">'.truncate_utf8($account->signature, 55, $wordsafe = false, $dots = TRUE).'</p>
										<input type="checkbox" checked="checked"/>
									</div>
									<div class="dpdianpu-dy">
										<p>'.user_relationships_load(array("requestee_id" => $account->uid),array("count" => TRUE)).'</p>
										<p>已订阅</p>
									</div>						
								</div>';
				$count--;
			}
			while ($count) {
				$output.='<div class="dpdianpu-content '.($count%2?'odd':'even').'">
									<div class="dpdianpu-pic"><img src="/'.path_to_theme().'/images/3.jpg" alt="" width="65" height="65"/></div>
									<div class="dpdianpu-xx">
										<p class="dpdpname">小琳琳店铺</p>
										<p class="dpdpgg">主打针织袖点点广告</p>
										<input type="checkbox" checked="checked"/>
									</div>
									<div class="dpdianpu-dy">
										<p>232323</p>
										<p>已订阅</p>
									</div>
								</div>';
				$count--;
			}
			$output.='<div class="clear"></div></div>';
			$output.='  <div class="clear"></div>
					<div class="dpvp-more">'.l('最近更新>>','VCenter/Active').'</div>
				</div>';
			break;
		default:
			//订阅最多
			$sql='SELECT `requestee_id` FROM `user_relationships` where `rtid`=2  group by `requestee_id` order by count(1) desc limit 0,6';
			$results = db_query($sql);
			$count =6;
			$output.='<div class="dpbody-right-content VCenter-Sub_most">
					    <div class="dpright-content-top">
						    <div class="dpright-content-title">喜欢最多</div>
								<div class="dpright-content-right">
									<input type="checkbox" class="dpfxk" />
									<p>全选</p>
									<input type="button" class="dpdybtn"/>
									<div class="clear"></div>
								</div>
							<div class="clear"></div>
						  </div>
						  <div class="dpright-content-inner">';
			while($row = db_fetch_array($results)){
				$account = user_load($row['requestee_id']);		
				if($account->picture==''){$account->picture='sites/default/files/users/0.gif';}
				$output.='<div class="dpdianpu-content odd">
									<div class="dpdianpu-pic">'.
									l(theme('imagecache', '65x65', $account->picture, $account->name, $account->name, array('class'=>'vp-user-picture')),"UCenter/0/$account->uid",array('html'=>TRUE,'attributes'=>array('class'=>'Buyer-user-link')))
									.'</div>
									<div class="dpdianpu-xx">
										<p class="dpdpname">'.l($account->name,"UCenter/0/$account->uid").'</p>
										<p class="dpdpgg">'.truncate_utf8($account->signature, 55, $wordsafe = false, $dots = TRUE).'</p>
										<input type="checkbox" checked="checked"/>
									</div>
									<div class="dpdianpu-dy">
										<p>'.user_relationships_load(array("requestee_id" => $account->uid),array("count" => TRUE)).'</p>
										<p>已订阅</p>
									</div>						
								</div>';
				$count--;
			}
			while ($count--) {
				$output.='<div class="dpdianpu-content odd">
									<div class="dpdianpu-pic"><img src="/'.path_to_theme().'/images/3.jpg" alt="" width="65" height="65"/></div>
									<div class="dpdianpu-xx">
										<p class="dpdpname">小琳琳店铺</p>
										<p class="dpdpgg">主打针织袖点点广告</p>
										<input type="checkbox" checked="checked"/>
									</div>
									<div class="dpdianpu-dy">
										<p>232323</p>
										<p>已订阅</p>
									</div>
								</div>';
			}
			$output.='<div class="clear"></div></div>';
			$output.='<div class="clear"></div>					
					<div class="dpvp-more">'.l('订阅最多>>','VCenter/Sub_most').'</div>
				</div>';
			//最新加入
				$sql='select u.uid uid from users u  INNER JOIN users_roles users_roles ON u.uid = users_roles.uid
	 			WHERE (users_roles.rid = 3) AND (u.status <> 0) order by created desc limit 0,6';
			$results = db_query($sql);
			$count =6;
			$output.='<div class="dpbody-right-content VCenter-New">
					    <div class="dpright-content-top">
						    <div class="dpright-content-title">最新加入</div>
								<div class="dpright-content-right">
									<input type="checkbox" class="dpfxk" />
									<p>全选</p>
									<input type="button" class="dpdybtn"/>
									<div class="clear"></div>
								</div>
							<div class="clear"></div>
						  </div>
						  <div class="dpright-content-inner">';
			while($row = db_fetch_array($results)){
				$account = user_load($row['requestee_id']);		
				if($account->picture==''){$account->picture='sites/default/files/users/0.gif';}
				$output.='<div class="dpdianpu-content odd">
									<div class="dpdianpu-pic">'.
									l(theme('imagecache', '65x65', $account->picture, $account->name, $account->name, array('class'=>'vp-user-picture')),"UCenter/0/$account->uid",array('html'=>TRUE,'attributes'=>array('class'=>'Buyer-user-link')))
									.'</div>
									<div class="dpdianpu-xx">
										<p class="dpdpname">'.l($account->name,"UCenter/0/$account->uid").'</p>
										<p class="dpdpgg">'.truncate_utf8($account->signature, 55, $wordsafe = false, $dots = TRUE).'</p>
										<input type="checkbox" checked="checked"/>
									</div>
									<div class="dpdianpu-dy">
										<p>'.user_relationships_load(array("requestee_id" => $account->uid),array("count" => TRUE)).'</p>
										<p>已订阅</p>
									</div>						
								</div>';
				$count--;
			}
			while ($count--) {
				$output.='<div class="dpdianpu-content odd">
									<div class="dpdianpu-pic"><img src="/'.path_to_theme().'/images/3.jpg" alt="" width="65" height="65"/></div>
									<div class="dpdianpu-xx">
										<p class="dpdpname">小琳琳店铺</p>
										<p class="dpdpgg">主打针织袖点点广告</p>
										<input type="checkbox" checked="checked"/>
									</div>
									<div class="dpdianpu-dy">
										<p>232323</p>
										<p>已订阅</p>
									</div>
								</div>';
			}
			$output.='<div class="clear"></div></div>';
			$output.='<div class="clear"></div>
					<div class="dpvp-more">'.l('最近更新>>','VCenter/Active').'</div>
				</div>';
			//最近更新
			$sql='select n.uid uid from node n  INNER JOIN users_roles users_roles ON n.uid = users_roles.uid
 WHERE (users_roles.rid = 3)  group by n.uid order by count(1) desc limit 0,6';
			$results = db_query($sql);
			$count =6;
			$output.='<div class="dpbody-right-content VCenter-Active">
					    <div class="dpright-content-top">
						    <div class="dpright-content-title">最近更新</div>
								<div class="dpright-content-right">
									<input type="checkbox" class="dpfxk" />
									<p>全选</p>
									<input type="button" class="dpdybtn"/>
									<div class="clear"></div>
								</div>
							<div class="clear"></div>
						  </div>
						  <div class="dpright-content-inner">';
			while($row = db_fetch_array($results)){
				$account = user_load($row['requestee_id']);		
				if($account->picture==''){$account->picture='sites/default/files/users/0.gif';}
				$output.='<div class="dpdianpu-content '.($count%2?'odd':'even').'">
									<div class="dpdianpu-pic">'.
									l(theme('imagecache', '65x65', $account->picture, $account->name, $account->name, array('class'=>'vp-user-picture')),"UCenter/0/$account->uid",array('html'=>TRUE,'attributes'=>array('class'=>'Buyer-user-link')))
									.'</div>
									<div class="dpdianpu-xx">
										<p class="dpdpname">'.l($account->name,"UCenter/0/$account->uid").'</p>
										<p class="dpdpgg">'.truncate_utf8($account->signature, 55, $wordsafe = false, $dots = TRUE).'</p>
										<input type="checkbox" checked="checked"/>
									</div>
									<div class="dpdianpu-dy">
										<p>'.user_relationships_load(array("requestee_id" => $account->uid),array("count" => TRUE)).'</p>
										<p>已订阅</p>
									</div>						
								</div>';
				$count--;
			}
			while ($count) {
				$output.='<div class="dpdianpu-content '.($count%2?'odd':'even').'">
									<div class="dpdianpu-pic"><img src="/'.path_to_theme().'/images/3.jpg" alt="" width="65" height="65"/></div>
									<div class="dpdianpu-xx">
										<p class="dpdpname">小琳琳店铺</p>
										<p class="dpdpgg">主打针织袖点点广告</p>
										<input type="checkbox" checked="checked"/>
									</div>
									<div class="dpdianpu-dy">
										<p>232323</p>
										<p>已订阅</p>
									</div>
								</div>';
				$count--;
			}
			$output.='<div class="clear"></div></div>';
			$output.='  <div class="clear"></div>
					<div class="dpvp-more">'.l('最近更新>>','VCenter/Active').'</div>
				</div>';
			break;
	}
	
	
	return $output;
	 
}
/**
 * Menu callback; Generate a listing of promoted nodes.
 * copy form node_page_default() in node.module
 */
function sina_VpCenter_page($tid = NULL) {
	$sql='SELECT n.nid, n.sticky, n.created FROM {node} n WHERE n.type in ("weibo")  AND n.uid > 0 AND n.promote = 1 AND n.status = 1 ORDER BY n.sticky DESC, n.created DESC';
	if($tid){
		$sql='SELECT n.nid, n.sticky, n.created FROM {node} n 
				 INNER JOIN term_node term_node ON n.vid = term_node.vid
				 WHERE (n.type in ("weibo") AND n.promote = 1 AND n.uid > 0 AND n.status = 1) AND (term_node.tid = '.$tid.') ORDER BY n.sticky DESC, n.created DESC';
  }
  $result = pager_query(db_rewrite_sql($sql), variable_get('default_nodes_main', 10));
    $output = '';
	$first_show_num = 15-1;
 /* while ($node = db_fetch_object($result)) {    
		if($num_rows>$first_show_num){
			$unoutputed[]=$node->nid;
			continue;
		}
    $output .= node_view(node_load($node->nid), 1);
    $num_rows ++;		
  }
  if(is_array($unoutputed)){
  	dpm($unoutputed,'$unoutputed');
		$output .= drupal_get_form('show_more_form',$unoutputed);
  } 
 $pattern = '/#([^\\#|.]+)#/';
 $pattern_at = '/@([^\\@|\s{1}]+)/';
 $subject = '#请在这##里输##入自定##义话题# @admin';
 $counts = preg_match_all($pattern_at, $subject,$match);
 dpm($match,$counts);
 $subject = str_replace(' ', '', $subject);
 $counts = preg_match_all($pattern, $subject,$match);
 dpm($match,$counts);*/
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
  $counts = db_result(db_query(db_rewrite_sql($sql), $args));
	//dpm($show_more_nid,'显示的节点');
	//dpm($hidden,'hidden的节点');  pager_query_for_vp($query, $limit = 10, $element = 0, $count_query = NULL,$count)
	$result = pager_query(db_rewrite_sql($sql), variable_get('default_nodes_main', 10));
  $output = '';
  //$num_rows = FALSE;
  $num_rows = 0;
	while ($node = db_fetch_object($result)) {    
		if($num_rows>$first_show_num){
			$unoutputed[]=$node->nid;
			continue;
		}
		$node = node_load($node->nid);
  	if($pre_node && $pre_node->uid == $node->uid){			 
			//$output .= '<div class="hidden_node" id="node-'.$node->nid.'"><a>Display</a></div>';
  	}else{
	    $output .= node_view($node,  $teaser = TRUE, $page = FALSE, $links = TRUE);
			$num_rows ++;
  		if($show_more_nid && in_array($node->nid, $show_more_nid)){									
					$output .= drupal_get_form('collapse_node_form',$hidden[$node->nid],$parent_nid=array($node->nid));
					//$output .= '<div class="hidden_node" id="hidden-node-'.$node->nid.'"><a>Display</a></div>';
			}
  	}	
    //$num_rows = TRUE;    
		$pre_node = $node;	
  }
  if(is_array($unoutputed)){
  	//dpm($unoutputed,'$unoutputed');
		$output .= drupal_get_form('show_more_form',$unoutputed);
  } 
	
  if ($num_rows) {
    $feed_url = url('rss.xml', array('absolute' => TRUE));
    drupal_add_feed($feed_url, variable_get('site_name', 'Drupal') .' '. t('RSS'));
		drupal_add_css(drupal_get_path('module', 'sina_vp') .'/css/scroll.css');
		drupal_add_js(drupal_get_path('module', 'sina_vp') .'/js/scroll.js');
		
		drupal_add_js(drupal_get_path('module', 'sina_vp') .'/js/gotop.js');
		drupal_add_css(drupal_get_path('module', 'sina_vp') .'/css/gotop.css');
		$output .= '<a id="go-top" href="#top">
<span class="top-btn"><span class="sj">♦</span><span class="fk">▐</span>返回顶部</span>
</a>';
		//.$_REQUEST["REQUEST_URI"].
		//$output .= '<div class="hidden" id="showmore_node" >显示更多</div>';
		//dpm($_SERVER,$_REQUEST );//$_SERVER['HTTP_HOST']
		$output .= '<div class="showmore" id="showmore" onclick="insertcode();"><a class="handle" href="javascript:show()">显示更多结果</a></div>';
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
			drupal_set_message('权限不够!请申请买家或卖家角色！','error');
			drupal_goto('user/'.$user->uid.'/apply_for_role');
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
 * 买家中心，根据角色Seller Buyer而定.
 * copy form sina_vp_center_page_default() sina_vp_Comments_page
 */
function sina_UCenter_page($tid = NULL,$uid=NUll) {
	global $user;//买家个人访问的自己的页面
	if($user->uid==0){
		drupal_goto($_SERVER['HTTP_REFERER']);//.'&amp;destination='.urlencode($_GET['q'])
		return '请登录后访问！';
	}
	$account = $user;
	if(is_numeric($uid)){//其他人访问的买家页面  
		$bfw_user=user_load($uid);//被访问的user bfw
		drupal_set_title($bfw_user->name.'的微铺个人中心');
		if($bfw_user->uid){$account = $bfw_user;}else{return '您访问的用户不存在';}
		if(in_array('Seller',$bfw_user->roles)){return '';}//访问卖家信息，不需要下面的操作了～
	}
	$array=user_relationships_load(array("user" => $account->uid),array("sort" => 'requestee_id'));
	//$follows_uid ="'1'"; 
	foreach($array as $key=>$value){
		$follows_uid[] =$key; //取得粉丝用户uid
	}
	if(!count($array)){
		//$follows_uid[] ='1';//没有任何订阅时，默认推送管理员的信息 more....
	}
	$follows_uid[] =$account->uid; //他自己的微博
	$follows_uid = array_unique($follows_uid);
	$follows_uid = implode(",",$follows_uid);

		$sql = 'SELECT n.nid, n.sticky, n.created FROM {node} n ';
		$sql_where = ' WHERE n.type in ("weibo") AND n.promote = 1 AND n.status = 1 ';
		$sql_where_add = ' AND n.uid in('.$follows_uid.') ';
		$sql_order = ' ORDER BY n.sticky DESC, n.created DESC ';
  { //tid 0-6 all
 		if(is_null($tid)||$tid=='0'){ $tid='(1,2,3,4,5,6)';}else{$tid='('.$tid.')';}
  	$sql_join =' INNER JOIN term_node term_node ON n.vid = term_node.vid';
		$sql_where_add .=' AND (term_node.tid in '.$tid.')';
		$sql =$sql.$sql_join.$sql_where.$sql_where_add.$sql_order;
  }
	if($bfw_user&&in_array('Buyer',$bfw_user->roles)){//查看某买家的页面
		$sql = 'SELECT n.nid, n.sticky, n.created FROM {node} n ';
		$sql_where = ' WHERE n.type in ("weibo") AND n.promote = 1 AND n.status = 1 ';
		$sql_where_add = 'AND n.uid in('.$bfw_user->uid.') ';
		//if(is_null($tid)||$tid=='0'){ $tid='(1,2,3,4,5,6)';}else{$tid='('.$tid.')';}
  	$sql_join =' INNER JOIN term_node term_node ON n.vid = term_node.vid';
		$sql_where_add .=' AND (term_node.tid in '.$tid.')';
		$sql_order = ' ORDER BY n.sticky DESC, n.created DESC ';
		$sql =$sql.$sql_join.$sql_where.$sql_where_add.$sql_order;
		//if($tid=='0')$sql =$sql.$sql_where.$sql_where_add.$sql_order;
	}else{
			//drupal_set_message('Access deny error:1201091025!','error');//请申请买家或卖家后才能访问个人主页！ 您访问的用户不存在
			//drupal_goto('user/'.$user->uid.'/apply_for_role');//.'&amp;destination='.urlencode($_GET['q'])
		}	
  $result = pager_query(db_rewrite_sql($sql), variable_get('default_nodes_main', 5));//

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
  }else {
  	if(in_array('Seller',$user->roles)){		
	    $default_message = t('<h1 class="title">作为一个有身份的卖家是不会偷懒的！</h1><p>Please follow these steps to set up and start using your website:</p>');
	    $weibo_type_array=array('Product','Activity','Special','Sale');//,'Transfer','Show'
			$default_message .= '<ol>';	
			foreach($weibo_type_array as $type){
		    $default_message .= '<li>'. t('<strong>发布<a href="@VpCenter">'.t($type).'</a>信息</strong>', array('@VpCenter' => url($type))) .'</li>';
		    }
			$default_message .= '</ol>';  			
		 }elseif(in_array('Buyer',$user->roles)){			//if(in_array('Buyer',$user->roles))
	  	//一键关注js css
	  	if(arg(1)==0){
				$default_message = '<div class="wb_02 bor_top" id="node-275">
													  <div class="fb_time">
													     <p>15秒前</p></div>
													  <div class="fb_nr2">
													    <div class="fb_yh"><a href="/?q=UCenter/0/1">微铺街</a> <cite>[提醒]</cite></div>
													    <div class="wb_nr">
													      <div class="userPic"><a href="/?q=UCenter/0/1"> <img width="50" height="50" src="sites/default/files/users/picture-3.jpg"></a></div>
													      <div class="msgBox2">
													        <div class="msgCnt">
													        	<h4>欢迎您来到微铺街，您订阅的用户信息少的可怜，我们推荐您<a href="javascript:void(0)" class="focus_recommend">一键关注</a>更多街友！        	</h2>
													        	
													             <span><button class="btn success focus_recommend">一键关注</button></span>
													            
																	</div>	         
													      </div></div>
													  </div> </div>';
				$default_message.='<div id="sina_vp_recommend" >'.drupal_get_form('sina_vp_recommend').'</div>';
			}else{
				$default_message = '暂无信息！';
			}							
		}
	if(!regcode_user_load()){
			drupal_set_message('权限不够!请输入邀请码获取买家或卖家角色才能正常浏览！如果已经输入邀请码，请忽略此提醒...','warning');
			//drupal_goto('user/'.$user->uid.'/apply_for_role');
		}
    $output = '<div id="first-time">'. $default_message .'</div>';
  }
  if(isset($tid)){
  	$term_name = taxonomy_get_term($tid)->name;		
		if(user_access("add new Seller info")){
			//$output ="<p>点击<a href='?q=new/$tid'>这里</a>发布#".t($term_name)."#信息</p>".$output;
		}
  }
	if(is_null($tid)|| $tid=='Show' ||$tid=='Transfer' ){
		 return $output;		  		
	}
  return $output;
}
/**
 * 真人秀5
 * 转让潮6
 */
function sina_vp_BuyCenter($tid = NULL) {
	$sql='SELECT n.nid, n.sticky, n.created
				 FROM {node} n 
				 INNER JOIN term_node term_node ON n.vid = term_node.vid
				 WHERE (n.type in ("weibo") AND n.promote = 1 AND n.status = 1) AND (term_node.tid = '.$tid.') ORDER BY n.sticky DESC, n.created DESC';
  
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
	if($sina_vp_type=='Transfer' || $sina_vp_type=='Show' ||is_null($sina_vp_type)){
		 return $output;		  		
	}
	if(user_access("add new ".strtolower($sina_vp_type))){
		$output ="<p>点击<a href='?q=vp/add/$sina_vp_type'>这里</a>发布".t($sina_vp_type)."博文</p>".$output;
	}
  return $output;
}


/**
 * Perform a paged database query.
 *
 * Use this function when doing select queries you wish to be able to page. The
 * pager uses LIMIT-based queries to fetch only the records required to render a
 * certain page. However, it has to learn the total number of records returned
 * by the query to compute the number of pages (the number of records / records
 * per page). This is done by inserting "COUNT(*)" in the original query. For
 * example, the query "SELECT nid, type FROM node WHERE status = '1' ORDER BY
 * sticky DESC, created DESC" would be rewritten to read "SELECT COUNT(*) FROM
 * node WHERE status = '1' ORDER BY sticky DESC, created DESC". Rewriting the
 * query is accomplished using a regular expression.
 *
 * Unfortunately, the rewrite rule does not always work as intended for queries
 * that already have a "COUNT(*)" or a "GROUP BY" clause, and possibly for
 * other complex queries. In those cases, you can optionally pass a query that
 * will be used to count the records.
 *
 * For example, if you want to page the query "SELECT COUNT(*), TYPE FROM node
 * GROUP BY TYPE", pager_query() would invoke the incorrect query "SELECT
 * COUNT(*) FROM node GROUP BY TYPE". So instead, you should pass "SELECT
 * COUNT(DISTINCT(TYPE)) FROM node" as the optional $count_query parameter.
 *
 * @param $query
 *   The SQL query that needs paging.
 * @param $limit
 *   The number of query results to display per page.
 * @param $element
 *   An optional integer to distinguish between multiple pagers on one page.
 * @param $count_query
 *   An SQL query used to count matching records.
 * @param ...
 *   A variable number of arguments which are substituted into the query (and
 *   the count query) using printf() syntax. Instead of a variable number of
 *   query arguments, you may also pass a single array containing the query
 *   arguments.
 * @return
 *   A database query result resource, or FALSE if the query was not executed
 *   correctly.
 * $count 重新分页。要删除 dale2012 03 12
 * @ingroup database
 */
function pager_query_for_vp($query, $limit = 10, $element = 0, $count_query = NULL,$count) {
  global $pager_page_array, $pager_total, $pager_total_items;
  $page = isset($_GET['page']) ? $_GET['page'] : '';

  // Substitute in query arguments.
  $args = func_get_args();
  $args = array_slice($args, 4);
  // Alternative syntax for '...'
  if (isset($args[0]) && is_array($args[0])) {
    $args = $args[0];
  }

  // Construct a count query if none was given.
  if (!isset($count_query)) {
    $count_query = preg_replace(array('/SELECT.*?FROM /As', '/ORDER BY .*/'), array('SELECT COUNT(*) FROM ', ''), $query);
  }

  // Convert comma-separated $page to an array, used by other functions.
  $pager_page_array = explode(',', $page);

  // We calculate the total of pages as ceil(items / limit).
  $pager_total_items[$element] = $count;//db_result(db_query($count_query, $args))
  $pager_total[$element] = ceil($pager_total_items[$element] / $limit);
  $pager_page_array[$element] = max(0, min((int)$pager_page_array[$element], ((int)$pager_total[$element]) - 1));
  return db_query_range($query, $args, $pager_page_array[$element] * $limit, $limit);
}

///////////主题化
function sina_vp_Activity(){return '详见page-Activity.tpl.php';}
/**
 * $plaza_type 广场类型  News 新品 
 * $child_type 子类型 most_favor喜欢最多  most_comments评论最多 recently_active最新更新
 * $uid 某个卖家的类型广场
 */
function sina_vp_plaza($plaza_type='Shows',$child_type='all',$account=null){
		$plaza_type_obj = vpj_get_terms_by_alias($plaza_type);
		$plaza_tid = $plaza_type_obj->tid; //分类id
		$plaza_tname = $plaza_type_obj->name;
		
		if($account&&!is_object($account))
		$account = user_load($account);
		{
			if($account&&in_array('Buyer', $account->roles)){
			drupal_set_message('您好像走叉道了，亲～');
			return '<div class="gcbody-right-content">
			    <div class="gcright-content-title">喜欢最多</div>
				<div class="gcright-content-inner">
				    <div class="gcbottom-pk-righ-body">			       
                       <div class="gcvp-body-bottom-pk-right-img">
                            <img width="200" height="200" src="images/3.jpg">
						    <div class="gctm-content"></div>
						    <div class="gctm-pic"><img width="35" height="35" src="images/5.jpg"></div>
						    <div class="gctm-size">人人微铺街</div>
                       </div>
                       <div class="gcvp-body-bottom-pk-right-xx">
                           <div class="gcpk-img-name">黑色修身好看经点的</div>
                           <div class="gcpk-img-zz">
								<span class="gcvp-loveit"><div class="gclovepic"><img src="images/xin.gif"></div><div class="gclovexh">(2)</div></span>
								<span class="gcplfx">评论(3)&nbsp;&nbsp;分享(2)</span>
								<div class="clear"></div>
                           </div>
                       </div>
                   </div>
				    <div class="gcbottom-pk-righ-body">			       
                       <div class="gcvp-body-bottom-pk-right-img">
                            <img width="200" height="200" src="images/3.jpg">
						    <div class="gctm-content"></div>
						    <div class="gctm-pic"><img width="35" height="35" src="images/5.jpg"></div>
						    <div class="gctm-size">人人微铺街</div>
                       </div>
                       <div class="gcvp-body-bottom-pk-right-xx">
                           <div class="gcpk-img-name">黑色修身好看经点的</div>
                           <div class="gcpk-img-zz">
								<span class="gcvp-loveit"><div class="gclovepic"><img src="images/xin.gif"></div><div class="gclovexh">(2)</div></span>
								<span class="gcplfx">评论(3)&nbsp;&nbsp;分享(2)</span>
								<div class="clear"></div>
                           </div>
                       </div>
                   </div>
				    <div class="gcbottom-pk-righ-body">			       
                       <div class="gcvp-body-bottom-pk-right-img">
                            <img width="200" height="200" src="images/3.jpg">
						    <div class="gctm-content"></div>
						    <div class="gctm-pic"><img width="35" height="35" src="images/5.jpg"></div>
						    <div class="gctm-size">人人微铺街</div>
                       </div>
                       <div class="gcvp-body-bottom-pk-right-xx">
                           <div class="gcpk-img-name">黑色修身好看经点的</div>
                           <div class="gcpk-img-zz">
								<span class="gcvp-loveit"><div class="gclovepic"><img src="images/xin.gif"></div><div class="gclovexh">(2)</div></span>
								<span class="gcplfx">评论(3)&nbsp;&nbsp;分享(2)</span>
								<div class="clear"></div>
                           </div>
                       </div>
                   </div>
				    <div class="gcbottom-pk-righ-body">			       
                       <div class="gcvp-body-bottom-pk-right-img">
                            <img width="200" height="200" src="images/3.jpg">
						    <div class="gctm-content"></div>
						    <div class="gctm-pic"><img width="35" height="35" src="images/5.jpg"></div>
						    <div class="gctm-size">人人微铺街</div>
                       </div>
                       <div class="gcvp-body-bottom-pk-right-xx">
                           <div class="gcpk-img-name">黑色修身好看经点的</div>
                           <div class="gcpk-img-zz">
								<span class="gcvp-loveit"><div class="gclovepic"><img src="images/xin.gif"></div><div class="gclovexh">(2)</div></span>
								<span class="gcplfx">评论(3)&nbsp;&nbsp;分享(2)</span>
								<div class="clear"></div>
                           </div>
                       </div>
					    <div class="gcfudiv">
					    <div class="gcfudiv-top"><input type="text" class="gcfudivtext"><input type="button" class="gcfudivbtn" value="点评"><div class="clear"></div></div>
					    <div class="gcfudiv-bottom"><img width="35" height="35" src="images/5.jpg"><p><span class="gcvp-name">小精灵：</span>我觉的点点衣服好看看年年</p><div class="clear"></div></div>			
					    </div>	
                   </div>
			   
                    <div class="gcbottom-pk-righ-body">			       
                       <div class="gcvp-body-bottom-pk-right-img">
                            <img width="200" height="200" src="images/3.jpg">
						    <div class="gctm-content"></div>
						    <div class="gctm-pic"><img width="35" height="35" src="images/5.jpg"></div>
						    <div class="gctm-size">人人微铺街</div>
                       </div>
                       <div class="gcvp-body-bottom-pk-right-xx">
                           <div class="gcpk-img-name">黑色修身好看经点的</div>
                           <div class="gcpk-img-zz">
								<span class="gcvp-loveit"><div class="gclovepic"><img src="images/xin.gif"></div><div class="gclovexh">(2)</div></span>
								<span class="gcplfx">评论(3)&nbsp;&nbsp;分享(2)</span>
								<div class="clear"></div>
                           </div>
                       </div>
                   </div>
				    <div class="gcbottom-pk-righ-body">			       
                       <div class="gcvp-body-bottom-pk-right-img">
                            <img width="200" height="200" src="images/3.jpg">
						    <div class="gctm-content"></div>
						    <div class="gctm-pic"><img width="35" height="35" src="images/5.jpg"></div>
						    <div class="gctm-size">人人微铺街</div>
                       </div>
                       <div class="gcvp-body-bottom-pk-right-xx">
                           <div class="gcpk-img-name">黑色修身好看经点的</div>
                           <div class="gcpk-img-zz">
								<span class="gcvp-loveit"><div class="gclovepic"><img src="images/xin.gif"></div><div class="gclovexh">(2)</div></span>
								<span class="gcplfx">评论(3)&nbsp;&nbsp;分享(2)</span>
								<div class="clear"></div>
                           </div>
                       </div>
                   </div>				   
				   <div class="clear"></div>
				</div>
				<div class="gcvp-more">更多&gt;&gt;</div>
			</div>';
			}
		}//只考虑卖家才有广场相关页面。 
				 //ALL News
				 if(!is_array($plaza_tname))
					switch ($child_type) {
						case 'recently_active':		// (users_roles.rid = 3) 	真人秀转让潮不必是卖家的。所有每一个都单独处理。	
								if($plaza_tname=='真人秀'||$plaza_tname=='转让潮'){
									$sql="SELECT node.nid AS nid, node.created AS node_created FROM node node 
										LEFT JOIN term_node term_node ON node.vid = term_node.vid 								
										WHERE (node.status = 1)
										AND (node.type in ('weibo')) AND ((term_node.tid) = $plaza_tid)
										ORDER BY node_created DESC";//活动30条										
									if($uid){ //某一个卖家的 XX页面
										$sql="SELECT node.nid AS nid, node.created AS node_created FROM node node 
										LEFT JOIN term_node term_node ON node.vid = term_node.vid 								
										WHERE node.uid = $uid AND (node.status = 1)
										AND (node.type in ('weibo')) AND ((term_node.tid) = $plaza_tid) 
										ORDER BY node_created DESC";
									}
								}else{
									$sql="SELECT node.nid AS nid, node.created AS node_created FROM node node 
										INNER JOIN users_roles users_roles ON node.uid = users_roles.uid 
										LEFT JOIN term_node term_node ON node.vid = term_node.vid 								
										WHERE (users_roles.rid = 3) AND (node.status = 1)
										AND (node.type in ('weibo')) AND ((term_node.tid) = $plaza_tid) 
										ORDER BY node_created DESC";										
									if($uid){ //某一个卖家的 XX页面
										$sql="SELECT node.nid AS nid, node.created AS node_created FROM node node 
										INNER JOIN users_roles users_roles ON node.uid = users_roles.uid 
										LEFT JOIN term_node term_node ON node.vid = term_node.vid 								
										WHERE (users_roles.rid = 3) AND node.uid = $uid AND (node.status = 1)
										AND (node.type in ('weibo')) AND ((term_node.tid) = $plaza_tid) 
										ORDER BY node_created DESC";
									}
								}//$plaza_title = '最近更新';	
								$results = pager_query($sql,2);
								$output = plaza_output($results,$child_type,$plaza_type,'最近更新');
							break;
						case 'most_comments':	
									if($plaza_tname=='真人秀'||$plaza_tname=='转让潮'){
										$sql="SELECT count(node.nid) as count_here, node.nid AS nid, node.created AS node_created  FROM node node  
										 
										LEFT JOIN term_node term_node ON node.vid = term_node.vid 								
										LEFT JOIN comments comments ON comments.nid = node.nid 
										WHERE (node.status = 1)
										AND (node.type in ('weibo')) AND ((term_node.tid) = $plaza_tid) 
										group by node.nid ORDER BY count_here DESC ,node_created DESC ";//活动30条
										if($uid){ //某一个卖家的 XX页面
											$sql="SELECT count(node.nid) as count_here, node.nid AS nid, node.created AS node_created  FROM node node  
											 
											LEFT JOIN term_node term_node ON node.vid = term_node.vid 								
											LEFT JOIN comments comments ON comments.nid = node.nid 
											WHERE  node.uid = $uid AND  (node.status = 1)
											AND (node.type in ('weibo')) AND ((term_node.tid) = $plaza_tid) 
											group by node.nid ORDER BY count_here DESC ,node_created DESC ";
										}
									}else{
										$sql="SELECT count(node.nid) as count_here, node.nid AS nid, node.created AS node_created FROM node node  
										INNER JOIN users_roles users_roles ON node.uid = users_roles.uid 
										LEFT JOIN term_node term_node ON node.vid = term_node.vid 								
										LEFT JOIN comments comments ON comments.nid = node.nid 
										WHERE (users_roles.rid = 3) AND (node.status = 1)
										AND (node.type in ('weibo')) AND ((term_node.tid) = $plaza_tid) 
										group by node.nid ORDER BY count_here DESC ,node_created DESC ";//活动30条
										if($uid){ //某一个卖家的 XX页面
											$sql="SELECT count(node.nid) as count_here, node.nid AS nid, node.created AS node_created FROM node node  
											INNER JOIN users_roles users_roles ON node.uid = users_roles.uid 
											LEFT JOIN term_node term_node ON node.vid = term_node.vid 								
											LEFT JOIN comments comments ON comments.nid = node.nid 
											WHERE  node.uid = $uid AND (users_roles.rid = 3) AND (node.status = 1)
											AND (node.type in ('weibo')) AND ((term_node.tid) = $plaza_tid) 
											group by node.nid ORDER BY count_here DESC";//活动30条
										}
									}//$plaza_title = '点评最多';
								$results = db_query($sql); ///pager_query()?????
								$output = plaza_output($results,$child_type,$plaza_type,'点评最多');
							break;
							case 'most_favor':		
								if($plaza_tname=='真人秀'||$plaza_tname=='转让潮'){														
									$sql="SELECT node.nid AS nid, node.created AS node_created ,node.uid AS node_uid ,coalesce(flag.count,0)  AS counts1 FROM node AS node
									LEFT JOIN flag_counts  as flag ON flag.content_id = node.nid 
									LEFT JOIN term_node term_node ON node.vid = term_node.vid  
									WHERE (node.status = 1)
									AND (node.type in ('weibo')) AND ((term_node.tid) = $plaza_tid) 
									ORDER BY counts1 desc ,node_created DESC";//活动30条															
									if($uid){ //某一个卖家的 XX页面
										$sql="SELECT node.nid AS nid, node.created AS node_created ,node.uid AS node_uid ,coalesce(flag.count,0)  AS counts1 FROM node AS node
										LEFT JOIN flag_counts  as flag ON flag.content_id = node.nid 
										LEFT JOIN term_node term_node ON node.vid = term_node.vid  
										WHERE  node.uid = $uid AND (node.status = 1)
										AND (node.type in ('weibo')) AND ((term_node.tid) = $plaza_tid) 
										ORDER BY counts1 desc ,node_created DESC";//活动30条
									}
								}else{					
									$sql="SELECT node.nid AS nid, node.created AS node_created ,node.uid AS node_uid ,coalesce(flag.count,0)  AS counts1 FROM node AS node
									LEFT JOIN flag_counts  as flag ON flag.content_id = node.nid  
									LEFT JOIN users_roles users_roles ON node.uid = users_roles.uid 
									LEFT JOIN term_node term_node ON node.vid = term_node.vid  
									WHERE (users_roles.rid = 3) AND (node.status = 1)
									AND (node.type in ('weibo')) AND ((term_node.tid) = $plaza_tid) 
									ORDER BY counts1 desc ,node_created DESC";
									if($uid){ //某一个卖家的 XX页面
										$sql="SELECT node.nid AS nid, node.created AS node_created ,node.uid AS node_uid ,coalesce(flag.count,0)  AS counts1 FROM node AS node
										INNER JOIN users_roles users_roles ON node.uid = users_roles.uid 
										LEFT JOIN flag_counts  as flag ON flag.content_id = node.nid 
										LEFT JOIN term_node term_node ON node.vid = term_node.vid  
										WHERE  node.uid = $uid AND (users_roles.rid = 3) AND (node.status = 1)
										AND (node.type in ('weibo')) AND ((term_node.tid) = $plaza_tid) 
										ORDER BY counts1 desc ,node_created DESC";
									}
								}//$plaza_title = '喜欢最多';	
								$results = pager_query($sql,2);	
								$output = plaza_output($results,$child_type,$plaza_type,'喜欢最多');
							break;
						default: //三个sql
								if($plaza_tname=='真人秀'||$plaza_tname=='转让潮'){
									$sql="SELECT node.nid AS nid, node.created AS node_created ,node.uid AS node_uid ,coalesce(flag.count,0)  AS counts1 FROM node AS node
									LEFT JOIN flag_counts  as flag ON flag.content_id = node.nid  
									LEFT JOIN term_node term_node ON node.vid = term_node.vid  								
									LEFT JOIN term_data term_data ON term_node.tid = term_data.tid 
									WHERE (node.status = 1)
									AND (node.type in ('weibo')) AND ((term_node.tid) = $plaza_tid) 
									ORDER BY counts1 desc ,node_created DESC LIMIT 0 , 6";//喜欢最多
									$results = db_query($sql);
									//$output = plaza_output($results,'喜欢最多');
									$output = plaza_output($results,$child_type='most_favor',$plaza_type,'喜欢最多');
									$sql="SELECT node.nid AS nid, node.created AS node_created ,node.uid AS node_uid ,coalesce(flag.count,0)  AS counts1 FROM node AS node
									LEFT JOIN flag_counts  as flag ON flag.content_id = node.nid  
									
									LEFT JOIN term_node term_node ON node.vid = term_node.vid  	
									WHERE (node.status = 1)
									AND (node.type in ('weibo')) AND ((term_node.tid) = $plaza_tid) 
									ORDER BY counts1 desc ,node_created DESC LIMIT 0 , 6";
									$results = db_query($sql);
									//$output .= plaza_output($results,'点评最多');
									$output .= plaza_output($results,$child_type='most_comments',$plaza_type,'点评最多');
									$sql="SELECT node.nid AS nid, node.created AS node_created FROM node node  
									
									LEFT JOIN term_node term_node ON node.vid = term_node.vid 
									LEFT JOIN term_data term_data ON term_node.tid = term_data.tid 
									WHERE (node.status = 1)
									AND (node.type in ('weibo')) AND ((term_node.tid) = $plaza_tid) 
									ORDER BY node_created DESC LIMIT 0 , 6";			
									$results = db_query($sql);
									//$output .= plaza_output($results,'最新更新');
									$output .= plaza_output($results,$child_type='recently_active',$plaza_type,'最新更新');
								}else{
									$sql="SELECT node.nid AS nid, node.created AS node_created ,node.uid AS node_uid ,coalesce(flag.count,0)  AS counts1 FROM node AS node
									LEFT JOIN flag_counts  as flag ON flag.content_id = node.nid  
									LEFT JOIN users_roles users_roles ON node.uid = users_roles.uid 
									LEFT JOIN term_node term_node ON node.vid = term_node.vid  								
									LEFT JOIN term_data term_data ON term_node.tid = term_data.tid 
									WHERE (users_roles.rid = 3) AND (node.status = 1)
									AND (node.type in ('weibo')) AND ((term_node.tid) = $plaza_tid) 
									ORDER BY counts1 desc ,node_created DESC LIMIT 0 , 10";
									$results = db_query($sql);
									//$output = plaza_output($results,'喜欢最多');
									$output = plaza_output($results,$child_type='most_favor',$plaza_type,'喜欢最多');
									$sql="SELECT node.nid AS nid, node.created AS node_created ,node.uid AS node_uid ,coalesce(flag.count,0)  AS counts1 FROM node AS node
									LEFT JOIN flag_counts  as flag ON flag.content_id = node.nid  
									LEFT JOIN users_roles users_roles ON node.uid = users_roles.uid 
									LEFT JOIN term_node term_node ON node.vid = term_node.vid  	
									WHERE (users_roles.rid = 3) AND (node.status = 1)
									AND (node.type in ('weibo')) AND ((term_node.tid) = $plaza_tid) 
									ORDER BY counts1 desc ,node_created DESC LIMIT 0 , 10";
									$results = db_query($sql);
									//$output .= plaza_output($results,'点评最多');
									$output .= plaza_output($results,$child_type='most_comments',$plaza_type,'点评最多');
									$sql3="SELECT node.nid AS nid, node.created AS node_created FROM node node  
									INNER JOIN users_roles users_roles ON node.uid = users_roles.uid 
									LEFT JOIN term_node term_node ON node.vid = term_node.vid 
									LEFT JOIN term_data term_data ON term_node.tid = term_data.tid 
									WHERE (users_roles.rid = 3) AND (node.status = 1)
									AND (node.type in ('weibo')) AND ((term_node.tid) = $plaza_tid) 
									ORDER BY node_created DESC LIMIT 0 , 10";				
									$results = db_query($sql);
									//$output .= plaza_output($results,'最新更新');
									$output .= plaza_output($results,$child_type='recently_active',$plaza_type,'最新更新');
								}					
							break;
					}
				//<div class="more right"><a href="#">更多</a></div>
	   
		return $output;
	//return '详见page-Activity.tpl.php';
}

function plaza_output($results,$child_type,$plaza_type,$plaza_title){
	
	while($row = db_fetch_array($results)){
		      $news_nids [] = $row['nid'];
		    }
				$count=0;
				$output = '
				<div class="gcbody-right-content">
			    <div class="gcright-content-title">'.$plaza_title.'</div>
					<div class="gcright-content-inner">';
				if($news_nids)
				foreach ($news_nids as $activity_nid) {
						if($count==	18)break;
						$activity_node = node_load($activity_nid);
						if($weibo_image_filepath=$activity_node->field_weibo_image['0']['filepath']){							
							$share_sql = "SELECT count(*) FROM  {sina_vp_weibo2node} WHERE  `zid` =$activity_node->nid AND  `cid` =0";
							$share_counts = db_result(db_query($share_sql));
							$count++;
							
							$activity_user = user_load($activity_node->uid);							
							imagecache_generate_image('w200', $weibo_image_filepath);	
							$output .= '<div class="gcbottom-pk-righ-body">			       
	                       <div class="gcvp-body-bottom-pk-right-img">
	                            '.theme('imagecache', 'w200',$weibo_image_filepath, '', '').'
							    <div class="gctm-content"></div>
							    <div class="gctm-pic">'.theme('imagecache', '35x35', $activity_user->picture, $activity_user->name, '').'</div>
							    <div class="gctm-size">'.$activity_node->name.'</div>
	                       </div>
	                       <div class="gcvp-body-bottom-pk-right-xx">
	                           <div class="gcpk-img-name">'.$activity_node->title.'</div>
	                           <div class="gcpk-img-zz">
									<span class="gcvp-loveit"><div class="gclovepic"><img src="/'.path_to_theme().'/images/xin.gif"></div>
									<div class="gclovexh">'.flag_create_link('bookmarks', $activity_node->nid).'</div></span>
									<span class="gcplfx">评论('.$activity_node->comment_count.')&nbsp;&nbsp;分享('.$share_counts.')</span>
									<div class="clear"></div>
	                           </div>
	                       </div>
	                 </div>';
						}
				}

		 if(!$news_nids){
	    	$output .= '<div class="SaleCenter_detail none_info">暂无内容！</div>';
	    }
  	$output .= '<div class="clear"></div>
  			</div>';
		if(is_null(arg(2))){
			$output .= '<div class="gcvp-more">'.l('更多>>','plaza/'.$plaza_type.'/'.$child_type).'</div>';
			}
		$output .= '</div>';
		$output .= theme('pager', NULL, 2);
		return $output;
}
