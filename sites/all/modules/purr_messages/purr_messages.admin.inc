<?php

/**
 * Implements hook_settings().
 * Configuration settings page.
 *
 * @return
 *    Array of the system settings form.
 */
function purr_messages_settings() {
  $module_path = drupal_get_path('module', 'purr_messages');
  $form['purr_messages_code_status'] = array(
  '#type' => 'fieldset',
  '#title' => t('Purr Messages settings'),
  '#weight' => -10,
  '#description' => (purr_messages_status() !== FALSE) ?
    t('You have a custom css file in: <br/>/%path/purrcss/') :
    t('You are using the default purr style. To use custom css and images simply
      copy the <em>\'purrcss\'</em> folder from: <br/>%modulepath<br/> and place
      it in your theme folder: <br/>%path<br/>Then make the alterations you want
      to the copied version in your theme folder. This module will automatically
      pick up the changes and this message will change if you\'ve done it
      correctly.<br/>',
      array('%path' => path_to_theme(), '%modulepath' => $module_path)
    )
  );
  $form['purr_messages_settings'] = array(
    '#type' => 'fieldset',
    '#title' => 'Plugin settings',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['purr_messages_settings']['purr_messages_fade_in'] = array(
    '#type' => 'textfield',
    '#title' => t('Fade in speed'),
    '#field_suffix' => t('ms'),
    '#description' => t('The time in milliseconds for the notice to fade in.'),
    '#default_value' => variable_get('purr_messages_fade_in', PURR_FADE_IN),
  );
  $form['purr_messages_settings']['purr_messages_fade_out'] = array(
    '#type' => 'textfield',
    '#title' => t('Fade out speed'),
    '#field_suffix' => t('ms'),
    '#description' => t('The time in milliseconds for the notice to fade out.'),
    '#default_value' => variable_get('purr_messages_fade_out', PURR_FADE_OUT),
  );
  $form['purr_messages_settings']['purr_messages_timer'] = array(
    '#type' => 'textfield',
    '#title' => t('Show for'),
    '#field_suffix' => t('ms'),
    '#description' => t('The time in milliseconds the notice will display for, before fading out.'),
    '#default_value' => variable_get('purr_messages_timer', PURR_TIMER),
  );
  $form['purr_messages_settings']['purr_messages_attachto'] = array(
    '#type' => 'textfield',
    '#title' => t('Attach to'),
    '#description' => t('Enter the html element, using jQuery syntax, that the messages should to attach to in the DOM.
      You only need to change this if you want more control in concert with custom css.'),
    '#default_value' => variable_get('purr_messages_attachto', PURR_ATTACH_TO),
  );
  $form['purr_messages_settings']['purr_messages_hover'] = array(
    '#type' => 'checkbox',
    '#title' => t('Pause on hover'),
    '#description' => t('Check to pause when cursor is over the message.'),
    '#default_value' => variable_get('purr_messages_hover', TRUE),
  );
  $form['purr_messages_settings']['purr_messages_transparent'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use transparent png'),
    '#description' => t('Uncheck if you are not using 24bit pngs in your custom theme.'),
    '#default_value' => variable_get('purr_messages_transparent', TRUE),
  );

  $visibility = variable_get('purr_messages_visibility', 0);
  $pages = variable_get('purr_messages_pages', '');

  // Visibility settings.
  $form['visibility_title'] = array(
    '#type' => 'item',
    '#title' => t('Visibility settings'),
  );
  $form['visibility'] = array(
    '#type' => 'vertical_tabs',
  );
  // Per-path visibility.
  $form['visibility']['path'] = array(
    '#type' => 'fieldset',
    '#title' => t('Pages'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#group' => 'visibility',
    '#weight' => 0,
  );

  $access = user_access('use PHP for settings');
  if (isset($visibility) && $visibility == PURR_VISIBILITY_PHP && !$access) {
    $form['visibility']['path']['visibility'] = array(
      '#type' => 'value',
      '#value' => PURR_VISIBILITY_PHP,
    );
    $form['visibility']['path']['pages'] = array(
      '#type' => 'value',
      '#value' => isset($pages) ? $pages : '',
    );
  }
  else {
    $options = array(
      PURR_VISIBILITY_NOTLISTED => t('All pages except those listed'),
      PURR_VISIBILITY_LISTED => t('Only the listed pages'),
    );
    $description = t("Specify pages by using their paths. Enter one path per line. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.", array('%blog' => 'blog', '%blog-wildcard' => 'blog/*', '%front' => '<front>'));

    if (module_exists('php') && $access) {
      $options += array(PURR_VISIBILITY_PHP => t('Pages on which this PHP code returns <code>TRUE</code> (experts only)'));
      $title = t('Pages or PHP code');
      $description .= ' ' . t('If the PHP option is chosen, enter PHP code between %php. Note that executing incorrect PHP code can break your Drupal site.', array('%php' => '<?php ?>'));
    }
    else {
      $title = t('Pages');
    }
    $form['visibility']['path']['visibility'] = array(
      '#type' => 'radios',
      '#title' => t('Show purr messages on specific pages'),
      '#options' => $options,
      '#default_value' => isset($visibility) ? $visibility : PURR_VISIBILITY_NOTLISTED,
    );
    $form['visibility']['path']['pages'] = array(
      '#type' => 'textarea',
      '#title' => '<span class="element-invisible">' . $title . '</span>',
      '#default_value' => isset($pages) ? $pages : '',
      '#description' => $description,
    );
  }

  // Other visibility settings.
  $form['visibility']['other'] = array(
    '#type' => 'fieldset',
    '#title' => t('Other'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#group' => 'visibility',
    '#weight' => 10,
  );
  $form['visibility']['other']['purr_messages_admin_path'] = array(
    '#type' => 'checkbox',
    '#title' => t('Disable purr messages on admin paths'),
    '#description' => t('Checking this box will disable purr messages on admin paths,
      as determined by the path_is_admin() function.'),
    '#default_value' => variable_get('purr_messages_admin_path', FALSE),
  );
  $form['visibility']['other']['purr_messages_explicit'] = array(
    '#type' => 'checkbox',
    '#title' => t('Only show purr message style if explicitly called'),
    '#description' => t('This option will only display a purr style message
      if called explicitly with the \'purr\' parameter as the type in 
      <code>drupal_set_message()</code>.<br/> For example:
      <code>drupal_set_message(\'my message\', \'purr\');</code>'),
    '#default_value' => variable_get('purr_messages_explicit', FALSE),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );
  return $form;
}

/**
 * Form submission handler for the configuration form.
 *
 * @see purr_messages_settings()
 */
function purr_messages_settings_submit($form, &$form_state) {
  if (!form_get_errors()) {
    variable_set('purr_messages_fade_in', (int) $form_state['values']['purr_messages_fade_in']);
    variable_set('purr_messages_fade_out', (int) $form_state['values']['purr_messages_fade_out']);
    variable_set('purr_messages_timer', (int) $form_state['values']['purr_messages_timer']);
    variable_set('purr_messages_hover', (bool) $form_state['values']['purr_messages_hover']);
    variable_set('purr_messages_transparent', (bool) $form_state['values']['purr_messages_transparent']);
    variable_set('purr_messages_admin_path', (bool) $form_state['values']['purr_messages_admin_path']);
    variable_set('purr_messages_visibility', (int) $form_state['values']['visibility']);
    variable_set('purr_messages_pages', trim($form_state['values']['pages']));
    variable_set('purr_messages_explicit', (bool) $form_state['values']['purr_messages_explicit']);
    drupal_set_message(t('The configuration has been saved.'));
  }
}