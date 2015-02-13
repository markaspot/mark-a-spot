<?php
/**
 * @file
 * icon.api.php
 * Hooks and form elements provided by the Icon API module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Define render hook information.
 *
 * @return array
 *   An associative array containing:
 *   - render_name: A unique machine name and an associative array containing:
 *     - file: Optional, file where the preprocessing and theming hooks are
 *       defined. If omitted, this will fall back to the .module or template.php
 *       file respectively.
 *     - path: Optional, path where the file above is located. If omitted, this
 *       will fall back to the module or theme path that implemented this hook.
 *       This may be obvious to some, but if the path is not the default module
 *       or theme root path, the file must also be explicitly set.
 *
 * @see icon_icon_render_hooks()
 */
function hook_icon_render_hooks() {
  $hooks['my_render'] = array();
  $hooks['my_render_alt'] = array(
    'file' => 'icon.render.inc',
    'path' => drupal_get_path('module', 'my_module') . '/includes',
  );
  return $hooks;
}

/**
 * Allow extensions to alter icon render hooks before they become cached.
 */
function hook_icon_render_hooks_alter(&$hooks) {
  // Alter data from $hooks here.
}

/**
 * Define information about icon bundles.
 *
 * @return array
 *   An associative array containing:
 *   - bundle_name: A unique machine name and an associative array containing:
 *     - render: Required, name of the rendering hook this bundle should use.
 *       If omitted or the rendering hook is not implemented, then this bundle
 *       will be ignored.
 *     - icons: Required, an associative array containing icon data. This array
 *       requires a unique key name for each icon. The value may be a string or
 *       array and entirely depends on which render hook is being used. If
 *       unsure, study the render hook's theme_icon_RENDER() implementation.
 *     - title: Optional, human readable title for the bundle. If omitted, it
 *       will fall back to using the bundle_name.
 *     - provider: Optional, name of the bundle provider. If omitted, it will
 *       fall back to using the machine name of the extension that implements
 *       this hook.
 *     - url: Optional, URL for more information regarding the bundle.
 *     - version: Optional, supplemental information for identifying the bundle
 *       version.
 *     - path: Optional, path to where the bundle's resource files are located.
 *       If omitted, it will fall back to using the path of the extension that
 *       implements this hook.
 *     - settings: Optional, an array containing setting data. The structure of
 *       this array entirely depends on which render hook is be used. If unsure,
 *       study the render hook's theme_icon_RENDER() implementation.
 *     - #attached: Optional, an associative array containing additional
 *       resources to be loaded with the bundle. This allows render hooks the
 *       ability to specify external resources that contain the necessary code
 *       for the icons to function properly, such as a CSS file for sprites.
 *
 * @see icon_icon_bundles()
 * @see icon_bundle_load()
 * @see icon_bundles()
 * @see icon_bundle_defaults()
 */
function hook_icon_bundles() {
  $bundles['my_bundle'] = array(
    'render' => 'image',
    'title' => t('My Bundle'),
    'provider' => t('ACME Inc.'),
    'url' => 'http://example.com',
    'version' => 'v2',
    'path' => drupal_get_path('module', 'my_module') . '/icons',
    'icons' => array(
      'alert' => 'Message: Alert',
      'info' => 'Message: Info',
      'warning' => 'Message: Warning',
    ),
    'settings' => array(
      // Defaults to 'png' for the render hook: image.
      'extension' => 'gif',
    ),
    '#attached' => array(
      'css' => array(),
      'js' => array(),
      'library' => array(),
      // To invoke "callback_function($arg1, $arg2, $arg3)"
      'callback_function' => array(array('$arg1', '$arg2', '$arg3')),
    ),
  );
  return $bundles;
}

/**
 * Allow extensions to alter icon bundles before they become cached.
 */
function hook_icon_bundles_alter(&$bundle) {
}

/**
 * Allow extensions to alter a bundle listing before it's viewed.
 *
 * @param array $build
 *   The render array passed by reference.
 * @param array $bundle
 *   The bundle array used for context.
 *
 * @see icon_bundle_list()
 */
function hook_icon_bundle_list_alter(&$build, $bundle) {
}

/**
 * Implements hook_preprocess_icon().
 *
 * @see icon_preprocess_icon_image()
 * @see template_preprocess_icon()
 */
function hook_preprocess_icon(&$variables) {
  $bundle = &$variables['bundle'];
  if ($bundle['render'] === 'my_render_hook' || $bundle['provider'] === 'my_provider' || $bundle['name'] === 'my_bundle') {
    // Add an additional custom class.
    $variables['attributes']['class'][] = 'my-custom-class';
  }
}

/**
 * Implements hook_process_icon().
 */
function hook_process_icon(&$variables) {
  // Not likely needed, but inherently available nonetheless.
}

/**
 * Perform actions after a bundle has been deleted.
 *
 * This hook is invoked after a bundle has been removed from the database and
 * filesystem. This hook simply allows extensions to run additional tasks with
 * the bundle's data array as context after it has been successfully deleted.
 *
 * @see icon_bundle_delete()
 */
function hook_icon_bundle_delete($bundle) {
  // Do something in your custom module, such as removing variables or database
  // rows in a different table that your custom module is tracking.
}

/**
 * Perform actions after a bundle, provided in code, has been reset.
 *
 * This hook is invoked after a bundle's configuration has been removed from
 * the database. This hook simply allows extensions to run additional tasks
 * with the bundle's data array as context after it has been successfully reset.
 *
 * @see icon_bundle_delete()
 */
function hook_icon_bundle_reset($bundle) {
  // Do something in your custom module, such as removing variables or database
  // rows in a different table that your custom module is tracking.
}

/**
 * Allow extensions to alter the bundle record before saving it to the database.
 */
function hook_icon_bundle_save_alter(&$bundle) {
  if ($bundle['name'] === 'my_bundle' || $bundle['provider'] === 'my_provider') {
    // Rename the bundle, this would effectively clone an existing bundle.
    // Warning: make sure you don't rename to an existing bundle name as this
    // would overwrite/override it.
    $bundle['name'] = 'rename_bundle_machine_name';
    // Set/override bundle settings.
    $bundle['settings']['custom_property'] = 'custom_value';
    // Change the enabled status of the bundle.
    $bundle['status'] = variable_get('my_conditional_variable', 1);
  }
}

/**
 * Allow extensions to be grouped with Icon API on the permissions table.
 * 
 * @return array
 *   Same type of array in hook_permission().
 *
 * @see icon_permission()
 * @see icon_block_icon_permission()
 * @see hook_permission()
 */
function hook_icon_permission() {
  return array(
    'administer block icons' => array(
      'title' => t('Administer block icons'),
      'description' => t('Grants selected roles the ability to administer icons in blocks.'),
    ),
  );
}

/**
 * Define information about icon providers.
 *
 * To provide archived file import support, a provider must implement
 * hook_icon_PROVIDER_import_process() and hook_icon_PROVIDER_import_validate().
 *
 * @return array
 *   An associative array containing:
 *   - provider_name: A unique machine name and an associative array containing:
 *     - title: Optional, human readable title for the provider. If omitted, it
 *       will fall back to using the provider_name.
 *     - url: Optional, URL for more information regarding the provider.
 *     - default bundle: Optional, an array containing default bundle properties
 *       specific to this provider.
 *
 * @see icon_icon_providers()
 * @see icon_provider_load()
 * @see icon_providers()
 * @see icon_provider_defaults()
 * @see hook_icon_PROVIDER_import_process()
 * @see hook_icon_PROVIDER_import_validate()
 */
function hook_icon_providers() {
  $providers['my_provider'] = array(
    'title' => t('My Provider'),
    'url' => 'http://example.com',
    'default bundle' => array(
      'render' => 'sprite',
      'settings' => array(),
    ),
  );
  return $providers;
}

/**
 * Provider process archive import.
 *
 * Allow extensions to properly process the bundle after it has been extracted
 * to the proper location and successfully imported. This is typically the step
 * where most logic should take place to determine available icons and settings.
 * This is the step right before it is saved to the database.
 *
 * @see icon_provider_import_form_submit()
 */
function hook_icon_PROVIDER_import_process(&$bundle) {
  // Process the $bundle data here...
}

/**
 * Validate callback for 'hook_icon_PROVIDER_import_process'.
 *
 * Must return TRUE to claim bundle.
 *
 * @see icon_provider_import_form_validate()
 */
function hook_icon_PROVIDER_import_validate($bundle) {
  if ($bundle['condition'] === 'condition_met') {
    return TRUE;
  }
  $provider = icon_provider_load($bundle['provider']);
  return t('Invalid %provider bundle.', array('%provider' => $provider['title']));
}

/*
 * @} End of "addtogroup hooks".
 */


/**
 * @addtogroup themable
 * @{
 */

/**
 * Implements theme_icon_RENDER_HOOK().
 *
 * Return an image of the requested icon.
 *
 * @param array $variables
 *   An associative array containing:
 *   - attributes: Associative array of HTML attributes.
 *   - bundle: An associative array containing various properties.
 *   - icon: Name of the icon requested from the above bundle.
 *
 * @return string
 *   HTML markup for the requested icon.
 *
 * @see hook_icon_bundles()
 * @see theme_icon_image()
 */
function theme_icon_RENDER_HOOK($variables) {
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

/*
 * @} End of "addtogroup themable".
 */


/**
 * @addtogroup input_element
 * @{
 */

/**
 * Implements the icon_selector input element.
 *
 * The icon_selector sets a tree state of TRUE, values will be inside
 * the element's value tree as "bundle" and "icon".
 * 
 * string #type
 *   The type of element to render, must be: icon_selector.
 * string #title
 *   Optional, the title of the fieldset.
 *   Default, "Icon".
 * boolean #collapsible
 *   Optional, determine whether the fieldset is collapsible.
 *   Default, TRUE.
 * boolean #collapsed
 *   Optional, determine whether the fieldset should initialize in a collapsed
 *   state. Default, FALSE.
 *  string #default_bundle
 *   Optional, machine name of the default bundle to initialize with.
 * string #default_icon
 *   Optional, machine name of the default icon to initialize with.
 *  
 * @see icon_block_form_alter()
 * @see icon_block_form_submit()
 */
function _icon_selector_element_info() {
  return array(
    '#type' => 'icon_selector',
    '#title' => t('Icon'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#default_bundle' => '',
    '#default_icon' => '',
  );
}

/*
 * @} End of "addtogroup input_element".
 */
