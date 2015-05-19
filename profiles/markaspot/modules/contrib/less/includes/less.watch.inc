<?php

/**
 * @file
 *  Contains page callback for LESS watch functionality.
 */

/**
 * Page callback for 'ajax/less/watch'.
 * 
 * Handles AJAX requests to check for changes to files while in developer mode.
 */
function _less_watch() {
  
  global $theme;
        
  drupal_page_is_cacheable(FALSE);
  
  $changed_files = array();
  
  if (variable_get(LESS_WATCH, FALSE)) {
    
    $files = (isset($_POST['less_files']) && is_array($_POST['less_files'])) ? $_POST['less_files'] : array();
    
    foreach ($files as $file) {
      
      $file_url_parts = drupal_parse_url($file);
      
      if ($cache = cache_get('less:watch:' . drupal_hash_base64($file_url_parts['path']))) {
        
        $cached_data = $cache->data;
        
        $input_file = $cached_data['less']['input_file'];
        
        $output_file = $cached_data['less']['output_file'];
        
        $current_mtime = filemtime($output_file);
        
        $theme = $cached_data['less']['theme'];
        
        $styles = array(
          '#items' => array(
            $input_file => $cached_data,
          ),
        );
        
        $styles = _less_pre_render($styles);
        
        if (filemtime($styles['#items'][$input_file]['data']) > $current_mtime) {
          $changed_files[] = array(
            'old_file' => $file_url_parts['path'],
            'new_file' => file_create_url($styles['#items'][$input_file]['data']),
          );
        }
      }
    }
  }
  
  return $changed_files;
}

