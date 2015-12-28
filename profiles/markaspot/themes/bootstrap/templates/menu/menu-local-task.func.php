<?php
/**
 * @file
 * Stub file for bootstrap_menu_local_task().
 */

/**
 * Returns HTML for a single local task link.
 *
 * @param array $variables
 *   An associative array containing:
 *   - element: A render element containing:
 *     - #link: A menu link array with 'title', 'href', and 'localized_options'
 *       keys.
 *     - #active: A boolean indicating whether the local task is active.
 *
 * @return string
 *   The constructed HTML.
 *
 * @see theme_menu_local_task()
 *
 * @ingroup theme_functions
 */
function bootstrap_menu_local_task($variables) {
  $link = $variables['element']['#link'];
  $link_text = $link['title'];
  $attributes = array();

  if (!empty($variables['element']['#active'])) {
    // Add text to indicate active tab for non-visual users.
    $active = '<span class="element-invisible">' . t('(active tab)') . '</span>';

    // If the link does not contain HTML already, check_plain() it now.
    // After we set 'html'=TRUE the link will not be sanitized by l().
    if (empty($link['localized_options']['html'])) {
      $link['title'] = check_plain($link['title']);
    }
    $link['localized_options']['html'] = TRUE;
    $link_text = t('!local-task-title!active', array('!local-task-title' => $link['title'], '!active' => $active));

    $attributes['class'][] = 'active';
  }

  return '<li' . drupal_attributes($attributes) . '>' . l($link_text, $link['href'], $link['localized_options']) . "</li>\n";
}
