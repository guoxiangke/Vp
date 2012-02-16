<?php

/**
 * @file
 * Handles toggling of options.
 */

/**
 * Menu callback. Toggle options for a node if the action is confirmed via
 * POST. Otherwise, display a confirmation form.
 */
function fasttoggle_node_option($node, $option) {
  $options = fasttoggle_get_options('node', $node);

  // Check if the action is valid. This is essential to ensure the user has
  // access to the action.
  if (isset($options[$option]) && isset($_GET['token']) && drupal_valid_token($_GET['token'], $option .'_'. $node->nid, TRUE)) {
    // The action is confirmed: either via form submit or via AJAX/POST
    if (isset($_POST['confirm']) && $_POST['confirm']) {
      // Get the next ID.
      while (key($options[$option]) != $node->$option) next($options[$option]);
      if (next($options[$option]) === FALSE) reset($options[$option]);

      // Save the node.
      $node->$option = key($options[$option]);
      node_save($node);

      // Let other modules respond.
      module_invoke_all('fasttoggle_toggle', 'node', $node, $option);

      // Output the new status for the updated link text on AJAX changes
      if (isset($_POST['javascript']) && $_POST['javascript']) {
        drupal_set_header('Content-Type: text/javascript; charset=utf-8');
        echo drupal_to_js(array(
          'text' => $options[$option][intval($node->$option)],
          'callback' => 'node',
          'option' => $option,
          'status' => $node->$option,
          'className' => 'fasttoggle-status-node-'. $option .'-'. intval($node->$option),
        ));
        exit;
      }
      else {
        drupal_goto();
      }
    }
    else {
      // The action is not confirmed. The user came here through a regular link;
      // no AJAX was involved. That means, we need a confirmation form so that
      // we get a POST form.
      $labels = _fasttoggle_get_label('node_'. $option, FASTTOGGLE_LABEL_STATUS);
      return drupal_get_form('fasttoggle_node_option_confirm', $node, $labels[intval(!$node->$option)]);
    }
  }
  else {
    return MENU_NOT_FOUND;
  }
}


/**
 * Confirmation form for the option change of a node.
 */
function fasttoggle_node_option_confirm($form_state, $node, $option) {
  return confirm_form(array(),
    t('Are you sure you want to set the post %title to %option?', array('%title' => $node->title, '%option' => $option)),
    $_GET['destination'] ? $_GET['destination'] : 'node/'. $node->nid,
    '',
    t('Change'), t('Cancel')
  );
}


/**
 * Menu callback. Toggle the status of a user if the action is confirmed via
 * POST. Otherwise, display a confirmation form.
 * //follow_toggle/3/focus|follow/1/10 谁follow谁 谁unfollow谁 谁focus谁 谁unfocus谁 谁follow谁 谁unfollow谁 谁focus谁 谁unfocus谁
 */
function fasttoggle_user_option($requestee, $option, $rid=null) {	
	global $user;
	$requester = $user;
	$requestee = user_load($requestee);
	switch ($option) {
		case 'follow':
			$rtid = 1;
			break;		
		case 'focus':
			$rtid = 2;
			break;
		case 'unfollow':
			$rtid = 1;
			$option = 'follow';
			break;		
		case 'unfocus':
			$rtid = 2;
			$option = 'focus';
			break;
	}//follow_toggle/18/focus
	$my_follows =	user_relationships_load(array("requester_id" => $user->uid),array("sort" => 'requestee_id',"include_user_info" => TRUE));//所有的
	$follow_status=0;
	foreach(array_keys($my_follows) as $key){
		if($requestee->uid==$key){
			$follow_status=1; //已关注					
			$rid = $my_follows[$key][0]->rid;// 关系id		
		}
	}
	$options = sina_vp_follow_toggle_options($option);//asttoggle_get_options('user', $user); 获取用户粉它状态tag集。

  // Check if the action is valid. This is essential to ensure the user has
  // access to the action.
  if (isset($options[$option]) && isset($_GET['token']) && drupal_valid_token($_GET['token'], $option .'_'. $user->uid, TRUE)) {
   
    if (1||isset($_POST['confirm']) && $_POST['confirm']) {
      //$array = array($option => !$user->$option);
      //$user = user_save($user, $array);

        if(!$rid){ //save a relationship        	
				  $relationship->requester = $requester;
				  $relationship->requestee = $requestee;
				  $relationship->type = user_relationships_type_load($rtid);
				
				  $relationship = user_relationships_request_relationship($relationship);
				  if ($relationship === FALSE) {
				    drupal_set_message(user_relationships_ui_get_message('unknown_error', $relationship));
				  }
					//Output the new option for the updated link text on AJAX changes
		      if (isset($_POST['javascript']) && $_POST['javascript']) {
		        drupal_set_header('Content-Type: text/javascript; charset=utf-8');
							switch ($options[$option]['1']) {
								case 'focus':
									$title='订阅';
								case 'unfocus':
									$title='取消订阅';
									break;
								case 'unfollow':
									$title='已关注';
									break;
								case 'follow':
									$title='关注';
									break;
								default:
									
									break;
							}
		        echo drupal_to_js(array(
		          'text' => $title,
		          'className' => 'fasttoggle-status-user-'. $option .'-1',
		        ));
		        exit;
		      }
		      else {
		        drupal_goto();
		      }
	      }else{
	      	  $relationship = user_relationships_load($rid);
					      	 
					  if (!user_access('maintain own relationships') || (!user_access('administer user relationships') && $user->uid != $relationship->requester_id && $user->uid != $relationship->requestee_id)) {
					    drupal_access_denied();
					    exit();
					  }
					  if(is_array($relationship)){$relationship=$relationship['0'];}
					  user_relationships_delete_relationship($relationship, $user);
					 	drupal_set_message(user_relationships_ui_get_message('removed', $relationship));
						//Output the new option for the updated link text on AJAX changes
			      if (isset($_POST['javascript']) && $_POST['javascript']) {
			        drupal_set_header('Content-Type: text/javascript; charset=utf-8');
							switch ($options[$option]['0']) {
								case 'focus':
									$title='订阅';
								case 'unfocus':
									$title='订阅';
									break;
								case 'unfollow':
									$title='已关注';
									break;
								case 'follow':
									$title='关注';
									break;
								default:
									
									break;
							}
		        echo drupal_to_js(array(
		          'text' => $title,
		          	'className' => 'fasttoggle-status-user-'. $option .'-0',
			        ));
			        exit;
			      }
			      else {
			        drupal_goto();
			      }
	      }
			
      // Let other modules respond.
      //module_invoke_all('fasttoggle_toggle', 'user', $user, $option);

      /*/ */
    }
    else {
      // The action is not confirmed. The user came here through a regular link;
      // no AJAX was involved. That means, we need a confirmation form so that
      // we get a POST form.
      //$labels = _fasttoggle_get_label('user_'. $option, FASTTOGGLE_LABEL_STATUS);
      return drupal_get_form('fasttoggle_user_option_confirm', $user, $labels[intval(!$user->$option)]);
    }
  }
  else {
    return MENU_NOT_FOUND;
  }
}


/**
 * Confirmation form for the status change of a user.
 */
function fasttoggle_user_option_confirm($form_state, $user, $option) {
  return confirm_form(array(),
    t('Are you sure you want to set the user %user to %option?', array('%user' => $user->name, '%option' => $option)),
    $_GET['destination'] ? $_GET['destination'] : 'user/'. $user->uid,
    '',
    t('Change'), t('Cancel')
  );
}


/**
 * Menu callback. Toggle options for a comment if the action is confirmed via
 * POST. Otherwise, display a confirmation form.
 */
function fasttoggle_comment_option($comment, $option) {
  $options = fasttoggle_get_options('comment', $comment);

  // Check if the action is valid. This is essential to ensure the user has
  // access to the action.
  if (isset($options[$option]) && isset($_GET['token']) && drupal_valid_token($_GET['token'], $option .'_'. $comment->cid, TRUE)) {
    // The action is confirmed: either via form submit or via AJAX/POST
    if (isset($_POST['confirm']) && $_POST['confirm']) {
      $comment->$option = !$comment->$option;
      comment_save((array)$comment);

      // Let other modules respond.
      module_invoke_all('fasttoggle_toggle', 'comment', $comment, $option);

      // Output the new status for the updated link text on AJAX changes
      if (isset($_POST['javascript']) && $_POST['javascript']) {
        drupal_set_header('Content-Type: text/javascript; charset=utf-8');
        echo drupal_to_js(array(
          'text' => $options[$option][intval($comment->$option)],
          'callback' => 'comment',
          'option' => $option,
          'status' => $comment->$option,
          'className' => 'fasttoggle-status-comment-'. $option .'-'. (1 - intval($comment->$option)),
        ));
        exit;
      }
      else {
        drupal_goto();
      }
    }
    else {
      // The action is not confirmed. The user came here through a regular link;
      // no AJAX was involved. That means, we need a confirmation form so that
      // we get a POST form.
      $labels = _fasttoggle_get_label('comment_'. $option, FASTTOGGLE_LABEL_STATUS);
      return drupal_get_form('fasttoggle_comment_option_confirm', $comment, $labels[intval(!$comment->$option)]);
    }
  }
  else {
    return MENU_NOT_FOUND;
  }
}


/**
 * Confirmation form for the option change of a comment.
 */
function fasttoggle_comment_option_confirm($form_state, $comment, $option) {
  return confirm_form(array(),
    t('Are you sure you want to set the comment %title to %option?', array('%title' => $comment->subject, '%option' => $option)),
    $_GET['destination'] ? $_GET['destination'] : 'comment/'. $comment->cid,
    '',
    t('Change'), t('Cancel')
  );
}
