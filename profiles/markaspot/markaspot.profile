<?php

/**
 * Implements hook_form_FORM_ID_alter().
 *
 * Allows the profile to alter the site configuration form.
 */
function markaspot_form_install_configure_form_alter(&$form, $form_state) {
  // Pre-populate the site name with the server name.
  $form['site_information']['site_name']['#default_value'] = $_SERVER['SERVER_NAME'];
}



// Change install task on finish and load content
function markaspot_install_tasks_alter(&$tasks, $install_state){
	  $tasks['install_finished']['function'] = 'markaspot_install_finished';

}

function markaspot_install_finished(&$install_state) {

  // saving taonomies for status
  _createStatus();
  // saving taxonomies for category
  _createCategories();
  // creating example node
  _createNode();
  // activating blocks
  _build_blocks();
  // deleting dummy entries
  _delete_dummies();


  drupal_set_title(st('Mark-a-Spot installation complete'));
  $messages = drupal_set_message();

  $output = '<p>' . st('Congratulations, you installed Mark-a-Spot @drupal!', array('@drupal' => drupal_install_profile_distribution_name())) . '</p>';
  $output .= '<p>' . (isset($messages['error']) ? st('Review the messages above before visiting <a href="@url">your new site</a>.', array('@url' => url(''))) : st('<a href="@url">Visit your new site</a>.', array('@url' => url('')))) . '</p>';

  // Flush all caches to ensure that any full bootstraps during the installer
  // do not leave stale cached data, and that any content types or other items
  // registered by the install profile are registered correctly.
  drupal_flush_all_caches();

  // Remember the profile which was used.
  variable_set('install_profile', drupal_get_profile());

  // Install profiles are always loaded last
  db_update('system')
    ->fields(array('weight' => 1000))
    ->condition('type', 'module')
    ->condition('name', drupal_get_profile())
    ->execute();

  // Cache a fully-built schema.
  drupal_get_schema(NULL, TRUE);

  // Run cron to populate update status tables (if available) so that users
  // will be warned if they've installed an out of date Drupal version.
  // Will also trigger indexing of profile-supplied content or feeds.
  drupal_cron_run();

  return $output;
}

function _createStatus () {

  // // Create taxonomy vocabulary for status.
  // taxonomy_vocabulary_save((object) array(
  //   'name' => 'Status',
  //   'machine_name' => 'status',
  // ));


  // Get the vocabulary ID.
  $vid = db_query("SELECT vid FROM {taxonomy_vocabulary} WHERE machine_name = 'status'")->fetchField();

  // Define the terms, with description and color
  $terms[0] = array('Open', 'This is just a description which should be replaced', 'cc0000', 'pause');
  $terms[1] = array('In progress','This is just a description which should be replaced', 'ff6600', 'play');
  $terms[3] = array('Solved','This is just a description which should be replaced', '8fe83b', 'checkmark');
  $terms[4] = array('Archive','This is just a description which should be replaced', 'cccccc', 'drawer');

  // You may customize those entries
  $terms[5] = array('dummy','This is just a description which should be replaced', '8fe83b', '');
  $terms[6] = array('dummy','This is just a description which should be replaced', '8fe83b', '');
  $terms[7] = array('dummy','This is just a description which should be replaced', '8fe83b', '');
  $terms[8] = array('dummy','This is just a description which should be replaced', '8fe83b', '');
  $terms[9] = array('dummy','This is just a description which should be replaced', '8fe83b', '');

  foreach ($terms as $parent) {
    // Create the parent term.
    $term['vid'] = $vid;
    $term['name'] = $parent[0];
    $term['description'] = $parent[1];
    $term['field_status_hex']['und'][0]['value'] = $parent[2];
    $term['field_status_icon']['und'][0]['value'] = $parent[3];

    // taxonomy_term_save((object)$term);
    // $term = ;
    $status = taxonomy_term_save((object)$term);
    switch ($status) {
      case SAVED_NEW:
        drupal_set_message(t('Created new term %term.', array('%term' => $term['name'])));
        // watchdog('taxonomy', 'Created new term %term.', array('%term' => $term['name']), WATCHDOG_NOTICE, l(t('edit'), 'taxonomy/term/' . $term['tid'] . '/edit'));
        break;
      case SAVED_UPDATED:
        drupal_set_message(t('Updated term %term.', array('%term' => $term['name'])));
        // watchdog('taxonomy', 'Updated term %term.', array('%term' => $term['name']), WATCHDOG_NOTICE, l(t('edit'), 'taxonomy/term/' . $term['tid'] . '/edit'));
        // Clear the page and block caches to avoid stale data.
        cache_clear_all();
        break;
    }
  }
  return;
}


function _createCategories() {

  // Get the vocabulary ID.
  $vid = db_query("SELECT vid FROM {taxonomy_vocabulary} WHERE machine_name = 'category'")->fetchField();

  // Define the terms.
  $terms[0] = array('Abandoned Cars', 'abandoned, wrecked, dismantled, or inoperative cars on private property', '010', 'abandonedcar, cars, wreckedcar, car', '00008B','car');
  $terms[1] = array('Litter Basket Complaint','Litter Basket Request or Complaint', '011', 'litter, trash, garbage', '5F9EA0','trash') ;
  $terms[2] = array('Graffiti Report','Report graffiti on a building you own.', '012', 'graffiti, paintings', '8B0000', 'graffiti') ;
  $terms[3] = array('Building Construction Complaint','Dangerous Buildings and Vacant Property Operations historically has been known for the demolition of dangerous buildings, but recent adjustments in service delivery are focusing on inventorying vacant structures', '013', 'graffiti, demolition', '006400', 'office') ;


  foreach ($terms as $parent) {
    // Create the parent term.
    $term['vid'] = $vid;

    $term['name'] = $parent[0];
    $term['description'] = $parent[1];
    $term['field_category_id']['und'][0]['value'] = $parent[2];
    $term['field_hash']['und'][0]['value'] = $parent[3];
    $term['field_category_hex']['und'][0]['value'] = $parent[4];
    $term['field_category_icon']['und'][0]['value'] = $parent[5];

    // taxonomy_term_save((object)$term);
    // $term = ;
    $status = taxonomy_term_save((object)$term);
    switch ($status) {
      case SAVED_NEW:
        drupal_set_message(t('Created new term %term.', array('%term' => $term['name'])));

        break;
      case SAVED_UPDATED:
        drupal_set_message(t('Updated term %term.', array('%term' => $term['name'])));
        // watchdog('taxonomy', 'Updated term %term.', array('%term' => $term['name']), WATCHDOG_NOTICE, l(t('edit'), 'taxonomy/term/' . $term->tid . '/edit'));
        // Clear the page and block caches to avoid stale data.
        cache_clear_all();
        break;
    }
  }
  return;
}



function _createNode(){
  $i = 0;
  // now creating initial report
  $nodes[0] = array('Garbage Collection', 'Lorem Ipsum Lorem ipsum dolor sit amet, consectetur ing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam,  quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo', '50.8212596','6.8961028','Pingsdorfer Straße 88, 50321 Brühl','holger@markaspot.org','11', '1', '0', 'flickr_by_dancentury_garbage_collection_4585329947');

  $nodes[1] = array('Some graffiti', 'Lorem Ipsum Lorem ipsum dolor sit amet, consectetur ing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam,  quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo', '50.81812812677597','6.8905774494293155','Am Ringofen 21, 50321 Brühl','holger@markaspot.org', '12', '1', '0', 'flickr_by_striatic_grafitti_133146861');

  $nodes[2] = array('Abandoned car', 'Lorem Ipsum Lorem ipsum dolor sit amet, consectetur ing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam,  quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo', '50.82435019881909','6.895512714016718','Liblarer Straße 88, 50321, Brühl','holger@markaspot.org', '10', '3', '0', 'flickr_thomasbrandt');

  $nodes[3] = array('Danger at subway construction', 'Lorem Ipsum Lorem ipsum dolor sit amet, consectetur ing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam,  quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo', '50.8282131596655','6.90819419823913','Promenade, 50321 Brühl','holger@markaspot.org', '13','3', '0', 'flickr_holger_baustellenlage_ebertplatz');

  $nodes[4] = array('Really Abandoned car', 'Lorem Ipsum Lorem ipsum dolor sit amet, consectetur ing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam,  quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo', '50.8327211', '6.9032226','Friedrichstraße 23 50321 Brühl','holger@markaspot.org', '10', '4', '1','flickr_by_mikebaird_abandoned_car_4585329947');

  $nodes[5] = array('Garbage collection', 'Lorem Ipsum Lorem ipsum dolor sit amet, consectetur ing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam,  quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo', '50.826873' ,' 6.900167','Centre, 50321 Brühl','holger@markaspot.org', '11', '4', '1','flickr_by_realname_garbage-tonal-decay');


  foreach ($nodes as $node_data) {
    $node = new stdClass(); // Create a new node object
    $node->type = "report"; // Or page, or whatever content type you like

    node_object_prepare($node); // Set some default values

    $i++;
    $node->title    = $node_data[0];
    $node->language = 'und'; // Or e.g. 'en' if locale is enabled

    $node->uid = 1; // UID of the author of the node; or use $node->name
    $node->nid = $i;
    $node->language = 'und'; // language - neutral
    $node->body[$node->language][0]['value']   = $node_data[1];
    $node->body[$node->language][0]['format']  = 'filtered_html';
    $node->field_geo[$node->language][0]['lat'] = $node_data[2];
    $node->field_geo[$node->language][0]['lng'] = $node_data[3];
    $node->field_address[$node->language][0]['value'] = $node_data[4];
    $node->field_e_mail[$node->language][0]['value'] = $node_data[5];
    $node->field_category[$node->language][0]['tid'] = $node_data[6];
    $node->field_status[$node->language][0]['tid'] = $node_data[7];
    $node->field_common['und'] = $node_data[8];
    $node->is_new = true;
    $node->promote = 0;
    $filename = 'image_'.$node_data[9].'.jpg';
    // var_dump(base_path().'profiles/markaspot/themes/mas/images/'.$node_data[9]);
    $image = file_get_contents('profiles/markaspot/themes/mas/images/'.$node_data[9].'.jpg');
    $file = file_save_data($image, 'public://' . $filename, FILE_EXISTS_RENAME);
    $node->field_image = array(LANGUAGE_NONE => array('0' => (array)$file));


    // I prefer using pathauto, which would override the below path
    // $path = 'node_created_on' . date('YmdHis');
    // $node->path = array('alias' => $path);

    if($node = node_submit($node)) { // Prepare node for saving
        node_save($node);
        drupal_set_message(t('Created example node "%node."', array('%node' => $node->title)));

    }
  }
}





function _build_blocks() {
  $blocks[0] = array('markaspot_log', 'markaspot_activity', 'sidebar_second', 'mas', 'node/*', '1');
  $blocks[1] = array('markaspot_logic', 'taxonomy_category', 'sidebar_second', 'mas', 'map', '1');
  $blocks[2] = array('markaspot_logic', 'taxonomy_status', 'sidebar_second', 'mas', 'map', '1');
  $blocks[3] = array('markaspot_logic', 'markaspot_map', 'sidebar_second', 'mas', 'map', '1');
  $blocks[4] = array('markaspot_unpubished', 'recent', 'sidebar_second', 'mas', '<front>', '1');
  $blocks[5] = array('search', 'form', 'sidebar_second', 'mas', 'map', '1');
  $blocks[6] = array('system', 'navigation', 'sidebar_second', 'mas', '<front>', '1');
  $blocks[7] = array('user', 'login', 'sidebar_second', 'mas', '<front>', '1');
  $blocks[7] = array('user', 'login', 'sidebar_second', 'mas', '<front>', '1');
  $blocks[7] = array('user', 'login', 'sidebar_second', 'mas', '<front>', '1');
  $blocks[7] = array('user', 'login', 'sidebar_second', 'mas', '<front>', '1');
  $blocks[7] = array('user', 'login', 'sidebar_second', 'mas', '<front>', '1');

  foreach ($blocks as $block) {
    _activate_block($block[0],$block[1],$block[2],$block[3],$block[4],$block[5]);
  }


}

function _activate_block($module, $block, $region, $theme, $pages, $visibility) {
  drupal_set_message("Activating block $module:$block\n");
  db_merge('block')
  ->key(array('theme' => $theme, 'delta' => $block, 'module' => $module))
  ->fields(array(
    'region' => ($region == BLOCK_REGION_NONE ? '' : $region),
    'pages' => trim($pages),
    'status' => (int) ($region != BLOCK_REGION_NONE),
    'visibility' => $visibility,
  ))
  ->execute();
}

function _delete_dummies(){
  $taxonomies = taxonomy_get_tree(3, $parent = 0, $max_depth = 1, $load_entities = TRUE);

  foreach ($taxonomies as $term) {
  //print_r($term->name);
   if ($term->name == 'dummy') {
       // we can not delete via taxonomy_term_delete($term);
       db_delete('taxonomy_term_data')->condition('tid', $term->tid)->execute();
   }
  }
}

