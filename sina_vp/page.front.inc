<?php
// $Id$
function sina_vp_front_page()
{
  global $user;
  $roles = user_roles(TRUE);
  if( $user->uid )//用户已经登录
  { 
  	return '';
  }else{
    $output = '<div class="innav-pic"><a class="login_link" href="#"><img src="'.drupal_get_path('theme', 'vpj').'/images/1.jpg"></a></div>
    <div class="wblist pad10" id="outCont">';
   
			
			//滚动内容：6条卖家微博 3条一组，第一条为 活动，其他随机。
			$first_sql='SELECT node.nid AS nid, node.created AS node_created FROM node node  
			INNER JOIN users_roles users_roles ON node.uid = users_roles.uid 
			LEFT JOIN term_node term_node ON node.vid = term_node.vid 
			LEFT JOIN term_data term_data ON term_node.tid = term_data.tid 
			WHERE (users_roles.rid = 3) AND (node.status = 1) 
			AND (node.type in ("weibo")) AND ((term_data.name) = ("活动")) 
			ORDER BY node_created DESC LIMIT 0 , 1';//活动1条
			$second_sql='SELECT node.nid AS nid, node.created AS node_created FROM node node  
			INNER JOIN users_roles users_roles ON node.uid = users_roles.uid 
			LEFT JOIN term_node term_node ON node.vid = term_node.vid 
			LEFT JOIN term_data term_data ON term_node.tid = term_data.tid 
			WHERE (users_roles.rid = 5) AND (node.status = 1) 
			AND (node.type in ("weibo")) AND ((term_data.name) = ("真人秀")) 
			ORDER BY node_created DESC LIMIT 0 , 1';//真人秀1条
			$last_sql='SELECT node.nid AS nid, node.created AS node_created FROM node node  
			INNER JOIN users_roles users_roles ON node.uid = users_roles.uid 
			LEFT JOIN term_node term_node ON node.vid = term_node.vid 
			LEFT JOIN term_data term_data ON term_node.tid = term_data.tid 
			WHERE (users_roles.rid = 3) AND (node.status = 1) 
			AND (node.type in ("weibo")) AND ((term_data.name) != ("活动")) 
			ORDER BY node_created DESC LIMIT 0 , 9';//其他活动4条
								
			$show_nid[]=db_result(db_query($first_sql));
			$show_nid[]=db_result(db_query($second_sql));
			$result = db_query($last_sql);
			while($value = db_fetch_array($result)){
				 $show_nid[]=$value['nid'];
			}
		drupal_add_js(drupal_get_path('theme', 'vpj').'/page-front.js');
		drupal_add_css(drupal_get_path('theme', 'vpj').'/page-front.css');
    $output .= '<ul id="sliderlck" class="sliderlck">';
		if($show_nid)
			foreach($show_nid as $nid) {
				$node = node_load($nid);
				$account = user_load($node->uid);
				$taxonomys = $node->taxonomy;
				foreach($taxonomys as $key=>$obj){
					if($obj->vid==2){ //微博频道
						$taxonomy_name=$obj->name;
						$taxonomy_id = 		$obj->tid;	
					}
		}
				if(!$account->picture)
					$account->picture = variable_get(user_picture_default, '/sites/default/files/users/0.gif');
					imagecache_generate_image('small_picture',  $user->picture);
		$output .= '<li >
      <div class="vp-conetnt">
					<div class="vp-conetnt-left">
						<div class="vp-user-pic login_link">
							'.
            theme('imagecache', 'small_picture', $account->picture, '', '', array('class'=>'front-user-picture')
            ,'#'
            ,array('html'=>'true','attributes'=>array('class'=>'login_link')))
            .'
						</div>
					</div>
					<div class="vp-conetnt-right">
						<div class="content-info-top">
							<div class="content-user vp-float">
								<span class="username login_link"><a href="javascript:void(0)">'.$account->name.'</a></span>
								<span class="vp-term">['.$taxonomy_name.']</span>
								<span class="vp-front-time">'.sina_vp_time_format($node->created).'</span>
							</div>        
						</div>
						<div class="vp-context">
							<div class="vp-context-body">'.$node->title.'</div>						
							<div class="clear"></div>					
						</div>                
					</div>
					<div class="clear"></div>
				</div>'; 
    $output2 .='
        <div class="fb_nr">
          <div class="fb_yh">
          	<a href="javascript:void(0);" class="login_link">
          	'.$account->name.'</a> <cite>['.$taxonomy_name.']</cite></div>
          <div class="wb_nr">
            <div class="userPic"><a href="javascript:void(0);" class="login_link"> '.
            theme('imagecache', 'small_picture', $account->picture, $account->name, $account->name, array('class'=>'front-user-picture'))
            .'</a></div>
            <div class="msgBox">
              <div class="msgCnt">'.$node->title.'</div>
            </div>
          </div>
        </div>
      </div></li>';
      }
      $output .= '</ul><script type="text/javascript">
													new sliderlck({id:"sliderlck"});
											</script>';
       $output .= '</div>';
		//登录弹出框
		//drupal_add_js(drupal_get_path('module', 'sina_vp_theme').'/plugins/jquery-1.7.1.min.js');
		//drupal_add_css(drupal_get_path('module', 'sina_vp_theme').'/plugins/bootstrap/css/bootstrap.css');
		//drupal_add_js(drupal_get_path('module', 'sina_vp_theme').'/plugins/bootstrap/js/bootstrap-modal.js');
	  //drupal_add_js(drupal_get_path('module', 'sina_vp_theme').'/plugins/bootstrap/js/bootstrap-transtion.js');
		//滚动代码
		
		//drupal_add_css(drupal_get_path('module', 'sina_vp_theme').'/plugins/twitter/dialog.css');
		jquery_ui_add('ui.draggable');
    //drupal_add_css("sites/all/libraries/jquery.ui/themes/base/ui.all.css");
    $output .= '<div class="modal fade" id="loginModal" style="display: none;">
            <div class="modal-header">
            </div>
            <div class="modal-body">
            '.drupal_get_form('user_login').'
            </div>
          </div>';
		
    return $output;
  }
}
