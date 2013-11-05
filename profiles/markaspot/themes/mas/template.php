<?php

/**
 * Implements hook_form_alter().
 */
function mas_form_alter(&$form, &$form_state, $form_id) {

  switch($form_id) {

    case 'report_node_form': // the value we stole from the rendered form

      drupal_set_title(t('Add your report'));
      $form['revision_information']['#access'] = false;
      // $form['instruction'] = array(
      // '#type' => 'item',
      // '#markup' => '<div><p class="span6">In drei Schritten zu unseren Dienstleistungen: Suchen Sie die Strasse, beschreiben Sie Ihr Anliegen und laden Sie ggf. ein Foto hoch.</p>'.'<p class="span6 required-explained"><span class="form-required">*</span>'. t(' indicates required fields') .'</p></div>',
      // '#weight' => -1001, // Adjust so that you can place it whereever
      // );
      // break;
  }
}

/**
 * Implements hook_theme().
 */
function mas_theme(&$existing, $type, $theme, $path) {
  // If we are auto-rebuilding the theme registry, warn about the feature.
  if (
    // Only display for site config admins.
    isset($GLOBALS['user']) && function_exists('user_access') && user_access('administer site configuration')
    && theme_get_setting('mas_rebuild_registry')
    // Always display in the admin section, otherwise limit to three per hour.
    && (arg(0) == 'admin' || flood_is_allowed($GLOBALS['theme'] . '_rebuild_registry_warning', 3))
  ) {
    flood_register_event($GLOBALS['theme'] . '_rebuild_registry_warning');
    drupal_set_message(t('For easier theme development, the theme registry is being rebuilt on every page request. It is <em>extremely</em> important to <a href="!link">turn off this feature</a> on production websites.', array('!link' => url('admin/appearance/settings/' . $GLOBALS['theme']))), 'warning', FALSE);
  }

  return array(
    'mas_links' => array(
      'variables' => array(
        'links' => array(),
        'attributes' => array(),
        'heading' => NULL
      ),
    ),
    'mas_btn_dropdown' => array(
      'variables' => array(
        'links' => array(),
        'attributes' => array(),
        'type' => NULL
      ),
    ),
    'mas_modal' => array(
      'variables' => array(
        'heading' => '',
        'body' => '',
        'footer' => '',
        'attributes' => array(),
        'html_heading' => FALSE,
      ),
    ),
    'mas_accordion' => array(
      'variables' => array(
        'id' => '',
        'elements' => array(),
      ),
    ),
    'mas_search_form_wrapper' => array(
      'render element' => 'element',
    ),
  );
}

/**
 * Override theme_breadrumb().
 *
 * Print breadcrumbs as a list, with separators.
 */
function mas_breadcrumb($variables) {
  $breadcrumb = $variables['breadcrumb'];

  if (!empty($breadcrumb)) {
    $breadcrumbs = '<ul class="breadcrumb">';

    $count = count($breadcrumb) - 1;
    foreach ($breadcrumb as $key => $value) {
      if ($count != $key) {
        $breadcrumbs .= '<li>' . $value . '<span class="divider">/</span></li>';
      }
      else{
        $breadcrumbs .= '<li>' . $value . '</li>';
      }
    }
    $breadcrumbs .= '</ul>';

    return $breadcrumbs;
  }
}

/**
 * Override or insert variables in the html_tag theme function.
 */
function mas_process_html_tag(&$variables) {
  $tag = &$variables['element'];

  if ($tag['#tag'] == 'style' || $tag['#tag'] == 'script') {
    // Remove redundant type attribute and CDATA comments.
    unset($tag['#attributes']['type'], $tag['#value_prefix'], $tag['#value_suffix']);

    // Remove media="all" but leave others unaffected.
    if (isset($tag['#attributes']['media']) && $tag['#attributes']['media'] === 'all') {
      unset($tag['#attributes']['media']);
    }
  }
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


/**
 * Add body classes if certain regions have content.
 */
function mas_preprocess_html(&$variables) {

  if (!empty($variables['page']['footer_firstcolumn'])
    || !empty($variables['page']['footer_secondcolumn'])
    || !empty($variables['page']['footer_thirdcolumn'])
    || !empty($variables['page']['footer_fourthcolumn'])) {
    $variables['classes_array'][] = 'footer-columns';
  }
}




/**
 * Preprocess variables for page.tpl.php
 *
 * @see page.tpl.php
 */
function mas_preprocess_page(&$variables) {
  // Add information about the number of sidebars.
  if (!empty($variables['page']['sidebar_first']) && !empty($variables['page']['sidebar_second'])) {
    $variables['columns'] = 3;
  }
  elseif (!empty($variables['page']['sidebar_first'])) {
    $variables['columns'] = 2;
  }
  elseif (!empty($variables['page']['sidebar_second'])) {
    $variables['columns'] = 2;
  }
  else {
    $variables['columns'] = 1;
  }

  // Primary nav
  $variables['primary_nav'] = FALSE;
  if ($variables['main_menu']) {
    // Build links
    $variables['primary_nav'] = menu_tree(variable_get('menu_main_links_source', 'main-menu'));
    // Provide default theme wrapper function
    $variables['primary_nav']['#theme_wrappers'] = array('menu_tree__primary');
  }

  // Secondary nav
  $variables['secondary_nav'] = FALSE;
  if ($variables['secondary_menu']) {
    // Build links
    $variables['secondary_nav'] = menu_tree(variable_get('menu_secondary_links_source', 'user-menu'));
    // Provide default theme wrapper function
    $variables['secondary_nav']['#theme_wrappers'] = array('menu_tree__secondary');
  }

}


/**
 * Bootstrap theme wrapper function for the primary menu links
 */
function mas_menu_tree__primary(&$variables) {
  return '<ul class="menu nav">' . $variables['tree'] . '</ul>';
}

/**
 * Bootstrap theme wrapper function for the secondary menu links
 */
function mas_menu_tree__secondary(&$variables) {
  return '<ul class="menu nav pull-right">' . $variables['tree'] . '</ul>';
}

/**
 * Returns HTML for a single local action link.
 *
 * This function overrides theme_menu_local_action() to add the icons that ship
 * with Bootstrap to the action links.
 *
 * @param $variables
 *   An associative array containing:
 *   - element: A render element containing:
 *     - #link: A menu link array with "title", "href", "localized_options", and
 *       "icon" keys. If "icon" is not passed, it defaults to "plus-sign".
 *
 * @ingroup themeable
 *
 * @see theme_menu_local_action().
 */
function mas_menu_local_action($variables) {
  $link = $variables['element']['#link'];

  // Build the icon rendering element.
  if (empty($link['icon'])) {
    $link['icon'] = 'plus-sign';
  }
  $icon = '<i class="' . drupal_clean_css_identifier('icon-' . $link['icon']) . '"></i>';

  // Format the action link.
  $output = '<li>';
  if (isset($link['href'])) {
    $options = isset($link['localized_options']) ? $link['localized_options'] : array();

    // If the title is not HTML, sanitize it.
    if (empty($link['localized_options']['html'])) {
      $link['title'] = check_plain($link['title']);
    }

    // Force HTML so we can add the icon rendering element.
    $options['html'] = TRUE;
    $output .= l($icon . $link['title'], $link['href'], $options);
  }
  elseif (!empty($link['localized_options']['html'])) {
    $output .= $icon . $link['title'];
  }
  else {
    $output .= $icon . check_plain($link['title']);
  }
  $output .= "</li>\n";

  return $output;
}

/**
 * Preprocess variables for node.tpl.php
 *
 * @see node.tpl.php
 */
function mas_preprocess_node(&$variables) {
  if ($variables['teaser']) {
    $variables['classes_array'][] = 'row-fluid';
  }
}

/**
 * Preprocess variables for region.tpl.php
 *
 * @see region.tpl.php
 */
function mas_preprocess_region(&$variables, $hook) {
  if ($variables['region'] == 'content') {
    $variables['theme_hook_suggestions'][] = 'region__no_wrapper';
  }

  if (strstr($variables['region'], 'footer_')) {
    $variables['classes_array'][] = 'span3';
  }

}

/**
 * Preprocess variables for block.tpl.php
 *
 * @see block.tpl.php
 */
function mas_preprocess_block(&$variables, $hook) {
  // Use a bare template for the page's main content.
  if ($variables['block_html_id'] == 'block-system-main') {
    $variables['theme_hook_suggestions'][] = 'block__no_wrapper';
  }
  if ($variables['block_html_id'] == 'block-markaspot-logic-taxonomy-category') {
    $variables['classes_array'][] = 'span3';
  }
  if ($variables['block_html_id'] == 'block-markaspot-logic-taxonomy-status') {
    $variables['classes_array'][] = 'span3';
  }
  if ($variables['block_html_id'] == 'block-markaspot-logic-markaspot-map') {
    $variables['classes_array'][] = 'span3';
  }
  $variables['title_attributes_array']['class'][] = 'block-title';
}

/**
 * Override or insert variables into the block templates.
 *
 * @param $variables
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("block" in this case.)
 */
function mas_process_block(&$variables, $hook) {
  // Drupal 7 should use a $title variable instead of $block->subject.
  $variables['title'] = $variables['block']->subject;
}

/**
 * Returns the correct span class for a region
 */
function _mas_content_span($columns = 1) {
  $class = FALSE;

  switch($columns) {
    case 1:
      $class = 'span12';
      break;
    case 2:
      $class = 'span9';
      break;
    case 3:
      $class = 'span6';
      break;
  }

  return $class;
}

/**
 * Adds the search form's submit button right after the input element.
 *
 * @ingroup themable
 */
function mas_search_form_wrapper(&$variables) {
  $output = '<div class="input-append">';
  $output .= $variables['element']['#children'];
  $output .= '<button type="submit" class="btn">';
  $output .= '<i class="icon-search"></i>';
  $output .= '<span class="element-invisible">' . t('Search') . '</span>';
  $output .= '</button>';
  $output .= '</div>';
  return $output;
 }
