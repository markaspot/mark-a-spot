<?php
/**
 * @file
 * admin.inc
 * Provides administrative callbacks and tasks.
 */

/**
 * Menu callback for 'icon_bundle_overview_form'.
 */
function icon_bundle_overview_form($form, &$form_state) {
  // Attach UI fonts for Icon API.
  $module_path = drupal_get_path('module', 'icon');
  $form['#attached'] = array(
    'css' => array(
      $module_path . '/css/icon.admin.css',
      $module_path . '/css/iconapi-embedded.css',
      array(
        'type' => 'file',
        'data' => $module_path . '/css/iconapi-ie7.css',
        'browser' => array(
          '!IE' => FALSE,
          'IE' => 'IE 7',
        ),
      ),
    ),
  );
  // Determine access to elements based on user permissions.
  $admin = user_access('administer icons');
  $form['bundles'] = array(
    '#tree' => TRUE,
  );
  foreach (icon_bundles() as $bundle) {
    if (!$admin && !$bundle['status']) {
      continue;
    }
    $form['bundles'][$bundle['name']]['#bundle'] = $bundle;
    $form['bundles'][$bundle['name']]['status'] = array(
      '#access' => $admin,
      '#type' => 'checkbox',
      '#default_value' => $bundle['status'],
    );
  }
  $form['global'] = array(
    '#access' => $admin,
    '#type' => 'fieldset',
    '#title' => t('Global Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $view_path = variable_get('icon_api_view_path_alias', 'icons');
  $form['global']['icon_api_view_path_alias'] = array(
    '#type' => 'textfield',
    '#title' => t('View Path'),
    '#field_prefix' => base_path(),
    '#input_group' => TRUE,
    '#description' => t('If provided, users with the "view icons" permission will be able to view enabled icons at this URL (defaults to: <code>!path</code>. Change this url if it conflicts with other paths on your site. Leave empty to disable.', array(
      '!path' => base_path() . 'icons',
    )),
    '#default_value' => $view_path,
  );
  // Show warning message for servers running apache.
  $apache_show_warning = ($view_path === 'icons' && strpos(drupal_strtolower($_SERVER['SERVER_SOFTWARE']), 'apache') !== FALSE);
  $apache_suppress_warning = variable_get('icon_api_apache_suppress_warning', FALSE);
  if ($apache_show_warning && !$apache_suppress_warning) {
    drupal_set_message(t('<strong>WARNING:</strong> Apache installations are typically configured with a server level alias that redirects "/icons" to an internal directory on the server. It is highly recommended that this be removed from the configuration file for the view path to work properly. If this modification has been made to the server, you may surpress this messages in the global settings below. The only alternative is to change the view path in the global settings below. See: <a href="!link">!link</a>.', array(
      '!link' => 'https://drupal.org/node/2198427',
    )), 'warning', FALSE);
  }
  // Provide toggle for suppressing warning message.
  $form['global']['icon_api_apache_suppress_warning'] = array(
    '#access' => $apache_show_warning,
    '#type' => 'checkbox',
    '#title' => t('Suppress the Apache server alias warning'),
    '#default_value' => $apache_suppress_warning,
  );
  $form['actions'] = array(
    '#access' => $admin,
    '#type' => 'actions',
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );
  return $form;
}

/**
 * Validate callback for 'icon_bundle_overview_form'.
 */
function icon_bundle_overview_form_validate($form, $form_state) {
  // Ensure only authorized users can submit the form.
  if (!user_access('administer icons')) {
    form_set_error('', t('You are not authorized to submit this form.'));
  }
  $view_path_alias = $form_state['values']['icon_api_view_path_alias'];
  if ($form['global']['icon_api_view_path_alias']['#default_value'] !== $form_state['values']['icon_api_view_path_alias'] && menu_get_item($view_path_alias)) {
    form_set_error('icon_api_view_path_alias', t('The view path alias provided, "%url", is already in use. Please enter a different path.', array(
      '%url' => $view_path_alias,
    )));
  }
}

/**
 * Submit callback for 'icon_bundle_overview_form'.
 */
function icon_bundle_overview_form_submit($form, $form_state) {
  $view_path_alias = $form_state['values']['icon_api_view_path_alias'];
  if ($form['global']['icon_api_view_path_alias']['#default_value'] !== $view_path_alias) {
    variable_set('icon_api_view_path_alias', $view_path_alias);
    if (!empty($view_path_alias)) {
      drupal_set_message(t('The view path alias, "%url", has been created for users with the %permission permission.', array(
        '%permission' => 'view icon',
        '%url' => base_path() . $view_path_alias,
      )));
    }
    else {
      drupal_set_message(t('The view path alias is disabled. Icons are now only viewable in the administrative area.'));

    }
    menu_rebuild();
  }
  if (isset($form_state['values']['icon_api_apache_suppress_warning'])) {
    variable_set('icon_api_apache_suppress_warning', $form_state['values']['icon_api_apache_suppress_warning']);
    // Remove message just set by building the form again.
    unset($_SESSION['messages']['warning']);
  }
  $bundles = icon_bundles();
  $saved_bundles = $form_state['values']['bundles'];
  foreach ($bundles as $name => $bundle) {
    if (!isset($saved_bundles[$name]['status'])) {
      continue;
    }
    $status = $saved_bundles[$name]['status'];
    if ($status !== $bundle['status']) {
      if ($status) {
        icon_bundle_enable($bundle);
      }
      else {
        icon_bundle_disable($bundle);
      }
    }
  }
}

/**
 * Theme implementation for 'icon_bundle_overview_form'.
 */
function theme_icon_bundle_overview_form($variables) {
  $output = '';
  $form = $variables['form'];
  $admin = user_access('administer icons');
  $header = array(
    'enable' => t('Enabled'),
    'bundle' => t('Bundle'),
    'count' => t('Count'),
    'provider' => t('Provider'),
    'operations' => t('Operations'),
  );
  // Remove the "Enabled" header for non-admin users.
  if (!$admin) {
    array_shift($header);
  }
  $rows = array();
  foreach (element_children($form['bundles']) as $key) {
    $bundle = $form['bundles'][$key]['#bundle'];
    $row = array();
    if ($admin) {
      $row['status'] = array(
        'data' => drupal_render($form['bundles'][$key]['status']),
        'class' => 'compact center',
      );
    }
    $row['bundle'] = $bundle['title'] . (!empty($bundle['version']) ? ' - ' . $bundle['version'] : '');
    $row['count'] = array(
      'data' => count($bundle['icons']),
      'class' => 'compact center',
    );
    $row['provider']['class'] = 'compact nowrap';
    if ($provider = icon_provider_load($bundle['provider'])) {
      $row['provider']['data'] = !empty($provider['url']) ? l($provider['title'], $provider['url']) : $provider['title'];
    }
    else {
      $row['provider']['data'] = $bundle['provider'];
    }
    // Build operations for each bundle.
    $row['operations'] = array('class' => 'compact nowrap');
    $operations = array();
    if ($bundle['status']) {
      $operations['view'] = array(
        'title' => '<i class="iconapi-view"></i>' . t('View Icons'),
        'href' => ($admin && arg(0) === 'admin' ? ICON_ADMIN_PATH . '/bundle/' : variable_get('icon_api_view_path_alias', 'icons') . '/') . $bundle['name'],
        'html' => TRUE,
      );
      if ($admin) {
        $operations['configure'] = array(
          'title' => '<i class="iconapi-configure"></i>' . t('Configure'),
          'href' => ICON_ADMIN_PATH . '/bundle/' . $bundle['name'] . '/configure',
          'html' => TRUE,
        );
      }
    }
    if ($admin && !empty($bundle['import'])) {
      $operations['delete'] = array(
        'title' => '<i class="iconapi-delete"></i>' . t('Delete'),
        'href' => ICON_ADMIN_PATH . '/bundle/' . $bundle['name'] . '/delete',
        'html' => TRUE,
      );
    }
    elseif ($admin && !empty($bundle['overridden'])) {
      $operations['reset'] = array(
        'title' => '<i class="iconapi-throbber"></i>' . t('Reset'),
        'href' => ICON_ADMIN_PATH . '/bundle/' . $bundle['name'] . '/reset',
        'html' => TRUE,
      );
    }
    if (!empty($operations)) {
      $row['operations']['data'] = array(
        '#theme' => 'links',
        '#links' => $operations,
        '#attributes' => array('class' => array('links', 'inline')),
      );
    }
    $rows[] = $row;
  }
  $output .= theme('table', array(
    'header' => $header,
    'rows' => $rows,
    'empty' => t('No bundles are currently installed.'),
  ));
  $output .= drupal_render_children($form);
  return $output;
}

/**
 * Menu callback for 'icon_bundle_configure_form'.
 */
function icon_bundle_configure_form($form, &$form_state, $bundle) {
  drupal_set_title(t('Configure %bundle icons', array('%bundle' => $bundle['title'])), PASS_THROUGH);
  $form_state['bundle'] = $bundle;
  $form['status'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enabled'),
    '#default_value' => $bundle['status'],
  );
  $form['settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Bundle settings'),
    '#tree' => TRUE,
  );
  foreach (icon_extension_implements('icon_bundle_configure') as $extension => $type) {
    icon_extension_invoke($type, $extension, 'icon_bundle_configure', $form['settings'], $form_state, $form);
  }
  $settings = element_children($form['settings']);
  if (empty($settings)) {
    $form['settings']['#access'] = FALSE;
  }
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save bundle'),
  );
  if (!empty($bundle['import'])) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete bundle'),
    );
  }
  elseif (!empty($bundle['overridden'])) {
    $form['actions']['reset'] = array(
      '#type' => 'submit',
      '#value' => t('Reset bundle'),
    );
  }
  return $form;
}

/**
 * Submit callback for 'icon_bundle_configure_form'.
 */
function icon_bundle_configure_form_submit($form, &$form_state) {
  $destination = drupal_get_destination();
  $bundle = $form_state['bundle'];
  $button = $form_state['triggering_element']['#value'];
  if ($button === t('Reset bundle')) {
    $form_state['redirect'] = array(ICON_ADMIN_PATH . '/bundle/' . $bundle['name'] . '/reset', array('query' => $destination));
    return;
  }
  elseif ($button === t('Delete bundle')) {
    $form_state['redirect'] = array(ICON_ADMIN_PATH . '/bundle/' . $bundle['name'] . '/delete', array('query' => $destination));
    return;
  }
  $form_state['redirect'] = array(ICON_ADMIN_PATH);
  form_state_values_clean($form_state);
  $bundle = icon_array_merge_recursive($bundle, $form_state['values']);
  if (icon_bundle_save($bundle)) {
    drupal_set_message(t('Configuration for icon bundle %bundle has been saved.', array('%bundle' => $bundle['title'])));
  }
}

/**
 * Menu callback for 'icon_bundle_delete_form'.
 */
function icon_bundle_delete_form($form, $form_state, $bundle) {
  return confirm_form($form, t('Delete %bundle icon bundle?', array('%bundle' => $bundle['title'])), ICON_ADMIN_PATH, NULL, t('Delete'));
}

/**
 * Submit callback for 'icon_bundle_delete_form'.
 */
function icon_bundle_delete_form_submit($form, &$form_state) {
  // Redirect to the overview form after submit.
  if (isset($_GET['destination'])) {
    unset($_GET['destination']);
  }
  $form_state['redirect'] = array(ICON_ADMIN_PATH);
  if (!empty($form_state['values']['confirm'])) {
    icon_bundle_delete($form_state['build_info']['args'][0]);
  }
}

/**
 * Menu callback for 'icon_bundle_reset_form'.
 */
function icon_bundle_reset_form($form, $form_state, $bundle) {
  if (empty($bundle['overridden'])) {
    drupal_set_message(t('The icon bundle %bundle is not currently overridden in the database.', array('%bundle' => $bundle['title'])), 'error');
    drupal_goto(ICON_ADMIN_PATH);
  }
  return confirm_form($form, t('Reset %bundle icon bundle?', array('%bundle' => $bundle['title'])), ICON_ADMIN_PATH, NULL, t('Reset'));
}

/**
 * Submit callback for 'icon_bundle_reset_form'.
 */
function icon_bundle_reset_form_submit($form, &$form_state) {
  // Redirect to the overview form after submit.
  $destination = ICON_ADMIN_PATH;
  if (isset($_GET['destination'])) {
    $destination = array('destination' => $_GET['destination']);
  }
  $form_state['redirect'] = array($destination);
  if (!empty($form_state['values']['confirm'])) {
    icon_bundle_delete($form_state['build_info']['args'][0]);
  }
}

/**
 * Menu callback for 'icon_bundle_list'.
 */
function icon_bundle_list($bundle) {
  $build = array();
  foreach ($bundle['icons'] as $icon => $value) {
    $build[] = array(
      '#theme' => 'icon',
      '#bundle' => $bundle['name'],
      '#icon' => $icon,
      '#attributes' => array(
        'title' => $icon,
      ),
    );
  }
  // Allow other modules and themes to alter a bundle's listing.
  $bundle_context = $bundle;
  drupal_alter('icon_bundle_list', $build, $bundle_context);
  return $build;
}
