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
class conglomerateclient_plugin_display_sync extends views_plugin_display {

  private $conglomerate_types = array();

  /**
   * Constructor method.
   */
  public function __construct() {
    $this->conglomerate_types = array(
      CONGLOMERATE_TYPE_NONE => t('None'),
      CONGLOMERATE_TYPE_POST => t('Post'),
      CONGLOMERATE_TYPE_EVENT => t('Event'),
      CONGLOMERATE_TYPE_LOCATION => t('Location'),
    );
  }

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
    $options['content_type'] = array('default' => '');
    $options['server_type'] = array('default' => CONGLOMERATE_TYPE_NONE);
    return $options;
  }

  /**
   * Provide the summary for page options in the views UI.
   *
   * This output is returned as an array.
   */
  function options_summary(&$categories, &$options) {
    parent::options_summary($categories, $options);

    $categories['synchronization'] = array(
      'title' => t('Synchronization'),
    );

    $content_type = $this->get_option('content_type');
    if (!empty($content_type)) {
      $content_types = node_get_types();
      $content_type = $content_types[$content_type]->name;
    }
    else {
      $content_type = t('None');
    }

    $options['content_type'] = array(
      'category' => 'synchronization',
      'title' => t('Content type'),
      'value' => $content_type,
    );

    $server_type = $this->get_option('server_type');

    $options['server_type'] = array(
      'category' => 'synchronization',
      'title' => t('Server type'),
      'value' => $this->conglomerate_types[$server_type],
    );

    $server = $this->get_option('server');
    if (!empty($server)) {
      $servers = conglomerateclient_get_servers();
      if (isset($servers[$server])) {
        $server = $servers[$server]['name'];
      }
      else {
        $server = t('Unknown server');
      }
    }
    else {
      $server = t('None');
    }
    $options['server'] = array(
      'category' => 'synchronization',
      'title' => t('Server'),
      'value' => $server,
    );
  }

  /**
   * Provide the default form for setting options.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $content_types = array('');
    foreach (node_get_types() as $content_type) {
      $content_types[$content_type->type] = $content_type->name;
    }

    switch ($form_state['section']) {
      case 'content_type':
        $form['#title'] .= t('Map this display against a type');
        $form['content_type'] = array(
          '#type' => 'select',
          '#description' => t("Select what local content type you want to use this display to be used for."),
          '#default_value' => $this->get_option('content_type'),
          '#options' => $content_types,
        );
        break;
      case 'server_type':
        $form['#title'] .= t('Map this display against a type');
        $form['server_type'] = array(
          '#type' => 'select',
          '#description' => t("Select the type to be relayed to the server."),
          '#default_value' => $this->get_option('server_type'),
          '#options' => $this->conglomerate_types,
        );
        break;
      case 'server':
        $servers = conglomerateclient_get_servers();
        $options = array(
          '' => t('None'),
        );
        foreach ($servers as $sid => $info) {
          $options[$sid] = $info['name'];
        }

        $form['#title'] .= t('Choose a server to synchronize with');
        $form['server'] = array(
          '#type' => 'select',
          '#description' => t("Select a server."),
          '#default_value' => $this->get_option('server'),
          '#options' => $options,
        );
        break;
    }
  }

  function options_submit($form, &$form_state) {
    parent::options_submit($form, $form_state);

    switch ($form_state['section']) {
      case 'content_type':
        $this->set_option('content_type', $form_state['values']['content_type']);
        break;
      case 'server_type':
        $this->set_option('server_type', $form_state['values']['server_type']);
        break;
      case 'server':
        $this->set_option('server', $form_state['values']['server']);
        break;
    }
  }

  function validate() {
    $errors = parent::validate();

    $content_type = $this->get_option('content_type');
    if (empty($content_type)) {
      $errors[] = t('Display @display does not have a valid Content type defined for synchronization.', array('@display' => $this->display->display_title));
    }

    $server_type = $this->get_option('server_type');
    if (empty($server_type) || $server_type == CONGLOMERATE_TYPE_NONE) {
      $errors[] = t('Display @display does not have a valid Server type defined for  synchronization.', array('@display' => $this->display->display_title));
    }

    $server = $this->get_option('server');
    if (empty($server)) {
      $errors[] = t('Display @display does not have a valid server defined for synchronization.', array('@display' => $this->display->display_title));
    }

    return $errors;
  }

}
