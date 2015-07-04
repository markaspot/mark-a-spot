<?php
/**
 * @file
 * Openlayers integration.
 */

/**
 * Implements hook_default_sources_alter().
 */
function views_geojson_default_openlayers_sources_alter(&$sources) {
  foreach (views_get_all_views() as $view) {
    foreach ($view->display as $display => $data) {
      $view->set_display($display);
      if (in_array($view->display_handler->get_option('style_plugin'), array(
        'views_geojson_feed',
        'views_geojson',
      ))) {
        $machine_name = 'views_geojson_' . $view->name . '_' . $display;
        // Check if the configuration for this source was already defined or if
        // it has been modified. If so don't add it, to keep the source
        // exportable.
        $custom_version = ctools_export_crud_load('openlayers_sources', $machine_name);
        if (!isset($sources[$machine_name]) && empty($custom_version)) {
          $ol_source = new stdClass();
          $ol_source->api_version = 1;
          $ol_source->machine_name = $machine_name;
          $ol_source->name = 'Views GeoJSON source: ' . $view->name . '_' . $display;
          $ol_source->description = 'This source is automatically generated. See View ' . $view->name . ' at display ' . $display;
          $ol_source->factory_service = 'openlayers.Source:GeoJSON';

          $ol_source->options = array(
            'url' => url($view->display_handler->get_url(), array('absolute' => TRUE)),
          );

          // Determine if we should use a BBox strategy.
          if ($arguments = $view->display_handler->get_option('arguments')) {
            foreach ($arguments as $id => $argument) {
              if (strpos($id, 'bbox') !== FALSE && $argument['default_argument_type'] == 'querystring') {
                if (isset($argument['default_argument_options'])) {
                  $ol_source->options['useBBOX'] = TRUE;
                }
              }
            }
          }
          $sources[$ol_source->machine_name] = $ol_source;
        }
      }
    }
  }
}
