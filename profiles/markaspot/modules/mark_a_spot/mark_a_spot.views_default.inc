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
  $handler->display->display_options['use_more'] = TRUE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['reset_button_label'] = 'Zurücksetzen';
  $handler->display->display_options['exposed_form']['options']['autosubmit'] = TRUE;
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['style_options']['grouping'] = '';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Relationship: Content: Category (field_category) */
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
  /* Relationship: Content: Vote results */
  $handler->display->display_options['relationships']['votingapi_cache_1']['id'] = 'votingapi_cache_1';
  $handler->display->display_options['relationships']['votingapi_cache_1']['table'] = 'node';
  $handler->display->display_options['relationships']['votingapi_cache_1']['field'] = 'votingapi_cache';
  $handler->display->display_options['relationships']['votingapi_cache_1']['votingapi'] = array(
    'value_type' => '',
    'tag' => 'vote',
    'function' => 'count',
  );
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Content: Category */
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
    'image_style' => 'thumbnail',
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
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  $handler->display->display_options['sorts']['created']['exposed'] = TRUE;
  $handler->display->display_options['sorts']['created']['expose']['label'] = 'Datum';
  /* Sort criterion: Comment Sort */
  $handler->display->display_options['sorts']['comment_count']['id'] = 'comment_count';
  $handler->display->display_options['sorts']['comment_count']['table'] = 'node_comment_statistics';
  $handler->display->display_options['sorts']['comment_count']['field'] = 'comment_count';
  $handler->display->display_options['sorts']['comment_count']['ui_name'] = 'Comment Sort';
  $handler->display->display_options['sorts']['comment_count']['order'] = 'DESC';
  $handler->display->display_options['sorts']['comment_count']['exposed'] = TRUE;
  $handler->display->display_options['sorts']['comment_count']['expose']['label'] = 'Anzahl an Kommentaren';
  /* Sort criterion: Vote results: Function */
  $handler->display->display_options['sorts']['function']['id'] = 'function';
  $handler->display->display_options['sorts']['function']['table'] = 'votingapi_cache';
  $handler->display->display_options['sorts']['function']['field'] = 'function';
  $handler->display->display_options['sorts']['function']['relationship'] = 'votingapi_cache';
  $handler->display->display_options['sorts']['function']['order'] = 'DESC';
  $handler->display->display_options['sorts']['function']['exposed'] = TRUE;
  $handler->display->display_options['sorts']['function']['expose']['label'] = 'Stimmen';
  /* Sort criterion: Content: Category (field_category) */
  $handler->display->display_options['sorts']['field_category_tid']['id'] = 'field_category_tid';
  $handler->display->display_options['sorts']['field_category_tid']['table'] = 'field_data_field_category';
  $handler->display->display_options['sorts']['field_category_tid']['field'] = 'field_category_tid';
  $handler->display->display_options['sorts']['field_category_tid']['exposed'] = TRUE;
  $handler->display->display_options['sorts']['field_category_tid']['expose']['label'] = 'Choose Category';
  /* Sort criterion: Content: Status (field_status) */
  $handler->display->display_options['sorts']['field_status_tid']['id'] = 'field_status_tid';
  $handler->display->display_options['sorts']['field_status_tid']['table'] = 'field_data_field_status';
  $handler->display->display_options['sorts']['field_status_tid']['field'] = 'field_status_tid';
  $handler->display->display_options['sorts']['field_status_tid']['exposed'] = TRUE;
  $handler->display->display_options['sorts']['field_status_tid']['expose']['label'] = 'Choose Status';
  /* Contextual filter: Content: Category (field_category) */
  $handler->display->display_options['arguments']['field_category_tid']['id'] = 'field_category_tid';
  $handler->display->display_options['arguments']['field_category_tid']['table'] = 'field_data_field_category';
  $handler->display->display_options['arguments']['field_category_tid']['field'] = 'field_category_tid';
  $handler->display->display_options['arguments']['field_category_tid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_category_tid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_category_tid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_category_tid']['summary_options']['items_per_page'] = '25';
  /* Contextual filter: Content: Status (field_status) */
  $handler->display->display_options['arguments']['field_status_tid']['id'] = 'field_status_tid';
  $handler->display->display_options['arguments']['field_status_tid']['table'] = 'field_data_field_status';
  $handler->display->display_options['arguments']['field_status_tid']['field'] = 'field_status_tid';
  $handler->display->display_options['arguments']['field_status_tid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_status_tid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_status_tid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_status_tid']['summary_options']['items_per_page'] = '25';
  /* Contextual filter: Content: Nid */
  $handler->display->display_options['arguments']['nid']['id'] = 'nid';
  $handler->display->display_options['arguments']['nid']['table'] = 'node';
  $handler->display->display_options['arguments']['nid']['field'] = 'nid';
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
  /* Filter criterion: Content: Category (field_category) */
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
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['exclude'] = TRUE;
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  /* Field: Content: Category */
  $handler->display->display_options['fields']['field_category']['id'] = 'field_category';
  $handler->display->display_options['fields']['field_category']['table'] = 'field_data_field_category';
  $handler->display->display_options['fields']['field_category']['field'] = 'field_category';
  $handler->display->display_options['fields']['field_category']['label'] = '';
  $handler->display->display_options['fields']['field_category']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_category']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_category']['alter']['text'] = 'Category: [field_category]';
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
  $handler->display->display_options['fields']['field_status']['alter']['text'] = 'Status: [field_status]';
  $handler->display->display_options['fields']['field_status']['element_type'] = '0';
  $handler->display->display_options['fields']['field_status']['element_class'] = 'marker-status col-[field_status_hex]';
  $handler->display->display_options['fields']['field_status']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_status']['hide_alter_empty'] = FALSE;
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
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_2']['id'] = 'nothing_2';
  $handler->display->display_options['fields']['nothing_2']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_2']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_2']['label'] = '';
  $handler->display->display_options['fields']['nothing_2']['alter']['text'] = '<div class="cat-stat-wrapper"><span class="marker-category col-[field_category_hex]">[field_category]</span> <span class="marker-status col-[field_status_hex]">[field_status]</span></div>';
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
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = '';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '<span class="meta-info">von [name] | [created] | zuletzt bearbeitet vor [changed] </span>';
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
  /* Field: Vote results: Value */
  $handler->display->display_options['fields']['value']['id'] = 'value';
  $handler->display->display_options['fields']['value']['table'] = 'votingapi_cache';
  $handler->display->display_options['fields']['value']['field'] = 'value';
  $handler->display->display_options['fields']['value']['relationship'] = 'votingapi_cache';
  $handler->display->display_options['fields']['value']['label'] = '';
  $handler->display->display_options['fields']['value']['exclude'] = TRUE;
  $handler->display->display_options['fields']['value']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['value']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['value']['precision'] = '0';
  /* Field: Content: Non located report */
  $handler->display->display_options['fields']['field_common']['id'] = 'field_common';
  $handler->display->display_options['fields']['field_common']['table'] = 'field_data_field_common';
  $handler->display->display_options['fields']['field_common']['field'] = 'field_common';
  $handler->display->display_options['fields']['field_common']['label'] = '';
  $handler->display->display_options['fields']['field_common']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_common']['alter']['text'] = 'Non located report';
  $handler->display->display_options['fields']['field_common']['element_label_colon'] = FALSE;
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
  $handler->display->display_options['fields']['field_address']['element_label_colon'] = FALSE;
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_1']['id'] = 'nothing_1';
  $handler->display->display_options['fields']['nothing_1']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_1']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_1']['label'] = '';
  $handler->display->display_options['fields']['nothing_1']['alter']['text'] = '<div id="address-block">
<p class="label">Adresse</p>
<p class="field">[field_address]<br>
[field_zip] [field_city]</p>
</div>';
  $handler->display->display_options['fields']['nothing_1']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['nothing_1']['hide_alter_empty'] = TRUE;
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Content: Nid */
  $handler->display->display_options['arguments']['nid']['id'] = 'nid';
  $handler->display->display_options['arguments']['nid']['table'] = 'node';
  $handler->display->display_options['arguments']['nid']['field'] = 'nid';
  $handler->display->display_options['arguments']['nid']['default_action'] = 'not found';
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
  $handler->display->display_options['exposed_form']['options']['reset_button_label'] = 'Zurücksetzen';
  $handler->display->display_options['exposed_form']['options']['exposed_sorts_label'] = 'Sortieren';
  $handler->display->display_options['exposed_form']['options']['sort_asc_label'] = 'Aufsteigend';
  $handler->display->display_options['exposed_form']['options']['sort_desc_label'] = 'Absteigend';
  $handler->display->display_options['exposed_form']['options']['autosubmit'] = TRUE;
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['style_options']['row_class'] = 'report';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['exposed_block'] = TRUE;
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
  $handler->display->display_options['fields']['field_status']['alter']['text'] = 'Status: [field_status]';
  $handler->display->display_options['fields']['field_status']['element_type'] = '0';
  $handler->display->display_options['fields']['field_status']['element_class'] = 'marker-status col-[field_status_hex]';
  $handler->display->display_options['fields']['field_status']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_status']['hide_alter_empty'] = FALSE;
  /* Field: Content: Category */
  $handler->display->display_options['fields']['field_category']['id'] = 'field_category';
  $handler->display->display_options['fields']['field_category']['table'] = 'field_data_field_category';
  $handler->display->display_options['fields']['field_category']['field'] = 'field_category';
  $handler->display->display_options['fields']['field_category']['label'] = '';
  $handler->display->display_options['fields']['field_category']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_category']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_category']['alter']['text'] = 'Category: [field_category]';
  $handler->display->display_options['fields']['field_category']['element_type'] = '0';
  $handler->display->display_options['fields']['field_category']['element_class'] = 'marker-category col-[field_category_hex]';
  $handler->display->display_options['fields']['field_category']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_category']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['field_category']['type'] = 'taxonomy_term_reference_plain';
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
  /* Field: Meta (name, timestamp) */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['ui_name'] = 'Meta (name, timestamp)';
  $handler->display->display_options['fields']['nothing']['label'] = '';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '<span class="meta-info">by [name] | [created] </span>';
  $handler->display->display_options['fields']['nothing']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['nothing']['hide_alter_empty'] = TRUE;
  /* Field: Category / Status Bar */
  $handler->display->display_options['fields']['nothing_1']['id'] = 'nothing_1';
  $handler->display->display_options['fields']['nothing_1']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_1']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_1']['ui_name'] = 'Category / Status Bar';
  $handler->display->display_options['fields']['nothing_1']['label'] = '';
  $handler->display->display_options['fields']['nothing_1']['alter']['text'] = '<div class="cat-stat-wrapper"><span class="marker-category col-[field_category_hex]">[field_category]</span> <span class="marker-status col-[field_status_hex]">[field_status]</span></div>';
  $handler->display->display_options['fields']['nothing_1']['element_label_colon'] = FALSE;
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
  /* Field: Content: Non located report */
  $handler->display->display_options['fields']['field_common']['id'] = 'field_common';
  $handler->display->display_options['fields']['field_common']['table'] = 'field_data_field_common';
  $handler->display->display_options['fields']['field_common']['field'] = 'field_common';
  $handler->display->display_options['fields']['field_common']['label'] = '';
  $handler->display->display_options['fields']['field_common']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_common']['alter']['text'] = 'Non located content';
  $handler->display->display_options['fields']['field_common']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_common']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['field_common']['empty'] = '<div class="show-markers" title="Show marker in map" id="marker_[nid]"><span>&lt;</span></div>
';
  $handler->display->display_options['fields']['field_common']['empty_zero'] = TRUE;
  $handler->display->display_options['fields']['field_common']['type'] = 'list_key';
  /* Field: Content: Image */
  $handler->display->display_options['fields']['field_image']['id'] = 'field_image';
  $handler->display->display_options['fields']['field_image']['table'] = 'field_data_field_image';
  $handler->display->display_options['fields']['field_image']['field'] = 'field_image';
  $handler->display->display_options['fields']['field_image']['label'] = '';
  $handler->display->display_options['fields']['field_image']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_image']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['field_image']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_image']['settings'] = array(
    'image_style' => 'thumbnail',
    'image_link' => '',
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
  $handler->display->display_options['fields']['body']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['body']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['body']['settings'] = array(
    'trim_length' => '200',
  );
  /* Field: Vote results: Value */
  $handler->display->display_options['fields']['value']['id'] = 'value';
  $handler->display->display_options['fields']['value']['table'] = 'votingapi_cache';
  $handler->display->display_options['fields']['value']['field'] = 'value';
  $handler->display->display_options['fields']['value']['relationship'] = 'votingapi_cache';
  $handler->display->display_options['fields']['value']['label'] = '';
  $handler->display->display_options['fields']['value']['exclude'] = TRUE;
  $handler->display->display_options['fields']['value']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['value']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['value']['precision'] = '0';
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
  $handler->display->display_options['fields']['comment_count']['alter']['text'] = '<a href="[view_node]">[comment_count]</a>';
  $handler->display->display_options['fields']['comment_count']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['comment_count']['alter']['path'] = 'node/[nid]';
  $handler->display->display_options['fields']['comment_count']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['comment_count']['separator'] = '.';
  $handler->display->display_options['fields']['comment_count']['format_plural'] = TRUE;
  $handler->display->display_options['fields']['comment_count']['format_plural_singular'] = '1 Comment';
  $handler->display->display_options['fields']['comment_count']['format_plural_plural'] = '@count Comments';
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
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  $handler->display->display_options['path'] = 'report-list';

  /* Display: JSON View */
  $handler = $view->new_display('page', 'JSON View', 'page_2');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['exposed_form'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['reset_button'] = TRUE;
  $handler->display->display_options['exposed_form']['options']['reset_button_label'] = 'Zurücksetzen';
  $handler->display->display_options['exposed_form']['options']['exposed_sorts_label'] = 'Sortieren';
  $handler->display->display_options['exposed_form']['options']['sort_asc_label'] = 'Aufsteigend';
  $handler->display->display_options['exposed_form']['options']['sort_desc_label'] = 'Absteigend';
  $handler->display->display_options['exposed_form']['options']['autosubmit'] = TRUE;
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
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
  $handler->display->display_options['arguments']['nid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['nid']['default_argument_options']['argument'] = 'all';
  $handler->display->display_options['arguments']['nid']['default_argument_skip_url'] = TRUE;
  $handler->display->display_options['arguments']['nid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['nid']['summary_options']['items_per_page'] = '25';
  /* Contextual filter: Content: Category (field_category) */
  $handler->display->display_options['arguments']['field_category_tid']['id'] = 'field_category_tid';
  $handler->display->display_options['arguments']['field_category_tid']['table'] = 'field_data_field_category';
  $handler->display->display_options['arguments']['field_category_tid']['field'] = 'field_category_tid';
  $handler->display->display_options['arguments']['field_category_tid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_category_tid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_category_tid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_category_tid']['summary_options']['items_per_page'] = '25';
  /* Contextual filter: Content: Status (field_status) */
  $handler->display->display_options['arguments']['field_status_tid']['id'] = 'field_status_tid';
  $handler->display->display_options['arguments']['field_status_tid']['table'] = 'field_data_field_status';
  $handler->display->display_options['arguments']['field_status_tid']['field'] = 'field_status_tid';
  $handler->display->display_options['arguments']['field_status_tid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_status_tid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_status_tid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_status_tid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['path'] = 'reports/json';

  /* Display: Unpublished Reports */
  $handler = $view->new_display('page', 'Unpublished Reports', 'reports_unpublished');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['exposed_form'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['reset_button_label'] = 'Zurücksetzen';
  $handler->display->display_options['exposed_form']['options']['exposed_sorts_label'] = 'Sortieren';
  $handler->display->display_options['exposed_form']['options']['sort_asc_label'] = 'Aufsteigend';
  $handler->display->display_options['exposed_form']['options']['sort_desc_label'] = 'Absteigend';
  $handler->display->display_options['exposed_form']['options']['autosubmit'] = TRUE;
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['grouping'] = '';
  $handler->display->display_options['style_options']['columns'] = array(
    'title' => 'title',
    'changed' => 'changed',
    'field_status' => 'field_status',
    'body' => 'body',
    'field_attachment' => 'field_attachment',
    'field_category' => 'field_category',
    'field_status_hex' => 'field_status_hex',
    'field_category_hex' => 'field_category_hex',
    'value' => 'value',
    'field_relevance' => 'field_relevance',
    'field_common' => 'field_common',
    'field_category_id' => 'field_category_id',
    'field_relevance_1' => 'field_relevance_1',
    'nid' => 'nid',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'title' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'changed' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_status' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'body' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_attachment' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_category' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_status_hex' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_category_hex' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'value' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_relevance' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_common' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_category_id' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_relevance_1' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'nid' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['exposed_block'] = TRUE;
  $handler->display->display_options['defaults']['empty'] = FALSE;
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['label'] = 'This is great';
  $handler->display->display_options['empty']['area']['content'] = 'By now we have no unedited reports. Do you know a place where things can be cleaned up?';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Status */
  $handler->display->display_options['fields']['field_status']['id'] = 'field_status';
  $handler->display->display_options['fields']['field_status']['table'] = 'field_data_field_status';
  $handler->display->display_options['fields']['field_status']['field'] = 'field_status';
  $handler->display->display_options['fields']['field_status']['label'] = '';
  $handler->display->display_options['fields']['field_status']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_status']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_status']['alter']['text'] = 'Status: [field_status]';
  $handler->display->display_options['fields']['field_status']['element_type'] = '0';
  $handler->display->display_options['fields']['field_status']['element_class'] = 'marker-status col-[field_status_hex]';
  $handler->display->display_options['fields']['field_status']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_status']['hide_alter_empty'] = FALSE;
  /* Field: Content: Category */
  $handler->display->display_options['fields']['field_category']['id'] = 'field_category';
  $handler->display->display_options['fields']['field_category']['table'] = 'field_data_field_category';
  $handler->display->display_options['fields']['field_category']['field'] = 'field_category';
  $handler->display->display_options['fields']['field_category']['label'] = '';
  $handler->display->display_options['fields']['field_category']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_category']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_category']['alter']['text'] = 'Category: [field_category]';
  $handler->display->display_options['fields']['field_category']['element_type'] = '0';
  $handler->display->display_options['fields']['field_category']['element_class'] = 'marker-category col-[field_category_hex]';
  $handler->display->display_options['fields']['field_category']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_category']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['field_category']['type'] = 'taxonomy_term_reference_plain';
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
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_1']['id'] = 'nothing_1';
  $handler->display->display_options['fields']['nothing_1']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_1']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_1']['label'] = '';
  $handler->display->display_options['fields']['nothing_1']['alter']['text'] = '<div class="cat-stat-wrapper"><span class="marker-category col-[field_category_hex]">[field_category]</span> <span class="marker-status col-[field_status_hex]">[field_status]</span></div>';
  $handler->display->display_options['fields']['nothing_1']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['nothing_1']['hide_alter_empty'] = TRUE;
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
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = '';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '<span class="meta-info">von [name] | [created] Uhr</span>';
  $handler->display->display_options['fields']['nothing']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['nothing']['hide_alter_empty'] = TRUE;
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
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  /* Field: Vote results: Value */
  $handler->display->display_options['fields']['value']['id'] = 'value';
  $handler->display->display_options['fields']['value']['table'] = 'votingapi_cache';
  $handler->display->display_options['fields']['value']['field'] = 'value';
  $handler->display->display_options['fields']['value']['relationship'] = 'votingapi_cache';
  $handler->display->display_options['fields']['value']['label'] = '';
  $handler->display->display_options['fields']['value']['exclude'] = TRUE;
  $handler->display->display_options['fields']['value']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['value']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['value']['precision'] = '0';
  /* Field: Content: Non located report */
  $handler->display->display_options['fields']['field_common']['id'] = 'field_common';
  $handler->display->display_options['fields']['field_common']['table'] = 'field_data_field_common';
  $handler->display->display_options['fields']['field_common']['field'] = 'field_common';
  $handler->display->display_options['fields']['field_common']['type'] = 'list_key';
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['label'] = '';
  $handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
  /* Field: Content: Link */
  $handler->display->display_options['fields']['view_node']['id'] = 'view_node';
  $handler->display->display_options['fields']['view_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['view_node']['field'] = 'view_node';
  $handler->display->display_options['fields']['view_node']['exclude'] = TRUE;
  $handler->display->display_options['fields']['view_node']['text'] = 'mehr lesen';
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '0';
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
  /* Filter criterion: Content: Category (field_category) */
  $handler->display->display_options['filters']['field_category_tid']['id'] = 'field_category_tid';
  $handler->display->display_options['filters']['field_category_tid']['table'] = 'field_data_field_category';
  $handler->display->display_options['filters']['field_category_tid']['field'] = 'field_category_tid';
  $handler->display->display_options['filters']['field_category_tid']['group'] = 1;
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
  $handler->display->display_options['filters']['field_status_tid']['group'] = 1;
  $handler->display->display_options['filters']['field_status_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_status_tid']['expose']['operator_id'] = 'field_status_tid_op';
  $handler->display->display_options['filters']['field_status_tid']['expose']['label'] = 'Status';
  $handler->display->display_options['filters']['field_status_tid']['expose']['operator'] = 'field_status_tid_op';
  $handler->display->display_options['filters']['field_status_tid']['expose']['identifier'] = 'field_status_tid';
  $handler->display->display_options['filters']['field_status_tid']['type'] = 'select';
  $handler->display->display_options['filters']['field_status_tid']['vocabulary'] = 'status';
  $handler->display->display_options['path'] = 'report-list-unpublished';

  /* Display: JSON View App */
  $handler = $view->new_display('page', 'JSON View App', 'page_1');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['exposed_form'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
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
  /* Filter criterion: Content: Category (field_category) */
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

  /* Display: JSON View App Map */
  $handler = $view->new_display('page', 'JSON View App Map', 'page_3');
  $handler->display->display_options['display_description'] = 'Shows Reports for App Map';
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['exposed_form'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['reset_button'] = TRUE;
  $handler->display->display_options['exposed_form']['options']['reset_button_label'] = 'Zurücksetzen';
  $handler->display->display_options['exposed_form']['options']['exposed_sorts_label'] = 'Sortieren';
  $handler->display->display_options['exposed_form']['options']['sort_asc_label'] = 'Aufsteigend';
  $handler->display->display_options['exposed_form']['options']['sort_desc_label'] = 'Absteigend';
  $handler->display->display_options['exposed_form']['options']['autosubmit'] = TRUE;
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
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
  /* Filter criterion: Content: Category (field_category) */
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
  $handler->display->display_options['filters']['field_geo_lat']['operator'] = '>=';
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
  $handler->display->display_options['filters']['field_geo_lat_1']['operator'] = '<=';
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
  $handler->display->display_options['path'] = 'reports/app/map';
  $export['markers'] = $view;

  return $export;
}
