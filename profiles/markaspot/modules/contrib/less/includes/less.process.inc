<?php

/**
 * @file
 *   Contains functions related to compiling .less files.
 */


/**
 * Attach LESS settings to each file as appropriate.
 *
 * @param array[] $item
 * @param string  $key
 */
function _less_attach_settings(&$item, $key) {
  
  $defaults = array(
    'less' => less_get_settings(), // Bare defaults for LESS.
  );

  // These items must be reset for consistent operation.
  $nullify = array(
    'less' => array(
      'output_file' => NULL,
      'build_required' => NULL,
    ),
  );
  
  // Merge in any info from $item.
  $item = array_replace_recursive($defaults, $item, $nullify);
  
  $item['less']['input_file'] = $item['data'];
  
  $less_settings = less_get_settings(_less_file_owner($item['less']['input_file']));
  
  // array_replace_recursive() works on keys, flip to not use numeric keys.
  $less_settings['paths'] = array_flip($less_settings['paths']);
  $item['less']['paths'] = array_flip($item['less']['paths']);
  
  // Merge defaults with any per file settings.
  $item['less'] = array_replace_recursive($less_settings, $item['less']);
  
  // First array_flips before merge removed duplicates, so just flip back.
  $item['less']['paths'] = array_flip($item['less']['paths']);
}

/**
 * Determine output filename and add it to the settings array.
 *
 * @param array[] $item
 * @param string  $key
 */
function _less_output_path(&$item, $key) {

  $input_file = $item['less']['input_file'];
  
  $less_settings = $item['less'];
  
  // array_multisort() the data so that the hash returns the same hash regardless order of data.
  array_multisort($less_settings);
  
  $output_path_array = array(
    '!less_output_dir' => LESS_DIRECTORY,
    // Strip '.css' extension of filenames following the RTL extension pattern.
    '!input_file_basename' => basename(basename($input_file, '.less'), '.css'),
    // drupal_json_encode() is used because serialize() throws an error with lambda functions.
    '!settings_hash' => drupal_hash_base64(drupal_json_encode($less_settings)),
  );
  
  $output_path = format_string('!less_output_dir/!input_file_basename.!settings_hash.css', $output_path_array);
  
  $item['less']['output_file'] = $output_path;
}

/**
 * Check if the file needs to be rebuilt based on changes to @import'ed files.
 *
 * @param array[] $item
 * @param string  $key
 */
function _less_check_build(&$item, $key) {
  
  $input_file = $item['less']['input_file'];

  $build_required = FALSE;
  
  // Set $rebuild if this file or its children have been modified.
  if ($less_file_cache = cache_get('less:devel:' . drupal_hash_base64($input_file))) {
    
    // Iterate over each file and check if there are any changes.
    foreach ($less_file_cache->data as $filepath => $filemtime) {
      
      // Only rebuild if there has been a change to a file.
      if (is_file($filepath) && filemtime($filepath) > $filemtime) {
        $build_required = TRUE;
        break;
      }
    }
  }
  else {
    
    // No cache data, force a rebuild for later comparison.
    $build_required = TRUE;
  }
  
  $item['less']['build_required'] = $build_required;
}

/**
 * Process a .less file and save the compiled styles.
 *
 * @param array[] $item
 * @param string  $key
 *
 * @see \LessEngineInterface
 */
function _less_process_file(&$item, $key) {

  $less_settings = $item['less'];
  
  // $output_file doesn't exist or is flagged for build.
  if (!is_file($item['less']['output_file']) || !empty($item['less']['build_required'])) {
    
    $output_data = NULL;

    try {

      $engine = less_get_engine($less_settings['input_file']);

      $engine->setImportDirectories($less_settings['paths']);

      if ($less_settings[LESS_DEVEL]) {

        $engine->setSourceMaps($less_settings[LESS_SOURCE_MAPS], DRUPAL_ROOT, base_path());
      }

      $engine->modifyVariables($less_settings['variables']);

      $output_data = $engine->compile();

      if ($less_settings[LESS_DEVEL]) {

        _less_cache_dependencies($less_settings['input_file'], $engine->getDependencies());
      }
    }
    catch (Exception $e) {

      $message_vars = array(
        '@message' => $e->getMessage(),
        '%input_file' => $item['less']['input_file'],
      );

      watchdog('LESS', 'LESS error: @message, %input_file', $message_vars, WATCHDOG_ERROR);

      if (user_access(LESS_PERMISSION)) {
        drupal_set_message(t('LESS error: @message, %input_file', $message_vars), 'error');
      }
    }

    if (isset($output_data)) {
      
      // Fix paths for images as .css is in different location.
      $output_data = _less_rewrite_paths($item['less']['input_file'], $output_data);

      // Ensure the destination directory exists.
      if (_less_ensure_directory(dirname($item['less']['output_file']))) {

        file_unmanaged_save_data($output_data, $item['less']['output_file'], FILE_EXISTS_REPLACE);
      }
    }
    
    if (is_file($item['less']['output_file']) && $item['less'][LESS_AUTOPREFIXER]) {
      
      if (($lessautoprefixer_library = libraries_load('lessautoprefixer')) && $lessautoprefixer_library['installed']) {

        try {

          LessAutoprefixer::create(drupal_realpath($item['less']['output_file']))->compile();
        }
        catch (Exception $e) {

          $message_vars = array(
            '@message' => $e->getMessage(),
            '%input_file' => $item['less']['output_file'],
          );

          watchdog('LESS', 'Autoprefixer error: @message, %input_file', $message_vars, WATCHDOG_ERROR);

          if (user_access(LESS_PERMISSION)) {
            drupal_set_message(t('Autoprefixer error: @message, %input_file', $message_vars), 'error');
          }
        }
      }
    }
  }

  if (is_file($item['less']['output_file'])) {
    
    // Set render path of the stylesheet to the compiled output.
    $item['data'] = $item['less']['output_file'];
  }
}

/**
 * @param array[] $item
 * @param string  $key
 */
function _less_store_cache_info(&$item, $key) {
  
  // Only match when output_file exists.
  if ($item['data'] === $item['less']['output_file']) {
    
    $less_watch_cache = $item;
    
    $less_watch_cache['data'] = $item['less']['input_file'];
    
    cache_set('less:watch:' . drupal_hash_base64(file_create_url($item['less']['output_file'])), $less_watch_cache);
    
    // 'preprocess' being FALSE generates a discreet <link /> rather than an @import.
    $item['preprocess'] = FALSE;
  }
}

/**
 * Normalize keeping track of changed files.
 * 
 * @param string $input_file
 *   Path of source file.
 * @param string[] $dependencies
 *   Array of files that are @import'ed in $input_file, recursively.
 */
function _less_cache_dependencies($input_file, $dependencies = array()) {

  // Add $input_file to $dependencies as it is not in return from some engines.
  $dependencies = array_merge(array($input_file), (array) $dependencies);
  
  $watched_files = array();
  
  foreach ($dependencies as $dependency) {
    
    // Full path on file should enforce uniqueness in associative array.
    $watched_files[drupal_realpath($dependency)] = filemtime($dependency);
  }
  
  cache_set('less:devel:' . drupal_hash_base64($input_file), $watched_files);
}

/**
 * Copied functionality from drupal_build_css_cache() for our own purposes.
 * 
 * This function processes $contents and rewrites relative paths to be absolute
 * from web root. This is mainly used to ensure that compiled .less files still
 * reference images at their original paths.
 *
 * @param string $input_filepath
 * @param string $contents
 *
 * @return string
 *   Processed styles with replaced paths.
 * 
 * @see drupal_build_css_cache()
 */
function _less_rewrite_paths($input_filepath, $contents) {
  $output = '';
  
  // Build the base URL of this CSS file: start with the full URL.
  $css_base_url = file_create_url($input_filepath);
  // Move to the parent.
  $css_base_url = substr($css_base_url, 0, strrpos($css_base_url, '/'));
  // Simplify to a relative URL if the stylesheet URL starts with the
  // base URL of the website.
  if (substr($css_base_url, 0, strlen($GLOBALS['base_root'])) == $GLOBALS['base_root']) {
    $css_base_url = substr($css_base_url, strlen($GLOBALS['base_root']));
  }
  
  _drupal_build_css_path(NULL, $css_base_url . '/');
  // Anchor all paths in the CSS with its base URL, ignoring external and absolute paths.
  $output .= preg_replace_callback('/url\(\s*[\'"]?(?![a-z]+:|\/+)([^\'")]+)[\'"]?\s*\)/i', '_drupal_build_css_path', $contents);
  
  return $output;
}
