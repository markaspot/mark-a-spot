<?php

/**
 * @file
 * Contains implementations of Less module hooks.
 */

/**
 * Implements hook_less_variables().
 */
function less_demo_less_variables() {

  return array(
    '@text_glow' => 'black', // This will take precedence over .info file value.
    '@test' => '#ccc', // This value is defined for the first time here.
  );
}

/**
 * Implements hook_less_variables_alter().
 */
function less_demo_less_variables_alter(&$less_variables, $system_name) {

  if ($system_name === 'less_demo') {

    $less_variables['@text_glow'] = 'blue';
  }
}

/**
 * Implements hook_less_variables_SYSTEM_NAME_alter().
 */
function less_demo_less_variables_less_demo_alter(&$less_variables) {

  $less_variables['@test'] = 'green';
}

/**
 * Implements hook_less_functions().
 */
function less_demo_less_functions() {

  return array(
    'swap' => '_less_demo_reverse',
  );
}

/**
 * Implements hook_less_functions_alter().
 */
function less_demo_less_functions_alter(&$less_functions, $system_name) {

  if ($system_name === 'less_demo') {

  }
}

/**
 * Implements hook_less_functions_SYSTEM_NAME_alter().
 */
function less_demo_less_functions_less_demo_alter(&$less_functions) {

}

/**
 *  Implements hook_less_paths().
 */
function less_demo_less_paths() {

  return array(
    drupal_get_path('module', 'less_demo') . '/libs',
  );
}

/**
 * LESS function that takes a series of arguments and reverses their order.
 */
function _less_demo_reverse($arg) {
  list($type, $delimiter, $value) = $arg;

  if ($type === 'list') {
    $arg = array($type, $delimiter, array_reverse($value));
  }

  return $arg;
}
