<?php

/**
 * @file
 * This file has some views functions
 */

/**
 * Implements hook_views_data().
 */
function markaspot_log_views_data() {
  $data['markaspot_log']['table']['group'] = t('Mark-a-Spot Log');

  $data['markaspot_log']['table']['base'] = array(
    'field' => 'id',
    'title' => 'Mark-a-Spot Log entries',
    'help' => 'Log file entries',
  );

  // $data['markaspot_log']['id'] = array(
  //   'title' => t('ID'),
  //   'help' => t('The unique ID'),
  //   'field' => array(
  //     'handler' => 'views_handler_field_numeric',
  //     'click sortable' => TRUE,
  //   ),
  //   'filter' => array(
  //     'handler' => 'views_handler_filter_numeric',
  //   ),
  //   'sort' => array(
  //     'handler' => 'views_handler_sort',
  //   ),
  // );

  $data['markaspot_log']['nid'] = array(
    'title' => t('nid'),
    'help' => t('The Node-Id which has been manipulated'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['markaspot_log']['message'] = array(
    'title' => t('Log Message'),
    'help' => t('The actual message'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );

  $data['markaspot_log']['created'] = array(
    'title' => t('Log date'),
    'help' => t('The Log date'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );
  return $data;
}