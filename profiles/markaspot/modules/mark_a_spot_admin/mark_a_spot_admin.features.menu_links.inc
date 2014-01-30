<?php
/**
 * @file
 * mark_a_spot_admin.features.menu_links.inc
 */

/**
 * Implements hook_menu_default_menu_links().
 */
function mark_a_spot_admin_menu_default_menu_links() {
  $menu_links = array();

  // Exported menu link: management:admin/track-content
  $menu_links['management:admin/track-content'] = array(
    'menu_name' => 'management',
    'link_path' => 'admin/track-content',
    'router_path' => 'admin/track-content',
    'link_title' => 'MaS Admin',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '-50',
    'parent_path' => 'admin',
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('MaS Admin');

  return $menu_links;
}
