<?php
/**
 * @file
 * mark_a_spot.services.inc
 */

/**
 * Implements hook_default_services_endpoint().
 */
function mark_a_spot_default_services_endpoint() {
  $export = array();

  $endpoint = new stdClass();
  $endpoint->disabled = FALSE; /* Edit this to true to make a default endpoint disabled initially */
  $endpoint->api_version = 3;
  $endpoint->name = 'georeport_v2';
  $endpoint->server = 'rest_server';
  $endpoint->path = 'georeport';
  $endpoint->authentication = array(
    'services' => 'services',
  );
  $endpoint->server_settings = array(
    'rest_server' => array(
      'formatters' => array(
        'json' => TRUE,
        'rss' => TRUE,
        'xml' => TRUE,
        'bencode' => FALSE,
        'jsonp' => FALSE,
        'php' => FALSE,
        'yaml' => FALSE,
      ),
      'parsers' => array(
        'application/x-www-form-urlencoded' => TRUE,
        'application/json' => FALSE,
        'application/vnd.php.serialized' => FALSE,
        'application/x-yaml' => FALSE,
        'multipart/form-data' => FALSE,
      ),
    ),
  );
  $endpoint->resources = array(
    'discovery' => array(
      'operations' => array(
        'index' => array(
          'enabled' => 1,
        ),
      ),
    ),
    'requests' => array(
      'operations' => array(
        'retrieve' => array(
          'enabled' => 1,
        ),
        'create' => array(
          'enabled' => 1,
        ),
        'index' => array(
          'enabled' => 1,
        ),
      ),
    ),
    'services' => array(
      'operations' => array(
        'retrieve' => array(
          'enabled' => 1,
        ),
        'index' => array(
          'enabled' => 1,
        ),
      ),
    ),
  );
  $endpoint->debug = 0;
  $export['georeport_v2'] = $endpoint;

  return $export;
}
