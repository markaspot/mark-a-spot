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
  // Define the base group of this table. Fields that don't
  // have a group defined will go into this field by default.
  $data['markaspot_log']['table']['group']  = t('Report Log');

  $data['markaspot_log']['table']['base'] = array(
    'field' => 'id',
    'title' => t('Log entry'),
    'help' => t("Log entries log report history."),
  );
  $data['markaspot_log']['table']['entity type'] = 'markaspot_log';

  // Provide a "default relationship" to keep older views from choking.
  $data['markaspot_log']['table']['default_relationship'] = array(
    'node' => array(
      'table' => 'node',
      'field' => 'nid',
    ),
  );

  // This table references the {node} table. The declaration below creates an
  // 'implicit' relationship to the node table, so that when 'node' is the base
  // table, the fields are automatically available.
  $data['markaspot_log']['table']['join'] = array(

    // Index this array by the table name to which this table refers.
    // 'left_field' is the primary key in the referenced table.
    // 'field' is the foreign key in this table.
    'node' => array(
      'left_field' => 'nid',
      'field' => 'nid',
    ),
  );

  $data['markaspot_log']['nid'] = array(
    'title' => t('Nid'),
    'help' => t('The node ID the log message is related to.'),
    'relationship' => array(
      'title' => t('Report'),
      'help' => t('The report content to which the log message is related to.'),
      'base' => 'node',
      'base field' => 'nid',
      'handler' => 'views_handler_relationship',
      'label' => t('Report'),
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
    ),
  );

  $data['markaspot_log']['css_class'] = array(
    'title' => t('Log Message Css Class'),
    'help' => t('CSS class, eg. for icons'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => FALSE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );
  $data['markaspot_log']['level'] = array(
    'title' => t('Log Message Level'),
    'help' => t('The Log Level, mostly warning, info alert as known from Drupal'),
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
