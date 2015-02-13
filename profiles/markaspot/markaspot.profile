<?php
/**
 * @file
 * Profile File, handling Install tasks.
 */

/**
 * Get term names.
 *
 * @param string $termname
 *   Term-name
 *
 * @return int
 *   $term->tic The term id
 */
function markaspot_get_term_id($termname) {
  $terms = taxonomy_get_term_by_name($termname);
  foreach ($terms as $term) {
    if ($term->name == $termname) {
      return $term->tid;
    }
  }
}


/**
 * Implements hook_form_FORM_ID_alter().
 *
 * Allows the profile to alter the site configuration form.
 */
function markaspot_form_install_configure_form_alter(&$form, $form_state) {
  // Pre-populate the site name with the server name.
  $form['site_information']['site_name']['#default_value'] = $_SERVER['SERVER_NAME'];
}

/**
 * Implements hook_install_tasks_alter().
 *
 * Allows the profile to alter the site configuration form.
 */
function markaspot_install_tasks_alter(&$tasks, $install_state) {
  $tasks['install_select_locale']['function'] = 'markaspot_locale_selection';
  $tasks['install_finished']['function'] = 'markaspot_install_finished';
}

/**
 * Set default language to en
 *
 * @param $install_state
 */
function markaspot_locale_selection(&$install_state){
  $install_state['parameters']['locale'] = 'en';
}
