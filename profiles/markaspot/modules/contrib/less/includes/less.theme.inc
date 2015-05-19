<?php

/**
 * @file
 * Contains alterations to theme settings forms.
 */

/**
 * Implements hook_form_alter().
 */
function less_form_alter(&$form, &$form_state, $form_id) {

  if ($form_id === 'system_theme_settings') {

    preg_match('/^theme_(?P<current_theme_key>\w+)_settings$/', $form['var']['#value'], $matches);

    // only show settings on forms for themes, not "Global settings"
    if (!empty($matches['current_theme_key'])) {

      $current_theme_key = $matches['current_theme_key'];

      $saved_settings = theme_get_setting('less', $current_theme_key);

      $less_settings_form = array();

      $theme_items = system_list('theme');

      if (isset($theme_items[$current_theme_key]) && !empty($theme_items[$current_theme_key]->info['less']['vars'])) {
        $less_settings_form[$current_theme_key] = _less_setting_form_element($current_theme_key, $theme_items[$current_theme_key]->info['less']['vars'], $saved_settings[$current_theme_key]);
      }

      // showing settings form for modules
      $system_items = system_list('module_enabled');

      $less_module_settings = array();

      foreach ($system_items as $system_item_name => $system_item) {

        if (!empty($system_item->info['less']['vars'])) {

          $item_saved_settings = isset($saved_settings[$system_item_name]) ? $saved_settings[$system_item_name] : array();

          $less_module_settings[$system_item_name] = _less_setting_form_element($system_item_name, $system_item->info['less']['vars'], $item_saved_settings, $system_item->info['name']);
          $less_module_settings[$system_item_name]['#group'] = 'less][modules_list';
        }
      }

      if (!empty($less_module_settings)) {

        $less_settings_form['modules'] = array(
          '#type' => 'fieldset',
          '#title' => t('Modules'),
          '#collapsible' => TRUE,
          '#collapsed' => TRUE,
          '#parents' => array('less'),
          // make sure all settings are stored under the system name immediately inside the themes less settings array
        );

        $less_settings_form['modules']['modules_list'] = array(
          '#type' => 'vertical_tabs',
        );

        $less_settings_form['modules'] += $less_module_settings;

      }

      if (!empty($less_settings_form)) {

        $form['less'] = array(
          '#type' => 'vertical_tabs',
          '#prefix' => t('<h3>LESS settings</h3>'),
          '#tree' => TRUE,
          '#description' => t('These settings are per theme. Delete a value to use the default.'),
          '#attached' => array(
            'css' => array(
              drupal_get_path('module', 'less') . '/styles/less.theme.css',
            ),
          ),
        );

        $form['less'] += $less_settings_form;
      }
    }
  }
}

function _less_setting_form_element($system_name, $less_settings, $saved_settings, $title = NULL) {

  $form = array(
    '#type' => 'fieldset',
    '#title' => isset($title) ? $title : $system_name,
    '#attributes' => array(
      'class' => array(
        'less-theme-settings',
      ),
    ),
  );

  if (module_exists($system_name)) {

    $less_settings = array_replace($less_settings, (array) module_invoke($system_name, 'less_variables'));
  }

  foreach ($less_settings as $var_name => $var_value) {
    
    $form[$var_name] = array(
      '#type' => 'textfield',
      '#title' => $var_name,
      '#default_value' => !empty($saved_settings[$var_name]) ? $saved_settings[$var_name] : NULL,
      '#description' => t('Default value: %variable_value', array('%variable_value' => $var_value)),
      '#attributes' => array('placeholder' => $var_value),
    );
  }
  
  return $form;
}
