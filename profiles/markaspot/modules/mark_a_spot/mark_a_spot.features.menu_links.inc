<?php
/**
 * @file
 * mark_a_spot.features.menu_links.inc
 */

/**
 * Implements hook_menu_default_menu_links().
 */
function mark_a_spot_menu_default_menu_links() {
  $menu_links = array();

  // Exported menu link: main-menu:list
  $menu_links['main-menu:list'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'list',
    'router_path' => 'list',
    'link_title' => 'Report List',
    'options' => array(
      'attributes' => array(),
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '0',
  );
  // Exported menu link: main-menu:map
  $menu_links['main-menu:map'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'map',
    'router_path' => 'map',
    'link_title' => 'Map view',
    'options' => array(),
    'module' => 'system',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '0',
  );
  // Exported menu link: main-menu:node/add/report
  $menu_links['main-menu:node/add/report'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'node/add/report',
    'router_path' => 'node/add/report',
    'link_title' => 'Report an Issue',
    'options' => array(
      'attributes' => array(),
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '-49',
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('Map view');
  t('Report List');
  t('Report an Issue');


  return $menu_links;
}
