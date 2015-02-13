<?php
/**
 * @file
 * theme.inc
 * Provides hooks and functions for theme related tasks.
 */

/**
 * Implements hook_theme().
 */
function icon_theme($existing, $type, $theme, $path) {
  $include_path = drupal_get_path('module', 'icon') . '/includes';
  $hooks['icon_bundle_overview_form'] = array(
    'render element' => 'form',
    'file' => 'admin.inc',
    'path' => $include_path,
  );
  $hooks['icon'] = array(
    'file' => 'theme.inc',
    'path' => $include_path,
    'variables' => array(
      'attributes' => array(),
      'icon' => NULL,
      'bundle' => NULL,
    ),
  );
  // Add theming hooks for render hooks so they can control their own output.
  // Due to a nasty bug in template suggestions and preprocessing, we can't do
  // something like icon_RENDER_HOOK__BUNDLE and have it be completely dynamic
  // as it probably should be.
  // @see: http://drupal.org/node/956520
  $render_hooks = array();
  foreach (icon_render_hooks() as $hook) {
    if (!isset($render_hooks[$hook['name']])) {
      $hooks['icon_' . $hook['name']] = array(
        'file' => $hook['file'],
        'path' => $hook['path'],
        'variables' => array(
          'attributes' => array(),
          'icon' => NULL,
          'bundle' => NULL,
        ),
      );
    }
  }
  return $hooks;
}

/**
 * Implements hook_preprocess_icon().
 */
function template_preprocess_icon(&$variables) {
  $bundle = &$variables['bundle'];
  $icon = &$variables['icon'];
  if (empty($bundle) || empty($icon)) {
    return;
  }
  if (is_string($bundle)) {
    $bundle = icon_bundle_load($bundle);
  }
  $render_hooks = array_keys(icon_render_hooks());
  // Check to see if the bundle is valid, the icon is a part of the bundle and
  // the bundle has a valid render hook assigned.
  if (!$bundle || !$bundle['status'] || !(isset($bundle['icons'][$icon]) || in_array($icon, $bundle['icons']) || !in_array($bundle['render'], $render_hooks))) {
    $bundle = FALSE;
    $icon = FALSE;
    return;
  }
  // Add default icon class.
  $attributes = &$variables['attributes'];
  if (!isset($attributes['class'])) {
    $attributes['class'] = array();
  }
  $attributes['class'][] = 'icon';
  // Prevent screen readers from reading icons.
  $attributes['aria-hidden'] = 'true';
  // Load additional libraries, CSS, JavaScript an other custom
  // attached data associated with this bundle.
  icon_process_attached($bundle);
}

/**
 * Theming responsibility is always passed to the bundle provider.
 *
 * WARNING: This theming hook should never be overridden.
 */
function theme_icon($variables) {
  $bundle = $variables['bundle'];
  $icon = $variables['icon'];
  if (!empty($bundle) && !empty($icon)) {
    return theme('icon_' . $bundle['render'], $variables);
  }
  return '';
}
