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
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['style_options']['default_row_class'] = FALSE;
  $handler->display->display_options['style_options']['row_class_special'] = FALSE;
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
  $handler->display->display_options['fields']['field_image_1']['element_class'] = 'img-responsive';
  $handler->display->display_options['fields']['field_image_1']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_image_1']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['field_image_1']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_image_1']['settings'] = array(
    'image_style' => 'mas_medium',
    'image_link' => 'content',
    'field_formatter_class' => 'img-thumbnail',
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
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '    <div class="col-md-6 col-xs-6 col-lg-6">
      [field_image_1]
      <h5>[title]</h5>
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
  $translatables['Gallery'] = array(
    t('Master'),
    t('Beiträge mit Bild'),
    t('mehr'),
    t('Anwenden'),
    t('Zurücksetzen'),
    t('Sortieren nach'),
    t('Aufsteigend'),
    t('Absteigend'),
    t('    <div class="col-md-6 col-xs-6 col-lg-6">
      [field_image_1]
      <h5>[title]</h5>
    </div>'),
    t('Block'),
    t('more'),
    t('Seite'),
  );
  $export['Gallery'] = $view;

  $view = new view();
  $view->name = 'markers';
  $view->description = 'View for all type of reports';
  $view->tag = 'default';
  $view->base_table = 'search_api_index_default_report_index';
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
  $handler->display->display_options['query']['options']['search_api_bypass_access'] = 0;
  $handler->display->display_options['query']['options']['entity_access'] = 0;
  $handler->display->display_options['query']['options']['parse_mode'] = 'direct';
  $handler->display->display_options['exposed_form']['type'] = 'better_exposed_filters';
  $handler->display->display_options['exposed_form']['options']['expose_sort_order'] = FALSE;
  $handler->display->display_options['exposed_form']['options']['autosubmit'] = TRUE;
  $handler->display->display_options['exposed_form']['options']['autosubmit_hide'] = FALSE;
  $handler->display->display_options['exposed_form']['options']['bef'] = array(
    'general' => array(
      'allow_secondary' => 0,
      'secondary_label' => 'Advanced options',
    ),
    'field_category' => array(
      'bef_format' => 'default',
      'more_options' => array(
        'bef_select_all_none' => FALSE,
        'bef_collapsible' => 0,
        'is_secondary' => 0,
        'any_label' => '',
        'bef_filter_description' => '',
        'tokens' => array(
          'available' => array(
            0 => 'global_types',
          ),
        ),
        'rewrite' => array(
          'filter_rewrite_values' => '',
        ),
      ),
    ),
    'field_status' => array(
      'bef_format' => 'default',
      'more_options' => array(
        'bef_select_all_none' => FALSE,
        'bef_collapsible' => 0,
        'is_secondary' => 0,
        'any_label' => '',
        'bef_filter_description' => '',
        'tokens' => array(
          'available' => array(
            0 => 'global_types',
          ),
        ),
        'rewrite' => array(
          'filter_rewrite_values' => '',
        ),
      ),
    ),
    'search_api_views_fulltext' => array(
      'bef_format' => 'default',
      'more_options' => array(
        'is_secondary' => 0,
        'any_label' => '',
        'bef_filter_description' => '',
        'tokens' => array(
          'available' => array(
            0 => 'global_types',
          ),
        ),
        'rewrite' => array(
          'filter_rewrite_values' => '',
        ),
      ),
    ),
  );
  $handler->display->display_options['exposed_form']['options']['input_required'] = 0;
  $handler->display->display_options['exposed_form']['options']['text_input_required_format'] = 'filtered_html';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['style_options']['grouping'] = '';
  $handler->display->display_options['row_plugin'] = 'entity';
  $handler->display->display_options['row_options']['view_mode'] = 'report_list';
  /* Sort criterion: Indexed Node: Node ID */
  $handler->display->display_options['sorts']['nid']['id'] = 'nid';
  $handler->display->display_options['sorts']['nid']['table'] = 'search_api_index_default_report_index';
  $handler->display->display_options['sorts']['nid']['field'] = 'nid';
  $handler->display->display_options['sorts']['nid']['order'] = 'DESC';
  $handler->display->display_options['filter_groups']['operator'] = 'OR';
  $handler->display->display_options['filter_groups']['groups'] = array(
    1 => 'AND',
    3 => 'AND',
    2 => 'OR',
  );
  /* Filter criterion: Indexed Node: Category */
  $handler->display->display_options['filters']['field_category']['id'] = 'field_category';
  $handler->display->display_options['filters']['field_category']['table'] = 'search_api_index_default_report_index';
  $handler->display->display_options['filters']['field_category']['field'] = 'field_category';
  $handler->display->display_options['filters']['field_category']['value'] = array();
  $handler->display->display_options['filters']['field_category']['group'] = 1;
  $handler->display->display_options['filters']['field_category']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_category']['expose']['operator_id'] = 'field_category_op';
  $handler->display->display_options['filters']['field_category']['expose']['label'] = 'Category';
  $handler->display->display_options['filters']['field_category']['expose']['operator'] = 'field_category_op';
  $handler->display->display_options['filters']['field_category']['expose']['identifier'] = 'field_category';
  $handler->display->display_options['filters']['field_category']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  $handler->display->display_options['filters']['field_category']['expose']['reduce'] = 0;
  /* Filter criterion: Indexed Node: Status */
  $handler->display->display_options['filters']['field_status']['id'] = 'field_status';
  $handler->display->display_options['filters']['field_status']['table'] = 'search_api_index_default_report_index';
  $handler->display->display_options['filters']['field_status']['field'] = 'field_status';
  $handler->display->display_options['filters']['field_status']['value'] = array();
  $handler->display->display_options['filters']['field_status']['group'] = 1;
  $handler->display->display_options['filters']['field_status']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_status']['expose']['operator_id'] = 'field_status_op';
  $handler->display->display_options['filters']['field_status']['expose']['label'] = 'Status';
  $handler->display->display_options['filters']['field_status']['expose']['operator'] = 'field_status_op';
  $handler->display->display_options['filters']['field_status']['expose']['identifier'] = 'field_status';
  $handler->display->display_options['filters']['field_status']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  $handler->display->display_options['filters']['field_status']['expose']['reduce'] = 0;
  /* Filter criterion: Search: Fulltext search */
  $handler->display->display_options['filters']['search_api_views_fulltext']['id'] = 'search_api_views_fulltext';
  $handler->display->display_options['filters']['search_api_views_fulltext']['table'] = 'search_api_index_default_report_index';
  $handler->display->display_options['filters']['search_api_views_fulltext']['field'] = 'search_api_views_fulltext';
  $handler->display->display_options['filters']['search_api_views_fulltext']['exposed'] = TRUE;
  $handler->display->display_options['filters']['search_api_views_fulltext']['expose']['operator_id'] = 'search_api_views_fulltext_op';
  $handler->display->display_options['filters']['search_api_views_fulltext']['expose']['label'] = 'Search';
  $handler->display->display_options['filters']['search_api_views_fulltext']['expose']['operator'] = 'search_api_views_fulltext_op';
  $handler->display->display_options['filters']['search_api_views_fulltext']['expose']['identifier'] = 'query';
  $handler->display->display_options['filters']['search_api_views_fulltext']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  $handler->display->display_options['filters']['search_api_views_fulltext']['min_length'] = '2';
  /* Filter criterion: Indexed Node: Status */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'search_api_index_default_report_index';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = array(
    1 => '1',
  );

  /* Display: List view */
  $handler = $view->new_display('page', 'List view', 'report_list');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['pager']['options']['expose']['items_per_page_label'] = 'Elemente pro Seite';
  $handler->display->display_options['pager']['options']['tags']['previous'] = '‹ previous Page';
  $handler->display->display_options['pager']['options']['tags']['next'] = 'next Page ›';
  $handler->display->display_options['pager']['options']['tags']['last'] = 'last Page »';
  $handler->display->display_options['exposed_block'] = TRUE;
  $handler->display->display_options['defaults']['header'] = FALSE;
  $handler->display->display_options['defaults']['empty'] = FALSE;
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['label'] = 'No Results';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'No Results found';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  $handler->display->display_options['empty']['area']['tokenize'] = TRUE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
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
  $handler->display->display_options['fields']['nothing_1']['alter']['text'] = '<div class="cat-stat-wrapper"><span class="label marker-category col-[field_category_hex] span6"><i class="icon-li icon-[field_category_icon]"></i> [field_category]</span> <span class="label marker-status col-[field_status_hex] span6"><i class="icon-li icon-[field_status_icon]"></i> [field_status]</span></div>';
  $handler->display->display_options['fields']['nothing_1']['element_type'] = '0';
  $handler->display->display_options['fields']['nothing_1']['element_label_type'] = '0';
  $handler->display->display_options['fields']['nothing_1']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['nothing_1']['element_wrapper_type'] = '0';
  $handler->display->display_options['fields']['nothing_1']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['nothing_1']['hide_alter_empty'] = TRUE;
  /* Field: Content: Link */
  $handler->display->display_options['fields']['view_node']['id'] = 'view_node';
  $handler->display->display_options['fields']['view_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['view_node']['field'] = 'view_node';
  $handler->display->display_options['fields']['view_node']['label'] = '';
  $handler->display->display_options['fields']['view_node']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['view_node']['text'] = 'Weiterlesen';
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  /* Sort criterion: Indexed Node: Node ID */
  $handler->display->display_options['sorts']['nid']['id'] = 'nid';
  $handler->display->display_options['sorts']['nid']['table'] = 'search_api_index_default_report_index';
  $handler->display->display_options['sorts']['nid']['field'] = 'nid';
  $handler->display->display_options['sorts']['nid']['order'] = 'DESC';
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['filter_groups']['operator'] = 'OR';
  $handler->display->display_options['filter_groups']['groups'] = array(
    1 => 'AND',
    2 => 'OR',
  );
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Indexed Node: Category */
  $handler->display->display_options['filters']['field_category']['id'] = 'field_category';
  $handler->display->display_options['filters']['field_category']['table'] = 'search_api_index_default_report_index';
  $handler->display->display_options['filters']['field_category']['field'] = 'field_category';
  $handler->display->display_options['filters']['field_category']['value'] = array();
  $handler->display->display_options['filters']['field_category']['group'] = 1;
  $handler->display->display_options['filters']['field_category']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_category']['expose']['operator_id'] = 'field_category_op';
  $handler->display->display_options['filters']['field_category']['expose']['label'] = 'Category';
  $handler->display->display_options['filters']['field_category']['expose']['operator'] = 'field_category_op';
  $handler->display->display_options['filters']['field_category']['expose']['identifier'] = 'category';
  $handler->display->display_options['filters']['field_category']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  $handler->display->display_options['filters']['field_category']['expose']['reduce'] = 0;
  /* Filter criterion: Indexed Node: Status */
  $handler->display->display_options['filters']['field_status']['id'] = 'field_status';
  $handler->display->display_options['filters']['field_status']['table'] = 'search_api_index_default_report_index';
  $handler->display->display_options['filters']['field_status']['field'] = 'field_status';
  $handler->display->display_options['filters']['field_status']['value'] = array();
  $handler->display->display_options['filters']['field_status']['group'] = 1;
  $handler->display->display_options['filters']['field_status']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_status']['expose']['operator_id'] = 'field_status_op';
  $handler->display->display_options['filters']['field_status']['expose']['label'] = 'Status';
  $handler->display->display_options['filters']['field_status']['expose']['operator'] = 'field_status_op';
  $handler->display->display_options['filters']['field_status']['expose']['identifier'] = 'status';
  $handler->display->display_options['filters']['field_status']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  $handler->display->display_options['filters']['field_status']['expose']['reduce'] = 0;
  /* Filter criterion: Indexed Node: Status */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'search_api_index_default_report_index';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = array(
    1 => '1',
  );
  $handler->display->display_options['filters']['status']['group'] = 1;
  /* Filter criterion: Fulltext search */
  $handler->display->display_options['filters']['search_api_views_fulltext']['id'] = 'search_api_views_fulltext';
  $handler->display->display_options['filters']['search_api_views_fulltext']['table'] = 'search_api_index_default_report_index';
  $handler->display->display_options['filters']['search_api_views_fulltext']['field'] = 'search_api_views_fulltext';
  $handler->display->display_options['filters']['search_api_views_fulltext']['ui_name'] = 'Fulltext search';
  $handler->display->display_options['filters']['search_api_views_fulltext']['group'] = 1;
  $handler->display->display_options['filters']['search_api_views_fulltext']['exposed'] = TRUE;
  $handler->display->display_options['filters']['search_api_views_fulltext']['expose']['operator_id'] = 'search_api_views_fulltext_op';
  $handler->display->display_options['filters']['search_api_views_fulltext']['expose']['label'] = 'Fulltext search';
  $handler->display->display_options['filters']['search_api_views_fulltext']['expose']['operator'] = 'search_api_views_fulltext_op';
  $handler->display->display_options['filters']['search_api_views_fulltext']['expose']['identifier'] = 'query';
  $handler->display->display_options['filters']['search_api_views_fulltext']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    5 => 0,
    4 => 0,
  );
  $handler->display->display_options['filters']['search_api_views_fulltext']['min_length'] = '2';
  $handler->display->display_options['filters']['search_api_views_fulltext']['fields'] = array(
    'body:value' => 'body:value',
    'field_address' => 'field_address',
    'title' => 'title',
    'uuid' => 'uuid',
  );
  $handler->display->display_options['path'] = 'report-list';

  /* Display: GeoJSON List */
  $handler = $view->new_display('page', 'GeoJSON List', 'geojson');
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'views_geojson';
  $handler->display->display_options['style_options']['data_source']['value'] = 'latlon';
  $handler->display->display_options['style_options']['data_source']['latitude'] = 'field_geo';
  $handler->display->display_options['style_options']['data_source']['longitude'] = 'field_geo_1';
  $handler->display->display_options['style_options']['data_source']['wkt'] = 'title';
  $handler->display->display_options['style_options']['data_source']['name_field'] = 'title';
  $handler->display->display_options['style_options']['data_source']['description_field'] = '';
  $handler->display->display_options['style_options']['jsonp_prefix'] = '';
  $handler->display->display_options['style_options']['using_views_api_mode'] = 0;
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Indexed Node: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'search_api_index_default_report_index';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['link_to_entity'] = 0;
  /* Field: Indexed Node: Date created */
  $handler->display->display_options['fields']['created_1']['id'] = 'created_1';
  $handler->display->display_options['fields']['created_1']['table'] = 'search_api_index_default_report_index';
  $handler->display->display_options['fields']['created_1']['field'] = 'created';
  $handler->display->display_options['fields']['created_1']['label'] = 'created';
  $handler->display->display_options['fields']['created_1']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['created_1']['date_format'] = 'short';
  $handler->display->display_options['fields']['created_1']['second_date_format'] = 'long';
  $handler->display->display_options['fields']['created_1']['link_to_entity'] = 0;
  /* Field: Position Longitude */
  $handler->display->display_options['fields']['field_geo_1']['id'] = 'field_geo_1';
  $handler->display->display_options['fields']['field_geo_1']['table'] = 'search_api_index_default_report_index';
  $handler->display->display_options['fields']['field_geo_1']['field'] = 'field_geo';
  $handler->display->display_options['fields']['field_geo_1']['ui_name'] = 'Position Longitude';
  $handler->display->display_options['fields']['field_geo_1']['label'] = '';
  $handler->display->display_options['fields']['field_geo_1']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_geo_1']['click_sort_column'] = 'lat';
  $handler->display->display_options['fields']['field_geo_1']['type'] = 'geolocation_longitude';
  $handler->display->display_options['fields']['field_geo_1']['settings'] = array(
    'field_formatter_class' => '',
  );
  /* Field: Position Latitude */
  $handler->display->display_options['fields']['field_geo']['id'] = 'field_geo';
  $handler->display->display_options['fields']['field_geo']['table'] = 'search_api_index_default_report_index';
  $handler->display->display_options['fields']['field_geo']['field'] = 'field_geo';
  $handler->display->display_options['fields']['field_geo']['ui_name'] = 'Position Latitude';
  $handler->display->display_options['fields']['field_geo']['label'] = '';
  $handler->display->display_options['fields']['field_geo']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_geo']['click_sort_column'] = 'lat';
  $handler->display->display_options['fields']['field_geo']['type'] = 'geolocation_latitude';
  $handler->display->display_options['fields']['field_geo']['settings'] = array(
    'field_formatter_class' => '',
  );
  /* Field: Indexed Node: UUID */
  $handler->display->display_options['fields']['uuid_1']['id'] = 'uuid_1';
  $handler->display->display_options['fields']['uuid_1']['table'] = 'search_api_index_default_report_index';
  $handler->display->display_options['fields']['uuid_1']['field'] = 'uuid';
  $handler->display->display_options['fields']['uuid_1']['label'] = 'uuid';
  $handler->display->display_options['fields']['uuid_1']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['uuid_1']['link_to_entity'] = 0;
  /* Field: Indexed Node: Node ID */
  $handler->display->display_options['fields']['nid_1']['id'] = 'nid_1';
  $handler->display->display_options['fields']['nid_1']['table'] = 'search_api_index_default_report_index';
  $handler->display->display_options['fields']['nid_1']['field'] = 'nid';
  $handler->display->display_options['fields']['nid_1']['label'] = 'nid';
  $handler->display->display_options['fields']['nid_1']['separator'] = '';
  $handler->display->display_options['fields']['nid_1']['link_to_entity'] = 0;
  /* Field: Indexed Node: Address */
  $handler->display->display_options['fields']['field_address']['id'] = 'field_address';
  $handler->display->display_options['fields']['field_address']['table'] = 'search_api_index_default_report_index';
  $handler->display->display_options['fields']['field_address']['field'] = 'field_address';
  $handler->display->display_options['fields']['field_address']['label'] = 'address';
  $handler->display->display_options['fields']['field_address']['settings'] = array(
    'field_formatter_class' => '',
  );
  /* Field: Category » Marker and Background Color: The color of the color field. (indexed) */
  $handler->display->display_options['fields']['field_category_field_category_hex_rgb']['id'] = 'field_category_field_category_hex_rgb';
  $handler->display->display_options['fields']['field_category_field_category_hex_rgb']['table'] = 'search_api_index_default_report_index';
  $handler->display->display_options['fields']['field_category_field_category_hex_rgb']['field'] = 'field_category_field_category_hex_rgb';
  $handler->display->display_options['fields']['field_category_field_category_hex_rgb']['label'] = 'category_hex';
  $handler->display->display_options['fields']['field_category_field_category_hex_rgb']['link_to_entity'] = 0;
  /* Field: Status » Marker and Background Color: The color of the color field. (indexed) */
  $handler->display->display_options['fields']['field_status_field_status_hex_rgb']['id'] = 'field_status_field_status_hex_rgb';
  $handler->display->display_options['fields']['field_status_field_status_hex_rgb']['table'] = 'search_api_index_default_report_index';
  $handler->display->display_options['fields']['field_status_field_status_hex_rgb']['field'] = 'field_status_field_status_hex_rgb';
  $handler->display->display_options['fields']['field_status_field_status_hex_rgb']['label'] = 'status_hex';
  $handler->display->display_options['fields']['field_status_field_status_hex_rgb']['link_to_entity'] = 0;
  /* Field: Status » Icon: Icon Class (indexed) */
  $handler->display->display_options['fields']['field_status_field_status_icon_class']['id'] = 'field_status_field_status_icon_class';
  $handler->display->display_options['fields']['field_status_field_status_icon_class']['table'] = 'search_api_index_default_report_index';
  $handler->display->display_options['fields']['field_status_field_status_icon_class']['field'] = 'field_status_field_status_icon_class';
  $handler->display->display_options['fields']['field_status_field_status_icon_class']['label'] = 'status_icon';
  $handler->display->display_options['fields']['field_status_field_status_icon_class']['link_to_entity'] = 0;
  /* Field: Category » Icon: Icon Class (indexed) */
  $handler->display->display_options['fields']['field_category_field_category_icon_class']['id'] = 'field_category_field_category_icon_class';
  $handler->display->display_options['fields']['field_category_field_category_icon_class']['table'] = 'search_api_index_default_report_index';
  $handler->display->display_options['fields']['field_category_field_category_icon_class']['field'] = 'field_category_field_category_icon_class';
  $handler->display->display_options['fields']['field_category_field_category_icon_class']['label'] = 'category_icon';
  $handler->display->display_options['fields']['field_category_field_category_icon_class']['link_to_entity'] = 0;
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  /* Sort criterion: Indexed Node: Node ID */
  $handler->display->display_options['sorts']['nid']['id'] = 'nid';
  $handler->display->display_options['sorts']['nid']['table'] = 'search_api_index_default_report_index';
  $handler->display->display_options['sorts']['nid']['field'] = 'nid';
  $handler->display->display_options['sorts']['nid']['order'] = 'DESC';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['filter_groups']['operator'] = 'OR';
  $handler->display->display_options['filter_groups']['groups'] = array(
    1 => 'AND',
    2 => 'OR',
  );
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Indexed Node: Category */
  $handler->display->display_options['filters']['field_category']['id'] = 'field_category';
  $handler->display->display_options['filters']['field_category']['table'] = 'search_api_index_default_report_index';
  $handler->display->display_options['filters']['field_category']['field'] = 'field_category';
  $handler->display->display_options['filters']['field_category']['value'] = array();
  $handler->display->display_options['filters']['field_category']['group'] = 1;
  $handler->display->display_options['filters']['field_category']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_category']['expose']['operator_id'] = 'field_category_op';
  $handler->display->display_options['filters']['field_category']['expose']['label'] = 'Category';
  $handler->display->display_options['filters']['field_category']['expose']['operator'] = 'field_category_op';
  $handler->display->display_options['filters']['field_category']['expose']['identifier'] = 'category';
  $handler->display->display_options['filters']['field_category']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  $handler->display->display_options['filters']['field_category']['expose']['reduce'] = 0;
  /* Filter criterion: Indexed Node: Status */
  $handler->display->display_options['filters']['field_status']['id'] = 'field_status';
  $handler->display->display_options['filters']['field_status']['table'] = 'search_api_index_default_report_index';
  $handler->display->display_options['filters']['field_status']['field'] = 'field_status';
  $handler->display->display_options['filters']['field_status']['value'] = array();
  $handler->display->display_options['filters']['field_status']['group'] = 1;
  $handler->display->display_options['filters']['field_status']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_status']['expose']['operator_id'] = 'field_status_op';
  $handler->display->display_options['filters']['field_status']['expose']['label'] = 'Status';
  $handler->display->display_options['filters']['field_status']['expose']['operator'] = 'field_status_op';
  $handler->display->display_options['filters']['field_status']['expose']['identifier'] = 'status';
  $handler->display->display_options['filters']['field_status']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  $handler->display->display_options['filters']['field_status']['expose']['reduce'] = 0;
  /* Filter criterion: Indexed Node: Status */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'search_api_index_default_report_index';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = array(
    1 => '1',
  );
  $handler->display->display_options['filters']['status']['group'] = 1;
  /* Filter criterion: Search: Fulltext search */
  $handler->display->display_options['filters']['search_api_views_fulltext']['id'] = 'search_api_views_fulltext';
  $handler->display->display_options['filters']['search_api_views_fulltext']['table'] = 'search_api_index_default_report_index';
  $handler->display->display_options['filters']['search_api_views_fulltext']['field'] = 'search_api_views_fulltext';
  $handler->display->display_options['filters']['search_api_views_fulltext']['group'] = 1;
  $handler->display->display_options['filters']['search_api_views_fulltext']['exposed'] = TRUE;
  $handler->display->display_options['filters']['search_api_views_fulltext']['expose']['operator_id'] = 'search_api_views_fulltext_op';
  $handler->display->display_options['filters']['search_api_views_fulltext']['expose']['label'] = 'Fulltext search';
  $handler->display->display_options['filters']['search_api_views_fulltext']['expose']['operator'] = 'search_api_views_fulltext_op';
  $handler->display->display_options['filters']['search_api_views_fulltext']['expose']['identifier'] = 'query';
  $handler->display->display_options['filters']['search_api_views_fulltext']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    5 => 0,
    4 => 0,
  );
  $handler->display->display_options['filters']['search_api_views_fulltext']['min_length'] = '2';
  $handler->display->display_options['filters']['search_api_views_fulltext']['fields'] = array(
    'body:value' => 'body:value',
    'field_address' => 'field_address',
    'field_statement_comment' => 'field_statement_comment',
    'title' => 'title',
    'uuid' => 'uuid',
  );
  $handler->display->display_options['path'] = 'reports/geojson';

  /* Display: GeoJSON Map */
  $handler = $view->new_display('page', 'GeoJSON Map', 'page_1');
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'views_geojson';
  $handler->display->display_options['style_options']['data_source']['value'] = 'latlon';
  $handler->display->display_options['style_options']['data_source']['latitude'] = 'field_geo';
  $handler->display->display_options['style_options']['data_source']['longitude'] = 'field_geo_1';
  $handler->display->display_options['style_options']['data_source']['wkt'] = 'title';
  $handler->display->display_options['style_options']['data_source']['name_field'] = 'title';
  $handler->display->display_options['style_options']['data_source']['description_field'] = '';
  $handler->display->display_options['style_options']['jsonp_prefix'] = '';
  $handler->display->display_options['style_options']['using_views_api_mode'] = 0;
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Indexed Node: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'search_api_index_default_report_index';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['link_to_entity'] = 0;
  /* Field: Indexed Node: Date created */
  $handler->display->display_options['fields']['created_1']['id'] = 'created_1';
  $handler->display->display_options['fields']['created_1']['table'] = 'search_api_index_default_report_index';
  $handler->display->display_options['fields']['created_1']['field'] = 'created';
  $handler->display->display_options['fields']['created_1']['label'] = 'created';
  $handler->display->display_options['fields']['created_1']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['created_1']['date_format'] = 'short';
  $handler->display->display_options['fields']['created_1']['second_date_format'] = 'long';
  $handler->display->display_options['fields']['created_1']['link_to_entity'] = 0;
  /* Field: Position Longitude */
  $handler->display->display_options['fields']['field_geo_1']['id'] = 'field_geo_1';
  $handler->display->display_options['fields']['field_geo_1']['table'] = 'search_api_index_default_report_index';
  $handler->display->display_options['fields']['field_geo_1']['field'] = 'field_geo';
  $handler->display->display_options['fields']['field_geo_1']['ui_name'] = 'Position Longitude';
  $handler->display->display_options['fields']['field_geo_1']['label'] = '';
  $handler->display->display_options['fields']['field_geo_1']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_geo_1']['click_sort_column'] = 'lat';
  $handler->display->display_options['fields']['field_geo_1']['type'] = 'geolocation_longitude';
  $handler->display->display_options['fields']['field_geo_1']['settings'] = array(
    'field_formatter_class' => '',
  );
  /* Field: Position Latitude */
  $handler->display->display_options['fields']['field_geo']['id'] = 'field_geo';
  $handler->display->display_options['fields']['field_geo']['table'] = 'search_api_index_default_report_index';
  $handler->display->display_options['fields']['field_geo']['field'] = 'field_geo';
  $handler->display->display_options['fields']['field_geo']['ui_name'] = 'Position Latitude';
  $handler->display->display_options['fields']['field_geo']['label'] = '';
  $handler->display->display_options['fields']['field_geo']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_geo']['click_sort_column'] = 'lat';
  $handler->display->display_options['fields']['field_geo']['type'] = 'geolocation_latitude';
  $handler->display->display_options['fields']['field_geo']['settings'] = array(
    'field_formatter_class' => '',
  );
  /* Field: Indexed Node: UUID */
  $handler->display->display_options['fields']['uuid_1']['id'] = 'uuid_1';
  $handler->display->display_options['fields']['uuid_1']['table'] = 'search_api_index_default_report_index';
  $handler->display->display_options['fields']['uuid_1']['field'] = 'uuid';
  $handler->display->display_options['fields']['uuid_1']['label'] = 'uuid';
  $handler->display->display_options['fields']['uuid_1']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['uuid_1']['link_to_entity'] = 0;
  /* Field: Indexed Node: Node ID */
  $handler->display->display_options['fields']['nid_1']['id'] = 'nid_1';
  $handler->display->display_options['fields']['nid_1']['table'] = 'search_api_index_default_report_index';
  $handler->display->display_options['fields']['nid_1']['field'] = 'nid';
  $handler->display->display_options['fields']['nid_1']['label'] = 'nid';
  $handler->display->display_options['fields']['nid_1']['separator'] = '';
  $handler->display->display_options['fields']['nid_1']['link_to_entity'] = 0;
  /* Field: Indexed Node: Address */
  $handler->display->display_options['fields']['field_address']['id'] = 'field_address';
  $handler->display->display_options['fields']['field_address']['table'] = 'search_api_index_default_report_index';
  $handler->display->display_options['fields']['field_address']['field'] = 'field_address';
  $handler->display->display_options['fields']['field_address']['label'] = 'address';
  $handler->display->display_options['fields']['field_address']['settings'] = array(
    'field_formatter_class' => '',
  );
  /* Field: Category » Marker and Background Color: The color of the color field. (indexed) */
  $handler->display->display_options['fields']['field_category_field_category_hex_rgb']['id'] = 'field_category_field_category_hex_rgb';
  $handler->display->display_options['fields']['field_category_field_category_hex_rgb']['table'] = 'search_api_index_default_report_index';
  $handler->display->display_options['fields']['field_category_field_category_hex_rgb']['field'] = 'field_category_field_category_hex_rgb';
  $handler->display->display_options['fields']['field_category_field_category_hex_rgb']['label'] = 'category_hex';
  $handler->display->display_options['fields']['field_category_field_category_hex_rgb']['link_to_entity'] = 0;
  /* Field: Status » Marker and Background Color: The color of the color field. (indexed) */
  $handler->display->display_options['fields']['field_status_field_status_hex_rgb']['id'] = 'field_status_field_status_hex_rgb';
  $handler->display->display_options['fields']['field_status_field_status_hex_rgb']['table'] = 'search_api_index_default_report_index';
  $handler->display->display_options['fields']['field_status_field_status_hex_rgb']['field'] = 'field_status_field_status_hex_rgb';
  $handler->display->display_options['fields']['field_status_field_status_hex_rgb']['label'] = 'status_hex';
  $handler->display->display_options['fields']['field_status_field_status_hex_rgb']['link_to_entity'] = 0;
  /* Field: Status » Icon: Icon Class (indexed) */
  $handler->display->display_options['fields']['field_status_field_status_icon_class']['id'] = 'field_status_field_status_icon_class';
  $handler->display->display_options['fields']['field_status_field_status_icon_class']['table'] = 'search_api_index_default_report_index';
  $handler->display->display_options['fields']['field_status_field_status_icon_class']['field'] = 'field_status_field_status_icon_class';
  $handler->display->display_options['fields']['field_status_field_status_icon_class']['label'] = 'status_icon';
  $handler->display->display_options['fields']['field_status_field_status_icon_class']['link_to_entity'] = 0;
  /* Field: Category » Icon: Icon Class (indexed) */
  $handler->display->display_options['fields']['field_category_field_category_icon_class']['id'] = 'field_category_field_category_icon_class';
  $handler->display->display_options['fields']['field_category_field_category_icon_class']['table'] = 'search_api_index_default_report_index';
  $handler->display->display_options['fields']['field_category_field_category_icon_class']['field'] = 'field_category_field_category_icon_class';
  $handler->display->display_options['fields']['field_category_field_category_icon_class']['label'] = 'category_icon';
  $handler->display->display_options['fields']['field_category_field_category_icon_class']['link_to_entity'] = 0;
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Indexed Node: Status */
  $handler->display->display_options['filters']['field_status']['id'] = 'field_status';
  $handler->display->display_options['filters']['field_status']['table'] = 'search_api_index_default_report_index';
  $handler->display->display_options['filters']['field_status']['field'] = 'field_status';
  $handler->display->display_options['filters']['field_status']['value'] = array();
  $handler->display->display_options['filters']['field_status']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_status']['expose']['operator_id'] = 'field_status_op';
  $handler->display->display_options['filters']['field_status']['expose']['label'] = 'Status';
  $handler->display->display_options['filters']['field_status']['expose']['operator'] = 'field_status_op';
  $handler->display->display_options['filters']['field_status']['expose']['identifier'] = 'status';
  $handler->display->display_options['filters']['field_status']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    5 => 0,
    4 => 0,
    3 => 0,
  );
  $handler->display->display_options['filters']['field_status']['expose']['reduce'] = 0;
  /* Filter criterion: Indexed Node: Category */
  $handler->display->display_options['filters']['field_category']['id'] = 'field_category';
  $handler->display->display_options['filters']['field_category']['table'] = 'search_api_index_default_report_index';
  $handler->display->display_options['filters']['field_category']['field'] = 'field_category';
  $handler->display->display_options['filters']['field_category']['value'] = array();
  $handler->display->display_options['filters']['field_category']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_category']['expose']['operator_id'] = 'field_category_op';
  $handler->display->display_options['filters']['field_category']['expose']['label'] = 'Category';
  $handler->display->display_options['filters']['field_category']['expose']['operator'] = 'field_category_op';
  $handler->display->display_options['filters']['field_category']['expose']['identifier'] = 'category';
  $handler->display->display_options['filters']['field_category']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    5 => 0,
    4 => 0,
    3 => 0,
  );
  $handler->display->display_options['filters']['field_category']['expose']['reduce'] = 0;
  /* Filter criterion: Indexed Node: Status */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'search_api_index_default_report_index';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = array(
    1 => '1',
  );
  $handler->display->display_options['path'] = 'reports/geojson/map';
  $translatables['markers'] = array(
    t('Master'),
    t('Reports Mark-a-Spot'),
    t('mehr'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Select any filter and click on Apply to see results'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('« first'),
    t('‹ previous'),
    t('next ›'),
    t('last »'),
    t('Category'),
    t('Status'),
    t('Search'),
    t('List view'),
    t('more'),
    t('Elemente pro Seite'),
    t('‹ previous Page'),
    t('next Page ›'),
    t('last Page »'),
    t('No Results'),
    t('No Results found'),
    t('<span class="submitted meta-info"> [created] </span>'),
    t('<div class="cat-stat-wrapper"><span class="label marker-category col-[field_category_hex] span6"><i class="icon-li icon-[field_category_icon]"></i> [field_category]</span> <span class="label marker-status col-[field_status_hex] span6"><i class="icon-li icon-[field_status_icon]"></i> [field_status]</span></div>'),
    t('Weiterlesen'),
    t('Fulltext search'),
    t('GeoJSON List'),
    t('Title'),
    t('created'),
    t('uuid'),
    t('nid'),
    t('.'),
    t('address'),
    t('category_hex'),
    t('status_hex'),
    t('status_icon'),
    t('category_icon'),
    t('GeoJSON Map'),
  );
  $export['markers'] = $view;

  return $export;
}
