<?php
/**
 * @file
 * mark_a_spot_log.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function mark_a_spot_log_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'report_log';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'markaspot_log';
  $view->human_name = 'Report Log';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Report Log';
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'list';
  $handler->display->display_options['style_options']['default_row_class'] = FALSE;
  $handler->display->display_options['style_options']['wrapper_class'] = '.timeline';
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['row_options']['default_field_elements'] = FALSE;
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'No Log entries yet';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  /* Relationship: Report Log: Report */
  $handler->display->display_options['relationships']['nid']['id'] = 'nid';
  $handler->display->display_options['relationships']['nid']['table'] = 'markaspot_log';
  $handler->display->display_options['relationships']['nid']['field'] = 'nid';
  /* Field: Report Log: Log date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'markaspot_log';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = '';
  $handler->display->display_options['fields']['created']['exclude'] = TRUE;
  $handler->display->display_options['fields']['created']['element_label_colon'] = FALSE;
  /* Field: Report Log: Log Message */
  $handler->display->display_options['fields']['message']['id'] = 'message';
  $handler->display->display_options['fields']['message']['table'] = 'markaspot_log';
  $handler->display->display_options['fields']['message']['field'] = 'message';
  $handler->display->display_options['fields']['message']['label'] = '';
  $handler->display->display_options['fields']['message']['exclude'] = TRUE;
  $handler->display->display_options['fields']['message']['element_label_colon'] = FALSE;
  /* Field: Report Log: Log Message Css Class */
  $handler->display->display_options['fields']['css_class']['id'] = 'css_class';
  $handler->display->display_options['fields']['css_class']['table'] = 'markaspot_log';
  $handler->display->display_options['fields']['css_class']['field'] = 'css_class';
  $handler->display->display_options['fields']['css_class']['label'] = '';
  $handler->display->display_options['fields']['css_class']['exclude'] = TRUE;
  $handler->display->display_options['fields']['css_class']['element_label_colon'] = FALSE;
  /* Field: Report Log: Log Message Level */
  $handler->display->display_options['fields']['level']['id'] = 'level';
  $handler->display->display_options['fields']['level']['table'] = 'markaspot_log';
  $handler->display->display_options['fields']['level']['field'] = 'level';
  $handler->display->display_options['fields']['level']['label'] = '';
  $handler->display->display_options['fields']['level']['exclude'] = TRUE;
  $handler->display->display_options['fields']['level']['element_label_colon'] = FALSE;
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = '';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '<div class="timeline-badge [level]"><i class="glyphicon glyphicon-[css_class]"></i></div>
          <div class="timeline-panel">
            <div class="timeline-heading">
              <h4 class="timeline-title">[message]</h4>
              <p><small class="text-muted"><i class="glyphicon glyphicon-time"></i> [created] </small></p>
            </div>
            <div class="timeline-body">
              <p></p>
            </div>
          </div>';
  $handler->display->display_options['fields']['nothing']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['nothing']['element_default_classes'] = FALSE;
  /* Contextual filter: Content: Nid */
  $handler->display->display_options['arguments']['nid']['id'] = 'nid';
  $handler->display->display_options['arguments']['nid']['table'] = 'node';
  $handler->display->display_options['arguments']['nid']['field'] = 'nid';
  $handler->display->display_options['arguments']['nid']['relationship'] = 'nid';
  $handler->display->display_options['arguments']['nid']['default_action'] = 'default';
  $handler->display->display_options['arguments']['nid']['default_argument_type'] = 'node';
  $handler->display->display_options['arguments']['nid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['nid']['summary_options']['items_per_page'] = '25';

  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'report_log');
  $translatables['report_log'] = array(
    t('Master'),
    t('Report Log'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('« first'),
    t('‹ previous'),
    t('next ›'),
    t('last »'),
    t('No Log entries yet'),
    t('Report'),
    t('<div class="timeline-badge [level]"><i class="glyphicon glyphicon-[css_class]"></i></div>
          <div class="timeline-panel">
            <div class="timeline-heading">
              <h4 class="timeline-title">[message]</h4>
              <p><small class="text-muted"><i class="glyphicon glyphicon-time"></i> [created] </small></p>
            </div>
            <div class="timeline-body">
              <p></p>
            </div>
          </div>'),
    t('All'),
    t('Block'),
  );
  $export['report_log'] = $view;

  return $export;
}
