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

  function option_definition() {
    $options = parent::option_definition();
    $options['path'] = array('default' => CONGLOMERATE_TYPE_NONE);
    return $options;
  }

  /**
   * Provide the summary for page options in the views UI.
   *
   * This output is returned as an array.
   */
  function options_summary(&$categories, &$options) {
    parent::options_summary($categories, $options);

    $categories['type'] = array(
      'title' => t('Synchronization'),
    );

    $options['type'] = array(
      'category' => 'type',
      'title' => t('Content type'),
      'value' => t('None'),
    );
  }

  /**
   * Provide the default form for setting options.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    switch ($form_state['section']) {
      case 'type':
        $form['#title'] .= t('Map this display against a type');
        $form['type'] = array(
          '#type' => 'select',
          '#description' => t('This is used to map local content types against remote ones.'),
          '#default_value' => $this->get_option('type'),
		  '#options' => array(
			CONGLOMERATE_TYPE_NONE => '',
			CONGLOMERATE_TYPE_POST => t('Post'),
			CONGLOMERATE_TYPE_EVENT => t('Event'),
		  ),
        );
        break;
    }
  }

  function options_validate($form, &$form_state) {
    parent::options_validate($form, $form_state);

    switch ($form_state['section']) {
      case 'type':
        if (strpos($form_state['values']['type'], '$arg') !== FALSE) {
          form_error($form['type'], t('"$arg" is no longer supported. Use % instead.'));
        }

        if (strpos($form_state['values']['type'], '%') === 0) {
          form_error($form['type'], t('"%" may not be used for the first segment of a path.'));
        }

        // automatically remove '/' from path.
        $form_state['values']['type'] = trim($form_state['values']['type'], '/');

        break;
    }
  }

  function options_submit($form, &$form_state) {
    parent::options_submit($form, $form_state);

    switch ($form_state['section']) {
      case 'type':
        $this->set_option('type', $form_state['values']['type']);
        break;
    }
  }

  function validate() {
    $errors = parent::validate();

    $type = $this->get_option('type');
    if (empty($type) || $type == CONGLOMERATE_TYPE_NONE) {
      $errors[] = t('Display @display does not have a valid synchronization type defined.', array('@display' => $this->display->display_title));
    }

    return $errors;
  }

}
