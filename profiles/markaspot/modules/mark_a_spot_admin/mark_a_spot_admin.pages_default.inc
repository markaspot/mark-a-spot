<?php
/**
 * @file
 * mark_a_spot_admin.pages_default.inc
 */

/**
 * Implements hook_default_page_manager_pages().
 */
function mark_a_spot_admin_default_page_manager_pages() {
  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'page_admin';
  $page->task = 'page';
  $page->admin_title = 'page-admin';
  $page->admin_description = '';
  $page->path = 'admin/track-content';
  $page->access = array(
    'type' => 'none',
    'settings' => NULL,
    'plugins' => array(
      0 => array(
        'name' => 'role',
        'settings' => array(
          'rids' => array(
            0 => 3,
            1 => 5,
          ),
        ),
        'context' => 'logged-in-user',
        'not' => FALSE,
      ),
    ),
    'logic' => 'and',
  );
  $page->menu = array(
    'type' => 'normal',
    'title' => 'Mark-a-Spot',
    'name' => 'management',
    'weight' => '0',
    'parent' => array(
      'type' => 'none',
      'title' => '',
      'name' => 'navigation',
      'weight' => '0',
    ),
  );
  $page->arguments = array();
  $page->conf = array(
    'admin_paths' => TRUE,
  );
  $page->default_handlers = array();
  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'page_page_admin_panel_context';
  $handler->task = 'page';
  $handler->subtask = 'page_admin';
  $handler->handler = 'panel_context';
  $handler->weight = 0;
  $handler->conf = array(
    'title' => 'Panel',
    'no_blocks' => 0,
    'pipeline' => 'standard',
    'body_classes_to_remove' => '',
    'body_classes_to_add' => '',
    'css_id' => 'markaspot-admin',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(),
  );
  $display = new panels_display();
  $display->layout = 'twocol_bricks';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'top' => NULL,
      'left' => NULL,
      'right' => NULL,
      'bottom' => NULL,
      'middle' => NULL,
      'left_above' => NULL,
      'right_above' => NULL,
      'left_below' => NULL,
      'right_below' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = 'Adminstration of Reports and Comments';
  $display->uuid = '91133508-9dc3-46bc-8105-680639ffb897';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-a900f521-ac29-4b46-8da1-02f3fc3ea164';
    $pane->panel = 'bottom';
    $pane->type = 'block';
    $pane->subtype = 'markaspot_stats-markaspot_stats';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'override_title' => 0,
      'override_title_text' => 'Statistik',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = 'a900f521-ac29-4b46-8da1-02f3fc3ea164';
    $display->content['new-a900f521-ac29-4b46-8da1-02f3fc3ea164'] = $pane;
    $display->panels['bottom'][0] = 'new-a900f521-ac29-4b46-8da1-02f3fc3ea164';
    $pane = new stdClass();
    $pane->pid = 'new-e88561ee-6c43-4cc7-9000-759d21e2e1f3';
    $pane->panel = 'bottom';
    $pane->type = 'block';
    $pane->subtype = 'user-online';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'override_title' => 0,
      'override_title_text' => '',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 1;
    $pane->locks = array();
    $pane->uuid = 'e88561ee-6c43-4cc7-9000-759d21e2e1f3';
    $display->content['new-e88561ee-6c43-4cc7-9000-759d21e2e1f3'] = $pane;
    $display->panels['bottom'][1] = 'new-e88561ee-6c43-4cc7-9000-759d21e2e1f3';
    $pane = new stdClass();
    $pane->pid = 'new-418bdcd3-349a-4b33-b25c-839ce9f5beb0';
    $pane->panel = 'middle';
    $pane->type = 'views';
    $pane->subtype = 'mark_a_spot_admin_reports';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'override_pager_settings' => 0,
      'use_pager' => 1,
      'nodes_per_page' => '10',
      'pager_id' => '1',
      'offset' => '0',
      'more_link' => 0,
      'feed_icons' => 0,
      'panel_args' => 0,
      'link_to_view' => 0,
      'args' => '',
      'url' => '',
      'display' => 'block_4',
      'override_title' => 0,
      'override_title_text' => 'Active reports',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = '418bdcd3-349a-4b33-b25c-839ce9f5beb0';
    $display->content['new-418bdcd3-349a-4b33-b25c-839ce9f5beb0'] = $pane;
    $display->panels['middle'][0] = 'new-418bdcd3-349a-4b33-b25c-839ce9f5beb0';
    $pane = new stdClass();
    $pane->pid = 'new-82662f43-c45b-46d3-becb-070c1309960e';
    $pane->panel = 'middle';
    $pane->type = 'views';
    $pane->subtype = 'mark_a_spot_admin_reports';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'override_pager_settings' => 0,
      'use_pager' => 1,
      'nodes_per_page' => '10',
      'pager_id' => '2',
      'offset' => '0',
      'more_link' => 0,
      'feed_icons' => 0,
      'panel_args' => 0,
      'link_to_view' => 0,
      'args' => '',
      'url' => '',
      'display' => 'block_1',
      'override_title' => 0,
      'override_title_text' => 'Closed Reports',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 1;
    $pane->locks = array();
    $pane->uuid = '82662f43-c45b-46d3-becb-070c1309960e';
    $display->content['new-82662f43-c45b-46d3-becb-070c1309960e'] = $pane;
    $display->panels['middle'][1] = 'new-82662f43-c45b-46d3-becb-070c1309960e';
    $pane = new stdClass();
    $pane->pid = 'new-3953aa26-23ed-4035-9fbe-80540831c5db';
    $pane->panel = 'right_below';
    $pane->type = 'views';
    $pane->subtype = 'mark_a_spot_admin_comments';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'override_pager_settings' => 0,
      'use_pager' => 1,
      'nodes_per_page' => '3',
      'pager_id' => '0',
      'offset' => '0',
      'more_link' => 0,
      'feed_icons' => 0,
      'panel_args' => 0,
      'link_to_view' => 0,
      'args' => '',
      'url' => '',
      'display' => 'default',
      'override_title' => 1,
      'override_title_text' => 'Letzte Bearbeitungskommentare',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = '3953aa26-23ed-4035-9fbe-80540831c5db';
    $display->content['new-3953aa26-23ed-4035-9fbe-80540831c5db'] = $pane;
    $display->panels['right_below'][0] = 'new-3953aa26-23ed-4035-9fbe-80540831c5db';
    $pane = new stdClass();
    $pane->pid = 'new-0d7d6544-d329-4779-8175-a4a91da57162';
    $pane->panel = 'top';
    $pane->type = 'custom';
    $pane->subtype = 'custom';
    $pane->shown = FALSE;
    $pane->access = array();
    $pane->configuration = array(
      'admin_title' => '',
      'title' => 'Hints',
      'body' => 'Lorem Ipsum dolor sit amet',
      'format' => 'filtered_html',
      'substitute' => TRUE,
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = '0d7d6544-d329-4779-8175-a4a91da57162';
    $display->content['new-0d7d6544-d329-4779-8175-a4a91da57162'] = $pane;
    $display->panels['top'][0] = 'new-0d7d6544-d329-4779-8175-a4a91da57162';
  $display->hide_title = PANELS_TITLE_NONE;
  $display->title_pane = 'new-3953aa26-23ed-4035-9fbe-80540831c5db';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $pages['page_admin'] = $page;

  return $pages;

}
