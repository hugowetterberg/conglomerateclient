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

  $_SESSION['conglomerate_login_request_key'] = $request_token->key;
  $_SESSION['conglomerate_login_request_secret'] = $request_token->secret;

  header('Location: ' . $auth_url);
  exit;
}

function _conglomerateclient_login_external_user($external_user) {
  global $user;

  $internal = db_fetch_array(db_query("SELECT uid FROM {users_skane} WHERE euid=%d", array(
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

    // Save the user and add a entry for the external user
    $user = user_save((object)$internal, $internal);
    drupal_write_record('users_skane', $values=array(
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