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
    $output = ' <div class="ad">
    <a href="javascript:void(0);" class="ups login_link"> 
    <img src="'.drupal_get_path('theme', 'vp').'/images/ad01.jpg" width="628" height="120" />
    </a></div>
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

    $output .= '<ul>';
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
		$output .= '<li>
      <div class="wb_01 bor_top">
        <div class="fb_time">'.sina_vp_time_format($node->created).'</div>';
    $output .='
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
      $output .= '</ul>';
       $output .= '<ul><script type="text/javascript">
				function slideDown(ContId,interTime,speed)
				{
					var d = document,isFirst=true;
					var contOut = d.getElementById(ContId);
					var ul = contOut.getElementsByTagName("ul")[0];		
					var intval,timeOut,sInterval;
					function run(){
						
						clearInterval(intval);			
						intval = setInterval(function(){				
							var li = contOut.getElementsByTagName("li");
							var liNum = li.length;								
							var tempLi = li[liNum-1].cloneNode(true);
							var tHeight = li[liNum-1].offsetHeight;				
							ul.insertBefore(tempLi,li[0]);
							ul.style.top= -tHeight+"px";
							var runDown = function(){
								clearInterval(sInterval);
								sInterval = setInterval(function(){						
									var uTop = parseInt(ul.offsetTop);
									if(Math.abs(uTop)>1){
										var top = Math.abs(parseInt(ul.offsetTop));
										ul.style.top = -(top - Math.ceil(top/20))+"px";		
										//ul.style.top = -(top-1)+"px";								
									}else{
										ul.style.top = 0+"px";								
										clearInterval(sInterval);							
									}
								},speed);
								ul.removeChild(li[liNum]);						
							}
							runDown();
						},interTime);
					}
					run();
					function stop(){
						clearInterval(intval);			
					}
					contOut.onmouseover = function(){
						stop();			
					}
					contOut.onmouseout = function(){		
						run();			
					}
				}
				slideDown("outCont",5000,15);
				/*$(".login_link").click(function(){
					var logoin = $("#user-login-form").html();
					$("#user_login_re").show().html("<div><a id =close_login >关闭</a>"+logoin+"</div>"); 
					$("#close_login").one("click", function(){$("#user_login_re").slideUp();});
				});*/
				
			</script>	
			<style type="text/css">				
				#outCont{position:relative;overflow:hidden;margin:0 auto;}
				#outCont ul{position:absolute;} 
			</style>
    </div>';
		//登录弹出框
drupal_add_css(drupal_get_path('theme', 'vp').'/dialog.css');
drupal_add_js(drupal_get_path('theme', 'vp').'/dialog.js');
jquery_ui_add('ui.dialog'); 
    drupal_add_css("sites/all/libraries/jquery.ui/themes/base/ui.all.css");
    $output .= drupal_get_form('user_login');
    return $output;
  }
}
