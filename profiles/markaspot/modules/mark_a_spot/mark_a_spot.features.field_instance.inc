<?php
/**
 * @file
 * mark_a_spot.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function mark_a_spot_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance: 'comment-comment_node_report-comment_body'
  $field_instances['comment-comment_node_report-comment_body'] = array(
    'bundle' => 'comment_node_report',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(
          'field_formatter_class' => '',
          'semantic_field_format' => 0,
        ),
        'type' => 'text_default',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'comment',
    'field_name' => 'comment_body',
    'label' => 'Comment',
    'required' => 1,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'maxlength_js' => 500,
        'maxlength_js_enforce' => 0,
        'maxlength_js_label' => 'Content limited to @limit characters, remaining: <strong>@remaining</strong>',
        'maxlength_js_truncate_html' => 0,
        'rows' => 10,
      ),
      'type' => 'text_textarea',
      'weight' => 0,
    ),
  );

  // Exported field_instance: 'node-report-body'
  $field_instances['node-report-body'] = array(
    'bundle' => 'report',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Enter details of the problem',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(
          'field_formatter_class' => '',
        ),
        'type' => 'text_default',
        'weight' => 2,
      ),
      'example_node_list' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'full' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(
          'field_formatter_class' => '',
          'semantic_field_format' => 0,
        ),
        'type' => 'text_default',
        'weight' => 0,
      ),
      'report_list' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(
          'field_formatter_class' => '',
          'trim_length' => 300,
        ),
        'type' => 'text_trimmed',
        'weight' => 2,
      ),
      'search_result' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(
          'field_formatter_class' => '',
          'semantic_field_format' => 0,
        ),
        'type' => 'text_default',
        'weight' => 0,
      ),
      'status_list' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(
          'field_formatter_class' => '',
          'semantic_field_format' => 0,
        ),
        'type' => 'text_default',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(
          'field_formatter_class' => '',
        ),
        'type' => 'text_default',
        'weight' => 1,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'body',
    'label' => 'Report details',
    'required' => 0,
    'settings' => array(
      'display_summary' => 0,
      'text_processing' => 1,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 5,
        'summary_rows' => 5,
      ),
      'type' => 'text_textarea_with_summary',
      'weight' => -3,
    ),
  );

  // Exported field_instance: 'node-report-field_address'
  $field_instances['node-report-field_address'] = array(
    'bundle' => 'report',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(
          'field_formatter_class' => '',
        ),
        'type' => 'text_plain',
        'weight' => 5,
      ),
      'report_list' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(
          'field_formatter_class' => '',
        ),
        'type' => 'text_plain',
        'weight' => 3,
      ),
      'teaser' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(
          'field_formatter_class' => '',
        ),
        'type' => 'text_default',
        'weight' => 3,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_address',
    'label' => 'Address',
    'required' => 0,
    'settings' => array(
      'better_formats' => array(
        'allowed_formats' => array(
          'filtered_html' => 'filtered_html',
          'full_html' => 'full_html',
          'plain_text' => 'plain_text',
        ),
        'allowed_formats_toggle' => 0,
        'default_order_toggle' => 0,
        'default_order_wrapper' => array(
          'formats' => array(
            'filtered_html' => array(
              'weight' => 0,
            ),
            'full_html' => array(
              'weight' => 1,
            ),
            'plain_text' => array(
              'weight' => 10,
            ),
          ),
        ),
      ),
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 16,
    ),
  );

  // Exported field_instance: 'node-report-field_attachment'
  $field_instances['node-report-field_attachment'] = array(
    'bundle' => 'report',
    'deleted' => 0,
    'description' => 'This field can be used for attaching official documents',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 9,
      ),
      'example_node_list' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'full' => array(
        'label' => 'above',
        'module' => 'file',
        'settings' => array(
          'field_formatter_class' => '',
          'semantic_field_format' => 0,
        ),
        'type' => 'file_table',
        'weight' => 9,
      ),
      'report_list' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 9,
      ),
      'search_result' => array(
        'label' => 'above',
        'module' => 'file',
        'settings' => array(
          'field_formatter_class' => '',
          'semantic_field_format' => 0,
        ),
        'type' => 'file_table',
        'weight' => 3,
      ),
      'status_list' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 8,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 9,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_attachment',
    'label' => 'Attachments and Documents',
    'required' => 0,
    'settings' => array(
      'description_field' => 1,
      'file_directory' => '',
      'file_extensions' => 'txt jpg png gif pdf',
      'max_filesize' => '6MB',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'file',
      'settings' => array(
        'progress_indicator' => 'throbber',
      ),
      'type' => 'file_generic',
      'weight' => 3,
    ),
  );

  // Exported field_instance: 'node-report-field_category'
  $field_instances['node-report-field_category'] = array(
    'bundle' => 'report',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'taxonomy',
        'settings' => array(
          'field_formatter_class' => '',
        ),
        'type' => 'taxonomy_term_reference_plain',
        'weight' => 3,
      ),
      'full' => array(
        'label' => 'above',
        'module' => 'taxonomy',
        'settings' => array(
          'field_formatter_class' => '',
          'semantic_field_format' => 0,
        ),
        'type' => 'taxonomy_term_reference_plain',
        'weight' => 4,
      ),
      'report_list' => array(
        'label' => 'hidden',
        'module' => 'taxonomy',
        'settings' => array(
          'field_formatter_class' => '',
        ),
        'type' => 'taxonomy_term_reference_plain',
        'weight' => 5,
      ),
      'search_result' => array(
        'label' => 'above',
        'module' => 'taxonomy',
        'settings' => array(
          'field_formatter_class' => '',
          'semantic_field_format' => 0,
        ),
        'type' => 'taxonomy_term_reference_link',
        'weight' => 4,
      ),
      'status_list' => array(
        'label' => 'above',
        'module' => 'taxonomy',
        'settings' => array(
          'field_formatter_class' => '',
          'semantic_field_format' => 0,
        ),
        'type' => 'taxonomy_term_reference_link',
        'weight' => 2,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 8,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_category',
    'label' => 'Category',
    'required' => 1,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'apply_chosen' => 0,
      ),
      'type' => 'options_select',
      'weight' => -4,
    ),
  );

  // Exported field_instance: 'node-report-field_e_mail'
  $field_instances['node-report-field_e_mail'] = array(
    'bundle' => 'report',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Please leave an e-Mail address, so that we can contact you, in case you don\'t want to register',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 8,
      ),
      'full' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'report_list' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 8,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 6,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_e_mail',
    'label' => 'E-Mail',
    'required' => 1,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 8,
    ),
  );

  // Exported field_instance: 'node-report-field_geo'
  $field_instances['node-report-field_geo'] = array(
    'bundle' => 'report',
    'default_value' => array(
      0 => array(
        'address' => array(
          'field' => 'Fritz-Wündisch-Straße 52, 50321 Brühl',
        ),
        'lat' => '50.81834062114927',
        'lng' => '6.8978047370910645',
      ),
    ),
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'geolocation_osm',
        'settings' => array(
          'field_formatter_class' => '',
          'leafletImagePath' => '/profiles/markaspot/libraries/leaflet/images',
          'map_dimensions' => '100%x300px',
          'map_scrollwheel' => 1,
          'map_zoomlevel' => 16,
          'marker_icon' => 'profiles/markaspot/libraries/leaflet/images/marker-icon.png',
          'tile_server_attribution' => '<a href="http://www.openstreetmap.org/copyright">OpenStreetMap</a> contributors, &copy; <a href="http://cartodb.com/attributions">CartoDB</a>',
          'tile_server_dynamic' => 'http://{s}.basemaps.cartocdn.com/light_all/{z}/{x}/{y}.png',
        ),
        'type' => 'geolocation_osm_dynamic',
        'weight' => 1,
      ),
      'example_node_list' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'full' => array(
        'label' => 'above',
        'module' => 'geolocation_googlemaps',
        'settings' => array(
          'field_formatter_class' => '',
          'map_height' => '300px',
          'map_imageformat' => 'png',
          'map_link' => 0,
          'map_maptype' => 'roadmap',
          'map_width' => '300px',
          'map_zoomlevel' => 17,
          'marker_icon' => '',
          'semantic_field_format' => 0,
        ),
        'type' => 'geolocation_googlemaps_static',
        'weight' => 1,
      ),
      'report_list' => array(
        'label' => 'hidden',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 1,
      ),
      'search_result' => array(
        'label' => 'above',
        'module' => 'geolocation_googlemaps',
        'settings' => array(
          'field_formatter_class' => '',
          'map_height' => '300px',
          'map_imageformat' => 'png32',
          'map_link' => 0,
          'map_maptype' => 'roadmap',
          'map_width' => '300px',
          'map_zoomlevel' => 17,
          'marker_icon' => '',
          'semantic_field_format' => 0,
        ),
        'type' => 'geolocation_googlemaps_static',
        'weight' => 1,
      ),
      'status_list' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 7,
      ),
      'teaser' => array(
        'label' => 'hidden',
        'module' => 'geolocation_osm',
        'settings' => array(
          'field_formatter_class' => 'pull-left',
          'leafletImagePath' => 'profiles/markaspot/libraries/leaflet/images/',
          'map_dimensions' => '90x90',
          'map_imageformat' => 'png',
          'map_key' => '',
          'map_zoomlevel' => 13,
          'mapbox' => 0,
          'marker_icon' => '',
          'tile_server_static' => 'http://staticmap.openstreetmap.de/staticmap.php',
        ),
        'type' => 'geolocation_osm_static',
        'weight' => 2,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_geo',
    'label' => 'Place your report',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'geolocation_osm',
      'settings' => array(
        'bbox' => '6.832,50.8464,6.937,50.8043',
        'geocode_service' => 'http://nominatim.openstreetmap.org',
        'leafletImagePath' => '/profiles/markaspot/libraries/leaflet/images',
        'map_key' => '',
        'map_zoomlevel' => 0,
        'mapbox' => 0,
        'marker_draggable' => 1,
        'nominatimEmail' => 'holger@markaspot.org',
        'scrollwheel' => 0,
        'tile_server_attribution' => '<a href="http://www.openstreetmap.org/copyright">OpenStreetMap</a> contributors, &copy; <a href="http://cartodb.com/attributions">CartoDB</a>',
        'tile_server_dynamic' => 'http://{s}.basemaps.cartocdn.com/light_all/{z}/{x}/{y}.png',
      ),
      'type' => 'geolocation_osm',
      'weight' => 15,
    ),
  );

  // Exported field_instance: 'node-report-field_image'
  $field_instances['node-report-field_image'] = array(
    'bundle' => 'report',
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'image',
        'settings' => array(
          'field_formatter_class' => 'img-thumbnail',
          'image_link' => 'content',
          'image_style' => 'mas_medium',
        ),
        'type' => 'image',
        'weight' => 0,
      ),
      'full' => array(
        'label' => 'above',
        'module' => 'image',
        'settings' => array(
          'field_formatter_class' => '',
          'image_link' => 'file',
          'image_style' => 'medium',
          'semantic_field_format' => 0,
        ),
        'type' => 'image',
        'weight' => 2,
      ),
      'report_list' => array(
        'label' => 'hidden',
        'module' => 'image',
        'settings' => array(
          'field_formatter_class' => 'pull-left',
          'image_link' => '',
          'image_style' => 'mas_thumb',
        ),
        'type' => 'image',
        'weight' => 0,
      ),
      'search_result' => array(
        'label' => 'above',
        'module' => 'image',
        'settings' => array(
          'field_formatter_class' => '',
          'image_link' => 'file',
          'image_style' => 'medium',
          'semantic_field_format' => 0,
        ),
        'type' => 'image',
        'weight' => 2,
      ),
      'status_list' => array(
        'label' => 'above',
        'module' => 'image',
        'settings' => array(
          'field_formatter_class' => '',
          'image_link' => 'file',
          'image_style' => 'medium',
          'semantic_field_format' => 0,
        ),
        'type' => 'image',
        'weight' => 1,
      ),
      'teaser' => array(
        'label' => 'hidden',
        'module' => 'image',
        'settings' => array(
          'field_formatter_class' => '',
          'image_link' => '',
          'image_style' => 'mas_thumb',
        ),
        'type' => 'image',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_image',
    'label' => 'Image',
    'required' => 0,
    'settings' => array(
      'alt_field' => 0,
      'default_image' => 0,
      'file_directory' => '',
      'file_extensions' => 'png gif jpg jpeg',
      'max_filesize' => '3 MB',
      'max_resolution' => '2048x2048',
      'min_resolution' => '200x200',
      'title_field' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'image',
      'settings' => array(
        'preview_image_style' => 'medium',
        'progress_indicator' => 'throbber',
      ),
      'type' => 'image_image',
      'weight' => 7,
    ),
  );

  // Exported field_instance: 'node-report-field_notes'
  $field_instances['node-report-field_notes'] = array(
    'bundle' => 'report',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(
          'field_formatter_class' => '',
        ),
        'type' => 'text_default',
        'weight' => 10,
      ),
      'report_list' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_notes',
    'label' => 'Internal Notes',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 5,
      ),
      'type' => 'text_textarea',
      'weight' => 19,
    ),
  );

  // Exported field_instance: 'node-report-field_notify_user'
  $field_instances['node-report-field_notify_user'] = array(
    'bundle' => 'report',
    'default_value' => array(
      0 => array(
        'value' => 0,
      ),
    ),
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 7,
      ),
      'report_list' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 6,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 4,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_notify_user',
    'label' => 'Notify User?',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'display_label' => 1,
      ),
      'type' => 'options_onoff',
      'weight' => 18,
    ),
  );

  // Exported field_instance: 'node-report-field_statement'
  $field_instances['node-report-field_statement'] = array(
    'bundle' => 'report',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'node_reference',
        'settings' => array(
          'field_formatter_class' => '',
        ),
        'type' => 'node_reference_default',
        'weight' => 6,
      ),
      'report_list' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 7,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 5,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_statement',
    'label' => 'Adminstration statement',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 5,
      ),
      'type' => 'text_textarea',
      'weight' => 17,
    ),
  );

  // Exported field_instance: 'node-report-field_statement_comment'
  $field_instances['node-report-field_statement_comment'] = array(
    'bundle' => 'report',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Use this field in case you want to enunciate something more freely',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(
          'field_formatter_class' => '',
        ),
        'type' => 'text_default',
        'weight' => 11,
      ),
      'report_list' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_statement_comment',
    'label' => 'Administration Comment',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 5,
      ),
      'type' => 'text_textarea',
      'weight' => 20,
    ),
  );

  // Exported field_instance: 'node-report-field_status'
  $field_instances['node-report-field_status'] = array(
    'bundle' => 'report',
    'default_value' => array(
      0 => array(
        'tid' => 5,
      ),
    ),
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'taxonomy',
        'settings' => array(
          'field_formatter_class' => '',
        ),
        'type' => 'taxonomy_term_reference_plain',
        'weight' => 4,
      ),
      'full' => array(
        'label' => 'above',
        'module' => 'taxonomy',
        'settings' => array(
          'field_formatter_class' => '',
          'semantic_field_format' => 0,
        ),
        'type' => 'taxonomy_term_reference_plain',
        'weight' => 5,
      ),
      'report_list' => array(
        'label' => 'hidden',
        'module' => 'taxonomy',
        'settings' => array(
          'field_formatter_class' => '',
        ),
        'type' => 'taxonomy_term_reference_plain',
        'weight' => 4,
      ),
      'search_result' => array(
        'label' => 'above',
        'module' => 'taxonomy',
        'settings' => array(
          'field_formatter_class' => '',
          'semantic_field_format' => 0,
        ),
        'type' => 'taxonomy_term_reference_link',
        'weight' => 5,
      ),
      'status_list' => array(
        'label' => 'above',
        'module' => 'taxonomy',
        'settings' => array(
          'field_formatter_class' => '',
          'semantic_field_format' => 0,
        ),
        'type' => 'taxonomy_term_reference_link',
        'weight' => 3,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 7,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_status',
    'label' => 'Status',
    'required' => 1,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_buttons',
      'weight' => 5,
    ),
  );

  // Exported field_instance: 'taxonomy_term-category-field_category_hex'
  $field_instances['taxonomy_term-category-field_category_hex'] = array(
    'bundle' => 'category',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'color_field',
        'settings' => array(
          'field_formatter_class' => '',
        ),
        'type' => 'color_field_default_formatter',
        'weight' => 6,
      ),
    ),
    'entity_type' => 'taxonomy_term',
    'field_name' => 'field_category_hex',
    'label' => 'Marker and Background Color',
    'required' => 1,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'color_field',
      'settings' => array(),
      'type' => 'color_field_default_widget',
      'weight' => 31,
    ),
  );

  // Exported field_instance: 'taxonomy_term-category-field_category_icon'
  $field_instances['taxonomy_term-category-field_category_icon'] = array(
    'bundle' => 'category',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'markaspot_icon_field',
        'settings' => array(
          'field_formatter_class' => 'blah',
        ),
        'type' => 'markaspot_icon_field_default',
        'weight' => 7,
      ),
    ),
    'entity_type' => 'taxonomy_term',
    'field_name' => 'field_category_icon',
    'label' => 'Icon',
    'required' => 0,
    'settings' => array(
      'bundle' => NULL,
      'icon' => NULL,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'markaspot_icon_field',
      'settings' => array(),
      'type' => 'markaspot_icon_field',
      'weight' => 32,
    ),
  );

  // Exported field_instance: 'taxonomy_term-category-field_category_id'
  $field_instances['taxonomy_term-category-field_category_id'] = array(
    'bundle' => 'category',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'field_formatter_css_class',
        'settings' => array(
          'depth' => 0,
          'field_formatter_class' => '',
          'hidden' => 1,
          'target' => 'node',
        ),
        'type' => 'cssclass_formatter',
        'weight' => 5,
      ),
      'full' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(
          'field_formatter_class' => '',
          'semantic_field_format' => 0,
        ),
        'type' => 'text_default',
        'weight' => 1,
      ),
      'token' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(
          'field_formatter_class' => '',
          'semantic_field_format' => 0,
        ),
        'type' => 'text_default',
        'weight' => 1,
      ),
    ),
    'entity_type' => 'taxonomy_term',
    'field_name' => 'field_category_id',
    'label' => 'ID',
    'required' => 1,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 6,
      ),
      'type' => 'text_textfield',
      'weight' => 2,
    ),
  );

  // Exported field_instance: 'taxonomy_term-category-field_hash'
  $field_instances['taxonomy_term-category-field_hash'] = array(
    'bundle' => 'category',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'This field applies to the tweet311 module. A Twitter-hashtag that citizens may use, applying to a certain service/service code/category',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(
          'field_formatter_class' => '',
        ),
        'type' => 'text_default',
        'weight' => 2,
      ),
      'full' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(
          'field_formatter_class' => '',
        ),
        'type' => 'text_default',
        'weight' => 3,
      ),
    ),
    'entity_type' => 'taxonomy_term',
    'field_name' => 'field_hash',
    'label' => 'Hash',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 4,
    ),
  );

  // Exported field_instance: 'taxonomy_term-category-field_tags'
  $field_instances['taxonomy_term-category-field_tags'] = array(
    'bundle' => 'category',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'taxonomy',
        'settings' => array(
          'field_formatter_class' => '',
        ),
        'type' => 'taxonomy_term_reference_link',
        'weight' => 1,
      ),
      'full' => array(
        'label' => 'above',
        'module' => 'taxonomy',
        'settings' => array(
          'field_formatter_class' => '',
        ),
        'type' => 'taxonomy_term_reference_link',
        'weight' => 2,
      ),
    ),
    'entity_type' => 'taxonomy_term',
    'field_name' => 'field_tags',
    'label' => 'Keywords',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'taxonomy',
      'settings' => array(
        'autocomplete_path' => 'taxonomy/autocomplete',
        'size' => 60,
      ),
      'type' => 'taxonomy_autocomplete',
      'weight' => 4,
    ),
  );

  // Exported field_instance: 'taxonomy_term-status-field_status_hex'
  $field_instances['taxonomy_term-status-field_status_hex'] = array(
    'bundle' => 'status',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'color_field',
        'settings' => array(
          'field_formatter_class' => '',
        ),
        'type' => 'color_field_default_formatter',
        'weight' => 6,
      ),
    ),
    'entity_type' => 'taxonomy_term',
    'field_name' => 'field_status_hex',
    'label' => 'Marker and Background Color',
    'required' => 1,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'color_field',
      'settings' => array(),
      'type' => 'color_field_default_widget',
      'weight' => 31,
    ),
  );

  // Exported field_instance: 'taxonomy_term-status-field_status_icon'
  $field_instances['taxonomy_term-status-field_status_icon'] = array(
    'bundle' => 'status',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'markaspot_icon_field',
        'settings' => array(
          'field_formatter_class' => '',
          'link_to_content' => FALSE,
        ),
        'type' => 'markaspot_icon_field_default',
        'weight' => 7,
      ),
    ),
    'entity_type' => 'taxonomy_term',
    'field_name' => 'field_status_icon',
    'label' => 'Icon',
    'required' => FALSE,
    'settings' => array(
      'bundle' => NULL,
      'icon' => NULL,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'module' => 'markaspot_icon_field',
      'settings' => array(),
      'type' => 'markaspot_icon_field',
      'weight' => 32,
    ),
  );

  // Exported field_instance: 'taxonomy_term-status-field_status_id'
  $field_instances['taxonomy_term-status-field_status_id'] = array(
    'bundle' => 'status',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'taxonomy_term',
    'field_name' => 'field_status_id',
    'label' => 'ID',
    'required' => 0,
    'settings' => array(
      'max' => '',
      'min' => '',
      'prefix' => '',
      'suffix' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'number',
      'settings' => array(),
      'type' => 'number',
      'weight' => 1,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Address');
  t('Administration Comment');
  t('Adminstration statement');
  t('Attachments and Documents');
  t('Category');
  t('Comment');
  t('E-Mail');
  t('Enter details of the problem');
  t('Hash');
  t('ID');
  t('Icon');
  t('Image');
  t('Internal Notes');
  t('Keywords');
  t('Marker and Background Color');
  t('Notify User?');
  t('Place your report');
  t('Please leave an e-Mail address, so that we can contact you, in case you don\'t want to register');
  t('Report details');
  t('Status');
  t('This field applies to the tweet311 module. A Twitter-hashtag that citizens may use, applying to a certain service/service code/category');
  t('This field can be used for attaching official documents');
  t('Use this field in case you want to enunciate something more freely');

  return $field_instances;
}
