<?php

/**
 * Implementation of hook_views_plugins().
 */
function conglomerateclient_views_plugins() {
  return array(
    'display' => array(
      'sync' => array(
        'title' => t('Sync'),
        'help' => t('A "filter" to decide what nodes and belonging fields to synchronize.'),
        'handler' => 'conglomerate_plugin_display_sync',
        'parent' => 'views_plugin_display',
        'path' => drupal_get_path('module', 'conglomerateclient') . '/views',
        'theme' => 'views_view',
        'theme path' => drupal_get_path('module', 'views') . '/theme',
        'admin' => t('Sync'),
        'help topic' => 'display-default',
      ),
    ),
  );
}
