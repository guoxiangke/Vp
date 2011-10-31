<?php 
// $Id$
function myweibo_page(){
	global $user;
	module_load_include('php', 'sina', 'api/weibooauth');
	module_load_include('php', 'sina', 'api/config'); //WB_AKEY , WB_SKEY	
	$path = drupal_get_path('module', 'sina');
	drupal_add_css($path .  '/css/sina.user.page.css', 'module', 'screen');	
	//require_once($path . '/css/sina.user.page.css');
	//$vars['styles'] = drupal_get_css($path .  '/css/sina.user.page.css');
	$token = sina_open_get_access_token($user->uid);
	if(empty($token)){
		module_load_include('inc', 'sina_open', 'sina_open.pages');
		return sina_open_page_config(user_load($user->id));
	}
	$last_key[oauth_token]=$token->key;
	$last_key[oauth_token_secret]=$token->secret;
	$_SESSION['last_key'] = $last_key;

	$c = new WeiboClient( WB_AKEY , WB_SKEY ,$token->key ,$token->secret );
	
	$ms  = $c->public_timeline($count = 20 ); //$c->user_timeline( $page = 1 , $count = 20 , $uid_or_name = NULL , $since_id = NULL, $max_id = NULL); // done
	if(empty($token)){
		$ms  = $c->public_timeline($count = 2 ); // done
	}
	$me = $c->verify_credentials();
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
		dpm(__FILE__.__LINE__,'信息获取成功!');
		//_sina_weibo2node($result->data);
  } else {
    drupal_set_message('信息获取失败', 'error');
    return false;
  }
	$ms  = json_decode($result->data);
	dpm($ms,"ms");
	 //Final output string
    $output = '<div id="myweibo_page">';
    //first   

    foreach($ms as $weibo_obj){
		$created_at=strtotime($weibo_obj->created_at); //$created_at = strtotime($json->created_at); 
		//if(now()-$created_at) {$created_at_str}
		$id=number_format($weibo_obj->id, 0, '', '');//==dpm($weibo_obj->mid);
		$mid=$weibo_obj->mid;
		$text=$weibo_obj->text;
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
		/*$w_uname=$w_user->'';
		$w_url='';
		$w_u_url='';*/
		$profile_image_url = $w_user->profile_image_url.'.jpg';
		$w_user_url='http://weibo.com/';
		$w_user_url  .=empty($w_user->domain)?$w_uid:$w_user->domain;
		$name = empty($w_user->screen_name)?$w_user->name:$w_user->screen_name;
		if(!empty($w_user->verified)){$verified='<img class="small_icon vip_blue" title="新浪机构认证" src="http://img.t.sinajs.cn/t35/style/images/common/transparent.gif">';}
		
		$counts = sina_open_t_set_request('http://api.t.sina.com.cn/statuses/counts.json', array('ids'=>$id));
		$counts = json_decode($counts->data);
		$comments =$counts[0]->comments;
		$rt =$counts[0]->rt;
		$output .= '<li id="" class="">';
    	$output .= '<div class="head_pic">
						<a href="'.$w_user_url.'" uid="'.$w_uid.'" namecard="true" action-type="namecard" action-data="????">
						<img src="'.$profile_image_url.'" imagetype="head" uid="'.$w_uid.'" title="'.$w_user->screen_name.'">
						</a>
					</div>';
		$output .= '<div class="MIB_feed_c">
						<p type="1" mid="'.$mid.'" class="sms">
							<a title="" href="'.$w_user_url.'" uid="'.$w_uid.'" namecard="true" action-type="namecard" 	action-data="">'.$name.$verified.'</a>
							：'.$text.'
						</p>
					</div>';
		$output .= '<div id="prev_'.$mid.'" class="feed_preview hide"></div>'; //预览评论区
		$output .= '<div class="feed_att MIB_linkbl MIB_txtbl">
						<div class="lf">
							<cite>
								<a onclick=";" href="http://weibo.com/??/xmOBMz7OZ">
								<strong date="'.$created_at.'">'.date('H:m',$created_at).'
								</strong></a>
							</cite>
							<strong lang="CL1006">来自</strong>
							<cite>'.$source.'</cite>
						</div>
						<div class="rt">
							<!-- 转发 -->
							<a action-data="??" action-type="forward" onclick="GB_SUDA._S_uaTrack();return false;" href="javascript:void(0);">
								<strong lang="CD0023">转发</strong>
								<strong type="rttCount" rid="'.$mid.'" id="num_'.$mid.'">('.$comments.')</strong>
							</a>
							<!-- 转发 -->
							<span class="MIB_line_l">|</span>
							<a onclick="App.addfavorite_miniblog();" href="javascript:void(0);"><strong lang="CL1003">收藏</strong></a>
							<!-- 评论 -->
							<span class="MIB_line_l">|</span>
							<a onclick="GB_SUDA._S_uaTrack(\'weibo_transmit\',\'origin_comment\');scope.loadCommentByRid(1618051664, \'miniblog2\', \'新浪微博\', \'3354596508542223\', \'\', \'\', \'\', 1, 0, 1);" href="javascript:void(0);" id="_comment_count_miniblog2_3354596508542223">
								<strong lang="CL1004">评论</strong>
								<strong type="commtCount" rid="3354596508542223">('.$rt.')</strong>
							</a>
							<!-- 评论 -->							 
						</div>
					</div>';
			$output .= '<div id="_comment_list_miniblog2_3354377297415764"></div>';	
		$output .= '</li>';	
    }
	
	$output .= '</div>';
  return $output;//'<pre>'.print_r($ms,true);//'<p>'.t('The page_example provides two pages, "simple" and "arguments". The <a href="@simple_link">simple page</a> just returns a string for display. The <a href="@arguments_link">arguments page</a> takes two arguments and displays them, as in @arguments_link', array('@simple_link' => url('examples/page_example/simple', array('absolute' => TRUE)), '@arguments_link' => url('examples/page_example/arguments/23/56', array('absolute' => TRUE)))).'<p>';
	
}
function customer_page() {
    global $user;
    module_load_include('inc', 'erp', 'erp.order');

    $path = drupal_get_path('module', 'erp');


    drupal_add_css($path . '/css/customer.page.css', 'module', 'screen');
    drupal_add_css($path . '/css/jquery.tablesorter.css', 'module', 'screen');
    drupal_add_css($path . '/css/order_contract.css', 'module', 'screen');
    drupal_add_css($path . '/css/shipment.css', 'module', 'screen');

    $jquery_ui_path = drupal_get_path('module', 'jquery_ui');
    drupal_add_css($jquery_ui_path . '/jquery.ui/themes/redmond/redmond.css', 'module', 'screen');
    jquery_ui_add(array('ui.datepicker', 'ui.draggable', 'ui.resizable', 'ui.dialog', 'effects.blind', 'effects.clip'));
//jquery_ui_add ??? output is noting.
    drupal_add_js($path . '/js/jquery.tablesorter.js', 'module', 'header');
    drupal_add_js($path . '/js/jquery.tablePager.js', 'module', 'header');
    drupal_add_js($path . '/js/common.js', 'module', 'header');
    drupal_add_js($path . '/js/customer.page.js', 'module', 'header');


    $destination = drupal_get_destination();//output of $destination is :destination=customer
    //Final output string
    $output = '<div id="customer-page">';


    //first
    $output .= '<div class="first-sidebar">';

    //Block-latest confirmed order 最新确认订单, 需要客户确认订单
    $result = db_query("SELECT * FROM {flooring_orders} WHERE status IN('unconfirm', 'customerconfirm') AND uid = '%d'ORDER BY updatetime DESC LIMIT 5", $user->uid);
    $orders_header = array(
        array('data' => t('Order no.'), 'class' => 'row-order-noW'),
        array('data' => t('Update time'), 'class' => 'row-confirm-time'),
        array('data' => t('Order status'), 'class' => 'row-status'),
        array('data' => t('Operating'), 'class' => 'row-operations')
    );

    $rows = array();
    while ($order = db_fetch_object($result)) {
        if ($order->status == 'customerconfirm') {
            $orderstatus = '<em class="icon icon-change"><span>changed</span></em>';
            $operations = l(t('Check changes'), 'order/' . $order->id . '/confirm-form', array('query' => $destination, 'attributes' => array('class' => 'show-confirm-form')));
        } else {
            $orderstatus = '';
            $operations = t('Waiting confirm');
        }
        $row = array();
        $row[] = array(
            'data' => l($order->order_no, 'order/' . $order->id, array('query' => $destination, 'attributes' => array('class' => 'show-order-info'), '' )) . $orderstatus,
            'class' => 'row-order-no'
        );

        $row[] = empty($order->updatetime) ? format_date($order->createtime, 'custom', 'Y/m/d') : format_date($order->updatetime, 'custom', 'Y/m/d');

        $row[] = _order_status_output($order->status);
        $row[] = array(
            'data' => $operations,
            'class' => 'row-operations'
        );
        $rows[] = $row;
    }
    $output .= '<div class="latest-confirmed-order">';
    $output .= '<h3><em>' . l(t('More orders'), 'customer/order') . '</em>' . t('Unconfirmed order') . '</h3>';
    if (count($rows) > 0) {
        $output .= '<div class="content">';
        $output .= theme('table', $orders_header, $rows, array('id' => 'latest-confirmed-table', 'class' => 'tablesorter'));
    } else {
        $output .= '<div class="no-content">';
        $output .= t('No posts available.');
    }
    $output .= '</div>';
    $output .= '</div>';

    //贸生产中订单
    $result = db_query("SELECT * FROM {flooring_orders} WHERE status IN ('confirmed', 'shipment') AND completetime = '' AND uid = '%d'ORDER BY createtime DESC LIMIT 5", $user->uid);
    $orders_header = array(
        array('data' => t('Order no.'), 'class' => 'row-order-noW'),
        array('data' => t('Produced/Total'), 'class' => 'gtr'),
        array('data' => t('Completion date'), 'class' => 'row-completetime'),
        array('data' => t('Delivery date'), 'class' => 'row-delivery-date')
    );

    $rows = array();
    while ($order = db_fetch_object($result)) {
        //销售计划状态判断
        switch (TRUE) {
            //完成
            case!empty($order->completetime):
                $due_type = 'complete';
                break;
            //要求完成时间小于预计完成时间
            //要求完成时间小于当前时间
            case $order->estimated_time > $order->delivery_date:
            case $order->delivery_date < time():
                $due_type = 'overdue';
                break;
            case ($order->delivery_date > $order->estimated_time) &&
            ($order->delivery_date - $order->estimated_time < 3600 * 24 * 7) && empty($order->completetime) :
            case empty($order->estimated_time) && ($order->delivery_date - time() < 3600 * 24 * 7):
                $due_type = 'due';
                break;
            default:
                $due_type = 'normal';
                break;
        }
        $row = array();
        $row[] = array(
            'data' => l($order->order_no, 'order/' . $order->id, array('query' => $destination, 'attributes' => array('class' => 'show-order-info'))),
            'class' => 'row-order-noW'
        );
        //$row[] = t('产品: @product - 数量: @quantity(托)', array('@product' => erp_get_product_name($order->pid), '@quantity' => $order->quantity));
        $row[] = array('data' => _order_produced_quantity($order->id) . '/' . _order_product_quantity($order->id), 'class' => 'gtr');
        $estimated_time = empty($order->estimated_time) ? '-' : format_date($order->estimated_time, 'custom', 'Y/m/d');
        $row[] = array('data' => $estimated_time, 'class' => 'row-completetime');
        $row[] = array('data' => format_date($order->delivery_date, 'custom', 'Y/m/d'), 'class' => 'row-delivery-date');
        $rows[] = array('data'=> $row, 'class' => 'row-'.$due_type);
    }
    $output .= '<div class="contract-list">';
    $output .= '<h3><em>' . l(t('More orders'), 'customer/order') . '</em>' . t('Order in production') . '</h3>';
    if ( count($rows) > 0) {
        $output .= '<div class="content">';
        $output .= theme('table', $orders_header, $rows, array('id' => 'contract-table', 'class' => 'tablesorter'));
    } else {
        $output .= '<div class="no-content">';
        $output .= t('No posts available.');
    }
    $output .= '</div>';
    $output .= '</div>';

    $sql = "SELECT *
          FROM {flooring_shipments}
           RIGHT JOIN {flooring_shipments WHERE type = 1 ORDER BY create_time DESC LIMIT 5";

    //最新收到发货合同
    $shipment_contracts_result = db_query("SELECT * FROM {flooring_shipments} WHERE type = 1 AND uid = $user->uid ORDER BY create_time DESC LIMIT 5");
    $shipment_contracts_header = array(
        array('data' => t('Shipment contract no.'), 'class' => 'row-order-noW'),
        array('data' => t('Place of delivery'), 'class' => 'row-delivery-address'),
        array('data' => t('Time of delivery'), 'class' => 'row-delivery-date'),
        array('data' => t('ETA'), 'class' => 'row-shipment-num'),
    );
    $rows = array();
    if ($shipment_contracts_result) {

    }
    while ($shipment_contract = db_fetch_object($shipment_contracts_result)) {
        $row = array();
        $row[] = array(
            'data' => l($shipment_contract->shipment_number, 'shipment/' . $shipment_contract->id, array('attributes' => array('class' => 'show-shipment-info'))),
            'class' => 'row-order-no'
        );
        $row[] = $shipment_contract->destinationpost;
        $row[] = format_date($shipment_contract->delivery_date, 'custom', 'Y/m/d');
        $row[] = array(
            'data' => empty($shipment_contract->sailing_date) ? '-' : format_date($shipment_contract->sailing_date, 'custom', 'Y/m/d'),
            'class' => 'row-sailing-date'
        );
        $rows[] = $row;
    }

    $output .= '<div class="shipments-contract-list">';
    $output .= '<h3><em>' . l(t('More delivery contract'), 'customer/shipment_contract') . '</em>' . t('Newly received delivery contract no.') . '</h3>';
    if ( count($rows) > 0) {
        $output .= '<div class="content">';
        $output .= theme('table', $shipment_contracts_header, $rows, array('id' => 'shipments-contract-table', 'class' => 'tablesorter'));
    } else {
        $output .= '<div class="no-content">';
        $output .= t('No posts available.');
    }
    $output .= '</div>';
    $output .= '</div>';
    $output .= '</div>';


    //second
    $output .= '<div class="second-sidebar">';
    $output .= '<div class="add-order-form">';
    $output .= '<h3>' . t('Add order') . '</h3>';
    $output .= '<div class="content">';
    $output .= drupal_get_form('order_form');
    $output .= '</div>';
    $output .= '</div>';
    $output .= '</div>';
    $output .= '</div>';
    $output .= '<div id="dialog"></div>';

    return $output;
}
