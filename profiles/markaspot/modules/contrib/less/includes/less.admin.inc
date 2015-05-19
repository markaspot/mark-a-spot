<?php

/**
 * @file
 * Contains the administration pages for LESS.
 */

/**
 * Form for LESS module settings.
 */
function less_settings_form($form, &$form_state) {

  $form['less_flush'] = array(
    '#type' => 'fieldset',
    '#collapsible' => FALSE,
    '#value' => t('Click this button to flag all LESS files for regeneration.'),
  );

  $form['less_flush']['flush'] = array(
    '#type' => 'submit',
    '#submit' => array('_flush_less'),
    '#value' => t('Flush LESS files'),
  );

  $registered_engines = _less_get_engines();

  $less_engines = array();

  foreach ($registered_engines as $library => $engine) {

    $less_engines[] = libraries_detect($library);
  }

  $less_engine_element = array(
    '#type' => 'radios',
    '#title' => t('LESS engine'),
    '#options' => array(),
    '#required' => TRUE,
    '#default_value' => variable_get('less_engine', 'lessphp'),
  );
  
  foreach ($less_engines as $less_engine) {
    
    $less_engine_element['#options'][$less_engine['machine name']] = $less_engine['name'];
    
    $less_engine_element[$less_engine['machine name']] = array(
      '#type' => 'radio',
      '#title' => t('@engine_name - <a href="@vendor_url">@vendor_url</a>', array('@engine_name' => $less_engine['name'], '@vendor_url' => $less_engine['vendor url'])),
      '#return_value' => $less_engine['machine name'],
      '#description' => t('Missing - Click vendor link above to read installation instructions.'),
      '#disabled' => empty($less_engine['installed']),
    );
    
    if ($less_engine['installed']) {
      $less_engine_element[$less_engine['machine name']]['#description'] = t('v%version Installed', array('%version' => $less_engine['version']));
    }
    
  }
  
  $form['less_engine'] = $less_engine_element;
  
  
  $lessautoprefixer_library = libraries_detect('lessautoprefixer');
  
  $form[LESS_AUTOPREFIXER] = array(
    '#type' => 'checkbox',
    '#title' => t('Use @name - <a href="@vendor_url">@vendor_url</a>', array('@name' => $lessautoprefixer_library['name'], '@vendor_url' => $lessautoprefixer_library['vendor url'])),
    '#description' => t('Enable automatic prefixing of vendor CSS extensions.'),
    '#default_value' => variable_get(LESS_AUTOPREFIXER, FALSE),
    '#disabled' => empty($lessautoprefixer_library['installed']),
  );
  
  if ($lessautoprefixer_library['installed']) {
    $form[LESS_AUTOPREFIXER]['#description'] .= '<br />'. t('v%version Installed', array('%version' => $lessautoprefixer_library['version']));
  }
  
  $form['developer_options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Developer Options'),
    '#collapsible' => TRUE,
    '#collapsed' => !(variable_get(LESS_DEVEL, FALSE)),
  );
  
  $form['developer_options'][LESS_DEVEL] = array(
    '#type' => 'checkbox',
    '#title' => t('Developer Mode'),
    '#description' => t('Enable developer mode to ensure LESS files are regenerated every page load.'),
    '#default_value' => variable_get(LESS_DEVEL, FALSE),
  );
  
  $form['developer_options'][LESS_SOURCE_MAPS] = array(
    '#type' => 'checkbox',
    '#title' => t('Source Maps'),
    '#description' => t('Enable source maps output while "Developer Mode" is enabled.'),
    '#default_value' => variable_get(LESS_SOURCE_MAPS, FALSE),
    '#states' => array(
      'enabled' => array(
        ':input[name="' . LESS_DEVEL . '"]' => array('checked' => TRUE),
      ),
    ),
  );
  
  $form['developer_options'][LESS_WATCH] = array(
    '#type' => 'checkbox',
    '#title' => t('Watch Mode'),
    '#description' => t('Enable watch mode while developer mode is active to automatically reload styles when changes are detected, including changes to @import-ed files. Does not cause a page reload.'),
    '#default_value' => variable_get(LESS_WATCH, FALSE),
    '#states' => array(
      'enabled' => array(
        ':input[name="' . LESS_DEVEL . '"]' => array('checked' => TRUE),
      ),
    ),
  );
  
  $form['#submit'] = array('less_settings_form_submit');
  
  return system_settings_form($form);
}

/**
 * Form submission function.
 * 
 * Trigger clear of LESS module cache data.
 */
function less_settings_form_submit($form, &$form_state) {
  cache_clear_all('less:', 'cache', TRUE);
}

/**
 * Submit handler for cache clear button.
 */
function _flush_less($form, &$form_state) {

  less_flush_caches();

  drupal_set_message(t('LESS files cache cleared.'), 'status');
}
