<?php
/**
 * @file
 * import.inc
 * Provides import administrative callbacks and tasks.
 */

/**
 * Menu callback for 'icon_provider_import_form'.
 */
function icon_provider_import_form($form, $form_state) {
  // Build a list of providers.
  $providers = array(
    'automatic' => t('Automatically detect'),
  );
  foreach (icon_providers_support_import() as $provider) {
    $providers[$provider['name']] = $provider['title'];
  }
  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Bundle title'),
    '#description' => t('The human readable name to identify the bundle.'),
    '#required' => TRUE,
  );
  $form['name'] = array(
    '#type' => 'machine_name',
    '#title' => t('Bundle name'),
    '#description' => t('A unique name to identify the bundle. It must only contain lowercase letters, numbers, hyphens and underscores.'),
    '#required' => TRUE,
    '#machine_name' => array(
      'exists' => 'icon_bundle_load',
      'source' => array('title'),
    ),
  );
  $form['provider'] = array(
    '#type' => 'select',
    '#title' => t('Provider'),
    '#options' => $providers,
    '#default_value' => 'automatic',
    '#required' => TRUE,
  );
  $form['file'] = array(
    '#type' => 'file',
    '#title' => t('Archive file'),
    '#description' => t('Files with the following extensions are allowed: %extensions', array('%extensions' => archiver_get_extensions())),
    '#size' => 40,
  );
  $form['bundle']['import'] = array(
    '#type' => 'submit',
    '#value' => t('Import'),
  );
  return $form;
}

/**
 * Validate callback for 'icon_provider_import_form'.
 */
function icon_provider_import_form_validate($form, &$form_state) {
  // Detect the selected provider.
  $form_state['provider'] = icon_providers($form_state['values']['provider']);
  $provider = &$form_state['provider'];
  // Construct the beginnings of a bundle.
  $form_state['bundle'] = icon_bundle_defaults();
  $bundle = &$form_state['bundle'];
  $bundle['title'] = $form_state['values']['title'];
  $bundle['name'] = $form_state['values']['name'];
  // Validate supported archive extensions.
  $valid_extensions = archiver_get_extensions();
  $validators = array('file_validate_extensions' => array($valid_extensions));
  if (!$file = file_save_upload('file', $validators, NULL, FILE_EXISTS_REPLACE)) {
    form_set_error('file');
    return;
  }
  // Extract the archive. This needs to be done now so providers can validate
  // against the archive file contents.
  $temp_dir = 'temporary://icon_api_' . md5(REQUEST_TIME);
  try {
    $archiver = icon_archive_extract(drupal_realpath($file->uri), drupal_realpath($temp_dir));
  }
  catch (Exception $e) {
    form_set_error('file', $e->getMessage());
    file_delete($file);
    file_unmanaged_delete_recursive($temp_dir);
    return;
  }
  // Set the archive extraction path temporarily as the bundle path.
  $bundle['path'] = ($archiver->sub_directory ? $temp_dir . '/' . $archiver->sub_directory : $temp_dir);
  // Automatic detection. Iterate over each provider and find the first one that
  // can import the archive file.
  if (!$provider) {
    foreach (icon_providers() as $_provider) {
      $bundle['provider'] = $_provider['name'];
      $validation = icon_extension_invoke($_provider['type'], $_provider[$_provider['type']], 'icon_' . $_provider['name'] . '_import_validate', $bundle);
      // Providers must return a validation of TRUE if it passes. Otherwise
      // they should send an error message to display instead.
      if ($validation === TRUE) {
        $provider = $_provider;
        break;
      }
    }
    if (!$provider) {
      form_set_error('file', t('Unable to automatically detect which provider should be used to import %file. Try manually selecting which provider to use or re-download the archive file from the provider.', array(
        '%file' => $file->filename,
      )));
      // Close opened archive before trying to delete it.
      $archiver->getArchive()->close();
      file_delete($file);
      file_unmanaged_delete_recursive($temp_dir);
      return;
    }
  }
  // A specific provider was selected, show the validation errors.
  else {
    $bundle['provider'] = $provider['name'];
    $validation = icon_extension_invoke($provider['type'], $provider[$provider['type']], 'icon_' . $provider['name'] . '_import_validate', $bundle);
    if ($validation !== TRUE) {
      form_set_error('file', $validation);
      // Close opened archive before trying to delete it.
      $archiver->getArchive()->close();
      file_delete($file);
      file_unmanaged_delete_recursive($temp_dir);
      return;
    }
  }
  // A provider has been determined. Merge in default bundle settings.
  $bundle = array_merge_recursive($bundle, $provider['default bundle']);
  $bundle['provider'] = $provider['name'];
  $bundle['import'] = TRUE;
  // Move the extracted archive to a permanent location.
  if (!$bundle['path'] = icon_file_move_recursive($bundle['path'], 'public://icon/' . $bundle['provider'], $bundle['name'])) {
    form_set_error('file', t('An error occured while attempting to extract the uploaded archive. Check the logs for more details.'));
  }
  // Close opened archive before trying to delete it.
  $archiver->getArchive()->close();
  // Delete the uploaded archive file and temporary directory.
  file_delete($file);
  file_unmanaged_delete_recursive($temp_dir);
}

/**
 * Submit callback for 'icon_provider_import_form'.
 */
function icon_provider_import_form_submit($form, &$form_state) {
  // Redirect to the overview form after submit.
  $form_state['redirect'] = array(ICON_ADMIN_PATH);
  // Get the bundle and provider set in validation.
  $bundle = &$form_state['bundle'];
  $provider = &$form_state['provider'];
  // Invoke hook_icon_import_process().
  icon_extension_invoke($provider['type'], $provider[$provider['type']], 'icon_' . $provider['name'] . '_import_process', $bundle);
  // Save the bundle to the database.
  if (!icon_bundle_save($bundle)) {
    drupal_set_message(t('An error occurred while attemping to import the %provider icon bundle: %bundle.', array('%provider' => $provider['title'], '%bundle' => $bundle['title'])), 'error');
    return;
  }
  drupal_set_message(t('The %provider icon bundle %bundle was successfully imported.', array('%provider' => $provider['title'], '%bundle' => $bundle['title'])));
  // Clear the caches so the new bundle shows up.
  icon_clear_all_caches();
  // Redirect to the bundle configuration if there are settings to configure.
  module_load_include('inc', 'icon', 'includes/admin');
  $config_form = drupal_get_form('icon_bundle_configure_form', $bundle);
  $settings = element_children($config_form['settings']);
  if (!empty($settings)) {
    $form_state['redirect'] = array(ICON_ADMIN_PATH . '/bundle/' . $bundle['name'] . '/configure');
  }
}
