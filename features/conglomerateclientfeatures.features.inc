<?php

/**
 * Implementation of hook_imagecache_default_presets().
 */
function conglomerateclientfeatures_imagecache_default_presets() {
  module_load_include('inc', 'conglomerateclientfeatures', 'conglomerateclientfeatures.defaults');
  $args = func_get_args();
  return call_user_func_array('_conglomerateclientfeatures_imagecache_default_presets', $args);
}
