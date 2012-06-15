<?php
/**
 * @file
 * mark_a_spot.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function mark_a_spot_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "field_group" && $api == "field_group") {
    return array("version" => "1");
  }
  list($module, $api) = func_get_args();
  if ($module == "page_manager" && $api == "pages_default") {
    return array("version" => "1");
  }
  list($module, $api) = func_get_args();
  if ($module == "services" && $api == "services") {
    return array("version" => "3");
  }
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function mark_a_spot_views_api() {
  return array("version" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function mark_a_spot_node_info() {
  $items = array(
    'report' => array(
      'name' => t('Report'),
      'base' => 'node_content',
      'description' => t('Mark-a-Spot reports '),
      'has_title' => '1',
      'title_label' => t('Concise Title'),
      'help' => t('Describe the issue with by choosing a concise title'),
    ),
  );
  return $items;
}
