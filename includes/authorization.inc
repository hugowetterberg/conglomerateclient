<?php

// $Id$

function _conglomerateclient_authorize_for_server($sid) {
  $servers = conglomerateclient_get_servers();
  $server = $servers[$sid];

  $consumer_token = DrupalOAuthConsumerToken::load($server['oauth_consumer']);
  $auth_url = oauth_common_get_request_token($consumer_token);

  header('Location: ' . $auth_url);
  exit;
}