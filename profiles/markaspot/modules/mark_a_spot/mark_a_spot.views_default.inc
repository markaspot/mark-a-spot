<?php
/**
 * @file
 * mark_a_spot.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function mark_a_spot_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'Gallery';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Gallery';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Beiträge mit Bild';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['use_more_text'] = 'mehr';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['distinct'] = TRUE;
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['submit_button'] = 'Anwenden';
  $handler->display->display_options['exposed_form']['options']['reset_button_label'] = 'Zurücksetzen';
  $handler->display->display_options['exposed_form']['options']['exposed_sorts_label'] = 'Sortieren nach';
  $handler->display->display_options['exposed_form']['options']['sort_asc_label'] = 'Aufsteigend';
  $handler->display->display_options['exposed_form']['options']['sort_desc_label'] = 'Absteigend';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '6';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'list';
  $handler->display->display_options['style_options']['row_class'] = 'span3';
  $handler->display->display_options['style_options']['default_row_class'] = FALSE;
  $handler->display->display_options['style_options']['row_class_special'] = FALSE;
  $handler->display->display_options['style_options']['class'] = 'thumbnails';
  $handler->display->display_options['style_options']['wrapper_class'] = 'gallery';
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['row_options']['inline'] = array(
    'title' => 'title',
  );
  $handler->display->display_options['row_options']['default_field_elements'] = FALSE;
  /* Field: Content: Image */
  $handler->display->display_options['fields']['field_image_1']['id'] = 'field_image_1';
  $handler->display->display_options['fields']['field_image_1']['table'] = 'field_data_field_image';
  $handler->display->display_options['fields']['field_image_1']['field'] = 'field_image';
  $handler->display->display_options['fields']['field_image_1']['label'] = '';
  $handler->display->display_options['fields']['field_image_1']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_image_1']['element_class'] = 'thumbnail';
  $handler->display->display_options['fields']['field_image_1']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_image_1']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['field_image_1']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_image_1']['settings'] = array(
    'image_style' => 'mas_medium',
    'image_link' => 'content',
  );
  $handler->display->display_options['fields']['field_image_1']['group_column'] = 'fid';
  $handler->display->display_options['fields']['field_image_1']['delta_limit'] = '1';
  $handler->display->display_options['fields']['field_image_1']['delta_offset'] = '0';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['exclude'] = TRUE;
  $handler->display->display_options['fields']['title']['alter']['max_length'] = '20';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['strip_tags'] = TRUE;
  $handler->display->display_options['fields']['title']['alter']['trim'] = TRUE;
  $handler->display->display_options['fields']['title']['alter']['preserve_tags'] = '<a>';
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['title']['element_default_classes'] = FALSE;
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = '';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '    <div class="thumbnail">
      [field_image_1] 
      <!-- h3>[title]</h3 -->
    </div>';
  $handler->display->display_options['fields']['nothing']['element_label_colon'] = FALSE;
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 0;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'report' => 'report',
  );
  /* Filter criterion: Content: Image (field_image:fid) */
  $handler->display->display_options['filters']['field_image_fid']['id'] = 'field_image_fid';
  $handler->display->display_options['filters']['field_image_fid']['table'] = 'field_data_field_image';
  $handler->display->display_options['filters']['field_image_fid']['field'] = 'field_image_fid';
  $handler->display->display_options['filters']['field_image_fid']['operator'] = 'not empty';
  $handler->display->display_options['filters']['field_image_fid']['group'] = '0';

  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block');

  /* Display: Seite */
  $handler = $view->new_display('page', 'Seite', 'page_1');
  $handler->display->display_options['path'] = 'reports/gallery';
  $export['Gallery'] = $view;

  $view = new view();
  $view->name = 'markers';
  $view->description = 'View for all type of reports';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Mark-a-Spot reports';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Reports Mark-a-Spot';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['use_more_text'] = 'mehr';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['submit_button'] = 'Anwenden';
  $handler->display->display_options['exposed_form']['options']['reset_button_label'] = 'Zurücksetzen';
  $handler->display->display_options['exposed_form']['options']['exposed_sorts_label'] = 'Sortieren nach';
  $handler->display->display_options['exposed_form']['options']['sort_asc_label'] = 'Aufsteigend';
  $handler->display->display_options['exposed_form']['options']['sort_desc_label'] = 'Absteigend';
  $handler->display->display_options['exposed_form']['options']['autosubmit'] = TRUE;
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['style_options']['grouping'] = '';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  /* Relationship: Content: Choose Service (field_category) */
  $handler->display->display_options['relationships']['field_category_tid']['id'] = 'field_category_tid';
  $handler->display->display_options['relationships']['field_category_tid']['table'] = 'field_data_field_category';
  $handler->display->display_options['relationships']['field_category_tid']['field'] = 'field_category_tid';
  /* Relationship: Content: Status (field_status) */
  $handler->display->display_options['relationships']['field_status_tid']['id'] = 'field_status_tid';
  $handler->display->display_options['relationships']['field_status_tid']['table'] = 'field_data_field_status';
  $handler->display->display_options['relationships']['field_status_tid']['field'] = 'field_status_tid';
  /* Relationship: Content: Author */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'node';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Content: Choose Service */
  $handler->display->display_options['fields']['field_category']['id'] = 'field_category';
  $handler->display->display_options['fields']['field_category']['table'] = 'field_data_field_category';
  $handler->display->display_options['fields']['field_category']['field'] = 'field_category';
  $handler->display->display_options['fields']['field_category']['label'] = 'categoryTitle';
  $handler->display->display_options['fields']['field_category']['alter']['max_length'] = '[]';
  $handler->display->display_options['fields']['field_category']['element_type'] = 'h5';
  $handler->display->display_options['fields']['field_category']['element_class'] = 'marker-category [field_category_hex]';
  $handler->display->display_options['fields']['field_category']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['field_category']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['field_category']['type'] = 'taxonomy_term_reference_plain';
  /* Field: Node Id */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['ui_name'] = 'Node Id';
  $handler->display->display_options['fields']['nid']['label'] = 'nid';
  $handler->display->display_options['fields']['nid']['hide_alter_empty'] = FALSE;
  /* Field: Content: Updated date */
  $handler->display->display_options['fields']['changed']['id'] = 'changed';
  $handler->display->display_options['fields']['changed']['table'] = 'node';
  $handler->display->display_options['fields']['changed']['field'] = 'changed';
  $handler->display->display_options['fields']['changed']['label'] = 'aktualisiert';
  $handler->display->display_options['fields']['changed']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['changed']['date_format'] = 'long';
  /* Field: Content: Post date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'node';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = 'created';
  $handler->display->display_options['fields']['created']['date_format'] = 'long';
  /* Field: Content: Status */
  $handler->display->display_options['fields']['field_status']['id'] = 'field_status';
  $handler->display->display_options['fields']['field_status']['table'] = 'field_data_field_status';
  $handler->display->display_options['fields']['field_status']['field'] = 'field_status';
  $handler->display->display_options['fields']['field_status']['label'] = 'statusTitle';
  $handler->display->display_options['fields']['field_status']['alter']['text'] = 'Status: [field_status]';
  $handler->display->display_options['fields']['field_status']['element_type'] = '0';
  $handler->display->display_options['fields']['field_status']['element_class'] = 'marker-status col-[field_status_hex]';
  $handler->display->display_options['fields']['field_status']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_status']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['field_status']['type'] = 'taxonomy_term_reference_plain';
  /* Field: Node Body */
  $handler->display->display_options['fields']['body']['id'] = 'body';
  $handler->display->display_options['fields']['body']['table'] = 'field_data_body';
  $handler->display->display_options['fields']['body']['field'] = 'body';
  $handler->display->display_options['fields']['body']['ui_name'] = 'Node Body';
  $handler->display->display_options['fields']['body']['label'] = 'description';
  $handler->display->display_options['fields']['body']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['body']['type'] = 'text_summary_or_trimmed';
  $handler->display->display_options['fields']['body']['settings'] = array(
    'trim_length' => '600',
  );
  /* Field: Status Hex Value */
  $handler->display->display_options['fields']['field_status_hex']['id'] = 'field_status_hex';
  $handler->display->display_options['fields']['field_status_hex']['table'] = 'field_data_field_status_hex';
  $handler->display->display_options['fields']['field_status_hex']['field'] = 'field_status_hex';
  $handler->display->display_options['fields']['field_status_hex']['relationship'] = 'field_status_tid';
  $handler->display->display_options['fields']['field_status_hex']['ui_name'] = 'Status Hex Value';
  $handler->display->display_options['fields']['field_status_hex']['hide_alter_empty'] = FALSE;
  /* Field: Taxonomy term: categoryHex */
  $handler->display->display_options['fields']['field_category_hex']['id'] = 'field_category_hex';
  $handler->display->display_options['fields']['field_category_hex']['table'] = 'field_data_field_category_hex';
  $handler->display->display_options['fields']['field_category_hex']['field'] = 'field_category_hex';
  $handler->display->display_options['fields']['field_category_hex']['relationship'] = 'field_category_tid';
  $handler->display->display_options['fields']['field_category_hex']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_category_hex']['element_class'] = '[field_category_hex]';
  $handler->display->display_options['fields']['field_category_hex']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_category_hex']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['field_category_hex']['type'] = 'text_plain';
  $handler->display->display_options['fields']['field_category_hex']['settings'] = array(
    'trim_length' => '600',
  );
  /* Field: Position Latitude */
  $handler->display->display_options['fields']['field_geo_1']['id'] = 'field_geo_1';
  $handler->display->display_options['fields']['field_geo_1']['table'] = 'field_data_field_geo';
  $handler->display->display_options['fields']['field_geo_1']['field'] = 'field_geo';
  $handler->display->display_options['fields']['field_geo_1']['ui_name'] = 'Position Latitude';
  $handler->display->display_options['fields']['field_geo_1']['label'] = 'positionLat';
  $handler->display->display_options['fields']['field_geo_1']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['field_geo_1']['click_sort_column'] = 'lat';
  $handler->display->display_options['fields']['field_geo_1']['type'] = 'geolocation_latitude';
  $handler->display->display_options['fields']['field_geo_1']['group_column'] = 'lat';
  /* Field: Position Longitude */
  $handler->display->display_options['fields']['field_geo_2']['id'] = 'field_geo_2';
  $handler->display->display_options['fields']['field_geo_2']['table'] = 'field_data_field_geo';
  $handler->display->display_options['fields']['field_geo_2']['field'] = 'field_geo';
  $handler->display->display_options['fields']['field_geo_2']['ui_name'] = 'Position Longitude';
  $handler->display->display_options['fields']['field_geo_2']['label'] = 'positionLng';
  $handler->display->display_options['fields']['field_geo_2']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['field_geo_2']['click_sort_column'] = 'lng';
  $handler->display->display_options['fields']['field_geo_2']['type'] = 'geolocation_longitude';
  $handler->display->display_options['fields']['field_geo_2']['group_column'] = 'lat';
  /* Field: Content: Image */
  $handler->display->display_options['fields']['field_image']['id'] = 'field_image';
  $handler->display->display_options['fields']['field_image']['table'] = 'field_data_field_image';
  $handler->display->display_options['fields']['field_image']['field'] = 'field_image';
  $handler->display->display_options['fields']['field_image']['label'] = 'imageUrl';
  $handler->display->display_options['fields']['field_image']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['field_image']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_image']['settings'] = array(
    'image_style' => 'mas_thumb',
    'image_link' => 'file',
  );
  $handler->display->display_options['fields']['field_image']['group_column'] = 'fid';
  $handler->display->display_options['fields']['field_image']['delta_offset'] = '0';
  /* Field: Content: Non located report */
  $handler->display->display_options['fields']['field_common']['id'] = 'field_common';
  $handler->display->display_options['fields']['field_common']['table'] = 'field_data_field_common';
  $handler->display->display_options['fields']['field_common']['field'] = 'field_common';
  /* Field: Taxonomy term: categoryId */
  $handler->display->display_options['fields']['field_category_id']['id'] = 'field_category_id';
  $handler->display->display_options['fields']['field_category_id']['table'] = 'field_data_field_category_id';
  $handler->display->display_options['fields']['field_category_id']['field'] = 'field_category_id';
  $handler->display->display_options['fields']['field_category_id']['relationship'] = 'field_category_tid';
  $handler->display->display_options['fields']['field_category_id']['settings'] = array(
    'thousand_separator' => ' ',
    'prefix_suffix' => 1,
  );
  /* Field: address */
  $handler->display->display_options['fields']['field_address']['id'] = 'field_address';
  $handler->display->display_options['fields']['field_address']['table'] = 'field_data_field_address';
  $handler->display->display_options['fields']['field_address']['field'] = 'field_address';
  $handler->display->display_options['fields']['field_address']['ui_name'] = 'address';
  $handler->display->display_options['fields']['field_address']['label'] = 'address';
  /* Field: Path */
  $handler->display->display_options['fields']['path']['id'] = 'path';
  $handler->display->display_options['fields']['path']['table'] = 'node';
  $handler->display->display_options['fields']['path']['field'] = 'path';
  $handler->display->display_options['fields']['path']['ui_name'] = 'Path';
  $handler->display->display_options['fields']['path']['label'] = 'path';
  /* Field: User Nickname */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name']['ui_name'] = 'User Nickname';
  $handler->display->display_options['fields']['name']['label'] = 'userNickname';
  $handler->display->display_options['fields']['name']['overwrite_anonymous'] = TRUE;
  $handler->display->display_options['fields']['name']['anonymous_text'] = 'Gast';
  /* Field: created timestamp */
  $handler->display->display_options['fields']['created_1']['id'] = 'created_1';
  $handler->display->display_options['fields']['created_1']['table'] = 'node';
  $handler->display->display_options['fields']['created_1']['field'] = 'created';
  $handler->display->display_options['fields']['created_1']['ui_name'] = 'created timestamp';
  $handler->display->display_options['fields']['created_1']['label'] = 'timestamp';
  $handler->display->display_options['fields']['created_1']['date_format'] = 'custom';
  $handler->display->display_options['fields']['created_1']['custom_date_format'] = 'U';
  /* Contextual filter: Content: Choose Service (field_category) */
  $handler->display->display_options['arguments']['field_category_tid']['id'] = 'field_category_tid';
  $handler->display->display_options['arguments']['field_category_tid']['table'] = 'field_data_field_category';
  $handler->display->display_options['arguments']['field_category_tid']['field'] = 'field_category_tid';
  $handler->display->display_options['arguments']['field_category_tid']['exception']['title'] = 'Alle';
  $handler->display->display_options['arguments']['field_category_tid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_category_tid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_category_tid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_category_tid']['summary_options']['items_per_page'] = '25';
  /* Contextual filter: Content: Status (field_status) */
  $handler->display->display_options['arguments']['field_status_tid']['id'] = 'field_status_tid';
  $handler->display->display_options['arguments']['field_status_tid']['table'] = 'field_data_field_status';
  $handler->display->display_options['arguments']['field_status_tid']['field'] = 'field_status_tid';
  $handler->display->display_options['arguments']['field_status_tid']['exception']['title'] = 'Alle';
  $handler->display->display_options['arguments']['field_status_tid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_status_tid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_status_tid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_status_tid']['summary_options']['items_per_page'] = '25';
  /* Contextual filter: Content: Nid */
  $handler->display->display_options['arguments']['nid']['id'] = 'nid';
  $handler->display->display_options['arguments']['nid']['table'] = 'node';
  $handler->display->display_options['arguments']['nid']['field'] = 'nid';
  $handler->display->display_options['arguments']['nid']['exception']['title'] = 'Alle';
  $handler->display->display_options['arguments']['nid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 0;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'report' => 'report',
  );
  $handler->display->display_options['filters']['type']['group'] = '0';
  /* Filter criterion: Content: Choose Service (field_category) */
  $handler->display->display_options['filters']['field_category_tid']['id'] = 'field_category_tid';
  $handler->display->display_options['filters']['field_category_tid']['table'] = 'field_data_field_category';
  $handler->display->display_options['filters']['field_category_tid']['field'] = 'field_category_tid';
  $handler->display->display_options['filters']['field_category_tid']['group'] = '0';
  $handler->display->display_options['filters']['field_category_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_category_tid']['expose']['operator_id'] = 'field_category_tid_op';
  $handler->display->display_options['filters']['field_category_tid']['expose']['label'] = 'Category';
  $handler->display->display_options['filters']['field_category_tid']['expose']['operator'] = 'field_category_tid_op';
  $handler->display->display_options['filters']['field_category_tid']['expose']['identifier'] = 'field_category_tid';
  $handler->display->display_options['filters']['field_category_tid']['type'] = 'select';
  $handler->display->display_options['filters']['field_category_tid']['vocabulary'] = 'category';
  /* Filter criterion: Content: Status (field_status) */
  $handler->display->display_options['filters']['field_status_tid']['id'] = 'field_status_tid';
  $handler->display->display_options['filters']['field_status_tid']['table'] = 'field_data_field_status';
  $handler->display->display_options['filters']['field_status_tid']['field'] = 'field_status_tid';
  $handler->display->display_options['filters']['field_status_tid']['group'] = '0';
  $handler->display->display_options['filters']['field_status_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_status_tid']['expose']['operator_id'] = 'field_status_tid_op';
  $handler->display->display_options['filters']['field_status_tid']['expose']['label'] = 'Status';
  $handler->display->display_options['filters']['field_status_tid']['expose']['operator'] = 'field_status_tid_op';
  $handler->display->display_options['filters']['field_status_tid']['expose']['identifier'] = 'field_status_tid';
  $handler->display->display_options['filters']['field_status_tid']['type'] = 'select';
  $handler->display->display_options['filters']['field_status_tid']['vocabulary'] = 'status';

  /* Display: Feed */
  $handler = $view->new_display('feed', 'Feed', 'feed_1');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['style_plugin'] = 'rss';
  $handler->display->display_options['style_options']['description'] = 'Reports RSS';
  $handler->display->display_options['row_plugin'] = 'node_rss';
  $handler->display->display_options['row_options']['item_length'] = 'title';
  $handler->display->display_options['exposed_block'] = TRUE;
  $handler->display->display_options['path'] = 'reports/rss';

  /* Display: Detail view */
  $handler = $view->new_display('page', 'Detail view', 'report_detail');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '1';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['expose']['items_per_page_label'] = 'Elemente pro Seite';
  $handler->display->display_options['pager']['options']['expose']['items_per_page_options_all_label'] = '- Alle -';
  $handler->display->display_options['pager']['options']['tags']['first'] = '« erste Seite';
  $handler->display->display_options['pager']['options']['tags']['previous'] = '‹ vorherige Seite';
  $handler->display->display_options['pager']['options']['tags']['next'] = 'nächste Seite ›';
  $handler->display->display_options['pager']['options']['tags']['last'] = 'letzte Seite »';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['style_options']['grouping'] = array(
    0 => array(
      'field' => 'title',
      'rendered' => 1,
    ),
  );
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Choose Service */
  $handler->display->display_options['fields']['field_category']['id'] = 'field_category';
  $handler->display->display_options['fields']['field_category']['table'] = 'field_data_field_category';
  $handler->display->display_options['fields']['field_category']['field'] = 'field_category';
  $handler->display->display_options['fields']['field_category']['label'] = '';
  $handler->display->display_options['fields']['field_category']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_category']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_category']['alter']['text'] = '[field_category]';
  $handler->display->display_options['fields']['field_category']['element_type'] = '0';
  $handler->display->display_options['fields']['field_category']['element_class'] = 'marker-category col-[field_category_hex]';
  $handler->display->display_options['fields']['field_category']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_category']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['field_category']['type'] = 'taxonomy_term_reference_plain';
  /* Field: Content: Status */
  $handler->display->display_options['fields']['field_status']['id'] = 'field_status';
  $handler->display->display_options['fields']['field_status']['table'] = 'field_data_field_status';
  $handler->display->display_options['fields']['field_status']['field'] = 'field_status';
  $handler->display->display_options['fields']['field_status']['label'] = '';
  $handler->display->display_options['fields']['field_status']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_status']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_status']['alter']['text'] = '[field_status]';
  $handler->display->display_options['fields']['field_status']['element_type'] = '0';
  $handler->display->display_options['fields']['field_status']['element_class'] = 'marker-status col-[field_status_hex]';
  $handler->display->display_options['fields']['field_status']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_status']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['field_status']['type'] = 'taxonomy_term_reference_plain';
  /* Field: Taxonomy term: statusHex */
  $handler->display->display_options['fields']['field_status_hex']['id'] = 'field_status_hex';
  $handler->display->display_options['fields']['field_status_hex']['table'] = 'field_data_field_status_hex';
  $handler->display->display_options['fields']['field_status_hex']['field'] = 'field_status_hex';
  $handler->display->display_options['fields']['field_status_hex']['relationship'] = 'field_status_tid';
  $handler->display->display_options['fields']['field_status_hex']['label'] = '';
  $handler->display->display_options['fields']['field_status_hex']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_status_hex']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_status_hex']['hide_alter_empty'] = FALSE;
  /* Field: Taxonomy term: categoryHex */
  $handler->display->display_options['fields']['field_category_hex']['id'] = 'field_category_hex';
  $handler->display->display_options['fields']['field_category_hex']['table'] = 'field_data_field_category_hex';
  $handler->display->display_options['fields']['field_category_hex']['field'] = 'field_category_hex';
  $handler->display->display_options['fields']['field_category_hex']['relationship'] = 'field_category_tid';
  $handler->display->display_options['fields']['field_category_hex']['label'] = '';
  $handler->display->display_options['fields']['field_category_hex']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_category_hex']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_category_hex']['hide_alter_empty'] = FALSE;
  /* Field: Category / Status Bar */
  $handler->display->display_options['fields']['nothing_2']['id'] = 'nothing_2';
  $handler->display->display_options['fields']['nothing_2']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_2']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_2']['ui_name'] = 'Category / Status Bar';
  $handler->display->display_options['fields']['nothing_2']['label'] = '';
  $handler->display->display_options['fields']['nothing_2']['alter']['text'] = '<div class="cat-stat-wrapper"><span class="label marker-category col-[field_category_hex]">[field_category]</span> <span class="label marker-status col-[field_status_hex]">[field_status]</span></div>';
  $handler->display->display_options['fields']['nothing_2']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['nothing_2']['hide_alter_empty'] = TRUE;
  /* Field: Content: Post date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'node';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = '';
  $handler->display->display_options['fields']['created']['exclude'] = TRUE;
  $handler->display->display_options['fields']['created']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['created']['date_format'] = 'medium';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name']['label'] = '';
  $handler->display->display_options['fields']['name']['exclude'] = TRUE;
  $handler->display->display_options['fields']['name']['element_label_colon'] = FALSE;
  /* Field: Content: Updated date */
  $handler->display->display_options['fields']['changed']['id'] = 'changed';
  $handler->display->display_options['fields']['changed']['table'] = 'node';
  $handler->display->display_options['fields']['changed']['field'] = 'changed';
  $handler->display->display_options['fields']['changed']['label'] = '';
  $handler->display->display_options['fields']['changed']['exclude'] = TRUE;
  $handler->display->display_options['fields']['changed']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['changed']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['changed']['date_format'] = 'raw time ago';
  /* Field: Meta (name, timestamp) */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['ui_name'] = 'Meta (name, timestamp)';
  $handler->display->display_options['fields']['nothing']['label'] = '';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '<span class="submitted meta-info"> [created] </span>';
  $handler->display->display_options['fields']['nothing']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['nothing']['hide_alter_empty'] = TRUE;
  /* Field: Content: Body */
  $handler->display->display_options['fields']['body']['id'] = 'body';
  $handler->display->display_options['fields']['body']['table'] = 'field_data_body';
  $handler->display->display_options['fields']['body']['field'] = 'body';
  $handler->display->display_options['fields']['body']['label'] = '';
  $handler->display->display_options['fields']['body']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['body']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['body']['hide_alter_empty'] = FALSE;
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['label'] = '';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
  /* Field: Content: Address */
  $handler->display->display_options['fields']['field_address']['id'] = 'field_address';
  $handler->display->display_options['fields']['field_address']['table'] = 'field_data_field_address';
  $handler->display->display_options['fields']['field_address']['field'] = 'field_address';
  $handler->display->display_options['fields']['field_address']['label'] = '';
  $handler->display->display_options['fields']['field_address']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_address']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_address']['alter']['text'] = '<div id="address-block">
<p class="label">Adresse</p>
<p class="field">[field_address]</p>
</div>';
  $handler->display->display_options['fields']['field_address']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_address']['hide_empty'] = TRUE;
  /* Field: Content: Non located report */
  $handler->display->display_options['fields']['field_common']['id'] = 'field_common';
  $handler->display->display_options['fields']['field_common']['table'] = 'field_data_field_common';
  $handler->display->display_options['fields']['field_common']['field'] = 'field_common';
  $handler->display->display_options['fields']['field_common']['label'] = '';
  $handler->display->display_options['fields']['field_common']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_common']['alter']['text'] = 'Unlocated report';
  $handler->display->display_options['fields']['field_common']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_common']['empty'] = '[field_address]';
  $handler->display->display_options['fields']['field_common']['empty_zero'] = TRUE;
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Content: Nid */
  $handler->display->display_options['arguments']['nid']['id'] = 'nid';
  $handler->display->display_options['arguments']['nid']['table'] = 'node';
  $handler->display->display_options['arguments']['nid']['field'] = 'nid';
  $handler->display->display_options['arguments']['nid']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['nid']['exception']['title'] = 'Alle';
  $handler->display->display_options['arguments']['nid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['nid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['nid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'report' => 'report',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  $handler->display->display_options['path'] = 'report';

  /* Display: List view */
  $handler = $view->new_display('page', 'List view', 'report_list');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['exposed_form'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['submit_button'] = 'Anwenden';
  $handler->display->display_options['exposed_form']['options']['reset_button_label'] = 'Zurücksetzen';
  $handler->display->display_options['exposed_form']['options']['exposed_sorts_label'] = 'Sortieren';
  $handler->display->display_options['exposed_form']['options']['sort_asc_label'] = 'Aufsteigend';
  $handler->display->display_options['exposed_form']['options']['sort_desc_label'] = 'Absteigend';
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['expose']['items_per_page_label'] = 'Elemente pro Seite';
  $handler->display->display_options['pager']['options']['expose']['items_per_page_options_all_label'] = '- Alle -';
  $handler->display->display_options['pager']['options']['tags']['first'] = '« erste Seite';
  $handler->display->display_options['pager']['options']['tags']['previous'] = '‹ vorherige Seite';
  $handler->display->display_options['pager']['options']['tags']['next'] = 'nächste Seite ›';
  $handler->display->display_options['pager']['options']['tags']['last'] = 'letzte Seite »';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['style_options']['row_class'] = 'report';
  $handler->display->display_options['style_options']['row_class_special'] = FALSE;
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['exposed_block'] = TRUE;
  $handler->display->display_options['defaults']['header'] = FALSE;
  $handler->display->display_options['defaults']['empty'] = FALSE;
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['label'] = 'No Results';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'Für diese Kategorie / Status haben wir keine aktuellen Beiträge';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  $handler->display->display_options['empty']['area']['tokenize'] = TRUE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['label'] = '';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
  /* Field: Content: Status */
  $handler->display->display_options['fields']['field_status']['id'] = 'field_status';
  $handler->display->display_options['fields']['field_status']['table'] = 'field_data_field_status';
  $handler->display->display_options['fields']['field_status']['field'] = 'field_status';
  $handler->display->display_options['fields']['field_status']['label'] = '';
  $handler->display->display_options['fields']['field_status']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_status']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_status']['alter']['text'] = '[field_status]';
  $handler->display->display_options['fields']['field_status']['element_type'] = '0';
  $handler->display->display_options['fields']['field_status']['element_class'] = 'marker-status col-[field_status_hex]';
  $handler->display->display_options['fields']['field_status']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_status']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['field_status']['type'] = 'taxonomy_term_reference_plain';
  /* Field: Content: Choose Service */
  $handler->display->display_options['fields']['field_category']['id'] = 'field_category';
  $handler->display->display_options['fields']['field_category']['table'] = 'field_data_field_category';
  $handler->display->display_options['fields']['field_category']['field'] = 'field_category';
  $handler->display->display_options['fields']['field_category']['label'] = '';
  $handler->display->display_options['fields']['field_category']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_category']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_category']['alter']['text'] = '[field_category]';
  $handler->display->display_options['fields']['field_category']['element_type'] = '0';
  $handler->display->display_options['fields']['field_category']['element_class'] = 'label marker-category col-[field_category_hex]';
  $handler->display->display_options['fields']['field_category']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_category']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['field_category']['type'] = 'taxonomy_term_reference_plain';
  /* Field: Content: Post date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'node';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = '';
  $handler->display->display_options['fields']['created']['exclude'] = TRUE;
  $handler->display->display_options['fields']['created']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['created']['date_format'] = 'medium';
  /* Field: Taxonomy term: statusHex */
  $handler->display->display_options['fields']['field_status_hex']['id'] = 'field_status_hex';
  $handler->display->display_options['fields']['field_status_hex']['table'] = 'field_data_field_status_hex';
  $handler->display->display_options['fields']['field_status_hex']['field'] = 'field_status_hex';
  $handler->display->display_options['fields']['field_status_hex']['relationship'] = 'field_status_tid';
  $handler->display->display_options['fields']['field_status_hex']['label'] = '';
  $handler->display->display_options['fields']['field_status_hex']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_status_hex']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_status_hex']['hide_alter_empty'] = FALSE;
  /* Field: Taxonomy term: categoryHex */
  $handler->display->display_options['fields']['field_category_hex']['id'] = 'field_category_hex';
  $handler->display->display_options['fields']['field_category_hex']['table'] = 'field_data_field_category_hex';
  $handler->display->display_options['fields']['field_category_hex']['field'] = 'field_category_hex';
  $handler->display->display_options['fields']['field_category_hex']['relationship'] = 'field_category_tid';
  $handler->display->display_options['fields']['field_category_hex']['label'] = '';
  $handler->display->display_options['fields']['field_category_hex']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_category_hex']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_category_hex']['hide_alter_empty'] = FALSE;
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name']['label'] = '';
  $handler->display->display_options['fields']['name']['exclude'] = TRUE;
  $handler->display->display_options['fields']['name']['element_label_colon'] = FALSE;
  /* Field: Meta (name, timestamp) */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['ui_name'] = 'Meta (name, timestamp)';
  $handler->display->display_options['fields']['nothing']['label'] = '';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '<span class="submitted meta-info"> [created] </span>';
  $handler->display->display_options['fields']['nothing']['element_type'] = '0';
  $handler->display->display_options['fields']['nothing']['element_label_type'] = '0';
  $handler->display->display_options['fields']['nothing']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['nothing']['element_wrapper_type'] = '0';
  $handler->display->display_options['fields']['nothing']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['nothing']['hide_alter_empty'] = TRUE;
  /* Field: Category / Status Bar */
  $handler->display->display_options['fields']['nothing_1']['id'] = 'nothing_1';
  $handler->display->display_options['fields']['nothing_1']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_1']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_1']['ui_name'] = 'Category / Status Bar';
  $handler->display->display_options['fields']['nothing_1']['label'] = '';
  $handler->display->display_options['fields']['nothing_1']['alter']['text'] = '<div class="cat-stat-wrapper"><span class="label marker-category col-[field_category_hex]">[field_category]</span> <span class="label marker-status col-[field_status_hex]">[field_status]</span></div>';
  $handler->display->display_options['fields']['nothing_1']['element_type'] = '0';
  $handler->display->display_options['fields']['nothing_1']['element_label_type'] = '0';
  $handler->display->display_options['fields']['nothing_1']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['nothing_1']['element_wrapper_type'] = '0';
  $handler->display->display_options['fields']['nothing_1']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['nothing_1']['hide_alter_empty'] = TRUE;
  /* Field: Content: Updated date */
  $handler->display->display_options['fields']['changed']['id'] = 'changed';
  $handler->display->display_options['fields']['changed']['table'] = 'node';
  $handler->display->display_options['fields']['changed']['field'] = 'changed';
  $handler->display->display_options['fields']['changed']['label'] = '';
  $handler->display->display_options['fields']['changed']['exclude'] = TRUE;
  $handler->display->display_options['fields']['changed']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['changed']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['changed']['date_format'] = 'raw time ago';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['title']['alter']['text'] = '<h3 class="list-title">[title]</h3>';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  /* Field: Content: Image */
  $handler->display->display_options['fields']['field_image']['id'] = 'field_image';
  $handler->display->display_options['fields']['field_image']['table'] = 'field_data_field_image';
  $handler->display->display_options['fields']['field_image']['field'] = 'field_image';
  $handler->display->display_options['fields']['field_image']['label'] = '';
  $handler->display->display_options['fields']['field_image']['element_type'] = 'div';
  $handler->display->display_options['fields']['field_image']['element_class'] = 'pull-left thumbnail';
  $handler->display->display_options['fields']['field_image']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_image']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['field_image']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['field_image']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_image']['settings'] = array(
    'image_style' => 'mas_thumb',
    'image_link' => 'content',
  );
  $handler->display->display_options['fields']['field_image']['group_column'] = 'fid';
  $handler->display->display_options['fields']['field_image']['delta_limit'] = '1';
  $handler->display->display_options['fields']['field_image']['delta_offset'] = '0';
  /* Field: Content: Body */
  $handler->display->display_options['fields']['body']['id'] = 'body';
  $handler->display->display_options['fields']['body']['table'] = 'field_data_body';
  $handler->display->display_options['fields']['body']['field'] = 'body';
  $handler->display->display_options['fields']['body']['label'] = '';
  $handler->display->display_options['fields']['body']['alter']['trim_whitespace'] = TRUE;
  $handler->display->display_options['fields']['body']['alter']['max_length'] = '200';
  $handler->display->display_options['fields']['body']['alter']['strip_tags'] = TRUE;
  $handler->display->display_options['fields']['body']['alter']['trim'] = TRUE;
  $handler->display->display_options['fields']['body']['element_type'] = 'div';
  $handler->display->display_options['fields']['body']['element_class'] = 'report-body body_[nid]';
  $handler->display->display_options['fields']['body']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['body']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['body']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['body']['settings'] = array(
    'trim_length' => '200',
  );
  /* Field: Content: Non located report */
  $handler->display->display_options['fields']['field_common']['id'] = 'field_common';
  $handler->display->display_options['fields']['field_common']['table'] = 'field_data_field_common';
  $handler->display->display_options['fields']['field_common']['field'] = 'field_common';
  $handler->display->display_options['fields']['field_common']['label'] = '';
  $handler->display->display_options['fields']['field_common']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_common']['alter']['text'] = '<div class="non-located">Unlocated Report</div><div class="no-markers"><span></span></div>';
  $handler->display->display_options['fields']['field_common']['element_type'] = '0';
  $handler->display->display_options['fields']['field_common']['element_label_type'] = '0';
  $handler->display->display_options['fields']['field_common']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_common']['element_wrapper_type'] = 'div';
  $handler->display->display_options['fields']['field_common']['element_wrapper_class'] = 'location';
  $handler->display->display_options['fields']['field_common']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['field_common']['empty'] = '<div class="show-markers" title="Show marker in map" id="marker_[nid]"><span class="icon-chevron-left"></span></div>
';
  $handler->display->display_options['fields']['field_common']['empty_zero'] = TRUE;
  $handler->display->display_options['fields']['field_common']['type'] = 'list_key';
  /* Field: Content: Link */
  $handler->display->display_options['fields']['view_node']['id'] = 'view_node';
  $handler->display->display_options['fields']['view_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['view_node']['field'] = 'view_node';
  $handler->display->display_options['fields']['view_node']['label'] = '';
  $handler->display->display_options['fields']['view_node']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['view_node']['text'] = 'read more';
  /* Field: Content: Comment count */
  $handler->display->display_options['fields']['comment_count']['id'] = 'comment_count';
  $handler->display->display_options['fields']['comment_count']['table'] = 'node_comment_statistics';
  $handler->display->display_options['fields']['comment_count']['field'] = 'comment_count';
  $handler->display->display_options['fields']['comment_count']['label'] = '';
  $handler->display->display_options['fields']['comment_count']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['comment_count']['alter']['text'] = '<span class="badge badge-info">[comment_count]</span> Comments';
  $handler->display->display_options['fields']['comment_count']['alter']['path'] = 'node/[nid]';
  $handler->display->display_options['fields']['comment_count']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['comment_count']['separator'] = '.';
  $handler->display->display_options['fields']['comment_count']['format_plural_singular'] = '<span class="badge badge-info">1</span> Comment';
  $handler->display->display_options['fields']['comment_count']['format_plural_plural'] = '<span class="badge badge-info">@count</span> Comments';
  /* Field: Flag Common */
  $handler->display->display_options['fields']['field_common_1']['id'] = 'field_common_1';
  $handler->display->display_options['fields']['field_common_1']['table'] = 'field_data_field_common';
  $handler->display->display_options['fields']['field_common_1']['field'] = 'field_common';
  $handler->display->display_options['fields']['field_common_1']['ui_name'] = 'Flag Common';
  $handler->display->display_options['fields']['field_common_1']['label'] = '';
  $handler->display->display_options['fields']['field_common_1']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_common_1']['alter']['text'] = 'common yes';
  $handler->display->display_options['fields']['field_common_1']['alter']['trim_whitespace'] = TRUE;
  $handler->display->display_options['fields']['field_common_1']['alter']['strip_tags'] = TRUE;
  $handler->display->display_options['fields']['field_common_1']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_common_1']['empty_zero'] = TRUE;
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = 0;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'report' => 'report',
  );
  $handler->display->display_options['filters']['type']['group'] = '0';
  /* Filter criterion: Content: Choose Service (field_category) */
  $handler->display->display_options['filters']['field_category_tid']['id'] = 'field_category_tid';
  $handler->display->display_options['filters']['field_category_tid']['table'] = 'field_data_field_category';
  $handler->display->display_options['filters']['field_category_tid']['field'] = 'field_category_tid';
  $handler->display->display_options['filters']['field_category_tid']['group'] = '0';
  $handler->display->display_options['filters']['field_category_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_category_tid']['expose']['operator_id'] = 'field_category_tid_op';
  $handler->display->display_options['filters']['field_category_tid']['expose']['label'] = 'Category';
  $handler->display->display_options['filters']['field_category_tid']['expose']['operator'] = 'field_category_tid_op';
  $handler->display->display_options['filters']['field_category_tid']['expose']['identifier'] = 'field_category_tid';
  $handler->display->display_options['filters']['field_category_tid']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['field_category_tid']['expose']['remember_roles'] = array(
    2 => 0,
    1 => 0,
    3 => 0,
    5 => 0,
    4 => 0,
  );
  $handler->display->display_options['filters']['field_category_tid']['type'] = 'select';
  $handler->display->display_options['filters']['field_category_tid']['vocabulary'] = 'category';
  /* Filter criterion: Content: Status (field_status) */
  $handler->display->display_options['filters']['field_status_tid']['id'] = 'field_status_tid';
  $handler->display->display_options['filters']['field_status_tid']['table'] = 'field_data_field_status';
  $handler->display->display_options['filters']['field_status_tid']['field'] = 'field_status_tid';
  $handler->display->display_options['filters']['field_status_tid']['group'] = '0';
  $handler->display->display_options['filters']['field_status_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_status_tid']['expose']['operator_id'] = 'field_status_tid_op';
  $handler->display->display_options['filters']['field_status_tid']['expose']['label'] = 'Status';
  $handler->display->display_options['filters']['field_status_tid']['expose']['operator'] = 'field_status_tid_op';
  $handler->display->display_options['filters']['field_status_tid']['expose']['identifier'] = 'field_status_tid';
  $handler->display->display_options['filters']['field_status_tid']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['field_status_tid']['expose']['remember_roles'] = array(
    2 => 0,
    1 => 0,
    3 => 0,
    5 => 0,
    4 => 0,
  );
  $handler->display->display_options['filters']['field_status_tid']['type'] = 'select';
  $handler->display->display_options['filters']['field_status_tid']['vocabulary'] = 'status';
  $handler->display->display_options['path'] = 'report-list';

  /* Display: JSON View */
  $handler = $view->new_display('page', 'JSON View', 'page_2');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['exposed_form'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['submit_button'] = 'Anwenden';
  $handler->display->display_options['exposed_form']['options']['reset_button'] = TRUE;
  $handler->display->display_options['exposed_form']['options']['reset_button_label'] = 'Zurücksetzen';
  $handler->display->display_options['exposed_form']['options']['exposed_sorts_label'] = 'Sortieren';
  $handler->display->display_options['exposed_form']['options']['sort_asc_label'] = 'Aufsteigend';
  $handler->display->display_options['exposed_form']['options']['sort_desc_label'] = 'Absteigend';
  $handler->display->display_options['exposed_form']['options']['autosubmit'] = TRUE;
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'mini';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['tags']['first'] = '« erste Seite';
  $handler->display->display_options['pager']['options']['tags']['previous'] = '‹ vorherige Seite';
  $handler->display->display_options['pager']['options']['tags']['next'] = 'nächste Seite ›';
  $handler->display->display_options['pager']['options']['tags']['last'] = 'letzte Seite »';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'views_json';
  $handler->display->display_options['style_options']['plaintext_output'] = 1;
  $handler->display->display_options['style_options']['jsonp_prefix'] = '';
  $handler->display->display_options['style_options']['using_views_api_mode'] = 0;
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['exposed_block'] = TRUE;
  $handler->display->display_options['defaults']['empty'] = FALSE;
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Content: Nid */
  $handler->display->display_options['arguments']['nid']['id'] = 'nid';
  $handler->display->display_options['arguments']['nid']['table'] = 'node';
  $handler->display->display_options['arguments']['nid']['field'] = 'nid';
  $handler->display->display_options['arguments']['nid']['default_action'] = 'default';
  $handler->display->display_options['arguments']['nid']['exception']['title'] = 'Alle';
  $handler->display->display_options['arguments']['nid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['nid']['default_argument_options']['argument'] = 'all';
  $handler->display->display_options['arguments']['nid']['default_argument_skip_url'] = TRUE;
  $handler->display->display_options['arguments']['nid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['nid']['summary_options']['items_per_page'] = '25';
  /* Contextual filter: Content: Choose Service (field_category) */
  $handler->display->display_options['arguments']['field_category_tid']['id'] = 'field_category_tid';
  $handler->display->display_options['arguments']['field_category_tid']['table'] = 'field_data_field_category';
  $handler->display->display_options['arguments']['field_category_tid']['field'] = 'field_category_tid';
  $handler->display->display_options['arguments']['field_category_tid']['exception']['title'] = 'Alle';
  $handler->display->display_options['arguments']['field_category_tid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_category_tid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_category_tid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_category_tid']['summary_options']['items_per_page'] = '25';
  /* Contextual filter: Content: Status (field_status) */
  $handler->display->display_options['arguments']['field_status_tid']['id'] = 'field_status_tid';
  $handler->display->display_options['arguments']['field_status_tid']['table'] = 'field_data_field_status';
  $handler->display->display_options['arguments']['field_status_tid']['field'] = 'field_status_tid';
  $handler->display->display_options['arguments']['field_status_tid']['exception']['title'] = 'Alle';
  $handler->display->display_options['arguments']['field_status_tid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_status_tid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_status_tid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_status_tid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['path'] = 'reports/json';

  /* Display: JSON View App */
  $handler = $view->new_display('page', 'JSON View App', 'page_1');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['exposed_form'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['submit_button'] = 'Anwenden';
  $handler->display->display_options['exposed_form']['options']['reset_button'] = TRUE;
  $handler->display->display_options['exposed_form']['options']['reset_button_label'] = 'Zurücksetzen';
  $handler->display->display_options['exposed_form']['options']['exposed_sorts_label'] = 'Sortieren';
  $handler->display->display_options['exposed_form']['options']['sort_asc_label'] = 'Aufsteigend';
  $handler->display->display_options['exposed_form']['options']['sort_desc_label'] = 'Absteigend';
  $handler->display->display_options['exposed_form']['options']['autosubmit'] = TRUE;
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'mini';
  $handler->display->display_options['pager']['options']['items_per_page'] = '5';
  $handler->display->display_options['pager']['options']['offset'] = '';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['expose']['items_per_page_label'] = 'Elemente pro Seite';
  $handler->display->display_options['pager']['options']['expose']['items_per_page_options_all_label'] = '- Alle -';
  $handler->display->display_options['pager']['options']['tags']['first'] = '« erste Seite';
  $handler->display->display_options['pager']['options']['tags']['previous'] = '‹ vorherige Seite';
  $handler->display->display_options['pager']['options']['tags']['next'] = 'nächste Seite ›';
  $handler->display->display_options['pager']['options']['tags']['last'] = 'letzte Seite »';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'views_json';
  $handler->display->display_options['style_options']['plaintext_output'] = 1;
  $handler->display->display_options['style_options']['jsonp_prefix'] = '';
  $handler->display->display_options['style_options']['using_views_api_mode'] = 0;
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['exposed_block'] = TRUE;
  $handler->display->display_options['defaults']['empty'] = FALSE;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Content: Nid */
  $handler->display->display_options['arguments']['nid']['id'] = 'nid';
  $handler->display->display_options['arguments']['nid']['table'] = 'node';
  $handler->display->display_options['arguments']['nid']['field'] = 'nid';
  $handler->display->display_options['arguments']['nid']['default_action'] = 'default';
  $handler->display->display_options['arguments']['nid']['exception']['title'] = 'Alle';
  $handler->display->display_options['arguments']['nid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['nid']['default_argument_options']['argument'] = 'all';
  $handler->display->display_options['arguments']['nid']['default_argument_skip_url'] = TRUE;
  $handler->display->display_options['arguments']['nid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['nid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 0;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'report' => 'report',
  );
  $handler->display->display_options['filters']['type']['group'] = '0';
  /* Filter criterion: Content: Choose Service (field_category) */
  $handler->display->display_options['filters']['field_category_tid']['id'] = 'field_category_tid';
  $handler->display->display_options['filters']['field_category_tid']['table'] = 'field_data_field_category';
  $handler->display->display_options['filters']['field_category_tid']['field'] = 'field_category_tid';
  $handler->display->display_options['filters']['field_category_tid']['group'] = '0';
  $handler->display->display_options['filters']['field_category_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_category_tid']['expose']['operator_id'] = 'field_category_tid_op';
  $handler->display->display_options['filters']['field_category_tid']['expose']['label'] = 'Category';
  $handler->display->display_options['filters']['field_category_tid']['expose']['operator'] = 'field_category_tid_op';
  $handler->display->display_options['filters']['field_category_tid']['expose']['identifier'] = 'field_category_tid';
  $handler->display->display_options['filters']['field_category_tid']['type'] = 'select';
  $handler->display->display_options['filters']['field_category_tid']['vocabulary'] = 'category';
  /* Filter criterion: Content: Status (field_status) */
  $handler->display->display_options['filters']['field_status_tid']['id'] = 'field_status_tid';
  $handler->display->display_options['filters']['field_status_tid']['table'] = 'field_data_field_status';
  $handler->display->display_options['filters']['field_status_tid']['field'] = 'field_status_tid';
  $handler->display->display_options['filters']['field_status_tid']['group'] = '0';
  $handler->display->display_options['filters']['field_status_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_status_tid']['expose']['operator_id'] = 'field_status_tid_op';
  $handler->display->display_options['filters']['field_status_tid']['expose']['label'] = 'Status';
  $handler->display->display_options['filters']['field_status_tid']['expose']['operator'] = 'field_status_tid_op';
  $handler->display->display_options['filters']['field_status_tid']['expose']['identifier'] = 'field_status_tid';
  $handler->display->display_options['filters']['field_status_tid']['type'] = 'select';
  $handler->display->display_options['filters']['field_status_tid']['vocabulary'] = 'status';
  /* Filter criterion: greater lat */
  $handler->display->display_options['filters']['field_geo_lat']['id'] = 'field_geo_lat';
  $handler->display->display_options['filters']['field_geo_lat']['table'] = 'field_data_field_geo';
  $handler->display->display_options['filters']['field_geo_lat']['field'] = 'field_geo_lat';
  $handler->display->display_options['filters']['field_geo_lat']['ui_name'] = 'greater lat';
  $handler->display->display_options['filters']['field_geo_lat']['operator'] = '>';
  $handler->display->display_options['filters']['field_geo_lat']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_geo_lat']['expose']['operator_id'] = 'field_geo_lat_op';
  $handler->display->display_options['filters']['field_geo_lat']['expose']['label'] = 'Place your report (field_geo:lat)';
  $handler->display->display_options['filters']['field_geo_lat']['expose']['operator'] = 'field_geo_lat_op';
  $handler->display->display_options['filters']['field_geo_lat']['expose']['identifier'] = 'field_geo_lat';
  /* Filter criterion: less lat */
  $handler->display->display_options['filters']['field_geo_lat_1']['id'] = 'field_geo_lat_1';
  $handler->display->display_options['filters']['field_geo_lat_1']['table'] = 'field_data_field_geo';
  $handler->display->display_options['filters']['field_geo_lat_1']['field'] = 'field_geo_lat';
  $handler->display->display_options['filters']['field_geo_lat_1']['ui_name'] = 'less lat';
  $handler->display->display_options['filters']['field_geo_lat_1']['operator'] = '<';
  $handler->display->display_options['filters']['field_geo_lat_1']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_geo_lat_1']['expose']['operator_id'] = 'field_geo_lat_1_op';
  $handler->display->display_options['filters']['field_geo_lat_1']['expose']['label'] = 'Place your report (field_geo:lat)';
  $handler->display->display_options['filters']['field_geo_lat_1']['expose']['operator'] = 'field_geo_lat_1_op';
  $handler->display->display_options['filters']['field_geo_lat_1']['expose']['identifier'] = 'field_geo_lat_1';
  /* Filter criterion: greater lng */
  $handler->display->display_options['filters']['field_geo_lng']['id'] = 'field_geo_lng';
  $handler->display->display_options['filters']['field_geo_lng']['table'] = 'field_data_field_geo';
  $handler->display->display_options['filters']['field_geo_lng']['field'] = 'field_geo_lng';
  $handler->display->display_options['filters']['field_geo_lng']['ui_name'] = 'greater lng';
  $handler->display->display_options['filters']['field_geo_lng']['operator'] = '>=';
  $handler->display->display_options['filters']['field_geo_lng']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_geo_lng']['expose']['operator_id'] = 'field_geo_lng_op';
  $handler->display->display_options['filters']['field_geo_lng']['expose']['label'] = 'Place your report (field_geo:lng)';
  $handler->display->display_options['filters']['field_geo_lng']['expose']['operator'] = 'field_geo_lng_op';
  $handler->display->display_options['filters']['field_geo_lng']['expose']['identifier'] = 'field_geo_lng';
  /* Filter criterion: less lng */
  $handler->display->display_options['filters']['field_geo_lng_1']['id'] = 'field_geo_lng_1';
  $handler->display->display_options['filters']['field_geo_lng_1']['table'] = 'field_data_field_geo';
  $handler->display->display_options['filters']['field_geo_lng_1']['field'] = 'field_geo_lng';
  $handler->display->display_options['filters']['field_geo_lng_1']['ui_name'] = 'less lng';
  $handler->display->display_options['filters']['field_geo_lng_1']['operator'] = '<=';
  $handler->display->display_options['filters']['field_geo_lng_1']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_geo_lng_1']['expose']['operator_id'] = 'field_geo_lng_1_op';
  $handler->display->display_options['filters']['field_geo_lng_1']['expose']['label'] = 'Place your report (field_geo:lng)';
  $handler->display->display_options['filters']['field_geo_lng_1']['expose']['operator'] = 'field_geo_lng_1_op';
  $handler->display->display_options['filters']['field_geo_lng_1']['expose']['identifier'] = 'field_geo_lng_1';
  $handler->display->display_options['path'] = 'reports/app';

  /* Display: JSON View Map */
  $handler = $view->new_display('page', 'JSON View Map', 'page_4');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['exposed_form'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['submit_button'] = 'Anwenden';
  $handler->display->display_options['exposed_form']['options']['reset_button'] = TRUE;
  $handler->display->display_options['exposed_form']['options']['reset_button_label'] = 'Zurücksetzen';
  $handler->display->display_options['exposed_form']['options']['exposed_sorts_label'] = 'Sortieren';
  $handler->display->display_options['exposed_form']['options']['sort_asc_label'] = 'Aufsteigend';
  $handler->display->display_options['exposed_form']['options']['sort_desc_label'] = 'Absteigend';
  $handler->display->display_options['exposed_form']['options']['autosubmit'] = TRUE;
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'mini';
  $handler->display->display_options['pager']['options']['items_per_page'] = '0';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['tags']['first'] = '';
  $handler->display->display_options['pager']['options']['tags']['previous'] = '';
  $handler->display->display_options['pager']['options']['tags']['next'] = '';
  $handler->display->display_options['pager']['options']['tags']['last'] = '';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'views_json';
  $handler->display->display_options['style_options']['plaintext_output'] = 1;
  $handler->display->display_options['style_options']['jsonp_prefix'] = '';
  $handler->display->display_options['style_options']['using_views_api_mode'] = 0;
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['exposed_block'] = TRUE;
  $handler->display->display_options['defaults']['empty'] = FALSE;
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Content: Nid */
  $handler->display->display_options['arguments']['nid']['id'] = 'nid';
  $handler->display->display_options['arguments']['nid']['table'] = 'node';
  $handler->display->display_options['arguments']['nid']['field'] = 'nid';
  $handler->display->display_options['arguments']['nid']['default_action'] = 'default';
  $handler->display->display_options['arguments']['nid']['exception']['title'] = 'Alle';
  $handler->display->display_options['arguments']['nid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['nid']['default_argument_options']['argument'] = 'all';
  $handler->display->display_options['arguments']['nid']['default_argument_skip_url'] = TRUE;
  $handler->display->display_options['arguments']['nid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['nid']['summary_options']['items_per_page'] = '25';
  /* Contextual filter: Content: Choose Service (field_category) */
  $handler->display->display_options['arguments']['field_category_tid']['id'] = 'field_category_tid';
  $handler->display->display_options['arguments']['field_category_tid']['table'] = 'field_data_field_category';
  $handler->display->display_options['arguments']['field_category_tid']['field'] = 'field_category_tid';
  $handler->display->display_options['arguments']['field_category_tid']['exception']['title'] = 'Alle';
  $handler->display->display_options['arguments']['field_category_tid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_category_tid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_category_tid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_category_tid']['summary_options']['items_per_page'] = '25';
  /* Contextual filter: Content: Status (field_status) */
  $handler->display->display_options['arguments']['field_status_tid']['id'] = 'field_status_tid';
  $handler->display->display_options['arguments']['field_status_tid']['table'] = 'field_data_field_status';
  $handler->display->display_options['arguments']['field_status_tid']['field'] = 'field_status_tid';
  $handler->display->display_options['arguments']['field_status_tid']['exception']['title'] = 'Alle';
  $handler->display->display_options['arguments']['field_status_tid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_status_tid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_status_tid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_status_tid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['path'] = 'reports/json/map';
  $export['markers'] = $view;

  return $export;
}
