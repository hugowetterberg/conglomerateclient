<?php

/**
 * Helper to implementation of hook_imagecache_default_presets().
 */
function _conglomerateclientfeatures_imagecache_default_presets() {
  $items = array(
    'conglomerate-image' => array(
      'presetname' => 'conglomerate-image',
      'actions' => array(
        '0' => array(
          'actionid' => '1',
          'presetid' => '1',
          'weight' => '0',
          'module' => 'imagecache',
          'action' => 'imagecache_scale_and_crop',
          'data' => array(
            'width' => '256',
            'height' => '256',
          ),
        ),
      ),
    ),
    'conglomerate-large-image' => array(
      'presetname' => 'conglomerate-large-image',
      'actions' => array(
        '0' => array(
          'actionid' => '2',
          'presetid' => '2',
          'weight' => '0',
          'module' => 'imagecache',
          'action' => 'imagecache_scale',
          'data' => array(
            'width' => '1200',
            'height' => '',
            'upscale' => 0,
          ),
        ),
        '1' => array(
          'actionid' => '3',
          'presetid' => '2',
          'weight' => '0',
          'module' => 'imagecache',
          'action' => 'imagecache_scale',
          'data' => array(
            'width' => '0',
            'height' => '1200',
            'upscale' => 0,
          ),
        ),
      ),
    ),
  );
  return $items;
}
