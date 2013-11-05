<?php
/**
 * @file
 * mark_a_spot_default_content.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function mark_a_spot_default_content_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}
