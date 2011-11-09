<?php

/**
 * Implementation of hook_imagecache_default_presets().
 */
function fox_slideshow_imagecache_default_presets() {
  $items = array(
    'Slideshow' => array(
      'presetname' => 'Slideshow',
      'actions' => array(
        '0' => array(
          'weight' => '0',
          'module' => 'imagecache',
          'action' => 'imagecache_scale_and_crop',
          'data' => array(
            'width' => '300',
            'height' => '240',
          ),
        ),
      ),
    ),
  );
  return $items;
}

/**
 * Implementation of hook_node_info().
 */
function fox_slideshow_node_info() {
  $items = array(
    'fox_slideshow' => array(
      'name' => t('Fox Slideshow'),
      'module' => 'features',
      'description' => t('Each Fox Slideshow node is one slide image in a slideshow, create a slideshow by adding some Fox Slideshow nodes. Created by Lugir (http://lugir.com).'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'has_body' => '1',
      'body_label' => t('Body'),
      'min_word_count' => '0',
      'help' => '',
    ),
  );
  return $items;
}

/**
 * Implementation of hook_views_api().
 */
function fox_slideshow_views_api() {
  return array(
    'api' => '2',
  );
}
