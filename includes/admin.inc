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

  $form['login_server'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use as login server'),
    '#default_value' => 0,
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

  $form['login_consumer_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Login consumer key'),
    '#description' => t('The login consumer key for this site on the conglomerate server.'),
    '#size' => 32,
    '#maxlength' => 32,
    '#required' => TRUE,
  );

  $form['login_consumer_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Login consumer secret'),
    '#description' => t('The login consumer secret for this site on the conglomerate server'),
    '#size' => 32,
    '#maxlength' => 32,
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
  $sid = conglomerateclient_add_server($values['name'], $values['url'], $values['endpoint'], $values['consumer_key'], $values['access_key']);
  _conglomerateclient_write_server_tokens($values);

  if ($values['login_server']) {
    variable_set('conglomerateclient_login_server', $sid);
  }

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

/**
 * Edit server information
 *
 * @param array $form_state
 *
 * @param int $sid
 *  ID of server to edit
 * @return array $form
 */
function _conglomerateclient_admin_edit_server($form_state, $sid) {
  $login_server = variable_get('conglomerateclient_login_server', 1);

  // Fetch server info
  $servers = conglomerateclient_get_servers();
  $server = $servers[$sid];

  $form = array();

  $form['sid'] = array(
    '#type' => 'value',
    '#value' => $sid,
  );

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#description' => t('The name of the conglomerate server.'),
    '#size' => 32,
    '#maxlength' => 255,
    '#required' => TRUE,
    '#default_value' => $server['name'],
  );

  $form['login_server'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use as login server'),
    '#default_value' => $login_server == $sid,
  );

  $form['url'] = array(
    '#type' => 'textfield',
    '#title' => t('Url'),
    '#description' => t('A url to the conglomerate server, typically to the front page.'),
    '#size' => 32,
    '#maxlength' => 255,
    '#required' => TRUE,
    '#default_value' => $server['url'],
  );

  $form['endpoint'] = array(
    '#type' => 'textfield',
    '#title' => t('Endpoint'),
    '#description' => t('The api endpoint for the server.'),
    '#size' => 32,
    '#maxlength' => 255,
    '#required' => TRUE,
    '#default_value' => $server['endpoint'],
  );

  $form['login_consumer_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Login consumer key'),
    '#description' => t('The login consumer key for this site on the conglomerate server.'),
    '#size' => 32,
    '#maxlength' => 32,
    '#required' => TRUE,
    '#default_value' => $server['login_oauth_consumer'],
  );

  $form['login_consumer_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Login consumer secret'),
    '#description' => t('The login consumer secret for this site on the conglomerate server'),
    '#size' => 32,
    '#maxlength' => 32,
    '#required' => TRUE,
    '#default_value' => DrupalOAuthConsumerToken::load($server['login_oauth_consumer'])->secret,
  );

  $form['consumer_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Consumer key'),
    '#description' => t('The consumer key for this site on the conglomerate server.'),
    '#size' => 32,
    '#maxlength' => 32,
    '#required' => TRUE,
    '#default_value' => $server['oauth_consumer'],
  );

  $form['consumer_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Consumer secret'),
    '#description' => t('The consumer secret for this site on the conglomerate server'),
    '#size' => 32,
    '#maxlength' => 32,
    '#required' => TRUE,
    '#default_value' => DrupalOAuthConsumerToken::load($server['oauth_consumer'])->secret,
  );

  $form['access_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Access key'),
    '#description' => t('The access key for this site on the conglomerate server.'),
    '#size' => 32,
    '#maxlength' => 32,
    '#default_value' => $server['oauth_token_key'],
  );

  $form['access_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Access secret'),
    '#description' => t('The access secret for this site on the conglomerate server'),
    '#size' => 32,
    '#maxlength' => 32,
    '#default_value' => DrupalOAuthToken::load($server['oauth_token_key'])->secret,
  );

  $form['create'] = array(
    '#type' => 'submit',
    '#value' => t('Update'),
  );

  return $form;
}

/**
 * Updates server settings.
 */
function _conglomerateclient_admin_edit_server_submit($form, &$form_state) {
  $values = $form_state['values'];
  $servers = conglomerateclient_get_servers();

  if ($values['login_server']) {
    variable_set('conglomerateclient_login_server', $values['sid']);
  }

  // Get originals
  $server = $servers[$values['sid']];
  $consumer = DrupalOAuthConsumerToken::load($server['oauth_consumer']);
  $login_consumer = DrupalOAuthConsumerToken::load($server['login_oauth_consumer']);
  $access = DrupalOAuthToken::load($server['oauth_token_key']);

  /**
   * Check consumer token
   */
  // Key has changed, create new, delete old
  if ($consumer->key !== $values['consumer_key']) {
    $consumer->delete();
    // Create new consumer token
    $consumer = new DrupalOAuthConsumerToken($values['consumer_key'], $values['consumer_secret'], array(
      'provider_url' => $values['url'],
    ));
    $consumer->write();
  }
  // Secret has changed, key has not, just update
  else if ($consumer->secret !== $values['consumer_secret']) {
    $consumer->secret = $values['consumer_secret'];
    $consumer->provider_url = $values['url'];
    $consumer->write(TRUE);
  }

  // Key has changed, create new, delete old
  if (!$login_consumer || $login_consumer->key !== $values['login_consumer_key']) {
    if ($login_consumer) {
      $login_consumer->delete();
    }
    // Create new login_consumer token
    $login_consumer = new DrupalOAuthConsumerToken($values['login_consumer_key'], $values['login_consumer_secret'], array(
      'provider_url' => $values['url'],
    ));
    $login_consumer->write();
  }
  // Secret has changed, key has not, just update
  else if ($login_consumer->secret !== $values['login_consumer_secret']) {
    $login_consumer->secret = $values['login_consumer_secret'];
    $login_consumer->provider_url = $values['url'];
    $login_consumer->write(TRUE);
  }

  /**
   * Check access token
   */
  // Key has changed, create new, delete old
  if (!$access || $access->key !== $values['access_key']) {
    if ($access) {
      $access->delete();
    }
    
    // Create new access token
    $a_token = new DrupalOAuthToken($values['access_key'], $values['access_secret']);
    $a_token->write();
  }
  // Secret has changed, key has not, just update
  else if ($access->secret !== $values['access_secret']) {
    $access->secret = $values['access_secret'];
    $access->write(TRUE);
  }

  // Update server
  conglomerateclient_update_server($values['sid'], $values['name'], $values['url'], $values['endpoint'], $values['login_consumer_key'], $values['consumer_key'], $values['oauth_token_key']);
}