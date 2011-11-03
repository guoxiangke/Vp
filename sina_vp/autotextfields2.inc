<?php

/**
 * @file
 * Show/hide textfields based on checkbox clicks.
 */
function ahah_example_autotextfields(&$form_state) {

  $default_first_name = !empty($form_state['values']['ask_first_name']) ? $form_state['values']['ask_first_name'] : FALSE;
  $default_last_name = !empty($form_state['values']['ask_last_name']) ? $form_state['values']['ask_last_name'] : FALSE;
  $form['ask_first_name'] = array(
    '#type' => 'checkbox',
    '#title' => t('Ask me my first name'),
    '#default_value' => $default_first_name,
    '#ahah' => array(
      'path' => 'examples/ahah_example/autotextfields/callback',
      'wrapper' => 'textfield-wrapper',
      'effect' => 'fade',
    )
  );
  $form['ask_last_name'] = array(
   '#type' => 'checkbox',
   '#title' => t('Ask me my last name'),
   '#default_value' => $default_last_name,

    '#ahah' => array(
      'path' => 'examples/ahah_example/autotextfields/callback',
      'wrapper' => 'textfield-wrapper',
      'effect' => 'fade',
      // 'event' => 'change', // default value: does not need to be set explicitly.

    ),
  );
  $form['textfields'] = array(
    '#title' => t("Generated text fields for first and last name"),
    '#prefix' => '<div id="textfield-wrapper">',
    '#suffix' => '</div>',
    '#type' => 'fieldset',
    '#description' => t('This is where we put automatically generated textfields'),
  );

   if (!empty($form_state['values']['ask_first_name'])) {
    $form['textfields']['first_name'] = array(
      '#type' => 'textfield',
      '#title' => t('First Name'),
    );
  }
  if (!empty($form_state['values']['ask_last_name'])) {
    $form['textfields']['last_name'] = array(
      '#type' => 'textfield',
      '#title' => t('Last Name'),
    );
  }


  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Click I'),
  );


  return $form;
}

/**
 * Submit handler for autotextfields.
 * Gets called even when our select is active, so we use the
 * $form_state to determine whether the submit handler should actually do
 * anything.
 */
function ahah_example_autotextfields_submit($form, &$form_state) {
  if (!empty($form_state['ahah_submission'])) {
    return;
  }

  // Continue to handle submit processing.
}
/**
 * Does the very standard things that must be done in any normal callback.
 * Used by each callback in this example module.
 */
function ahah_example_callback_helper2() {
  $form_state = array('storage' => NULL, 'submitted' => FALSE);
  $form_build_id = $_POST['form_build_id'];
  $form = form_get_cache($form_build_id, $form_state);
  $args = $form['#parameters'];
  $form_id = array_shift($args);
  $form_state['post'] = $form['#post'] = $_POST;
  // Enable the submit/validate handlers to determine whether AHAH-submittted.
  $form_state['ahah_submission'] = TRUE;
  $form['#programmed'] = $form['#redirect'] = FALSE;
  drupal_process_form($form_id, $form, $form_state);
	dpm($_POST,$form_id.'2');
	module_load_include('inc', 'sina_vp', 'sina_vp.pages');//
	dpm($form_state,'16130-->$form_state');
  $form = drupal_rebuild_form($form_id, $form_state, $args, $form_build_id);
	dpm($form_state,'16131-->$form_state');
  dpm($form,'ahah_example_callback_helper2->form_id');
  return $form;
}

function weibo_page_default_callback($nid) {
  $form = ahah_example_callback_helper2();
  $textfields = $form['textfields'][$nid];
	dpm($textfields,'$textfields');
  // Remove the prefix/suffix wrapper so we don't double it up.
  unset($textfields['#prefix'], $textfields['#suffix']);

  // Render the output.
  $output = theme('status_messages');
  $output .= drupal_render($textfields);

  // Final rendering callback. return $output;//
  drupal_json(array('status' => TRUE, 'data' => $output));
  exit();
}