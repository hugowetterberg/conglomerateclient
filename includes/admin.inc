<?php

function _conglomerateclient_admin_index() {
  $messages = array();
  $messages[] = array(
    'module' => 'conglomerateclient',
    'link' => '<front>',
    'message' => 'Some message with !placeholder',
    'message values' => array('!placeholder' => 'zero value'),
    'description' => 'This is a message to you !name!',
    'description values' => array('!name' => 'Rudy'),
  );

  return theme('conglomerateclient_admin_index', conglomerateclient_get_servers(), $messages);
}

function _conglomerateclient_admin_add_server($form_state) {
  $form = array();

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#description' => t('The name of the conglomerate server.'),
    '#size' => 32,
    '#maxlength' => 255,
    '#required' => TRUE,
  );

  $form['url'] = array(
    '#type' => 'textfield',
    '#title' => t('Url'),
    '#description' => t('A url to the conglomerate server, typically to the front page.'),
    '#size' => 32,
    '#maxlength' => 255,
    '#required' => TRUE,
  );

  $form['endpoint'] = array(
    '#type' => 'textfield',
    '#title' => t('Endpoint'),
    '#description' => t('The api endpoint for the server.'),
    '#size' => 32,
    '#maxlength' => 255,
    '#required' => TRUE,
  );

  $form['consumer_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Consumer key'),
    '#description' => t('The consumer key for this site on the conglomerate server.'),
    '#size' => 32,
    '#maxlength' => 32,
    '#required' => TRUE,
  );

  $form['consumer_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Consumer secret'),
    '#description' => t('The consumer secret for this site on the conglomerate server'),
    '#size' => 32,
    '#maxlength' => 32,
    '#required' => TRUE,
  );

  $form['direct_access'] = array(
    '#type' => 'fieldset',
    '#title' => t('Direct access'),
    '#description' => t('If you have access keys for the server you can enter them here'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );

  $form['direct_access']['access_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Consumer key'),
    '#description' => t('The access key for this site on the conglomerate server.'),
    '#size' => 32,
    '#maxlength' => 32,
  );

  $form['direct_access']['access_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Consumer secret'),
    '#description' => t('The access secret for this site on the conglomerate server'),
    '#size' => 32,
    '#maxlength' => 32,
  );

  $form['create'] = array(
    '#type' => 'submit',
    '#value' => t('Create'),
  );

  return $form;
}

/**
 * Submit handler for _conglomerateclient_admin_add_server().
 */
function _conglomerateclient_admin_add_server_submit($form, $form_state) {
  global $user;

  $values = $form_state['values'];
  conglomerateclient_add_server($values['name'], $values['url'], $values['endpoint'], $values['consumer_key'], $values['access_key']);
  _conglomerateclient_write_server_tokens($values);
  drupal_set_message(t('The server was added'), 'info');
  drupal_goto('admin/settings/conglomerateclient/index');
}

/**
 * Helper function used to persist consumer and access tokens.
 *
 * @param array $values
 *  The values from teh form submit
 * @return void
 */
function _conglomerateclient_write_server_tokens($values) {
  // Save a consumer token so that we can get the secret later
  $c_token = DrupalOAuthConsumerToken::load($values['consumer_key']);
  if ($c_token) {
    $c_token->secret = $values['consumer_secret'];
    $c_token->provider_url = $values['url'];
    $c_token->write(TRUE);
  }
  else {
    $c_token = new DrupalOAuthConsumerToken($values['consumer_key'], $values['consumer_secret'], array(
      'provider_url' => $values['url'],
    ));
    $c_token->write();
  }

  // Save or update the access token
  if (!empty($values['access_key']) && !empty($values['access_secret'])) {
    $token = DrupalOAuthToken::load($values['access_key']);
    if ($token) {
      // Correct the secret if needed
      if ($token->secret !== $values['access_secret']) {
        $token->secret = $values['access_secret'];
        $token->write(TRUE);
      }
    }
    else {
      // Create the secret if it doesn't exist
      $token = new DrupalOAuthToken($values['access_key'], $values['access_secret']);
      $token->provider_token = FALSE;
      $token->consumer_key = $values['consumer_key'];
      $token->type = 'access';
      $token->uid = $user->uid;
      $token->write();
    }
  }
}