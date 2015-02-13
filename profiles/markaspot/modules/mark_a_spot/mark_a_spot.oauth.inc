<?php
/**
 * @file
 * mark_a_spot.oauth.inc
 */

/**
 * Implements hook_default_oauth_common_context().
 */
function mark_a_spot_default_oauth_common_context() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 1;
  $context->name = 'open311';
  $context->title = 'Open311';
  $context->authorization_options = array(
    'access_token_lifetime' => NULL,
    'page_title' => NULL,
    'message' => NULL,
    'warning' => NULL,
    'deny_access_title' => NULL,
    'grant_access_title' => NULL,
    'disable_auth_level_selection' => NULL,
    'signature_methods' => array(
      0 => 'HMAC-SHA1',
      1 => 'HMAC-SHA256',
      2 => 'HMAC-SHA384',
      3 => 'HMAC-SHA512',
    ),
    'default_authorization_levels' => array(
      0 => 'key',
    ),
  );
  $context->authorization_levels = array(
    'key' => array(
      'name' => 'key',
      'title' => 'API Key',
      'default' => 1,
      'delete' => 0,
      'description' => '',
    ),
  );
  $export['open311'] = $context;

  return $export;
}
