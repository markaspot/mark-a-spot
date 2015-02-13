<?php
/**
 * @file
 * mark_a_spot_admin.pages_default.inc
 */

/**
 * Implements hook_default_page_manager_handlers().
 */
function mark_a_spot_admin_default_page_manager_handlers() {
  $export = array();

  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'node_edit_panel_context';
  $handler->task = 'node_edit';
  $handler->subtask = '';
  $handler->handler = 'panel_context';
  $handler->weight = 0;
  $handler->conf = array(
    'title' => 'Report Edit',
    'no_blocks' => 0,
    'pipeline' => 'standard',
    'body_classes_to_remove' => '',
    'body_classes_to_add' => '',
    'css_id' => '',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(),
    'access' => array(
      'plugins' => array(
        0 => array(
          'name' => 'entity_bundle:node',
          'settings' => array(
            'type' => array(
              'report' => 'report',
            ),
          ),
          'context' => 'argument_node_edit_1',
          'not' => FALSE,
        ),
        1 => array(
          'name' => 'role',
          'settings' => array(
            'rids' => array(
              0 => 2,
            ),
          ),
          'context' => 'logged-in-user',
          'not' => FALSE,
        ),
      ),
      'logic' => 'and',
    ),
  );
  $display = new panels_display();
  $display->layout = 'twocol_bricks';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'top' => NULL,
      'left_above' => NULL,
      'right_above' => NULL,
      'middle' => NULL,
      'left_below' => NULL,
      'right_below' => NULL,
      'bottom' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = '';
  $display->uuid = 'a8d7817b-2f8a-47c9-9b01-006791fad3cc';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-46d065bc-9405-4b79-9caf-181ec9576b90';
    $pane->panel = 'left_above';
    $pane->type = 'entity_form_field';
    $pane->subtype = 'node:field_category';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => '',
      'formatter' => '',
      'context' => 'argument_node_edit_1',
      'override_title' => 0,
      'override_title_text' => '',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = '46d065bc-9405-4b79-9caf-181ec9576b90';
    $display->content['new-46d065bc-9405-4b79-9caf-181ec9576b90'] = $pane;
    $display->panels['left_above'][0] = 'new-46d065bc-9405-4b79-9caf-181ec9576b90';
    $pane = new stdClass();
    $pane->pid = 'new-358a86de-9ae4-4f68-9f13-5781552a1904';
    $pane->panel = 'left_above';
    $pane->type = 'entity_form_field';
    $pane->subtype = 'node:field_geo';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => '',
      'formatter' => '',
      'context' => 'argument_node_edit_1',
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
    $pane->uuid = '358a86de-9ae4-4f68-9f13-5781552a1904';
    $display->content['new-358a86de-9ae4-4f68-9f13-5781552a1904'] = $pane;
    $display->panels['left_above'][1] = 'new-358a86de-9ae4-4f68-9f13-5781552a1904';
    $pane = new stdClass();
    $pane->pid = 'new-d0bbf83e-69cb-4478-9cc5-b827c2cc0e25';
    $pane->panel = 'left_above';
    $pane->type = 'entity_form_field';
    $pane->subtype = 'node:body';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => '',
      'formatter' => '',
      'context' => 'argument_node_edit_1',
      'override_title' => 0,
      'override_title_text' => '',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 2;
    $pane->locks = array();
    $pane->uuid = 'd0bbf83e-69cb-4478-9cc5-b827c2cc0e25';
    $display->content['new-d0bbf83e-69cb-4478-9cc5-b827c2cc0e25'] = $pane;
    $display->panels['left_above'][2] = 'new-d0bbf83e-69cb-4478-9cc5-b827c2cc0e25';
    $pane = new stdClass();
    $pane->pid = 'new-ba244e90-3b9b-40ad-8e4d-b68d5108d041';
    $pane->panel = 'left_above';
    $pane->type = 'entity_form_field';
    $pane->subtype = 'node:field_e_mail';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => '',
      'formatter' => '',
      'context' => 'argument_node_edit_1',
      'override_title' => 0,
      'override_title_text' => '',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 3;
    $pane->locks = array();
    $pane->uuid = 'ba244e90-3b9b-40ad-8e4d-b68d5108d041';
    $display->content['new-ba244e90-3b9b-40ad-8e4d-b68d5108d041'] = $pane;
    $display->panels['left_above'][3] = 'new-ba244e90-3b9b-40ad-8e4d-b68d5108d041';
    $pane = new stdClass();
    $pane->pid = 'new-10021fc2-e139-409b-8e4e-46ce8bbda783';
    $pane->panel = 'left_above';
    $pane->type = 'node_form_publishing';
    $pane->subtype = 'node_form_publishing';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'context' => 'argument_node_edit_1',
      'override_title' => 0,
      'override_title_text' => '',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 4;
    $pane->locks = array();
    $pane->uuid = '10021fc2-e139-409b-8e4e-46ce8bbda783';
    $display->content['new-10021fc2-e139-409b-8e4e-46ce8bbda783'] = $pane;
    $display->panels['left_above'][4] = 'new-10021fc2-e139-409b-8e4e-46ce8bbda783';
    $pane = new stdClass();
    $pane->pid = 'new-63c1b65f-497a-4ed9-b4f5-ac056fafd003';
    $pane->panel = 'right_above';
    $pane->type = 'entity_form_field';
    $pane->subtype = 'node:field_status';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => '',
      'formatter' => '',
      'context' => 'argument_node_edit_1',
      'override_title' => 0,
      'override_title_text' => '',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = '63c1b65f-497a-4ed9-b4f5-ac056fafd003';
    $display->content['new-63c1b65f-497a-4ed9-b4f5-ac056fafd003'] = $pane;
    $display->panels['right_above'][0] = 'new-63c1b65f-497a-4ed9-b4f5-ac056fafd003';
    $pane = new stdClass();
    $pane->pid = 'new-efc37c7c-1706-4840-a45a-5be87493aeed';
    $pane->panel = 'right_above';
    $pane->type = 'entity_form_field';
    $pane->subtype = 'node:field_image';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => '',
      'formatter' => '',
      'context' => 'argument_node_edit_1',
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
    $pane->uuid = 'efc37c7c-1706-4840-a45a-5be87493aeed';
    $display->content['new-efc37c7c-1706-4840-a45a-5be87493aeed'] = $pane;
    $display->panels['right_above'][1] = 'new-efc37c7c-1706-4840-a45a-5be87493aeed';
    $pane = new stdClass();
    $pane->pid = 'new-e7dce243-b73d-499c-b5ef-b988c5c69e8f';
    $pane->panel = 'right_above';
    $pane->type = 'node_form_buttons';
    $pane->subtype = 'node_form_buttons';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'context' => 'argument_node_edit_1',
      'override_title' => 0,
      'override_title_text' => '',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 2;
    $pane->locks = array();
    $pane->uuid = 'e7dce243-b73d-499c-b5ef-b988c5c69e8f';
    $display->content['new-e7dce243-b73d-499c-b5ef-b988c5c69e8f'] = $pane;
    $display->panels['right_above'][2] = 'new-e7dce243-b73d-499c-b5ef-b988c5c69e8f';
    $pane = new stdClass();
    $pane->pid = 'new-66eba5c1-a9b4-40b0-84c2-d52a8f02d23b';
    $pane->panel = 'right_above';
    $pane->type = 'entity_form_field';
    $pane->subtype = 'node:field_statement_comment';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => '',
      'formatter' => '',
      'context' => 'argument_node_edit_1',
      'override_title' => 0,
      'override_title_text' => '',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 3;
    $pane->locks = array();
    $pane->uuid = '66eba5c1-a9b4-40b0-84c2-d52a8f02d23b';
    $display->content['new-66eba5c1-a9b4-40b0-84c2-d52a8f02d23b'] = $pane;
    $display->panels['right_above'][3] = 'new-66eba5c1-a9b4-40b0-84c2-d52a8f02d23b';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = 'new-63c1b65f-497a-4ed9-b4f5-ac056fafd003';
  $handler->conf['display'] = $display;
  $export['node_edit_panel_context'] = $handler;

  return $export;
}

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
            2 => 6,
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
  $handler->name = 'page_page_admin_panel_context_2';
  $handler->task = 'page';
  $handler->subtask = 'page_admin';
  $handler->handler = 'panel_context';
  $handler->weight = 1;
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
  $display->uuid = '47b3a279-ce2b-47db-8a4a-492f665c7b7b';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-4d846969-0bb0-4542-9123-500d027c5c54';
    $pane->panel = 'bottom';
    $pane->type = 'views';
    $pane->subtype = 'mark_a_spot_admin_comments';
    $pane->shown = FALSE;
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
    $pane->uuid = '4d846969-0bb0-4542-9123-500d027c5c54';
    $display->content['new-4d846969-0bb0-4542-9123-500d027c5c54'] = $pane;
    $display->panels['bottom'][0] = 'new-4d846969-0bb0-4542-9123-500d027c5c54';
    $pane = new stdClass();
    $pane->pid = 'new-e5a2ddfb-e353-4312-a48a-01864fa27c5d';
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
    $pane->uuid = 'e5a2ddfb-e353-4312-a48a-01864fa27c5d';
    $display->content['new-e5a2ddfb-e353-4312-a48a-01864fa27c5d'] = $pane;
    $display->panels['bottom'][1] = 'new-e5a2ddfb-e353-4312-a48a-01864fa27c5d';
    $pane = new stdClass();
    $pane->pid = 'new-f02fff35-cef3-4b10-8c1f-ddc83113942c';
    $pane->panel = 'left_below';
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
      'display' => 'block_5',
      'override_title' => 0,
      'override_title_text' => '',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = 'f02fff35-cef3-4b10-8c1f-ddc83113942c';
    $display->content['new-f02fff35-cef3-4b10-8c1f-ddc83113942c'] = $pane;
    $display->panels['left_below'][0] = 'new-f02fff35-cef3-4b10-8c1f-ddc83113942c';
    $pane = new stdClass();
    $pane->pid = 'new-186bf302-379b-4eab-b66b-1013a58254d4';
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
    $pane->uuid = '186bf302-379b-4eab-b66b-1013a58254d4';
    $display->content['new-186bf302-379b-4eab-b66b-1013a58254d4'] = $pane;
    $display->panels['middle'][0] = 'new-186bf302-379b-4eab-b66b-1013a58254d4';
    $pane = new stdClass();
    $pane->pid = 'new-9fe4cc1e-79e6-4454-9435-7ce76bf1bb4b';
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
    $pane->uuid = '9fe4cc1e-79e6-4454-9435-7ce76bf1bb4b';
    $display->content['new-9fe4cc1e-79e6-4454-9435-7ce76bf1bb4b'] = $pane;
    $display->panels['middle'][1] = 'new-9fe4cc1e-79e6-4454-9435-7ce76bf1bb4b';
    $pane = new stdClass();
    $pane->pid = 'new-0dec60f5-1871-4091-a408-2d6119edaa3e';
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
    $pane->uuid = '0dec60f5-1871-4091-a408-2d6119edaa3e';
    $display->content['new-0dec60f5-1871-4091-a408-2d6119edaa3e'] = $pane;
    $display->panels['top'][0] = 'new-0dec60f5-1871-4091-a408-2d6119edaa3e';
  $display->hide_title = PANELS_TITLE_NONE;
  $display->title_pane = 'new-4d846969-0bb0-4542-9123-500d027c5c54';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $pages['page_admin'] = $page;

  return $pages;

}
