<?php

function imageblock_admin_settings_form() {
  $form['imageblock_image_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Image block image directory'),
    '#description' => t('Subdirectory in the file upload directory where images will be stored.'),
    '#default_value' => variable_get('imageblock_image_path', 'imageblock'),
    '#maxlength' => 255,
  );
  $form['imageblock_max_file_size'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum file size'),
    '#description' => t('Specify the size limit that applies to each image. Enter a value like "512" (bytes), "80K" (kilobytes) or "50M" (megabytes) in order to restrict the allowed file size. If you leave this empty the file sizes will be limited only by PHP\'s maximum post and file upload sizes (current limit <strong>%limit</strong>).', array('%limit' => format_size(file_upload_max_size()))),
    '#default_value' => variable_get('imageblock_max_file_size', 0),
    '#maxlength' => 15,
  );
  $form['imageblock_max_dimensions'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum dimensions'),
    '#description' => t('The maximum allowed image size expressed as WIDTHxHEIGHT (e.g. 640x480). Set to 0 for no restriction. If a larger image is uploaded, it will be resized to reflect the given width and height.'),
    '#default_value' => variable_get('imageblock_max_dimensions', 0),
    '#maxlength' => 15,
  );
  return system_settings_form($form);
}

function imageblock_admin_settings_form_validate($form, &$form_state) {
  $values = $form_state['values'];
  foreach (array('imageblock_max_file_size') as $size) {
    if (!empty($values[$size]) && !is_numeric(parse_size($values[$size]))) {
      form_error($size, t('The "@field" option must contain a valid value. You can either leave the text field empty or enter a string like "512" (bytes), "80K" (kilobytes) or "50M" (megabytes).', array('@field' => t('Maximum upload size per file'))));
    }
  }
  foreach (array('imageblock_max_dimensions') as $resolution) {
    if (!empty($values[$resolution]) && !preg_match('/^[0-9]+x[0-9]+$/', $values[$resolution])) {
      form_set_error($resolution, t('Please specify a resolution in the format WIDTHxHEIGHT (e.g. 640x480).'));
    }
  }
}

/**
 * Form builder for the add image block form.
 *
 * @see block_add_block_form_validate()
 * @see imageblock_add_block_form_submit()
 * @ingroup forms
 */
function imageblock_add_block_form() {
  module_load_include('inc', 'block', 'block.admin');
  $form = array();
  $form = block_admin_configure($form, $form_state, 'imageblock', NULL);
  $form['#validate'][] = 'block_add_block_form_validate';
  $form['#validate'][] = 'imageblock_configure_form_validate';
  return $form;
}

/**
 * Form submission handler for the add image block form.
 *
 * Saves the new custom image block.
 *
 * @see imageblock_add_block_form()
 * @see block_add_block_form_validate()
 */
function imageblock_add_block_form_submit($form, &$form_state) {
  $delta = db_insert('imageblock')
    ->fields(array(
      'body' => $form_state['values']['body']['value'],
      'info' => $form_state['values']['info'],
      'format' => $form_state['values']['body']['format'],
    ))
    ->execute();
  // Store block delta to allow other modules to work with new block.
  $form_state['values']['delta'] = $delta;

  $query = db_insert('block')->fields(array('visibility', 'pages', 'custom', 'title', 'module', 'theme', 'status', 'weight', 'delta', 'cache'));
  foreach (list_themes() as $key => $theme) {
    if ($theme->status) {
      $query->values(array(
        'visibility' => (int) $form_state['values']['visibility'],
        'pages' => trim($form_state['values']['pages']),
        'custom' => (int) $form_state['values']['custom'],
        'title' => $form_state['values']['title'],
        'module' => $form_state['values']['module'],
        'theme' => $theme->name,
        'status' => 0,
        'weight' => 0,
        'delta' => $delta,
        'cache' => DRUPAL_NO_CACHE,
      ));
    }
  }
  $query->execute();

  $query = db_insert('block_role')->fields(array('rid', 'module', 'delta'));
  foreach (array_filter($form_state['values']['roles']) as $rid) {
    $query->values(array(
      'rid' => $rid,
      'module' => $form_state['values']['module'],
      'delta' => $delta,
    ));
  }
  $query->execute();

  // Store regions per theme for this block
  foreach ($form_state['values']['regions'] as $theme => $region) {
    db_merge('block')
      ->key(array('theme' => $theme, 'delta' => $delta, 'module' => $form_state['values']['module']))
      ->fields(array(
        'region' => ($region == BLOCK_REGION_NONE ? '' : $region),
        'pages' => trim($form_state['values']['pages']),
        'status' => (int) ($region != BLOCK_REGION_NONE),
      ))
      ->execute();
  }

  imageblock_block_save($delta, $form_state['values']);

  drupal_set_message(t('The image block has been created.'));
  cache_clear_all();
  $form_state['redirect'] = 'admin/structure/block';
}

/**
 * Form builder for the image block deletion form.
 *
 * @param $delta
 *   The unique ID of the block within the context of $module.
 *
 * @see imageblock_custom_block_delete_submit()
 */
function imageblock_custom_block_delete($form, &$form_state, $delta) {
  $block = block_load('imageblock', $delta);
  $custom_block = block_imageblock_get($block->delta);
  $form['info'] = array('#type' => 'hidden', '#value' => $custom_block['info'] ? $custom_block['info'] : $custom_block['title']);
  $form['bid'] = array('#type' => 'hidden', '#value' => $block->delta);

  return confirm_form($form, t('Are you sure you want to delete the image block %name?', array('%name' => $custom_block['info'])), 'admin/structure/block', '', t('Delete'), t('Cancel'));
}

/**
 * Form submission handler for the image block deletion form.
 *
 * @see imageblock_custom_block_delete()
 */
function imageblock_custom_block_delete_submit($form, &$form_state) {
  // If there is a file, delete it.
  $file = imageblock_get_file($form_state['values']['bid']);
  if (!empty($file->fid)) {
    file_usage_delete($file, 'imageblock', 'imageblock', $form_state['values']['bid']);
    file_delete($file);
  }
  
  db_delete('imageblock')
    ->condition('bid', $form_state['values']['bid'])
    ->execute();
  db_delete('block')
    ->condition('module', 'imageblock')
    ->condition('delta', $form_state['values']['bid'])
    ->execute();
  db_delete('block_role')
    ->condition('module', 'imageblock')
    ->condition('delta', $form_state['values']['bid'])
    ->execute();
  
  drupal_set_message(t('The image block %name has been removed.', array('%name' => $form_state['values']['info'])));
  cache_clear_all();
  $form_state['redirect'] = 'admin/structure/block';
  return;
}
