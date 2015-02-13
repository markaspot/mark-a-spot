<?php
/**
 * @file
 * render.inc
 * Provides hooks and theme callbacks for default render hooks.
 * @TODO add a "sprite" render hook.
 */

/**
 * Define render hook information.
 *
 * @see hook_icon_render_hooks()
 */
function icon_icon_render_hooks() {
  $hooks['image'] = array(
    'file' => 'render.inc',
    'path' => drupal_get_path('module', 'icon') . '/includes',
  );
  $hooks['sprite'] = array(
    'file' => 'render.inc',
    'path' => drupal_get_path('module', 'icon') . '/includes',
  );
  return $hooks;
}

/**
 * Implements hook_preprocess_icon_RENDER_HOOK().
 */
function icon_preprocess_icon_image(&$variables) {
  $bundle = &$variables['bundle'];
  $icon = &$variables['icon'];
  if (!isset($bundle['settings']['extension'])) {
    $bundle['settings']['extension'] = 'png';
  }
  // Sanitize and add icon class here.
  $class = drupal_html_class($icon);
  // Check to see if image starts with a alpha-numeric character. If it doesn't,
  // prepend it so it can be a proper CSS class.
  if (is_numeric($class[0])) {
    $class = 'image-' . $class;
  }
  $variables['attributes']['class'][] = $class;
}

/**
 * Implements theme_icon_RENDER_HOOK().
 *
 * Return an image of the requested icon.
 */
function theme_icon_image($variables) {
  $output = '';
  $bundle = $variables['bundle'];
  $icon = $variables['icon'];
  $image = $bundle['path'] . '/' . $icon . '.' . $bundle['settings']['extension'];
  if (file_exists($image) && ($info = image_get_info($image))) {
    $output = theme('image', array(
      'path' => $image,
      'height' => $info['height'],
      'width' => $info['width'],
      'attributes' => $variables['attributes'],
    ));
  }
  return $output;
}

/**
 * Implements hook_preprocess_icon_RENDER_HOOK().
 */
function icon_preprocess_icon_sprite(&$variables) {
  $bundle = &$variables['bundle'];
  $icon = &$variables['icon'];
  // Add default settings.
  if (!isset($bundle['settings']['tag'])) {
    $bundle['settings']['tag'] = 'span';
  }
  if (!isset($bundle['settings']['content'])) {
    $bundle['settings']['content'] = '';
  }
  // Sanitize and add icon class here. Don't need to check for numeric because
  // these are already CSS class name based.
  $variables['attributes']['class'][] = drupal_html_class($icon);
}

/**
 * Implements theme_icon_RENDER_HOOK().
 *
 * Return a tag with sprite class name of requested icon.
 */
function theme_icon_sprite($variables) {
  $bundle = $variables['bundle'];
  $icon = $variables['icon'];
  $tag = $bundle['settings']['tag'];
  $content = $bundle['settings']['content'];
  return '<' . $tag . drupal_attributes($variables['attributes']) . '>' . $content . '</' . $tag . '>';
}
