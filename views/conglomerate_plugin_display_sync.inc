<?php
/**
 * @file
 * Contains the sync display plugin.
 */

/**
 * A plugin to handle defaults on a view.
 *
 * @ingroup views_display_plugins
 */
class conglomerate_plugin_display_sync extends views_plugin_display {

  /**
   * Determine if this display is the 'default' display which contains
   * fallback settings
   */
  function is_default_display() { return FALSE; }
 
  /**
   * The default execute handler fully renders the view.
   */
  function execute() {
    return $this->view->render($this->display->id);
  }

}
