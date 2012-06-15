<?php

/**
 * @file
 * Provides the theming functions for fivestar
 */

/**
 * Show a preview of a widget using a custom CSS file.
 */
function theme_fivestar_preview_widget($variables) {
  $path = drupal_get_path('module', 'fivestar');
  $form = array(
    '#post' => array(),
    '#programmed' => FALSE,
    '#tree' => FALSE,
    '#parents' => array(),
    '#array_parents' => array(),
    '#required' => FALSE,
    '#attributes' => array(),
    '#title_display' => 'before',
  );
  $form_state = form_state_defaults();
  $form_state['values'] = array();
  $form_state['process_input'] = array();
  $form_state['complete form'] = array();

  $form['vote'] = array(
    '#type' => 'fivestar',
    '#stars' => 5,
    '#auto_submit' => FALSE,
    '#allow_clear' => TRUE,
    '#widget' => array(
      'name' => isset($variables['name']) ? $variables['name'] : 'default',
      'css' => isset($variables['css']) && $variables['css'] != 'default' ? $variables['css'] : FALSE,
    ),
  );
  
  // Attach necessary JS settings
  $settings = array(
    'titleUser' => t('Your rating') .': ',
    'titleAverage' => t('Average') .': ',
    'feedbackSavingVote' => t('Saving your vote...'),
    'feedbackVoteSaved' => t('Your vote has been saved.'),
    'feedbackDeletingVote' => t('Deleting your vote...'),
    'feedbackVoteDeleted' => t('Your vote has been deleted.'),
  );

  drupal_add_js(array('fivestar' => $settings), 'setting');

  $form = form_builder('fivestar_preview', $form, $form_state);

  $output = '<div class="fivestar-star-preview fivestar-' . $form['vote']['#widget']['name'] . '">';
  $output .= drupal_render_children($form);
  $output .= '</div>';

  return $output;
}

function theme_fivestar_preview($variables) {
  extract($variables, EXTR_SKIP);
  $values = array(
    'average' => 50,
    'user' => 80,
    'count' => 20,
  );
  $settings = array(
    'stars' => $stars,
    'allow_clear' => $unvote,
    'style' => $style,
    'text' => $text,
    'title' => $title,
    'autosubmit' => FALSE,
    'tag' => 'vote',
  );

  $form = drupal_get_form('fivestar_custom_widget', $values, $settings);
  $form = drupal_render($form);
  // This regex is sadly necessary because having duplicate form_tokens or
  // form_id elements can cause the content type form to choke. Forms inside of
  // forms is also frowned upon, so this removes the wrapping form tag as well.
  $form = str_replace(array('<form', '</form>'), array('<div', '</div>'), $form);
  $form = preg_replace('/( method=".*?")|( action=".*?")|(<input.*?name="(form_token|form_id|destination|form_build_id)".*?\/>)/', '', $form);
  return $form;
}

function theme_fivestar_preview_wrapper($variables) {
  return '<div class="fivestar-preview fivestar-preview-'. $variables['type'] .'">'. $variables['content'] .'</div>';
}

/**
 * Theme function for 'default' fivestar field formatter.
 */
function theme_fivestar_formatter_default($variables) {
  $element = $variables['element'];
  if (empty($element['#instance_settings']['stars'])) {
    $element['#instance_settings']['stars'] = 5;
  }

  // Add CSS and JS
  $path = drupal_get_path('module', 'fivestar');
  drupal_add_js($path . '/js/fivestar.js');
  drupal_add_css($path . '/css/fivestar.css');

  $variables = array(
    'rating' => $element['#rating'],
    'stars' => $element['#instance_settings']['stars'],
    'widget' => $element['#widget'],
  );
  $star_display = theme('fivestar_static', $variables);
  return theme('fivestar_static_element', array('description' => $element['#description'], 'star_display' => $star_display));
}

/**
 * Theme function for 'rating' fivestar field formatter.
 */
function theme_fivestar_formatter_rating($variables) {
  $element = $variables['element'];

  if (empty($element['#item']['average'])) {
    $element['#item']['average'] = 0;
  }
  // Get number of stars.
  $stars = (empty($element['#instance_settings']['stars'])) ? 5 : $element['#instance_settings']['stars'];
  $average = $element['#item']['average'];
  // Rating is X out of Y stars.
  $rating = round(($average/100) * $stars, 1); 
  $output = $rating . '/' . $stars;

  return $output;
}

/**
 * Theme function for 'percentage' fivestar field formatter.
 */
function theme_fivestar_formatter_percentage($variables) {
  $element = $variables['element'];

  if (empty($element['#item']['average'])) {
    $element['#item']['average'] = 0;
  }

  return round($element['#item']['average'], 1) . '%';
}

/**
 * Theme the fivestar form element by adding necessary css and javascript.
 */
function theme_fivestar($variables) {
  $element = $variables['element'];

  return theme('form_element', array('element' => $element));
}

/**
 * Theme the straight HTML version of the fivestar select list. This is used
 * to remove the wrapping 'form-item' div from the select list.
 */
function theme_fivestar_select($variables) {
  $element = $variables['element'];
  element_set_attributes($element, array('id', 'name', 'size'));
  _form_set_class($element, array('form-select'));
  return '<select' . drupal_attributes($element['#attributes']) . '>' . form_select_options($element) . '</select>';
}
/**
 * Display a plain HTML view-only version of the widget with a specified rating.
 *
 * @param $rating
 *   The desired rating to display out of 100 (i.e. 80 is 4 out of 5 stars).
 * @param $stars
 *   The total number of stars this rating is out of.
 * @param $tag
 *   Allows multiple ratings per node.
 * @return
 *   A themed HTML string representing the star widget.
 */
function theme_fivestar_static($variables) {
  $rating  = $variables['rating'];
  $stars = $variables['stars'];
  $tag = $variables['tag'];
  $widget = $variables['widget'];
  if($widget['name'] != 'default') {
    drupal_add_css($widget['css']);
  }
  
  $output = '<div class="fivestar-' . $widget['name'] . '">';
  $output .= '<div class="fivestar-widget-static fivestar-widget-static-'. $tag .' fivestar-widget-static-'. $stars . ' clearfix">';
  if (empty($stars)) {
    $stars = 5;
  }
  $numeric_rating = $rating/(100/$stars);
  for ($n=1; $n <= $stars; $n++) {
    $star_value = ceil((100/$stars) * $n);
    $prev_star_value = ceil((100/$stars) * ($n-1));
    $zebra = ($n % 2 == 0) ? 'even' : 'odd';
    $first = $n == 1 ? ' star-first' : '';
    $last = $n == $stars ? ' star-last' : '';
    $output .= '<div class="star star-'. $n .' star-'. $zebra . $first . $last .'">';
    if ($rating < $star_value && $rating > $prev_star_value) {
      $percent = (($rating - $prev_star_value) / ($star_value - $prev_star_value)) * 100;
      $output .= '<span class="on" style="width: '. $percent .'%">';
    }
    elseif ($rating >= $star_value) {
      $output .= '<span class="on">';
    }
    else {
      $output .= '<span class="off">';
    }
    if ($n == 1)$output .= $numeric_rating;
    $output .= '</span></div>';
  }
  $output .= '</div></div>';
  return $output;
}

/**
 * Display the text associated with a static star display.
 *
 * Note that passing in explicit data types is extremely important when using
 * this function. A NULL value will exclude the value entirely from display,
 * while a 0 value indicates that the text should be shown but it has no value
 * yet.
 *
 * All ratings are from 0 to 100.
 *
 * @param $user_rating
 *   The current user's rating.
 * @param $average
 *   The average rating.
 * @param $votes
 *   The total number of votes.
 * @param $stars
 *   The number of stars being displayed.
 * @return
 *   A themed HTML string representing the star widget.
 */
function theme_fivestar_summary($variables) {
  $microdata = $variables['microdata'];
  extract($variables, EXTR_SKIP);
  $output = '';
  $div_class = '';
  $average_rating_microdata = '';
  if (isset($user_rating)) {
    $div_class = isset($votes) ? 'user-count' : 'user';
    $user_stars = round(($user_rating * $stars) / 100, 1);
    $output .= '<span class="user-rating">'. t('Your rating: <span>!stars</span>', array('!stars' => $user_rating ? $user_stars : t('None'))) .'</span>';
  }
  if (isset($user_rating) && isset($average_rating)) {
    $output .= ' ';
  }
  if (isset($average_rating)) {
    $div_class = isset($votes) ? 'average-count' : 'average';
    $average_stars = round(($average_rating * $stars) / 100, 1);
    if (!empty($microdata['average_rating']['#attributes'])) {
      $average_rating_microdata = drupal_attributes($microdata['average_rating']['#attributes']);
    }
    $output .= '<span class="average-rating">'. t('Average: <span !microdata>!stars</span>', array('!stars' => $average_stars, '!microdata' => $average_rating_microdata)) .'</span>';
  }
  if (isset($user_rating) && isset($average_rating)) {
    $div_class = 'combo';
  }

  if (isset($votes) && !(isset($user_rating) || isset($average_rating))) {
    $output .= ' <span class="total-votes">'. format_plural($votes, '<span>@count</span> vote', '<span>@count</span> votes') .'</span>';
    $div_class = 'count';
  }
  elseif (isset($votes)) {
    $output .= ' <span class="total-votes">('. format_plural($votes, '<span>@count</span> vote', '<span>@count</span> votes') .')</span>';
  }

  if ($votes === 0) {
    $output = '<span class="empty">'. t('No votes yet') .'</span>';
  }

  $output = '<div class="fivestar-summary fivestar-summary-'. $div_class . '">'. $output .'</div>';
  return $output;
}

/**
 * Display a static fivestar value as stars with a title and description.
 */
function theme_fivestar_static_element($variables) {
  $output = '';
  $output .= '<div class="fivestar-static-form-item">';
  $element = array(
    '#type' => 'item',
    '#title' => $variables['title'],
    '#description' => $variables['description'],
    '#children' => $variables['star_display'],
  );

  $output .= theme('form_element', array('element' => $element));
  $output .= '</div>';
  return $output;
}
