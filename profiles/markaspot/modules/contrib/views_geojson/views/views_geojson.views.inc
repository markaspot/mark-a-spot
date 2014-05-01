<?php

/**
 * @file
 * Views plugin definition.
 */

/**
 * Implements hook_views_plugins().
 *
 * Adds GeoJSON feed style.
 */
function views_geojson_views_plugins() {
  $path = drupal_get_path('module', 'views_geojson');
  return array(
    'style' => array(
      'views_geojson'      => array(
        'title'            => t('GeoJSON Feed'),
        'help'             => t('Displays nodes in the GeoJSON data format.'),
        'handler'          => 'views_plugin_style_geojson',
        'theme'            => 'views_views_geojson_style',
        'theme path'       => $path . '/views',
        'path'             => $path . '/views',
        'uses row plugin'  => FALSE,
        'uses fields'      => TRUE,
        'uses options'     => TRUE,
        //'uses grouping'    => FALSE,
        //'type'             => 'feed',
        'type'             => 'normal',
        //'help_topic'     => 'style-geojson',
        'even empty'     => TRUE,
      ),
    ),
    'argument default' => array(
      'querystring' => array(
        'title' => t('Bounding box from query string'),
        'handler' => 'views_plugin_argument_default_bboxquery',
      ),
    ),
  );
}

/**
 * Implements hook_views_handlers().
 *
 * Adds bounding box contextual filter.
 */
function views_geojson_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'views_geojson') . '/views', // path to view files
    ),
    'handlers' => array(
      'views_geojson_bbox_argument' => array(
        'parent' => 'views_handler_argument',
      )
    ),
  );
}

/**
 * Implements hook_views_data().
 *
 * Adds bounding box contextual filter.
 */
function views_geojson_views_data() {
  $data = array();

  $data['node']['bbox_argument'] = array(
    'group' => t('Custom'),
    'real field'  => 'bbox_argument',
    'title' => t('Bounding box'),
    'help' => t('Filter locations within a bounding box.'),
    'argument' => array(
      'handler' => 'views_geojson_bbox_argument'
    ),
  );

  return $data;
}