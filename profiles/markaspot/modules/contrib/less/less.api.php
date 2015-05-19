<?php

/**
 * @file
 * Hooks provided by the Less module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Define LESS variables.
 * 
 * Should return flat associative array, where key is variable name.
 * 
 * Variables are lazy evaluated, so variables that depend on others do not have
 * to appear in order.
 * 
 * Variables returned by this function are cached, therefore values returned
 * by this function should not change. If you need variables to change from page
 * to page, use hook_less_variables_alter().
 *
 * @return array
 *
 * @see hook_less_variables_alter()
 * @see hook_less_variables_SYSTEM_NAME_alter()
 */
function hook_less_variables() {
  return array(
    '@variable_name_1' => '#ccc',
    '@variable_name_2' => 'darken(@variable_name_1, 30%)',
  );
}

/**
 * Alter LESS variables provided by other modules or themes.
 *
 * This is called before hook_less_variables_SYSTEM_NAME_alter().
 * 
 * @param &string[] $less_variables
 *   Flat associative array of variables, where key is variable name.
 * @param string    $system_name
 *   A string of the system_name of the module or theme that this applies to.
 * 
 * @see hook_less_variables()
 * @see hook_less_variables_SYSTEM_NAME_alter()
 */
function hook_less_variables_alter(array &$less_variables, $system_name) {

  if ($system_name === 'less_demo') {
    $less_variables['@variable_name_1'] = '#ddd';
  }
}

/**
 * Alter LESS variables provided by other modules or themes.
 *
 * This is called after hook_less_variables_alter().
 * 
 * @param &string[] $less_variables
 *   Flat associative array of variables, where key is variable name.
 * 
 * @see hook_less_variables()
 * @see hook_less_variables_alter()
 */
function hook_less_variables_SYSTEM_NAME_alter(array &$less_variables) {

  $less_variables['@variable_name_2'] = 'lighten(@variable_name_1, 20%)';
}

/**
 * Provide a list of lookup paths for @import statements in .less files.
 *
 * @return string[]
 */
function hook_less_paths() {

  return array(
    drupal_get_path('module', 'less_demo') . '/libs',
  );
}

/**
 * Alter LESS include paths.
 *
 * @param &string[]  $less_paths
 * @param string     $system_name
 */
function hook_less_paths_alter(array &$less_paths, $system_name) {

  if ($system_name === 'less_demo') {
    $less_paths[] = drupal_get_path('module', 'less_demo') . '/other_path';
  }
}

/**
 * Alter LESS include paths for specific module/theme.
 *
 * @param &string[] $less_paths
 */
function hook_less_paths_SYSTEM_NAME_alter(array &$less_paths) {

}

/**
 * @deprecated
 *
 * Define LESS functions.
 * 
 * @return array
 *   An associative where keys are LESS functions and values are PHP function
 *   names or anonymous functions. Anonymous functions require PHP >= 5.3.
 *
 * @see hook_less_functions_alter()
 * @see hook_less_functions_SYSTEM_NAME_alter()
 *
 * @link http://leafo.net/lessphp/docs/#custom_functions
 */
function hook_less_functions() {

  return array(
    'less_func_1' => 'php_func_1',
    'less_func_2' => function ($arg) {
      list($type, $delimiter, $value) = $arg;
      
      return array($type, $delimiter, $value);
    },
  );
}

/**
 * @deprecated
 *
 * Alter LESS functions defined by modules/themes.
 * 
 * @param string[] $less_functions
 *   Flat associative array of functions, where key is LESS function name and
 *   value is PHP function name or Anonymous function: 
 *   (http://php.net/manual/en/functions.anonymous.php)
 * @param string   $system_name
 *   A string of the system_name of the module or theme that this applies to.
 *
 * @see hook_less_functions()
 * @see hook_less_functions_SYSTEM_NAME_alter()
 *
 * @link http://leafo.net/lessphp/docs/#custom_functions
 */
function hook_less_functions_alter(array &$less_functions, $system_name) {
  
}

/**
 * @deprecated
 *
 * Alter LESS functions provided by a specific module/theme.
 * 
 * @param string[] $less_functions
 *   Flat associative array of functions, where key is variable and value is
 *   function name or Anonymous function: 
 *   (http://php.net/manual/en/functions.anonymous.php)
 *
 * @see hook_less_functions()
 * @see hook_less_functions_alter()
 *
 * @link http://leafo.net/lessphp/docs/#custom_functions
 */
function hook_less_functions_SYSTEM_NAME_alter(array &$less_functions) {
  
}

/**
 * @} End of "addtogroup hooks".
 */
