<?php
/**
 * @file
 * Alters the geojson map query object.
 */

/**
 * Implements hook_search_api_views_query_alter().
 */
function markaspot_logic_search_api_views_query_alter(view &$view, SearchApiViewsQuery &$query) {
  $current_display = $view->display[$view->current_display]->id;
  if ($view->name == 'markers' && $current_display == 'geojson_map'){
    if (variable_get('markaspot_logic_archive_show') == 0) {
      $status_archives = array_values(variable_get('markaspot_logic_archive_tids', markaspot_logic_defaults('markaspot_logic_archive_tids')));

      foreach ($query->where as &$condition_group) {
        foreach ($condition_group['conditions'] as &$condition) {
          foreach ($status_archives as $status_archive) {
            $subfilter = $query->createFilter('AND');
            $base_filter = $query->createFilter('AND');

            $subfilter->condition('field_status', $status_archive, '<>');
            $base_filter->filter($subfilter);
            $query->filter($base_filter);
          }
        }
      }
    }
  }
}
