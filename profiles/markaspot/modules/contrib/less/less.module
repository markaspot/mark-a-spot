<?php

/**
 * @file
 * Handles compiling of .less files.
 *
 * The theme system allows for nearly all output of the Drupal system to be
 * customized by user themes.
 */

define('LESS_PERMISSION', 'administer less');

define('LESS_AUTOPREFIXER', 'less_autoprefixer');

define('LESS_DEVEL', 'less_devel');
define('LESS_WATCH', 'less_watch');
define('LESS_SOURCE_MAPS', 'less_source_maps');

define('LESS_DIRECTORY', 'public://less');

require_once dirname(__FILE__) . '/includes/less.libraries.inc';
require_once dirname(__FILE__) . '/includes/less.wysiwyg.inc';
require_once dirname(__FILE__) . '/includes/less.theme.inc';

/**
 * Implements hook_hook_info().
 */
function less_hook_info() {

  $less_hooks = array(
    'engines',
    'variables',
    'paths',
    'functions',
  );

  $hooks = array();

  /**
   * We don't have to worry about less_HOOK_SYSTEM_NAME_alter variations here
   * as less_HOOK_alter is run immediately before and should include the
   * MODULE.less.inc file containing any
   * less_HOOK_SYSTEM_NAME_alter() implementations.
   */
  foreach ($less_hooks as $hook) {
    $hooks[] = 'less_' . $hook;
    $hooks[] = 'less_' . $hook . '_alter';
  }

  return array_fill_keys($hooks, array(
    'group' => 'less',
  ));
}

/**
 * Implements hook_menu().
 */
function less_menu() {
  $items = array();

  $items['admin/config/development/less'] = array(
    'title' => 'LESS',
    'description' => 'Administer LESS settings',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('less_settings_form'),
    'access arguments' => array(LESS_PERMISSION),
    'file' => 'includes/less.admin.inc',
    'type' => MENU_NORMAL_ITEM,
  );

  $items['admin/config/development/less/settings'] = array(
    'title' => 'LESS Settings',
    'type' => MENU_DEFAULT_LOCAL_TASK,
  );

  $items['ajax/less/watch'] = array(
    'title' => 'LESS watch callback',
    'type' => MENU_CALLBACK,
    'page callback' => '_less_watch',
    'access callback' => 'variable_get',
    'access arguments' => array(LESS_WATCH, FALSE),
    'delivery callback' => 'drupal_json_output',
    'file' => 'includes/less.watch.inc',
  );

  return $items;
}

/**
 * Implements hook_permission().
 */
function less_permission() {
  return array(
    LESS_PERMISSION => array(
      'title' => t('Administer LESS'),
      'description' => t('Access the LESS settings page and view debug messages.'),
    ),
  );
}

/**
 * Implements hook_element_info_alter().
 */
function less_element_info_alter(&$type) {
  
  // Prepend to the list of #pre_render functions so it runs first.
  array_unshift($type['styles']['#pre_render'], '_less_pre_render');

  if (variable_get(LESS_DEVEL, FALSE)) {
    
    // Must run after drupal_pre_render_styles() to attach any attributes.
    array_push($type['styles']['#pre_render'], '_less_attach_src');
  }
}

/**
 * Add original .less file path as 'src' attribute to <link />.
 * 
 * @param array $styles
 *   CSS style tags after drupal_pre_render_styles() has run.
 * 
 * @return array
 *   Styles array with 'src' attributes on LESS files.
 * 
 * @see drupal_pre_render_styles()
 */
function _less_attach_src($styles) {
  
  foreach (element_children($styles) as $key) {
    
    // If its a <link />, then most likely its a compiled .less file.
    if ($styles[$key]['#tag'] == 'link') {
      
      // Hashes are generated based on the URL without the query portion.
      $file_url_parts = drupal_parse_url($styles[$key]['#attributes']['href']);
      
      // If we have a match, it means it is a compiled .less file.
      if ($cache = cache_get('less:watch:' . drupal_hash_base64($file_url_parts['path']))) {
        
        // Some inspectors allow 'src' attribute to open from a click.
        $styles[$key]['#attributes']['src'] = url($cache->data['less']['input_file']);
      }
    }
  }
  
  return $styles;
}

/**
 * Pre-render function for 'style' elements.
 * 
 * Key place where .less files are detected and processed.
 * 
 * @param array $styles
 *   All 'style' elements that are to display on the page.
 * 
 * @return array
 *   Modified style elements pointing to compiled LESS output.
 */
function _less_pre_render($styles) {
  
  $less_devel = (bool) variable_get(LESS_DEVEL, FALSE);
  
  if ($less_devel) {
    
    if (variable_get(LESS_WATCH, FALSE)) {
      drupal_add_js(drupal_get_path('module', 'less') . '/scripts/less.watch.js');
    }
    
    // Warn users once every hour that less is checking for file modifications.
    if (user_access(LESS_PERMISSION) && flood_is_allowed('less_devel_warning', 1)) {
      flood_register_event('less_devel_warning');
      
      $message_vars = array(
        '@url' => url('admin/config/development/less'),
      );
      drupal_set_message(t('LESS files are being checked for modifications on every request. Remember to <a href="@url">turn off</a> this feature on production websites.', $message_vars), 'status');
    }
  }
  
  $less_items = array_intersect_key($styles['#items'], array_flip(_less_children($styles['#items'])));

  if (!empty($less_items)) {
    
    require_once dirname(__FILE__) . '/includes/less.process.inc';
    
    // Attach settings to each item.
    array_walk($less_items, '_less_attach_settings');

    // Determine output path for each item.
    array_walk($less_items, '_less_output_path');

    // Check for rebuild each page.
    if ($less_devel) {

      array_walk($less_items, '_less_check_build');
    }

    // Compile '.less' files.
    array_walk($less_items, '_less_process_file');

    // Store cache information.
    if ($less_devel) {

      array_walk($less_items, '_less_store_cache_info');
    }

    $styles['#items'] = array_replace($styles['#items'], $less_items);
  }
  
  return $styles;
}

/**
 * Implements hook_admin_menu_cache_info().
 */
function less_admin_menu_cache_info() {
  
  $caches = array();
  
  // Add item to admin_menu's flush caches menu.
  $caches['less'] = array(
    'title' => t('LESS compiled files'),
    'callback' => 'less_flush_caches',
  );
  
  return $caches;
}

/**
 * Implements hook_cron_queue_info().
 *
 * This hook runs before cache flush during cron. Reliably lets us know if its
 * cron or not.
 */
function less_cron_queue_info() {

  drupal_static('less_cron', TRUE);
}

/**
 * Implements hook_flush_caches().
 *
 * Triggers rebuild of all LESS files during cache flush, except during cron.
 */
function less_flush_caches() {
  if (!drupal_static('less_cron')) {
    
    // Rebuild the less files directory.
    _less_get_dir(TRUE);
    cache_clear_all('less:', 'cache', TRUE);
  }

  less_clear_css_cache();

  return array();
}

/**
 * Deletes all stale compiled LESS files that are no longer in use.
 *
 * @see drupal_delete_file_if_stale().
 */
function less_clear_css_cache() {

  file_scan_directory(LESS_DIRECTORY, '/.+/', array('callback' => 'drupal_delete_file_if_stale'));
}

/**
 * Get/(re)generate current 'less_dir' variable.
 * 
 * @param bool $rebuild
 *   Flag to rebuild compiled output.
 * 
 * @return string
 *   current 'less_dir' Drupal variable value.
 */
function _less_get_dir($rebuild = FALSE) {
  $less_dir = variable_get('less_dir');
  
  // If drupal variable 'less_dir' is not set, empty, or manually reset, then
  // generate a new unique id and save it.
  if ($rebuild || empty($less_dir)) {
    
    // Set the less directory variable.
    variable_set('less_dir', drupal_hash_base64(uniqid('', TRUE)));
  }
  
  return variable_get('less_dir');
}

/**
 * Loads the selected LESS engine, or 'lessphp' for legacy reasons.
 * 
 * @return bool
 *   TRUE if selected LESS engine is loaded.
 */
function _less_inc() {
  static $loaded = NULL;
  
  if (!isset($loaded)) {
    
    $less_engine = variable_get('less_engine', 'lessphp');
    
    if (($less_engine_library = libraries_load($less_engine)) && $less_engine_library['installed']) {
      $loaded = $less_engine;
    }
  }
  
  return $loaded;
}

/**
 * Keeps track of .less file "ownership".
 * 
 * This keeps track of which modules and themes own which .less files, and any
 * variable defaults those system items define.
 * 
 * Only tracks .less files that are added through .info files.
 */
function _less_registry() {
  $static_stylesheets = &drupal_static('less_stylesheets');
  $static_defaults = &drupal_static('less_defaults');
  
  if (!isset($static_stylesheets) || !isset($static_defaults)) {
    
    if (($cache_stylesheets = cache_get('less:stylesheets')) && ($cache_defaults = cache_get('less:defaults'))) {
      $static_stylesheets = $cache_stylesheets->data;
      $static_defaults = $cache_defaults->data;
    }
    else {
      
      $system_types = array(
        'module_enabled',
        'theme',
      );
      
      foreach ($system_types as $system_type) {
        $system_items = system_list($system_type);
        
        foreach ($system_items as $system_item_name => $system_item) {
          
          // Register all globally included .less stylesheets.
          if (!empty($system_item->info['stylesheets'])) {
            foreach ($system_item->info['stylesheets'] as $stylesheets) {
              foreach ($stylesheets as $stylesheet) {
                if (_less_is_less_filename($stylesheet)) {
                  $static_stylesheets[$stylesheet] = $system_item_name;
                }
              }
            }
          }
          
          // Process LESS settings from .info files.
          if (isset($system_item->info['less']) && is_array($system_item->info['less'])) {
            
            // Register all non-global stylesheets.
            if (isset($system_item->info['less']['sheets']) && is_array($system_item->info['less']['sheets'])) {
              
              $system_item_path = drupal_get_path($system_item->type, $system_item->name);
              
              foreach ($system_item->info['less']['sheets'] as $stylesheet) {
                $static_stylesheets[$system_item_path . '/' . $stylesheet] = $system_item_name;
              }
            }
            
            // Register variable defaults.
            if (isset($system_item->info['less']['vars']) && is_array($system_item->info['less']['vars'])) {
              $static_defaults[$system_item_name] = $system_item->info['less']['vars'];
            }
          }
          
          // Invoke hook_less_variables(), results should be static.
          if (module_exists($system_item_name) && ($module_defaults = module_invoke($system_item_name, 'less_variables'))) {
            $static_defaults[$system_item_name] = array_replace((array) $static_defaults[$system_item_name], array_filter($module_defaults));
          }
        }
      }

      cache_set('less:stylesheets', $static_stylesheets);
      cache_set('less:defaults', $static_defaults);
    }
  }
  
}

/**
 * Returns .less file "owner".
 * 
 * Returns the owning module/theme for a passed in .less file, or NULL.
 * Only can resolve .less files that are added using .info files.
 * 
 * @param string $filepath
 *   System path to .less file, relative to DRUPAL_ROOT.
 * 
 * @return string|NULL
 *   System name of .less file "owner" or NULL in case of no known "owner".
 */
function _less_file_owner($filepath) {
  // Use the advanced drupal_static() pattern, since this is called very often.
  static $drupal_static_fast;
  if (!isset($drupal_static_fast)) {
    $drupal_static_fast['cache'] = &drupal_static('less_stylesheets');
    
    if (!isset($drupal_static_fast['cache'])) {
      _less_registry();
    }
  }
  $stylesheets_cache = &$drupal_static_fast['cache'];
  
  return isset($stylesheets_cache[$filepath]) ? $stylesheets_cache[$filepath] : NULL;
}

/**
 * Returns the compiled list of variables and functions for a module/theme.
 * 
 * @param string $system_name
 *   Module/theme system name. NULL is cast to empty string for array indexes.
 */
function less_get_settings($system_name = NULL) {
  
  // Use the advanced drupal_static() pattern, since this is called very often.
  static $drupal_static_fast;
  if (!isset($drupal_static_fast)) {
    $drupal_static_fast['cache'] = &drupal_static(__FUNCTION__);
  }
  $less_settings_static = &$drupal_static_fast['cache'];
  
  
  if (!isset($less_settings_static[$system_name])) {
    
    global $theme;

    $valid_module = !empty($system_name) && module_exists($system_name);

    $theme_settings = theme_get_setting('less', $theme);
    
    $defaults_cache = &drupal_static('less_defaults');
    
    if (!isset($defaults_cache)) {
      _less_registry();
    }
    
    // Defaults.
    $data = array(
      'build_cache_id' => _less_get_dir(),
      'variables' => array(),
      'functions' => array(
        'token' => '_less_token_replace',
      ),
      'paths' => array(),
      LESS_AUTOPREFIXER => (bool) variable_get(LESS_AUTOPREFIXER, FALSE),
      LESS_DEVEL => (bool) variable_get(LESS_DEVEL, FALSE),
      LESS_SOURCE_MAPS => (bool) variable_get(LESS_SOURCE_MAPS, FALSE),
      'theme' => $theme,
    );
    
    
    /*
     * Compile the LESS variables.
     */
    // Cached default variables from .info files and hook_less_variables().
    if (!empty($defaults_cache[$system_name])) {
      $data['variables'] = array_replace($data['variables'], array_filter($defaults_cache[$system_name]));
    }
    
    // Saved variable values from current theme.
    if (!is_null($theme_settings) && !empty($theme_settings[$system_name])) {
      $data['variables'] = array_replace($data['variables'], array_filter($theme_settings[$system_name]));
    }
    
    // Prevent $system_name from being altered.
    $alter_system_name = $system_name;
    // Invoke hook_less_variables_alter().
    drupal_alter('less_variables', $data['variables'], $alter_system_name);
    // Invoke hook_less_variables_SYSTEM_NAME_alter().
    drupal_alter('less_variables_' . $system_name, $data['variables']);
    
    
    /*
     * Grab the LESS functions.
     * 
     * LESS functions are not stored in the cache table since they could be
     * anonymous functions.
     */
    if ($valid_module && module_hook($system_name, 'less_functions')) {
      $data['functions'] = array_replace($data['functions'], (array) module_invoke($system_name, 'less_functions'));
    }
    
    // Prevent $system_name from being altered.
    $alter_system_name = $system_name;
    // Invoke hook_less_functions_alter().
    drupal_alter('less_functions', $data['functions'], $alter_system_name);
    // Invoke hook_less_functions_SYSTEM_NAME_alter().
    drupal_alter('less_functions_' . $system_name, $data['functions']);
    
    
    /*
     * Grab the LESS include paths.
     * 
     */
    if ($valid_module && module_hook($system_name, 'less_paths')) {
      $data['paths'] = array_unique(array_merge($data['paths'], (array) module_invoke($system_name, 'less_paths')));
    }
    
    // Prevent $system_name from being altered.
    $alter_system_name = $system_name;
    // Invoke hook_less_paths_alter().
    drupal_alter('less_paths', $data['paths'], $alter_system_name);
    // Invoke hook_less_paths_SYSTEM_NAME_alter().
    drupal_alter('less_paths_' . $system_name, $data['paths']);
    
    $data['paths'] = array_unique($data['paths']);
    
    $less_settings_static[$system_name] = $data;
  }
  
  // Don't need to test isset(), there will always be data at $system_name.
  return $less_settings_static[$system_name];
}

/**
 * Handler for LESS function token().
 *
 * @param string[] $arg
 *
 * @return array
 */
function _less_token_replace($arg) {
  list($type, $delimiter, $value) = $arg;
  
  return array($type, $delimiter, array(token_replace($value[0])));
}

/**
 * Helper function that attempts to create a folder if it doesn't exist.
 * 
 * Locks are used to help avoid concurrency collisions.
 * 
 * @param string $directory_path
 *   Directory of which to create/confirm existence.
 * 
 * @return bool
 *   Value indicating existence of directory.
 */
function _less_ensure_directory($directory_path) {
  
  $is_dir = is_dir($directory_path);
  
  if (!$is_dir) {
    
    $lock_id = 'less_directory_' . md5($directory_path);
    
    // Attempt to create directory only 3 times, else delay is too long.
    for ($i = 0; $i < 3; $i++) {
      
      if (lock_acquire($lock_id) && $is_dir = file_prepare_directory($directory_path, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS)) {
        // Creation was successful, cancel the 'for' loop;
        break;
      }
      
      lock_wait($lock_id, 1);
    }
    
    lock_release($lock_id);
    
    if (!$is_dir) {
      // There is a problem with the directory.
      $message_vars = array(
        '%dir' => $directory_path,
      );
      
      watchdog('LESS', 'LESS could not create a directory in %dir', $message_vars, WATCHDOG_ERROR);
      
      if (user_access(LESS_PERMISSION)) {
        drupal_set_message(t('LESS could not create a directory in %dir', $message_vars), 'error', FALSE);
      }
      
    }
  }
  
  return $is_dir;
}

/**
 * Return keys from array that match '.less' file extension.
 * 
 * @param array $items
 *   An array where keys are expected to be filepaths.
 * 
 * @return array
 *   Array of matching filepaths.
 */
function _less_children($items) {
  
  return array_filter(array_keys($items), '_less_is_less_filename');
  
}

/**
 * Check if filename has '.less' extension.
 * 
 * @param string $filename
 *   File name/path to search for '.less' extension.
 * 
 * @return bool
 *   TRUE if $filename does end with '.less'.
 */
function _less_is_less_filename($filename) {
  
  return drupal_substr($filename, -5) === '.less';
}

/**
 * Implements hook_less_engines().
 *
 * @return string[]
 */
function less_less_engines() {

  return array(
    'less.php' => 'LessEngineLess_php',
    'lessphp' => 'LessEngineLessphp',
    'less.js' => 'LessEngineLess_js',
  );
}

/**
 * @return \LessEngineInterface[]
 */
function _less_get_engines() {

  $registered_engines = module_invoke_all('less_engines');
  drupal_alter('less_engines', $registered_engines);

  return $registered_engines;
}

/**
 * @param $input_file_path
 *
 * @return \LessEngine
 *
 * @throws Exception
 */
function less_get_engine($input_file_path) {

  $engines = _less_get_engines();
  $selected_engine = _less_inc();

  if (!empty($engines[$selected_engine])) {

    $class_name = $engines[$selected_engine];

    return new $class_name($input_file_path);
  }
  else {

    throw new Exception('Unable to load LessEngine.');
  }
}
