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