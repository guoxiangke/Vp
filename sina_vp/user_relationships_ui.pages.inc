<?php
// $Id: user_relationships_ui.pages.inc,v 1.1.2.7 2009/11/07 21:24:54 alexk Exp $
/**
 * @file
 * Various user relationships admin and user pages
 */

/**
 * Relationship Types List
 */
function user_relationships_ui_types_list_page() {
  $relationship_types = user_relationships_types_load();

  $table['headers'] = array(t('Name'), t('Plural'), t('Type'), t('Requires Approval'), t('Expiration'), t('Operations'));
  $table['data'] = array();
  $table['rows'] = array();

  foreach ($relationship_types as $relationship) {
    $table['data'][] = $relationship;
    $table['rows'][] = array(
      $relationship->name,
      $relationship->plural_name,
      ($relationship->is_oneway ? ($relationship->is_reciprocal ? t('reciprocal') : t('one way')) : t('mutual')),
      ($relationship->requires_approval ? t('yes') : t('no')),
      ($relationship->expires_val ? t('@expires_val', array('@expires_val' => format_plural($relationship->expires_val, '1 day', '@count days'))) : t('Never')),
      l(t('edit'),    "admin/user/relationships/{$relationship->rtid}/edit") .' | '.
      l(t('delete'),  "admin/user/relationships/{$relationship->rtid}/delete")
    );
  }

  foreach (module_implements('user_relationships_page_alter') as $module) {
    $function = "{$module}_user_relationships_page_alter";
    $function('types list', $page, $table);
  }

  if (!sizeof($table['rows'])) {
    $table['rows'][] = array(array('data' => t('No relationships available.'), 'colspan' => sizeof($table['headers'])));
  }

  $page['relationships'] = array(
    '#type'   => 'fieldset',
    '#title'  => t('Relationship Types'),
    '#weight' => 0
  );
    $page['relationships']['list'] = array(
      '#value' => theme('table', $table['headers'], $table['rows'])
    );

  return drupal_render($page);
}

/**
 * Main list of relationships for a specified user
 */
function user_relationships_page($account = NULL, $rtid = NULL) {
  if (!$account) {
    global $user;
    $account = $user;
  }
	$follows_uid_array = get_follows();
	if(empty($array)){
		$follows_uid[] ='1';//没有粉丝....
	}
	
	// $Id: user_relationships.tpl.php,v 1.1.2.10 2009/10/15 15:23:26 aufumy Exp $
	//$relationships array is loaded in template_preprocess_user_relationships()
	if (template_preprocess_user_relationships2(&$variables)) {
	  foreach ($relationships as $relationship) {
	    $edit_access = ($user->uid == $account->uid && user_access('maintain own relationships')) || user_access('administer users');
	
	    $this_user_str  = $account->uid == $relationship->requestee_id ? 'requester' : 'requestee';
	    $this_user      = $relationship->{$this_user_str};
	
	    $row = array(
	      theme('username', $this_user),
	      ur_tt("user_relationships:rtid:$relationship->rtid:name", $relationship->name) . ($relationship->is_oneway ? ($this_user_str == 'requestee' ? t(' (You to Them)') : t(' (Them to You)')) : NULL),
	      $relationship->extra_for_display,
	      $edit_access ? theme('user_relationships_remove_link', $account->uid, $relationship->rid) : '&nbsp;',
	    );
	    if (variable_get('user_relationships_show_user_pictures', 0)) {
	      array_unshift($row, theme('user_picture', $this_user)); 
	    }
	    $rows[] = $row;
	  }
	
	  print
	    theme('table', array(), $rows, array('class' => 'user-relationships-listing-table')) .
	    theme('pager', NULL, $relationships_per_page);
	}
	else {
	  print t('No relationships found');
	}
  //return theme('user_relationships', $account, 2);
}
/**
 * Pre processor for user_relationships page
 */
function template_preprocess_user_relationships2(&$variables) {
  global $user;

  // readability
  $account  = $variables['account'];
  $rtid     = $variables['rtid'];

  $args = array('user' => $account->uid, 'approved' => TRUE);
  if (isset($rtid) && is_numeric($rtid)) {
    $relationship_type = user_relationships_type_load($rtid);
    $args['rtid'] = $rtid;
  }

  $query = _user_relationships_generate_query($args, array('include_user_info' => TRUE));

  // To Page or not to Page
  if ($variables['relationships_per_page'] = variable_get('user_relationships_relationships_per_page', 16)) {
    $results = pager_query($query['query'], $variables['relationships_per_page'], 0, $query['count'], $query['arguments']);
  }
  else {
    $results = db_query($query['query'], $query['arguments']);
  }

  $variables['relationships'] = array();
  while ($relationship = db_fetch_object($results)) {
    user_relationships_api_translate_user_info($relationship);
    $variables['relationships'][$relationship->rid] = $relationship;
  }

  if ($account->uid == $user->uid) {
    $msg = isset($relationship_type) 
           ? t("My %relationships", array('%relationships' => $relationship_type->plural_name ? ur_tt("user_relationships:rtid:$relationship_type->rtid:plural_name", $relationship_type->plural_name) : ur_tt("user_relationships:rtid:$relationship_type->rtid:name", $relationship_type->name))) 
           : t("All my relationships");
  }
  else {
    $msg = isset($relationship_type) 
           ? t("%username's %relationships", array('%username' => $account->name, '%relationships' => $relationship_type->plural_name ? ur_tt("user_relationships:rtid:$relationship_type->rtid:plural_name", $relationship_type->plural_name) : ur_tt("user_relationships:rtid:$relationship_type->rtid:name", $relationship_type->name))) 
           : t("All %username's relationships", array('%username' => $account->name));
  }

  $variables['title'] = $msg;
	return $relationships;
}
/**
 * List of pending requests from other users
 */
function user_relationships_pending_requests_page($account = NULL) {
  // Check that the uid is valid, not the anonymous user, and the user exists
  if (!$account) {
    global $user;
    $account = $user;
  }

  return theme('user_relationships_pending_requests', $account);
}