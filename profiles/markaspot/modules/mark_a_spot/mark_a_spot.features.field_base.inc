<?php
/**
 * @file
 * mark_a_spot.features.field_base.inc
 */

/**
 * Implements hook_field_default_field_bases().
 */
function mark_a_spot_field_default_field_bases() {
  $field_bases = array();

  // Exported field_base: 'body'
  $field_bases['body'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(
      0 => 'node',
    ),
    'field_name' => 'body',
    'field_permissions' => array(
      'type' => 0,
    ),
    'foreign keys' => array(
      'format' => array(
        'columns' => array(
          'format' => 'format',
        ),
        'table' => 'filter_format',
      ),
    ),
    'indexes' => array(
      'format' => array(
        0 => 'format',
      ),
    ),
    'locked' => 0,
    'module' => 'text',
    'settings' => array(
      'field_permissions' => NULL,
    ),
    'translatable' => 0,
    'type' => 'text_with_summary',
  );

  // Exported field_base: 'comment_body'
  $field_bases['comment_body'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(
      0 => 'comment',
    ),
    'field_name' => 'comment_body',
    'field_permissions' => array(
      'type' => 0,
    ),
    'foreign keys' => array(
      'format' => array(
        'columns' => array(
          'format' => 'format',
        ),
        'table' => 'filter_format',
      ),
    ),
    'indexes' => array(
      'format' => array(
        0 => 'format',
      ),
    ),
    'locked' => 0,
    'module' => 'text',
    'settings' => array(),
    'translatable' => 0,
    'type' => 'text_long',
  );

  // Exported field_base: 'field_address'
  $field_bases['field_address'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_address',
    'field_permissions' => array(
      'type' => 0,
    ),
    'foreign keys' => array(
      'format' => array(
        'columns' => array(
          'format' => 'format',
        ),
        'table' => 'filter_format',
      ),
    ),
    'indexes' => array(
      'format' => array(
        0 => 'format',
      ),
    ),
    'locked' => 0,
    'module' => 'text',
    'settings' => array(
      'max_length' => 255,
    ),
    'translatable' => 0,
    'type' => 'text',
  );

  // Exported field_base: 'field_attachment'
  $field_bases['field_attachment'] = array(
    'active' => 1,
    'cardinality' => 3,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_attachment',
    'field_permissions' => array(
      'type' => 2,
    ),
    'foreign keys' => array(
      'fid' => array(
        'columns' => array(
          'fid' => 'fid',
        ),
        'table' => 'file_managed',
      ),
    ),
    'indexes' => array(
      'fid' => array(
        0 => 'fid',
      ),
    ),
    'locked' => 0,
    'module' => 'file',
    'settings' => array(
      'display_default' => 1,
      'display_field' => 1,
      'field_permissions' => NULL,
      'uri_scheme' => 'public',
    ),
    'translatable' => 0,
    'type' => 'file',
  );

  // Exported field_base: 'field_category'
  $field_bases['field_category'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_category',
    'field_permissions' => array(
      'type' => 0,
    ),
    'foreign keys' => array(
      'tid' => array(
        'columns' => array(
          'tid' => 'tid',
        ),
        'table' => 'taxonomy_term_data',
      ),
    ),
    'indexes' => array(
      'tid' => array(
        0 => 'tid',
      ),
    ),
    'locked' => 0,
    'module' => 'taxonomy',
    'settings' => array(
      'allowed_values' => array(
        0 => array(
          'vocabulary' => 'category',
          'parent' => 0,
        ),
      ),
      'field_permissions' => NULL,
    ),
    'translatable' => 0,
    'type' => 'taxonomy_term_reference',
  );

  // Exported field_base: 'field_category_hex'
  $field_bases['field_category_hex'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_category_hex',
    'field_permissions' => array(
      'type' => 0,
    ),
    'foreign keys' => array(),
    'indexes' => array(
      'rgb' => array(
        0 => 'rgb',
      ),
    ),
    'locked' => 0,
    'module' => 'color_field',
    'settings' => array(
      'default_colors' => '#FF0000, #8B0000, #FFA500, #008000, #006400, #0000FF, #00008B, #800080, #871F78, #5F9EA0, #D3D3D3, #808080, #000000, #F5F5DC, #FFFFFF',
    ),
    'translatable' => 0,
    'type' => 'color_field_rgb',
  );

  // Exported field_base: 'field_category_id'
  $field_bases['field_category_id'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_category_id',
    'field_permissions' => array(
      'type' => 0,
    ),
    'foreign keys' => array(
      'format' => array(
        'columns' => array(
          'format' => 'format',
        ),
        'table' => 'filter_format',
      ),
    ),
    'indexes' => array(
      'format' => array(
        0 => 'format',
      ),
    ),
    'locked' => 0,
    'module' => 'text',
    'settings' => array(
      'max_length' => 6,
    ),
    'translatable' => 0,
    'type' => 'text',
  );

  // Exported field_base: 'field_e_mail'
  $field_bases['field_e_mail'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_e_mail',
    'field_permissions' => array(
      'type' => 0,
    ),
    'foreign keys' => array(
      'format' => array(
        'columns' => array(
          'format' => 'format',
        ),
        'table' => 'filter_format',
      ),
    ),
    'indexes' => array(
      'format' => array(
        0 => 'format',
      ),
    ),
    'locked' => 0,
    'module' => 'text',
    'settings' => array(
      'max_length' => 255,
    ),
    'translatable' => 0,
    'type' => 'text',
  );

  // Exported field_base: 'field_geo'
  $field_bases['field_geo'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_geo',
    'field_permissions' => array(
      'type' => 0,
    ),
    'foreign keys' => array(),
    'indexes' => array(
      'lat' => array(
        0 => 'lat',
      ),
      'lng' => array(
        0 => 'lng',
      ),
    ),
    'locked' => 0,
    'module' => 'geolocation',
    'settings' => array(),
    'translatable' => 0,
    'type' => 'geolocation_latlng',
  );

  // Exported field_base: 'field_hash'
  $field_bases['field_hash'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_hash',
    'field_permissions' => array(
      'type' => 0,
    ),
    'foreign keys' => array(
      'format' => array(
        'columns' => array(
          'format' => 'format',
        ),
        'table' => 'filter_format',
      ),
    ),
    'indexes' => array(
      'format' => array(
        0 => 'format',
      ),
    ),
    'locked' => 0,
    'module' => 'text',
    'settings' => array(
      'max_length' => 255,
    ),
    'translatable' => 0,
    'type' => 'text',
  );

  // Exported field_base: 'field_status_icon'
  $field_bases['field_status_icon'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_status_icon',
    'field_permissions' => array(
      'type' => 0,
    ),
    'foreign keys' => array(),
    'indexes' => array(
      'value' => array(
        0 => 'value',
      ),
    ),
    'locked' => 0,
    'module' => 'list',
    'settings' => array(
      'allowed_values' => array(
        'brightness-medium' => 'brightness-medium',
        'remove' => 'remove',
        'bullhorn' => 'bullhorn',
        'droplet' => 'droplet',
        'location2' => 'location2',
        'location' => 'location',
        'office' => 'office',
        'bubbles2' => 'bubbles2',
        'bug' => 'bug',
        'leaf' => 'leaf',
        'lab' => 'lab',
        'car' => 'car',
        'pause' => 'pause',
        'bubbles' => 'bubbles',
        'checkmark' => 'checkmark',
        'play' => 'play',
        'close' => 'close',
        'headphones' => 'headphones',
        'drawer' => 'drawer',
        'sign' => 'sign',
        'tree' => 'tree',
        'wi-fi' => 'wi-fi',
        'warning' => 'warning',
        'info' => 'info',
        'phone' => 'phone',
        'grin' => 'grin',
        'umbrella' => 'umbrella',
        'truck' => 'truck',
        'exclamation-sign' => 'exclamation-sign',
        'road' => 'road',
        'medkit' => 'medkit',
        'heart' => 'heart',
        'envelope' => 'envelope',
        'sun' => 'sun',
        'fire-extinguisher' => 'fire-extinguisher',
        'thumbs-up' => 'thumbs-up',
        'thumbs-down' => 'thumbs-down',
        'bell' => 'bell',
        'twitter' => 'twitter',
        'facebook' => 'facebook',
        'heart-outline' => 'heart-outline',
        'directions' => 'directions',
        'feather' => 'feather',
        'share' => 'share',
        'bell2' => 'bell2',
        'moon' => 'moon',
        'new' => 'new',
        'shuffle' => 'shuffle',
        'switch' => 'switch',
        'cc' => 'cc',
        'pedestrian' => 'pedestrian',
        'bike' => 'bike',
        'motobike' => 'motobike',
        'skate' => 'skate',
        'trash' => 'trash',
        'headphone2' => 'headphone2',
        'camera' => 'camera',
        'minus-circle' => 'minus-circle',
        'remove-circle' => 'remove-circle',
        'check-circle' => 'check-circle',
        'info-circle' => 'info-circle',
        'remove-circle-o' => 'remove-circle-o',
        'check-circle-o' => 'check-circle-o',
        'eye' => 'eye',
        'warning2' => 'warning2',
        'wrench' => 'wrench',
        'flash' => 'flash',
        'building' => 'building',
        'fighter-jet' => 'fighter-jet',
        'recycle' => 'recycle',
        'bridge' => 'bridge',
        'graffiti' => 'graffiti',
        'wheelchair' => 'wheelchair',
      ),
      'allowed_values_function' => '',
    ),
    'translatable' => 0,
    'type' => 'list_text',
  );



  // Exported field_base: 'field_category_icon'
  $field_bases['field_category_icon'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_category_icon',
    'field_permissions' => array(
      'type' => 0,
    ),
    'foreign keys' => array(),
    'indexes' => array(
      'value' => array(
        0 => 'value',
      ),
    ),
    'locked' => 0,
    'module' => 'list',
    'settings' => array(
      'allowed_values' => array(
        'brightness-medium' => 'brightness-medium',
        'remove' => 'remove',
        'bullhorn' => 'bullhorn',
        'droplet' => 'droplet',
        'location2' => 'location2',
        'location' => 'location',
        'office' => 'office',
        'bubbles2' => 'bubbles2',
        'bug' => 'bug',
        'leaf' => 'leaf',
        'lab' => 'lab',
        'car' => 'car',
        'pause' => 'pause',
        'bubbles' => 'bubbles',
        'checkmark' => 'checkmark',
        'play' => 'play',
        'close' => 'close',
        'headphones' => 'headphones',
        'drawer' => 'drawer',
        'sign' => 'sign',
        'tree' => 'tree',
        'wi-fi' => 'wi-fi',
        'warning' => 'warning',
        'info' => 'info',
        'phone' => 'phone',
        'grin' => 'grin',
        'umbrella' => 'umbrella',
        'truck' => 'truck',
        'exclamation-sign' => 'exclamation-sign',
        'road' => 'road',
        'medkit' => 'medkit',
        'heart' => 'heart',
        'envelope' => 'envelope',
        'sun' => 'sun',
        'fire-extinguisher' => 'fire-extinguisher',
        'thumbs-up' => 'thumbs-up',
        'thumbs-down' => 'thumbs-down',
        'bell' => 'bell',
        'twitter' => 'twitter',
        'facebook' => 'facebook',
        'heart-outline' => 'heart-outline',
        'directions' => 'directions',
        'feather' => 'feather',
        'share' => 'share',
        'bell2' => 'bell2',
        'moon' => 'moon',
        'new' => 'new',
        'shuffle' => 'shuffle',
        'switch' => 'switch',
        'cc' => 'cc',
        'pedestrian' => 'pedestrian',
        'bike' => 'bike',
        'motobike' => 'motobike',
        'skate' => 'skate',
        'trash' => 'trash',
        'headphone2' => 'headphone2',
        'camera' => 'camera',
        'minus-circle' => 'minus-circle',
        'remove-circle' => 'remove-circle',
        'check-circle' => 'check-circle',
        'info-circle' => 'info-circle',
        'remove-circle-o' => 'remove-circle-o',
        'check-circle-o' => 'check-circle-o',
        'eye' => 'eye',
        'warning2' => 'warning2',
        'wrench' => 'wrench',
        'flash' => 'flash',
        'building' => 'building',
        'fighter-jet' => 'fighter-jet',
        'recycle' => 'recycle',
        'bridge' => 'bridge',
        'graffiti' => 'graffiti',
        'wheelchair' => 'wheelchair',
      ),
      'allowed_values_function' => '',
    ),
    'translatable' => 0,
    'type' => 'list_text',
  );

  // Exported field_base: 'field_image'
  $field_bases['field_image'] = array(
    'active' => 1,
    'cardinality' => 3,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_image',
    'field_permissions' => array(
      'type' => 0,
    ),
    'foreign keys' => array(
      'fid' => array(
        'columns' => array(
          'fid' => 'fid',
        ),
        'table' => 'file_managed',
      ),
    ),
    'indexes' => array(
      'fid' => array(
        0 => 'fid',
      ),
    ),
    'locked' => 0,
    'module' => 'image',
    'settings' => array(
      'default_image' => 0,
      'field_permissions' => NULL,
      'uri_scheme' => 'public',
    ),
    'translatable' => 0,
    'type' => 'image',
  );

  // Exported field_base: 'field_note'
  $field_bases['field_note'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_note',
    'field_permissions' => array(
      'type' => 2,
    ),
    'foreign keys' => array(
      'format' => array(
        'columns' => array(
          'format' => 'format',
        ),
        'table' => 'filter_format',
      ),
    ),
    'indexes' => array(
      'format' => array(
        0 => 'format',
      ),
    ),
    'locked' => 0,
    'module' => 'text',
    'settings' => array(),
    'translatable' => 0,
    'type' => 'text_long',
  );

  // Exported field_base: 'field_notify_user'
  $field_bases['field_notify_user'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_notify_user',
    'field_permissions' => array(
      'type' => 2,
    ),
    'foreign keys' => array(),
    'indexes' => array(
      'value' => array(
        0 => 'value',
      ),
    ),
    'locked' => 0,
    'module' => 'list',
    'settings' => array(
      'allowed_values' => array(
        0 => 0,
        1 => 1,
      ),
      'allowed_values_function' => '',
    ),
    'translatable' => 0,
    'type' => 'list_boolean',
  );

  // Exported field_base: 'field_statement'
  $field_bases['field_statement'] = array(
    'active' => 1,
    'cardinality' => -1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_statement',
    'field_permissions' => array(
      'type' => 2,
    ),
    'foreign keys' => array(
      'nid' => array(
        'columns' => array(
          'nid' => 'nid',
        ),
        'table' => 'node',
      ),
    ),
    'indexes' => array(
      'nid' => array(
        0 => 'nid',
      ),
    ),
    'locked' => 0,
    'module' => 'node_reference',
    'settings' => array(
      'referenceable_types' => array(
        'article' => 0,
        'page' => 0,
        'report' => 0,
        'statement' => 'statement',
      ),
      'view' => array(
        'args' => array(),
        'display_name' => '',
        'view_name' => '',
      ),
    ),
    'translatable' => 0,
    'type' => 'node_reference',
  );

  // Exported field_base: 'field_status'
  $field_bases['field_status'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_status',
    'field_permissions' => array(
      'type' => 2,
    ),
    'foreign keys' => array(
      'tid' => array(
        'columns' => array(
          'tid' => 'tid',
        ),
        'table' => 'taxonomy_term_data',
      ),
    ),
    'indexes' => array(
      'tid' => array(
        0 => 'tid',
      ),
    ),
    'locked' => 0,
    'module' => 'taxonomy',
    'settings' => array(
      'allowed_values' => array(
        0 => array(
          'vocabulary' => 'status',
          'parent' => 0,
        ),
      ),
      'field_permissions' => NULL,
    ),
    'translatable' => 0,
    'type' => 'taxonomy_term_reference',
  );

  // Exported field_base: 'field_status_hex'
  $field_bases['field_status_hex'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_status_hex',
    'field_permissions' => array(
      'type' => 0,
    ),
    'foreign keys' => array(),
    'indexes' => array(
      'rgb' => array(
        0 => 'rgb',
      ),
    ),
    'locked' => 0,
    'module' => 'color_field',
    'settings' => array(
      'default_colors' => '#FF0000, #8B0000, #FFA500, #008000, #006400, #0000FF, #00008B, #800080, #871F78, #5F9EA0, #D3D3D3, #808080, #000000, #F5F5DC, #FFFFFF',
    ),
    'translatable' => 0,
    'type' => 'color_field_rgb',
  );

  // Exported field_base: 'field_status_id'
  $field_bases['field_status_id'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_status_id',
    'field_permissions' => array(
      'type' => 0,
    ),
    'foreign keys' => array(),
    'indexes' => array(),
    'locked' => 0,
    'module' => 'number',
    'settings' => array(),
    'translatable' => 0,
    'type' => 'number_integer',
  );

  // Exported field_base: 'field_tags'
  $field_bases['field_tags'] = array(
    'active' => 1,
    'cardinality' => -1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_tags',
    'field_permissions' => array(
      'type' => 1,
    ),
    'foreign keys' => array(
      'tid' => array(
        'columns' => array(
          'tid' => 'tid',
        ),
        'table' => 'taxonomy_term_data',
      ),
    ),
    'indexes' => array(
      'tid' => array(
        0 => 'tid',
      ),
    ),
    'locked' => 0,
    'module' => 'taxonomy',
    'settings' => array(
      'allowed_values' => array(
        0 => array(
          'vocabulary' => 'tags',
          'parent' => 0,
        ),
      ),
    ),
    'translatable' => 0,
    'type' => 'taxonomy_term_reference',
  );

  return $field_bases;
}
