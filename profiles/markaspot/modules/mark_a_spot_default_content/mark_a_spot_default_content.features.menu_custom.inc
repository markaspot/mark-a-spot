<?php
/**
 * @file
 * mark_a_spot_default_content.features.menu_custom.inc
 */

/**
 * Implements hook_menu_default_menu_custom().
 */
function mark_a_spot_default_content_menu_default_menu_custom() {
  $menus = array();

  // Exported menu: menu-secondary-navigation.
  $menus['menu-secondary-navigation'] = array(
    'menu_name' => 'menu-secondary-navigation',
    'title' => 'Secondary Navigation',
    'description' => '',
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('Secondary Navigation');

  return $menus;
}
