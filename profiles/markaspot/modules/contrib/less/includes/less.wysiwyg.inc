<?php

/**
 * @file
 * Contains functions that handle WYSIWYG module integration.
 */

/**
 * Implements hook_wysiwyg_editor_settings_alter().
 *
 * Check the CSS WYSIWYG setting for LESS files and replace with
 * generated CSS files where necessary.
 */
function less_wysiwyg_editor_settings_alter(&$settings, $context) {

  $wysiwyg = $context['editor']['name'];

  // Each editor has a different $settings array key for CSS files.
  $editors = array(
    'tinymce' => 'content_css',
    'fckeditor' => 'EditorAreaCSS',
    'ckeditor' => 'contentsCss',
  );

  if (!empty($editors[$wysiwyg]) && !empty($settings[$editors[$wysiwyg]])) {

    $stylesheets = $settings[$editors[$wysiwyg]];

    // Keep track if comma separated paths, or array of paths.
    $is_array = is_array($stylesheets);

    if ($is_array === FALSE) {

      // $stylesheets is a list of comma separated file paths.
      $stylesheets = explode(',', $stylesheets);
    }

    // Prepare an array that can be handled by normal LESS module processing.
    $styles = array(
      '#items' => array(),
    );
    
    foreach ($stylesheets as $stylesheet) {

      // Might contain ?query portion, separate parts.
      $parts = drupal_parse_url($stylesheet);

      // Paths are expected to be relative to DRUPAL_ROOT, trim leading '/'.
      $path = trim($parts['path'], '/');

      $styles['#items'][$path] = array(
        'data' => $path,
      );
    }

    $styles = _less_pre_render($styles);

    $processed_stylesheets = array();

    foreach ($styles['#items'] as $file) {
      $processed_stylesheets[] = file_create_url($file['data']);
    }

    // Recombine file paths into comma separated list.
    if ($is_array === FALSE) {
      $processed_stylesheets = implode(',', $processed_stylesheets);
    }

    $settings[$editors[$wysiwyg]] = $processed_stylesheets;
  }
}

/**
 * Implements hook_form_FORM_ID_alter().
 *
 * form_id = 'wysiwyg_profile'
 */
function less_form_wysiwyg_profile_form_alter(&$form, $form_state, $form_id) {
  $form['css']['css_path']['#description'] .= '<br />' . t('You may enter a path to a LESS file and it will be parsed automatically.');
}

/**
 * Implements hook_ckeditor_settings_alter().
 */
function less_ckeditor_settings_alter(&$settings) {

  $context = array(
    'editor' => array(
      'name' => 'ckeditor',
    ),
  );

  less_wysiwyg_editor_settings_alter($settings, $context);
}
