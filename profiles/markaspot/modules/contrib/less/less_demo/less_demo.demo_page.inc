<?php

/**
 * Callback that provides styleable content for demonstration purposes.
 */
function _less_demo_page() {
  global $theme_key;

  $file_less_settings = array(
    'variables' => array(
      '@gradient_end' => '#bada55',
    ),
  );

  $output = array(
    '#attached' => array(
      'css' => array(
        drupal_get_path('module', 'less_demo') . '/styles/less_demo.drupal_add_css.css.less' => array(
          'less' => $file_less_settings,
        )
      ),
    ),
  );

  $output['page'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'id' => 'less_demo_gradient',
    ),
  );

  $output['page']['less_demo_logo'] = array(
    '#type' => 'html_tag',
    '#tag' => 'div',
    '#attributes' => array(
      'class' => array(
        'less_demo_logo',
      ),
    ),
    '#value' => '', // '#value' is required for closing tag.
  );

  $output['page']['less_demo_theme_link'] = array(
    '#type' => 'html_tag',
    '#tag' => 'p',
    '#value' => l('Configure current theme', 'admin/appearance/settings/' . $theme_key),
  );

  return $output;
}
