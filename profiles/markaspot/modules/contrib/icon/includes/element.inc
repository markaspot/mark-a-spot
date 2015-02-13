<?php
/**
 * @file
 * element.inc
 * Provides hooks and functions for element related tasks.
 */

/**
 * Implements hook_element_info().
 */
function icon_element_info() {
  $types['icon_selector'] = array(
    '#input' => TRUE,
    '#tree' => TRUE,
    '#title' => t('Icon'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#default_bundle' => '',
    '#default_icon' => '',
    '#process' => array('form_process_fieldset', 'form_process_icon_selector'),
    '#pre_render' => array('form_pre_render_fieldset'),
    '#theme_wrappers' => array('fieldset'),
    '#attached' => array('library' => array(array('icon', 'icon_selector'))),
  );
  return $types;
}

/**
 * Processes an icon select list form element.
 */
function form_process_icon_selector($element) {
  // Get default values.
  $default_bundle = isset($element['#default_bundle']) ? $element['#default_bundle'] : icon_bundle_defaults();
  if (!is_array($default_bundle)) {
    $default_bundle = icon_bundle_load($default_bundle);
  }
  $default_icon = isset($element['#default_icon']) ? $element['#default_icon'] : '';
  $default_value = '';
  if ($default_bundle && !empty($default_bundle['name']) && !empty($default_icon)) {
    $default_value = $default_bundle['name'] . '|' . $default_icon;
  }
  // Build the options array for available icons per bundle.
  $options = array();
  foreach (icon_bundles() as $bundle_name => $bundle) {
    if (!$bundle['status']) {
      continue;
    }
    foreach ($bundle['icons'] as $icon_key => $icon_value) {
      $icon_name = is_string($icon_key) ? $icon_key : $icon_value;
      if (is_array($icon_value) && isset($icon_value['name'])) {
        $icon_name = $icon_value['name'];
      }
      $icon_title = is_string($icon_value) ? $icon_value : $icon_name;
      if (is_array($icon_value) && isset($icon_value['title'])) {
        $icon_title = $icon_value['title'];
      }
      $options[$bundle['title']][$bundle['name'] . '|' . $icon_name] = $icon_title;
    }
  }
  if (!count($options)) {
    $element['#description'] = t('There are no icons available to choose from. They either do not exist or are disabled. As a result, this icon selector has also been disable.');
    $element['#description'] .= '<br />' . l(t('Manage icon bundles'), ICON_ADMIN_PATH);
    $element['#disabled'] = TRUE;
    // Set the default value.
    if (!empty($default_value)) {
      $options = array($default_value => $default_icon);
    }
  }
  $element['icon'] = array(
    '#title' => t('Select Icon'),
    '#type' => 'select',
    '#options' => $options,
    '#default_value' => $default_value,
    '#empty_option' => '- No Icon -',
    '#weight' => -10,
  );
  if (isset($element['#default_value'])) {
    $element['icon']['#default_value'] = $element['#default_value'];
  }
  return $element;
}

/**
 * Implements form_type_ELEMENT_value().
 *
 * Value callback for the 'icon_selector' element.
 */
function form_type_icon_selector_value(&$element, $input = FALSE, &$form_state = array()) {
  if ($input !== FALSE && isset($input['icon']) && strpos($input['icon'], '|') !== FALSE) {
    list($bundle, $icon) = explode('|', $input['icon']);
    $parents = $element['#parents'];
    drupal_array_set_nested_value($form_state['values'], array_merge($element['#parents'], array('bundle')), $bundle, TRUE);
    drupal_array_set_nested_value($form_state['values'], array_merge($element['#parents'], array('icon')), $icon, TRUE);
  }
}
