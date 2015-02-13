<?php

/**
 * @file
 *   drush integration for fontawesome.
 */

/**
 * Implementation of hook_drush_command().
 *
 * In this hook, you specify which commands your
 * drush module makes available, what it does and
 * description.
 *
 * Notice how this structure closely resembles how
 * you define menu hooks.
 *
 * @See drush_parse_command() for a list of recognized keys.
 *
 * @return
 *   An associative array describing your command(s).
 */
function fontawesome_drush_command() {
  $items = array();

  $items['fa-download'] = array(
    'callback' => 'fontawesome_drush_lib_download',
    'description' => dt('Downloads the required Fontawesome library from http://fortawesome.github.io'),
    'aliases' => array('fadl'),
    'arguments' => array(
      'path' => dt('Optional. A path to the fontawesome module. If omitted Drush will use the default location.'),
    ),
  );
  return $items;
}

/**
 * Implementation of hook_drush_help().
 *
 * This function is called whenever a drush user calls
 * 'drush help <name-of-your-command>'
 *
 * @param
 *   A string with the help section (prepend with 'drush:')
 *
 * @return
 *   A string with the help text for your command.
 */
function fontawesome_drush_help($section) {
  switch ($section) {
    case 'drush:fa-download':
      return dt("Downloads the required Fontawesome library from "
              . "http://fortawesome.github.io.");
  }
}

/**
 * Example drush command callback.
 *
 * This is where the action takes place.
 *
 * In this function, all of Drupals API is (usually) available, including
 * any functions you have added in your own modules/themes.
 *
 * To print something to the terminal window, use drush_print().
 *
 */
function fontawesome_drush_lib_download() {
  $args = func_get_args();
  if ($args[0]) {
    $path = $args[0];
  }
  else {
    //we have dependencies on libraries module so no need to check for that
    //TODO: any way to get path for libraries directory?
    //Just in case if it is site specific? e.g. sites/domain.com/libraries ?
    $path = drush_get_context('DRUSH_DRUPAL_ROOT') . '/sites/all/libraries/fontawesome';
  }
  // Create the path if it does not exist yet. Added substr check for preventing 
  // any wrong attempts or hacks !
  if (substr($path, -11) == 'fontawesome' && !is_dir($path)) {
    drush_mkdir($path);
  }
  if (is_dir($path . '/css')) {
    drush_log(dt('Fontawesome already present at @path. No download required.', array('@path' => $path)), 'ok');
  }
  elseif (drush_op('chdir', $path) &&
      drush_shell_exec('wget ' . FONTAWESOME_DOWNLOAD_URL . '  -O fontawesome.zip') &&
      drush_shell_exec('unzip fontawesome.zip') &&
      drush_shell_exec('mv Font-Awesome-*/* .') &&
      drush_shell_exec('rm -rf Font-Awesome-*')) {
    drush_log(dt('The latest Fontawesome library has been downloaded to @path', array('@path' => $path)), 'success');
  }
  else {
    drush_log(dt('Drush was unable to download the Fontawesome library to @path', array('@path' => $path)), 'error');
  }
}

/**
 * Implements drush_MODULE_post_COMMAND().
 */
function drush_fontawesome_post_pm_enable() {
  $extensions = func_get_args();
  // Deal with comma delimited extension list.
  if (strpos($extensions[0], ',') !== FALSE) {
    $extensions = explode(',', $extensions[0]);
  }

  if (in_array('fontawesome', $extensions) && !drush_get_option('skip')) {
    fontawesome_drush_lib_download();
  }
}
