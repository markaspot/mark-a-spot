<?php
/**
 * @file
 * utilities.inc
 * Provides useful functions and common tasks.
 */

/**
 * Extracts an uploaded archive file to the specified directory.
 *
 * @param string $file
 *   The filepath or URI of the archive that should be extracted.
 * @param string $directory
 *   The directory or URI the archive should extract into.
 *
 * @return Archiver
 *   The Archiver object used to extract the archive.
 */
function icon_archive_extract($file, $directory) {
  $archiver = archiver_get_archiver($file);
  if (!$archiver) {
    throw new Exception(t('Cannot extract %file, not a valid archive.', array('%file' => $file)));
  }
  $files = $archiver->listContents();

  // Determine if archive lives in a wrapped directory matching filename.
  $info = pathinfo($file);
  $archiver->sub_directory = str_replace('.' . $info['extension'], '', $info['basename']);
  foreach ($files as $_file) {
    $pattern = '/^' . $archiver->sub_directory . '\//';
    if (!preg_match($pattern, $_file)) {
      $archiver->sub_directory = FALSE;
      break;
    }
  }
  $extract_location = $directory;
  if ($archiver->sub_directory) {
    $extract_location .= '/' . $archiver->sub_directory;
  }
  if (file_exists($extract_location)) {
    file_unmanaged_delete_recursive($extract_location);
  }
  if (!file_prepare_directory($directory, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS)) {
    throw new Exception(t('Unable to extract the archive. Please check that your Drupal <a href="!file">file system</a> is correctly configured.', array(
      '!file' => url('admin/config/media/file-system'),
    )));
  }
  try {
    $archiver->extract($directory);
  }
  catch (Exception $e) {
    // Close opened archive before trying to delete it.
    $archiver->getArchive()->close();
    if (file_exists($extract_location)) {
      file_unmanaged_delete_recursive($extract_location);
    }
    file_delete($file);
    throw new Exception($e->getMessage());
  }
  return $archiver;
}

/**
 * Icon API's custom implementation of array_merge_recursive_distinct.
 *
 * array_merge_recursive does indeed merge arrays, but it converts values with
 * duplicate keys to arrays rather than overwriting the value in the first
 * array with the duplicate value in the second array, as array_merge does.
 * I.e., with array_merge_recursive, this happens (documented behavior):
 *
 * @code
 *   array_merge_recursive(
 *     array('key' => 'org value'),
 *     array('key' => 'new value')
 *   ) === array('key' => array('org value', 'new value'));
 * @endcode
 * 
 * array_merge_recursive_distinct does not change the datatypes of the values
 * in the arrays. Matching keys' values in the second array overwrite those in
 * the first array, as is the case with array_merge, i.e.:
 *
 * @code
 *   array_merge_recursive_distinct(
 *     array('key' => 'org value'),
 *     array('key' => 'new value')
 *   ) === array('key' => 'new value');
 * @endcode
 * 
 * Parameters are passed by reference, though only for performance reasons.
 * They're not altered by this function.
 * 
 * @param array $array1
 *   Base array.
 * @param array $array2
 *   Additional array.
 *
 * @author daniel@danielsmedegaardbuus.dk
 * @return array
 *   The merged array.
 */
function &icon_array_merge_recursive(array &$array1, &$array2 = NULL) {
  $merged = $array1;
  if (is_array($array2)) {
    foreach ($array2 as $key => $val) {
      if (is_array($array2[$key])) {
        $merged[$key] = isset($merged[$key]) && is_array($merged[$key]) ? icon_array_merge_recursive($merged[$key], $array2[$key]) : $array2[$key];
      }
      else {
        $merged[$key] = $val;
      }
    }
  }
  return $merged;
}

/**
 * Determine the recursive differences between two arrays.
 *
 * @param array $array1
 *   The first array.
 * @param array $array2
 *   The second array.
 *
 * @return array
 *   If $array2 differs in any way from $array1, the differences will be
 *   returned in an array. If there are no differences, the array will be empty.
 */
function icon_array_diff_recursive(array $array1, array $array2) {
  $diff = array();
  foreach ($array2 as $key => $value) {
    if (array_key_exists($key, $array1)) {
      if (is_array($value)) {
        $recursive_diff = icon_array_diff_recursive($array1[$key], $value);
        if (count($recursive_diff)) {
          $diff[$key] = $recursive_diff;
        }
      }
      else {
        if ($value !== $array1[$key]) {
          $diff[$key] = $value;
        }
      }
    }
    else {
      $diff[$key] = $value;
    }
  }
  return $diff;
}

/**
 * Determines whether an extension implements a hook.
 *
 * @param string $type
 *   The type of extension (theme or module).
 * @param string $extension
 *   The name of the extension.
 * @param string $hook
 *   The name of the hook (e.g. "help" or "menu").
 *
 * @return bool
 *   TRUE if the extension is both installed and enabled, and the hook is
 *   implemented in that extension. FALSE if the extension does not implement
 *   the hook.
 */
function icon_extension_hook($type, $extension, $hook) {
  $hooks = &drupal_static(__FUNCTION__, array());
  // Ensure the extension is loaded.
  if (!isset($hooks[$type][$extension])) {
    if ('module' === $type) {
      drupal_load($type, $extension);
    }
    // Themes cannot be loaded via drupal_load(), instead the theme's include
    // file must be loaded.
    elseif ('theme' === $type && ($include = icon_find_theme_include($extension))) {
      include_once $include;
    }
  }
  // Check to see if the hook is implemented.
  if (!isset($hooks[$type][$extension][$hook])) {
    $hooks[$type][$extension][$hook] = ('module' === $type ? module_hook($extension, $hook) : function_exists($extension . '_' . $hook));
  }
  return $hooks[$type][$extension][$hook];
}

/**
 * Determines which extensions are implementing a hook.
 *
 * @param string $hook
 *   The name of the hook (e.g. "help" or "menu").
 *
 * @return array
 *   An array with the names of the extensions which are implementing this hook.
 */
function icon_extension_implements($hook) {
  $implements = &drupal_static(__FUNCTION__, array());
  if (!isset($implements[$hook])) {
    $implements[$hook] = array();
    // Gather the modules that implement the hook.
    foreach (module_implements($hook) as $module) {
      $implements[$hook][$module] = 'module';
    }
    // Due to how this module caches data, using the global $base_theme_info
    // variable will not work here. That array only contains the active
    // theme. We need to determine whether all enabled themes have a hook
    // definition. To do this, themes must define an 'icon.inc' file somewhere
    // in their directory structure.
    foreach (icon_enabled_themes() as $theme) {
      if ($include = icon_find_theme_include($theme)) {
        @include_once $include;
        if (function_exists($theme . '_' . $hook)) {
          $implements[$hook][$theme] = 'theme';
        }
      }
    }
  }
  return $implements[$hook];
}

/**
 * Invokes a hook in a particular extension.
 *
 * @param string $type
 *   The type of extension (theme or module).
 * @param string $extension
 *   The name of the extension.
 * @param string $hook
 *   The name of the hook to invoke.
 * @param mixed $data
 *   Arguments to pass to the hook implementation.
 * @param mixed $context1
 *   Arguments to pass to the hook implementation.
 * @param mixed $context2
 *   Arguments to pass to the hook implementation.
 *
 * The arguments listed for this function are passed by reference, however
 * more arguments sent if needed.
 *
 * @return mixed
 *   The return value of the hook implementation.
 */
function icon_extension_invoke($type, $extension, $hook, &$data = NULL, &$context1 = NULL, &$context2 = NULL) {
  $args = func_get_args();
  // Remove $type, $extension and $hook from the arguments.
  unset($args[0], $args[1], $args[2]);
  if (isset($args[3])) {
    $args[3] = &$data;
  }
  if (isset($args[4])) {
    $args[4] = &$context1;
  }
  if (isset($args[5])) {
    $args[5] = &$context2;
  }
  if (icon_extension_hook($type, $extension, $hook)) {
    return call_user_func_array($extension . '_' . $hook, $args);
  }
}

/**
 * Determine which themes are enabled.
 *
 * @return array
 *   An array of theme names.
 */
function icon_enabled_themes() {
  $themes = &drupal_static(__FUNCTION__);
  if (!isset($themes)) {
    $themes = array();
    foreach (list_themes() as $theme) {
      if (!$theme->status || in_array($theme->name, $themes)) {
        continue;
      }
      $themes[] = $theme->name;
      // Merge in the base themes if this is a sub-theme.
      if (isset($theme->base_themes)) {
        $themes += $theme->base_themes;
      }
    }
  }
  return $themes;
}

/**
 * Moves a file to a new location without database changes or hook invocation.
 *
 * @param string $source
 *   The filepath or URI where the original source file(s) reside.
 * @param string $destination
 *   The URI where $source should be moved to. This must be a stream wrapper
 *   URI. If this value is omitted, Drupal's default files scheme will be used,
 *   usually "public://".
 * @param bool|string $rename
 *   When moving a directory, the base name of $source will be used. If desired,
 *   you can effectively rename the top level directory on $destination by
 *   providing a new base name value to the $rename parameter.
 * @param int $replace
 *   Replace behavior when the destination file already exists:
 *     FILE_EXISTS_REPLACE - Replace the existing file (default behavior).
 *     FILE_EXISTS_RENAME - Append _{incrementing number} until the filename is
 *       unique.
 *     FILE_EXISTS_ERROR - Do nothing and return FALSE.
 * @param int $depth
 *   Internal use, do not use.
 *
 * @return string|false
 *   The URI of the moved file or directory, or FALSE in the event of an error.
 */
function icon_file_move_recursive($source, $destination = NULL, $rename = FALSE, $replace = FILE_EXISTS_REPLACE, $depth = 0) {
  if (is_dir(drupal_realpath($source))) {
    if (!$depth) {
      $destination .= '/' . ($rename ? $rename : drupal_basename($source));
      file_unmanaged_delete_recursive(drupal_realpath($destination));
    }
    if (!file_prepare_directory($destination, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS)) {
      return FALSE;
    }
    $dir = dir(drupal_realpath($source));
    while (($file = $dir->read()) !== FALSE) {
      if ($file == '.' || $file == '..') {
        continue;
      }
      $source_file = $source . '/' . $file;
      $destination_file = $destination . '/' . $file;
      if (!icon_file_move_recursive($source_file, $destination_file, FALSE, $replace, $depth + 1)) {
        $dir->close();
        return FALSE;
      }
    }
    $dir->close();
    if (!file_unmanaged_delete_recursive(drupal_realpath($source))) {
      return FALSE;
    }
    return $destination;
  }
  elseif (is_file(drupal_realpath($source))) {
    return file_unmanaged_copy($source, $destination, $replace);
  }
  return FALSE;
}

/**
 * Find a specific theme icon include file.
 *
 * @param string $theme
 *   The name of the theme to find an include file for.
 *
 * @return string|false
 *   The include path or FALSE if not found.
 */
function icon_find_theme_include($theme) {
  static $includes;
  $themes = &drupal_static(__FUNCTION__, array());
  if (!isset($themes[$theme])) {
    $themes[$theme] = FALSE;
    if (!isset($includes)) {
      $includes = array_keys(drupal_system_listing('/icon.inc|icons.inc$/', 'themes', 'uri', 0));
    }
    foreach ($includes as $uri) {
      $theme_path = drupal_get_path('theme', $theme);
      if (!empty($theme_path) && strpos($uri, $theme_path) !== FALSE) {
        $themes[$theme] = $uri;
        break;
      }
    }
  }
  return $themes[$theme];
}

/**
 * Wrapper function for drupal_process_attached().
 *
 * It only is invoked once per bundle, regardless of however many icons are
 * rendered from it.
 *
 * @param array $bundle
 *   The icon bundle array.
 *
 * @return bool
 *   FALSE if there were any missing library dependencies; TRUE if all library
 *   dependencies were met.
 */
function icon_process_attached(array $bundle = array()) {
  if (empty($bundle['name'])) {
    return FALSE;
  }
  $bundles = &drupal_static(__FUNCTION__, array());
  if (!isset($bundles[$bundle['name']])) {
    $bundles[$bundle['name']] = drupal_process_attached($bundle);
  }
  return $bundles[$bundle['name']];
}
