<?php

/**
 * @file
 * template.php
 */


/**
 * Implements hook_preprocess_html().
 */
function mas_preprocess_html(&$variables) {
  drupal_add_css('//fonts.googleapis.com/css?family=Open+Sans:400,600' , array('group' => CSS_THEME));

  // Hide HTML5 Geocoding feature for IE.
  drupal_add_css(path_to_theme() . '/css/ie.css', array(
      'group' => CSS_THEME, 'browsers' => array(
        'IE' => 'lte IE 9',
        '!IE' => FALSE),
      'weight' => 999,
      'preprocess' => FALSE)
  );

  $ie_render_engine = array(
    '#type' => 'html_tag',
    '#tag' => 'meta',
    '#attributes' => array(
      'http-equiv' => 'X-UA-Compatible',
      'content' =>  'IE=edge,chrome=1',
    ),
    '#weight' => '-99999',
  );
  drupal_add_html_head($ie_render_engine, 'meta_ie_render_engine');


}


/**
 * Implements hook_preprocess_image_style().
 */
function mas_preprocess_image(&$variables) {
  $variables['attributes']['class'] = "img-thumbnail";
}

/**
 * Preprocess variables for comment.tpl.php.
 *
 * @see comment.tpl.php
 */
function mas_preprocess_comment(&$variables) {

  $comment_author_uid = $variables['comment']->uid;
  $account = user_load_multiple(array('uid' => $comment_author_uid));

  if (in_array('Management', $account[$comment_author_uid]->roles) || in_array('administrator', $account[$comment_author_uid]->roles)) {
    $variables['classes_array'][] = "management-comment";
  }
}
