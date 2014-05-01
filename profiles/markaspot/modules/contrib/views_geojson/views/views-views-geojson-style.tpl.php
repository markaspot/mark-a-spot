<?php

/**
 * @file
 * Default template for the Views GeoJSON style plugin using the simple format
 *
 * Variables:
 * - $view: The View object
 * - $features: Features data to be serialized as GeoJSON
 * - $options: Array of options for this style
 *
 * @ingroup views_templates
 */

$jsonp_prefix = $options['jsonp_prefix'];

$features_collection = array(
  'type' => 'FeatureCollection',
  'features' => $features,
);

if ($view->override_path) {
  // We're inside a live preview where the GeoJSON is pretty-printed.
  $json = _views_geojson_encode_formatted($features_collection);
  if ($jsonp_prefix) $json = "$jsonp_prefix($json)";
  print "<code>$json</code>";
}
else {
  $json = drupal_json_encode($features_collection);
  if ($jsonp_prefix) {
    $json = "$jsonp_prefix($json)";
  }
  if ($options['using_views_api_mode']) {
    // We're in Views API mode.
    print $json;
  }
  else {
    // We want to send the JSON as a server response so switch the content
    // type and stop further processing of the page.
    $content_type = ($options['content_type'] == 'default') ? 'application/json' : $options['content_type'];
    drupal_add_http_header("Content-Type", "$content_type; charset=utf-8");
    print $json;
    //Don't think this is needed in .tpl.php files: module_invoke_all('exit');
    exit;
  }
}
