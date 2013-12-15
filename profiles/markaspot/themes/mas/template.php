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
