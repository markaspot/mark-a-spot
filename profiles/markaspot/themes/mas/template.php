<?php

/**
 * @file
 * template.php
 */


/**
 * THEME_preprocess_image_style() is also available.
 */
function mas_preprocess_image(&$variables) {
  $variables['attributes']['class'] = "img-responsive";
  //var_dump($variables);
}

/**
 * Preprocess variables for comment.tpl.php
 *
 * @see comment.tpl.php
 */
function mas_preprocess_comment(&$variables) {

  $comment_author_uid = $variables['comment']->uid;
  $account = user_load_multiple(array('uid'=>$comment_author_uid));

  if (in_array('Management', $account[$comment_author_uid]->roles)) {
    $variables['classes_array'][] = "management-comment";
  }
}
