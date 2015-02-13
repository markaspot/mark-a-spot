<?php
/**
 * @file
 * mark_a_spot.pages_default.inc
 */

/**
 * Implements hook_default_page_manager_handlers().
 */
function mark_a_spot_default_page_manager_handlers() {
  $export = array();

  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'node_view_panel_context';
  $handler->task = 'node_view';
  $handler->subtask = '';
  $handler->handler = 'panel_context';
  $handler->weight = 0;
  $handler->conf = array(
    'title' => 'Report View',
    'no_blocks' => 1,
    'pipeline' => 'standard',
    'css_id' => 'markers-single-view',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(),
    'access' => array(
      'plugins' => array(
        0 => array(
          'name' => 'node_type',
          'settings' => array(
            'type' => array(
              'report' => 'report',
            ),
          ),
          'context' => 'argument_entity_id:node_1',
          'not' => FALSE,
        ),
      ),
      'logic' => 'and',
    ),
  );
  $display = new panels_display();
  $display->layout = 'twocol';
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
      'left' => NULL,
      'right' => NULL,
    ),
    'right' => array(
      'style' => 'naked',
    ),
  );
  $display->cache = array();
  $display->title = '%node:title';
  $display->uuid = '188ca2e0-0023-436e-aa5d-6dfbc0b338fe';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-4aa33def-a71a-4a7d-8a9d-cd0eb5e7ac19';
    $pane->panel = 'left';
    $pane->type = 'views';
    $pane->subtype = 'markers';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'override_pager_settings' => 0,
      'use_pager' => 1,
      'nodes_per_page' => '1',
      'pager_id' => '0',
      'offset' => '0',
      'more_link' => 0,
      'feed_icons' => 0,
      'panel_args' => 0,
      'link_to_view' => 0,
      'args' => '',
      'url' => '',
      'display' => 'report_detail',
      'context' => array(
        0 => 'argument_entity_id:node_1.nid',
      ),
      'override_title' => 1,
      'override_title_text' => '',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array(
      'css_id' => '',
      'css_class' => '',
    );
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = '';
    $pane->uuid = '4aa33def-a71a-4a7d-8a9d-cd0eb5e7ac19';
    $display->content['new-4aa33def-a71a-4a7d-8a9d-cd0eb5e7ac19'] = $pane;
    $display->panels['left'][0] = 'new-4aa33def-a71a-4a7d-8a9d-cd0eb5e7ac19';
    $pane = new stdClass();
    $pane->pid = 'new-7bf0420c-eec8-47b9-8222-ee58e9777deb';
    $pane->panel = 'left';
    $pane->type = 'node_comment_form';
    $pane->subtype = 'node_comment_form';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'anon_links' => 1,
      'context' => 'argument_entity_id:node_1',
      'override_title' => 0,
      'override_title_text' => '',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array(
      'css_id' => '',
      'css_class' => '',
    );
    $pane->extras = array();
    $pane->position = 1;
    $pane->locks = '';
    $pane->uuid = '7bf0420c-eec8-47b9-8222-ee58e9777deb';
    $display->content['new-7bf0420c-eec8-47b9-8222-ee58e9777deb'] = $pane;
    $display->panels['left'][1] = 'new-7bf0420c-eec8-47b9-8222-ee58e9777deb';
    $pane = new stdClass();
    $pane->pid = 'new-fe323995-2b84-4bea-9539-00f1044b35b4';
    $pane->panel = 'left';
    $pane->type = 'node_comments';
    $pane->subtype = 'node_comments';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'mode' => '1',
      'comments_per_page' => '10',
      'context' => 'argument_entity_id:node_1',
      'override_title' => 0,
      'override_title_text' => '',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array(
      'css_id' => '',
      'css_class' => '',
    );
    $pane->extras = array();
    $pane->position = 2;
    $pane->locks = '';
    $pane->uuid = 'fe323995-2b84-4bea-9539-00f1044b35b4';
    $display->content['new-fe323995-2b84-4bea-9539-00f1044b35b4'] = $pane;
    $display->panels['left'][2] = 'new-fe323995-2b84-4bea-9539-00f1044b35b4';
    $pane = new stdClass();
    $pane->pid = 'new-ee63ab4d-08cf-4ba7-9fd5-3fa2991d86b9';
    $pane->panel = 'right';
    $pane->type = 'custom';
    $pane->subtype = 'custom';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'admin_title' => 'Map',
      'title' => '',
      'body' => '<div id="map" class="detail"></div>',
      'format' => 'full_html',
      'substitute' => 1,
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = '';
    $pane->uuid = 'ee63ab4d-08cf-4ba7-9fd5-3fa2991d86b9';
    $display->content['new-ee63ab4d-08cf-4ba7-9fd5-3fa2991d86b9'] = $pane;
    $display->panels['right'][0] = 'new-ee63ab4d-08cf-4ba7-9fd5-3fa2991d86b9';
    $pane = new stdClass();
    $pane->pid = 'new-340050b7-7a3b-4782-a595-678cc75577f6';
    $pane->panel = 'right';
    $pane->type = 'entity_field';
    $pane->subtype = 'node:field_image';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => 'hidden',
      'formatter' => 'image',
      'formatter_settings' => array(
        'image_style' => 'mas_medium',
        'image_link' => 'content',
      ),
      'context' => 'argument_entity_id:node_1',
      'override_title' => 0,
      'override_title_text' => '',
      'delta_limit' => '0',
      'delta_offset' => '0',
      'delta_reversed' => 0,
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array(
      'css_id' => '',
      'css_class' => 'thumbnail',
    );
    $pane->extras = array();
    $pane->position = 1;
    $pane->locks = '';
    $pane->uuid = '340050b7-7a3b-4782-a595-678cc75577f6';
    $display->content['new-340050b7-7a3b-4782-a595-678cc75577f6'] = $pane;
    $display->panels['right'][1] = 'new-340050b7-7a3b-4782-a595-678cc75577f6';
    $pane = new stdClass();
    $pane->pid = 'new-65418e64-a4c7-4e86-8b80-b12675fd7111';
    $pane->panel = 'right';
    $pane->type = 'block';
    $pane->subtype = 'markaspot_log-markaspot_activity';
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
    $pane->position = 2;
    $pane->locks = array();
    $pane->uuid = '65418e64-a4c7-4e86-8b80-b12675fd7111';
    $display->content['new-65418e64-a4c7-4e86-8b80-b12675fd7111'] = $pane;
    $display->panels['right'][2] = 'new-65418e64-a4c7-4e86-8b80-b12675fd7111';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = 'new-ee63ab4d-08cf-4ba7-9fd5-3fa2991d86b9';
  $handler->conf['display'] = $display;
  $export['node_view_panel_context'] = $handler;

  return $export;
}

/**
 * Implements hook_default_page_manager_pages().
 */
function mark_a_spot_default_page_manager_pages() {
  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'map';
  $page->task = 'page';
  $page->admin_title = 'page-map';
  $page->admin_description = '';
  $page->path = 'map';
  $page->access = array(
    'type' => 'none',
    'settings' => NULL,
  );
  $page->menu = array(
    'type' => 'normal',
    'title' => 'Map',
    'name' => 'main-menu',
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
    'admin_paths' => FALSE,
  );
  $page->default_handlers = array();
  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'page_map_panel_context';
  $handler->task = 'page';
  $handler->subtask = 'map';
  $handler->handler = 'panel_context';
  $handler->weight = 0;
  $handler->conf = array(
    'title' => 'Panel',
    'no_blocks' => 0,
    'pipeline' => 'standard',
    'body_classes_to_remove' => '',
    'body_classes_to_add' => '',
    'css_id' => '',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(),
  );
  $display = new panels_display();
  $display->layout = 'onecol';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'middle' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = '';
  $display->uuid = 'b97eea08-8ca2-467b-8802-367a30d70a78';
  $display->content = array();
  $display->panels = array();
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $pages['map'] = $page;

  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'page_list';
  $page->task = 'page';
  $page->admin_title = 'page-list';
  $page->admin_description = 'List reports';
  $page->path = 'list';
  $page->access = array(
    'type' => 'none',
    'settings' => NULL,
  );
  $page->menu = array(
    'type' => 'normal',
    'title' => 'Track Reports',
    'name' => 'main-menu',
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
    'admin_paths' => FALSE,
  );
  $page->default_handlers = array();
  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'page_page_list_panel_context';
  $handler->task = 'page';
  $handler->subtask = 'page_list';
  $handler->handler = 'panel_context';
  $handler->weight = 0;
  $handler->conf = array(
    'title' => 'Panel',
    'no_blocks' => 0,
    'pipeline' => 'standard',
    'css_id' => 'markers-list-view',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(),
  );
  $display = new panels_display();
  $display->layout = 'twocol_stacked';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'top' => NULL,
      'left' => NULL,
      'right' => NULL,
      'bottom' => NULL,
    ),
    'top' => array(
      'style' => 'default',
    ),
  );
  $display->cache = array();
  $display->title = '';
  $display->uuid = 'cb592afb-ad55-46d8-b7a8-d0b88bb18ec9';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-a5748f71-6415-4ed7-945d-9d532a04bf16';
    $pane->panel = 'top';
    $pane->type = 'custom';
    $pane->subtype = 'custom';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'admin_title' => '',
      'title' => 'Reports ',
      'body' => '<div class="jumbotron">
<p><a class="btn btn-large btn-success pull-right" href="node/add/report">Report a problem here.</a></p>

<p>Watch reports in your area.</p>
</div>
',
      'format' => 'full_html',
      'substitute' => TRUE,
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array(
      'css_id' => 'list-intro ',
      'css_class' => 'col-md-offset-4 col-md-8',
    );
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = '';
    $pane->uuid = 'a5748f71-6415-4ed7-945d-9d532a04bf16';
    $display->content['new-a5748f71-6415-4ed7-945d-9d532a04bf16'] = $pane;
    $display->panels['top'][0] = 'new-a5748f71-6415-4ed7-945d-9d532a04bf16';
    $pane = new stdClass();
    $pane->pid = 'new-596fb27f-973e-4069-8e96-f54403f6c661';
    $pane->panel = 'top';
    $pane->type = 'block';
    $pane->subtype = 'views--exp-markers-report_list';
    $pane->shown = TRUE;
    $pane->access = array(
      'logic' => 'and',
    );
    $pane->configuration = array(
      'inherit_path' => 1,
      'override_title' => 0,
      'override_title_text' => '',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array(
      'css_id' => '',
      'css_class' => 'col-md-offset-4 col-md-8',
    );
    $pane->extras = array();
    $pane->position = 1;
    $pane->locks = '';
    $pane->uuid = '596fb27f-973e-4069-8e96-f54403f6c661';
    $display->content['new-596fb27f-973e-4069-8e96-f54403f6c661'] = $pane;
    $display->panels['top'][1] = 'new-596fb27f-973e-4069-8e96-f54403f6c661';
    $pane = new stdClass();
    $pane->pid = 'new-869e3e3c-6298-47a7-82d9-9bbdba58da66';
    $pane->panel = 'top';
    $pane->type = 'views';
    $pane->subtype = 'markers';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'override_pager_settings' => 0,
      'use_pager' => 1,
      'nodes_per_page' => '10',
      'pager_id' => '0',
      'offset' => '0',
      'more_link' => 0,
      'feed_icons' => 0,
      'panel_args' => 1,
      'link_to_view' => 0,
      'args' => '',
      'url' => 'list',
      'display' => 'report_list',
      'override_title' => 1,
      'override_title_text' => '',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array(
      'css_id' => '',
      'css_class' => 'col-md-offset-4 col-md-8',
    );
    $pane->extras = array();
    $pane->position = 2;
    $pane->locks = '';
    $pane->uuid = '869e3e3c-6298-47a7-82d9-9bbdba58da66';
    $display->content['new-869e3e3c-6298-47a7-82d9-9bbdba58da66'] = $pane;
    $display->panels['top'][2] = 'new-869e3e3c-6298-47a7-82d9-9bbdba58da66';
  $display->hide_title = PANELS_TITLE_NONE;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $pages['page_list'] = $page;

  return $pages;

}
