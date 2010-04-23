<?php

function _conglomerateclient_login() {
  $sid = variable_get('conglomerateclient_login_server', 1);
  $servers = conglomerateclient_get_servers();
  $server = $servers[$sid];
  $consumer_token = DrupalOAuthConsumerToken::load($server['login_oauth_consumer']);

  $client = new DrupalOAuthClient($consumer_token);
  $request_token = $client->getRequestToken();
  $request_token->write();
  $auth_url = $client->getAuthorizationUrl();

  // Don't allow this page to be cached
  header("Cache-Control: no-cache, must-revalidate");
  header("Expires: Sat, 26 Jul 1997 05:00:00 GMT");

  // Regenerate our session and set the info that's needed to continue
  // the login process when the user returns;
  $_SESSION['conglomerate_login_request_key'] = $request_token->key;
  $_SESSION['conglomerate_login_request_secret'] = $request_token->secret;

  return drupal_get_form('conglomerateclient_login_form', $auth_url, $request_token->key);
}

function conglomerateclient_login_form($form_state, $auth_url, $key) {
  $form = array(
    '#pre_render' => array('conglomerateclient_login_form_pre_render'),
  );

  $sid = variable_get('conglomerateclient_login_server', 1);
  $servers = conglomerateclient_get_servers();
  $server = $servers[$sid];

  $form['authorization_url'] = array(
    '#type' => 'value',
    '#value' => $auth_url,
  );

  $form['request_token_key'] = array(
    '#type' => 'value',
    '#value' => $key,
  );

  $form['log_in'] = array(
    '#type' => 'item',
    '#title' => t('Log in'),
    '#value' => t('You will be sent to @server so that you can be logged in here at @sitename', array(
      '@server' => $server['url'],
      '@sitename' => variable_get('site_name', ''),
    )),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Log in'),
  );

  return $form;
}

function conglomerateclient_login_form_pre_render($form) {
  $fe = form_get_errors();
  if ($fe) {
    $form['#attributes']['class'] = 'error';
  }
  drupal_add_js(drupal_get_path('module', 'conglomerateclient') . '/conglomerateclient_auto_submit.js');
  drupal_add_css(drupal_get_path('module', 'conglomerateclient') . '/conglomerateclient_auto_submit.css');
  return $form;
}

function conglomerateclient_login_form_validate($form, $form_state) {
  $values = $form_state['values'];
  $sessval = $_SESSION['conglomerate_login_request_key'];

  if (empty($sessval) || $sessval !== $values['request_token_key']) {
    form_set_error('log_in', t('You must enable cookies in your web-browser to be able to login'));
  }
}

function conglomerateclient_login_form_submit($form, $form_state) {
  $values = $form_state['values'];
  // Redirect
  drupal_goto($values['authorization_url']);
}

function _conglomerateclient_login_external_user($external_user) {
  global $user;

  $internal = db_fetch_array(db_query("SELECT uid FROM {conglomerate_users} WHERE euid=%d", array(
    ':eid' => $external_user['uid'],
  )));
  if (!empty($internal)) {
    $user = user_load($internal);

    sess_regenerate();
    module_invoke_all('user', 'login', NULL, $user);
  }
  else {
    $internal = array(
      'pass' => user_password(32),
    ) + $external_user;
    unset($internal['uid']);
    unset($internal['roles']);

    // Save the user and add a entry for the external user
    $user = user_save((object)$internal, $internal);
    drupal_write_record('conglomerate_users', $values=array(
      'uid' => $user->uid,
      'euid' => $external_user['uid'],
    ));

    sess_regenerate();
    module_invoke_all('user', 'login', NULL, $user);
  }

  drupal_set_message(t('You\'re now logged in as @name', array(
    '@name' => $user->name,
  )));
  drupal_goto();
}