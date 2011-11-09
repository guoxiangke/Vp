<?php
/**
 * Menu callback; Generate a listing of promoted nodes.
 */
module_load_include('inc', 'sina_vp', 'sina_vp.pages');
function sina_vp_center_page_default($sina_vp_type = NULL) {
	//function node_page_default() {
	$sql='SELECT n.nid, n.sticky, n.created FROM {node} n WHERE n.type in ("weibo") AND n.promote = 1 AND n.status = 1 ORDER BY n.sticky DESC, n.created DESC';
	if($sina_vp_type){
			switch ($sina_vp_type) {
				case 'Product':
					$sina_vp_type_tid = 1;
					break;
				case 'Activity':
					$sina_vp_type_tid = 2;
					break;
				case 'Special':
					$sina_vp_type_tid = 3;
					break;
				case 'Sale':
					$sina_vp_type_tid = 4;
					break;
				case 'Transfer':
					$sina_vp_type_tid = 5;
					break;
				case 'Show':
					$sina_vp_type_tid = 6;
					break;
				default:
					drupal_set_message('no $sina_vp_type','warning');
					break;
			}
		}
  if($sina_vp_type){
  	//SELECT n.nid, n.sticky, n.created FROM {node} n WHERE n.type in ("weibo") AND n.promote = 1 AND n.status = 1 ORDER BY n.sticky DESC, n.created DESC	
  	$sql='SELECT n.nid, n.sticky, n.created
				 FROM {node} n 
				 INNER JOIN term_node term_node ON n.vid = term_node.vid
				 WHERE (n.type in ("weibo") AND n.promote = 1 AND n.status = 1) AND (term_node.tid = '.$sina_vp_type_tid.')';
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
  drupal_set_title('');
	if($sina_vp_type=='Transfer' || $sina_vp_type=='Show' ||is_null($sina_vp_type)){
		 return $output;		  		
	}
  return $output ="<h3><a href='?q=vp/add/$sina_vp_type'>点击发博文</a></h3>".$output;;
}