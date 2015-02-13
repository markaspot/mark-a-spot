<?php
/**
 * @file
 * icon_field.feeds.inc
 * Feeds mapping implementation for the Icon Field module.
 */

/**
 * Implements hook_feeds_processor_targets_alter().
 */
function icon_field_feeds_processor_targets_alter(&$targets, $entity_type, $bundle) {
  foreach (field_info_instances($entity_type, $bundle) as $name => $instance) {
    $info = field_info_field($name);
    if ($info['type'] == 'icon_field') {
      $targets[$name] = array(
        'name' => $instance['label'],
        'callback' => 'icon_field_feeds_set_target',
        'description' => $instance['description'],
        'summary_callback' => 'icon_field_feeds_summary_callback',
        'form_callback' => 'icon_field_feeds_form_callback',
      );
    }
  }
}

/**
 * Callback to return the settings for the icon mapping.
 */
function icon_field_feeds_summary_callback($mapping, $target, $form, $form_state) {
  $options = _icon_field_get_bundle_options();
  if (empty($mapping['icon_bundle'])) {
    return t('Create icons in bundle: <strong>@bundle</strong>', array('@bundle' => $options['lullacons_pack1']));
  }
  return t('Create icons in bundle: <strong>@bundle</strong>', array('@bundle' => $options[$mapping['icon_bundle']]));
}

/**
 * Callback to return the summary form for the icon mapping.
 */
function icon_field_feeds_form_callback($mapping, $target, $form, $form_state) {
  $form = array();

  $form['icon_bundle'] = array(
    '#type' => 'select',
    '#title' => t('Icon Bundle'),
    '#options' => _icon_field_get_bundle_options(),
    '#default_value' => !empty($mapping['icon_bundle']) ? $mapping['icon_bundle'] : 'lullacons_pack1',
  );

  return $form;
}

/**
 * Callback function to set value of a icon field.
 */
function icon_field_feeds_set_target($source, $entity, $target, $value, $mapping) {
  // Don't do anything if we weren't given any data.
  if (empty($value)) {
    return;
  }

  // Assume that the passed in value could really be any number of values.
  if (is_array($value)) {
    $values = $value;
  }
  else {
    $values = array($value);
  }

  // Add in default values.
  $mapping += array(
    'term_search' => 'lullacons_pack1',
  );

  $iterator = 0;
  $field = isset($entity->$target) ? $entity->$target : array();
  $language = isset($mapping['language']) ? $mapping['language'] : LANGUAGE_NONE;
  foreach ($values as $value) {
    $field[$language][$iterator]['bundle'] = $mapping['icon_bundle'];
    $field[$language][$iterator]['icon'] = $value;
    $iterator++;
  }

  // Add the field to the entity definition.
  $entity->{$target} = $field;
}

/**
 * Get the bundle options form options.
 */
function _icon_field_get_bundle_options() {
  $options = array();
  foreach (icon_bundles() as $name => $bundle) {
    $options[$name] = $bundle['title'];
  }
  return $options;
}
