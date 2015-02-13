<?
/**
 * @file
 * Contains Batch API callbacks used during installation.
 */

/**
 * BatchAPI callback. Create Status Terms.
 */
function _markaspot_create_status($operation, &$context) {

  // Get the vocabulary ID.
  $vid = db_query("SELECT vid FROM {taxonomy_vocabulary} WHERE machine_name = 'status'")->fetchField();

  // Define the terms, with description and color:
  $terms[0] = array(
    'Open',
    'This is just a description which should be replaced',
    '#FF0000',
    'pause',
    'markaspot-font',
  );
  $terms[1] = array(
    'In progress',
    'This is just a description which should be replaced',
    '#FFA500',
    'play',
    'markaspot-font',
  );
  $terms[3] = array(
    'Solved',
    'This is just a description which should be replaced',
    '#008000',
    'checkmark',
    'markaspot-font',
  );
  $terms[4] = array(
    'Archive',
    'This is just a description which should be replaced',
    '#5F9EA0',
    'drawer',
    'markaspot-font',
  );

  // You may customize those entries:
  $terms[5] = array(
    'dummy',
    'This is just a description which should be replaced',
    '#FFFFFF',
    '',
    '',
  );
  $terms[6] = array(
    'dummy',
    'This is just a description which should be replaced',
    '#FFFFFF',
    '',
    '',
  );
  $terms[7] = array(
    'dummy',
    'This is just a description which should be replaced',
    '#FFFFFF',
    '',
    '',
  );
  $terms[8] = array(
    'dummy',
    'This is just a description which should be replaced',
    '#FFFFFF',
    '',
    '',
  );
  $terms[9] = array(
    'dummy',
    'This is just a description which should be replaced',
    '#FFFFFF',
    '',
    '',
  );

  foreach ($terms as $parent) {
    // Create the parent term.
    $term['vid'] = $vid;
    $term['name'] = $parent[0];
    $term['description'] = $parent[1];
    $term['field_status_hex'][LANGUAGE_NONE][0]['rgb'] = $parent[2];
    $term['field_status_icon'][LANGUAGE_NONE][0]['icon'] = $parent[3];
    $term['field_status_icon'][LANGUAGE_NONE][0]['bundle'] = $parent[4];

    $status = taxonomy_term_save((object) $term);
  }
  $context['message'] = t('@operation', array('@operation' => $operation));

}

/**
 * BatchAPI callback. Create Services/Category Terms.
 */
function _markaspot_create_categories($operation, &$context) {
  $context['message'] = t('@operation', array('@operation' => $operation));

  // Get the vocabulary ID.
  $vid = db_query("SELECT vid FROM {taxonomy_vocabulary} WHERE machine_name = 'category'")->fetchField();

  // Define the terms.
  $terms[0] = array(
    'Abandoned Cars',
    'Abandoned, wrecked, dismantled, or inoperative cars on private property',
    '010',
    'abandonedcar, cars, wreckedcar, car',
    '#8B0000',
    'car',
    'markaspot-font',
  );
  $terms[1] = array(
    'Litter Basket Complaint',
    'Litter Basket Request or Complaint',
    '011',
    'litter, trash, garbage',
    '#5F9EA0',
    'trash',
    'markaspot-font',
  );
  $terms[2] = array(
    'Graffiti Report',
    'Report graffiti on a building you own.',
    '012',
    'graffiti, paintings',
    '#F5F5DC',
    'graffiti',
    'markaspot-font',
  );
  $terms[3] = array(
    'Building Construction Complaint',
    'Dangerous buildings and vacant property operations historically has been known for the demolition of dangerous buildings, but recent adjustments in service delivery are focusing on inventorying vacant structures',
    '013',
    'graffiti, demolition',
    '#006400',
    'office',
    'markaspot-font',
  );

  foreach ($terms as $parent) {
    // Create the parent term.
    $term['vid'] = $vid;

    $term['name'] = $parent[0];
    $term['description'] = $parent[1];
    $term['field_category_id'][LANGUAGE_NONE][0]['value'] = $parent[2];
    $term['field_hash'][LANGUAGE_NONE][0]['value'] = $parent[3];
    $term['field_category_hex'][LANGUAGE_NONE][0]['rgb'] = $parent[4];
    $term['field_category_icon'][LANGUAGE_NONE][0]['icon'] = $parent[5];
    $term['field_category_icon'][LANGUAGE_NONE][0]['bundle'] = $parent[6];

    $status = taxonomy_term_save((object) $term);

  }

}


/**
 * BatchAPI callback. Create Reports.
 */
function _markaspot_create_reports($operation, &$context) {

  $nodes[0] = array(
    'Garbage Collection',
    'Lorem Ipsum Lorem ipsum dolor sit amet, consectetur ing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam,  quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo',
    '50.8212596',
    '6.8961028',
    'Pingsdorfer Straße 88, 50321 Brühl',
    'holger@markaspot.org',
    'Litter Basket Complaint',
    'Open',
    'flickr_by_dancentury_garbage_collection_4585329947',
  );

  $nodes[1] = array(
    'Some graffiti',
    'Lorem Ipsum Lorem ipsum dolor sit amet, consectetur ing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam,  quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo',
    '50.81812812677597',
    '6.8905774494293155',
    'Am Ringofen 21, 50321 Brühl',
    'holger@markaspot.org',
    'Graffiti Report',
    'Open',
    'flickr_by_striatic_grafitti_133146861',
  );

  $nodes[2] = array(
    'Abandoned car',
    'Lorem Ipsum Lorem ipsum dolor sit amet, consectetur ing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam,  quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo',
    '50.82435019881909',
    '6.895512714016718',
    'Liblarer Straße 88, 50321, Brühl',
    'holger@markaspot.org',
    'Abandoned Cars',
    'Solved',
    'flickr_thomasbrandt',
  );

  $nodes[3] = array(
    'Danger at subway construction',
    'Lorem Ipsum Lorem ipsum dolor sit amet, consectetur ing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam,  quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo',
    '50.8282131596655',
    '6.90819419823913',
    'Promenade, 50321 Brühl',
    'holger@markaspot.org',
    'Building Construction Complaint',
    'In progress',
    'flickr_holger_baustellenlage_ebertplatz',
  );

  $nodes[4] = array(
    'Really Abandoned car',
    'Lorem Ipsum Lorem ipsum dolor sit amet, consectetur ing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam,  quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo',
    '50.8327211',
    '6.9032226',
    'Friedrichstraße 23 50321 Brühl',
    'holger@markaspot.org',
    'Abandoned Cars',
    'Archive',
    'flickr_by_mikebaird_abandoned_car_4585329947',
  );

  $nodes[5] = array(
    'Garbage collection',
    'Lorem Ipsum Lorem ipsum dolor sit amet, consectetur ing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam,  quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo',
    '50.826873',
    ' 6.900167',
    'Centre, 50321 Brühl',
    'holger@markaspot.org',
    'Litter Basket Complaint',
    'Solved',
    'flickr_by_realname_garbage-tonal-decay',
  );

  foreach ($nodes as $node_data) {
    // Create a new node object:
    $node = new stdClass();
    // Or page, or whatever content type you like:
    $node->type = "report";

    node_object_prepare($node);

    $nid = db_query("SELECT nid FROM {node} ORDER BY nid DESC LIMIT 1")->fetchField();
    $nid++;

    $node->title = $node_data[0];
    $node->language = LANGUAGE_NONE;

    $node->uid = 1;
    $node->nid = $nid;
    $node->language = LANGUAGE_NONE;
    $node->body[$node->language][0]['value'] = $node_data[1];
    $node->body[$node->language][0]['format'] = 'filtered_html';
    $node->field_geo[$node->language][0]['lat'] = $node_data[2];
    $node->field_geo[$node->language][0]['lng'] = $node_data[3];
    $node->field_address[$node->language][0]['value'] = $node_data[4];
    $node->field_e_mail[$node->language][0]['value'] = $node_data[5];
    $node->field_category[$node->language][0]['tid'] = markaspot_get_term_id($node_data[6]);
    $node->field_status[$node->language][0]['tid'] = markaspot_get_term_id($node_data[7]);
    $node->is_new = TRUE;
    $node->promote = 0;
    $filename = 'image_' . $node_data[8] . '.jpg';

    $image = file_get_contents('profiles/markaspot/themes/mas/images/' . $node_data[8] . '.jpg');
    $file = file_save_data($image, 'public://' . $filename, FILE_EXISTS_RENAME);
    $node->field_image = array(LANGUAGE_NONE => array('0' => (array) $file));

    if ($node = node_submit($node)) {
      node_save($node);
      // drupal_set_message(t('Created example node "%node."', array('%node' => $node->title)));
      if ($node->nid == 1) {
        variable_set('node_uuid', $node->uuid);
      }
    }
  }
  $context['message'] = t('@operation', array('@operation' => $operation));

}

/**
 * BatchAPI callback. Create Specification and About Pages.
 */
function _markaspot_create_pages($operation, &$context) {
  global $base_url;
  // Now creating articles:
  $nodes[0] = array(
    'Open311',
    '<p> <img src="/profiles/markaspot/themes/mas/images/open311georeportv2-type1.png" alt="Open311 Logo" style="width:300px; margin: 0 0 0 10px; height:114px;float:right"> Open311 is a form of technology that provides open channels of communication for issues that concern public space and public services. Primarily, Open311 refers to a standardized protocol for location-based collaborative issue-tracking. </p> <p> By offering free web API access to an existing 311 service, Open311 is an evolution of the phone-based 311 systems that many cities in North America offer. [taken from <a href="http://open311.org/learn/"> Open311 Learn </a>] </p> <p> Mark-a-Spot comes with a built-in GeoReport Server. See how it works: </p> <h3>Services</h3> <table class="spec-table"> <tbody> <tr> <th scope="row"> Purpose </th> <td> Provide a list of acceptable 311 service request types and their associated service codes. These request types can be unique to the city/jurisdiction. </td> </tr> <tr> <th scope="row"> URL </th> <td><a href="' . $base_url . '/georeport/v2/services.xml">' . $base_url . '/georeport/v2/services.[format]</a></td> </tr> <tr> <th scope="row"> Sample URL </th> <td> <a href="' . $base_url . '/georeport/v2/services.xml"> ' . $base_url . '/georeport/v2/services.xml </a> </td> </tr> <tr> <th scope="row"> Formats </th> <td> XML / JSON </td> </tr> <tr> <th scope="row"> HTTP Method </th> <td> GET </td> </tr> <tr> <th scope="row"> Requires API Key </th> <td> No </td> </tr> </tbody> </table> <h3> Requests </h3> <table class="spec-table"> <tbody> <tr> <th scope="row"> Purpose </th> <td> Query the current status of multiple requests </td> </tr> <tr> <th scope="row"> URL </th> <td> <a href="' . $base_url . '/georeport/v2/requests.json"> ' . $base_url . '/georeport/v2/requests.[format] </a> </td> </tr> <tr> <th scope="row"> Sample URL </th> <td> <a href="' . $base_url . '/georeport/v2/requests.json?start_date=' . date("Y-n-j") . '"> ' . $base_url . '/georeport/v2/requests.json?start_date=' . date("Y-n-j") . '... </a> </td> </tr> <tr> <th scope="row"> Formats </th> <td> XML, JSON </td> </tr> <tr> <th scope="row"> HTTP Method </th> <td> GET </td> </tr> <tr> <th scope="row"> Requires API Key </th> <td> No </td> </tr> </tbody> </table> <h4> Optionale Parameter </h4> <table class="spec-table arguments-table"> <tbody> <tr> <th> Field Name </th> <th class="spec-description"> Description </th> <th class="spec-requirements"> Notes &amp; Requirements </th> </tr> <tr> <td class="field"> <dl> <dt> service_code <br/> </dt> </dl> </td> <td> Specify the service type by calling the unique ID of the service_code. </td> <td> This defaults to all service codes when not declared; can be declared multiple times, comma delimited </td> </tr> <tr> <td class="field"> <dl> <dt> start_date <br/> </dt> </dl> </td> <td> Earliest datetime to include in search. When provided with end_date, allows one to search for requests which have a requested_datetime that matches a given range, but may not span more than 90 days. </td> <td> When not specified, the range defaults to most recent 90 days. Must use w3 format, eg 2010-01-01T00:00:00Z. </td> </tr> <tr> <td class="field"> <dl> <dt> end_date <br/> </dt> </dl> </td> <td> Latest datetime to include in search. When provided with start_date, allows one to search for requests which have a requested_datetime that matches a given range, but may not span more than 90 days. </td> <td> When not specified, the range defaults to most recent 90 days. Must use w3 format, eg 2010-01-01T00:00:00Z. </td> </tr> <tr> <td class="field"> <dl> <dt> status <br/> </dt> </dl> </td> <td> Allows one to search for requests which have a specific status. This defaults to all statuses; can be declared multiple times, comma delimited; </td> <td> Options: <span class="strict-value"> open </span> , <span class="strict-value"> closed </span> </td> </tr> </tbody> </table> <h3> Single Request </h3> <table class="spec-table"> <tbody> <tr> <th scope="row"> Purpose </th> <td> Query the current status of an individual request </td> </tr> <tr> <th scope="row"> URL </th> <td> <a href="' . $base_url . '/georeport/v2/requests/2.xml"> ' . $base_url . '/georeport/v2/request/#ID.[format] </a> </td> </tr> <tr> <th scope="row"> Sample URL </th> <td><a href="' . $base_url . '/georeport/v2/requests/' . variable_get('node_uuid') . '.json">' . $base_url . '/georeport/v2/requests/' . variable_get('node_uuid') . '.json</a></td> </tr> <tr> <th scope="row"> Formats </th> <td> XML, JSON </td> </tr> <tr> <th scope="row"> HTTP Method </th> <td> GET </td> </tr> <tr> <th scope="row"> Requires API Key </th> <td> No </td> </tr> </tbody> </table>',
    'slider_2',
  );

  // Now creating initial report:
  $nodes[1] = array(
    'About this platform',
    '<p>This platform is based on <a href="http://markaspot.de/en">Mark-a-Spot</a>, a Drupal distribution for public issue reporting or other geobased crowdsourcing.</p> <p>This text can be easily changed and <a href="?q=node/8/edit">customized</a>. If you feel like giving attribution, please leave a link to <a href="http://mark-a-spot.org">mark-a-spot.org</a> somewhere.</p> <p>If you need any help with customizing this platform, like theming, extending or even integrating it in your SOA infrastructure, please <a href="http://markaspot.de/en/contact">get in touch with us</a>.</p> <h3>Photo credits</h3> <ul> <li>Garbage Collection by Dan Century (Flickr)</li> <li>Garbage Collection by Tonal Decay (Flickr)</li> <li>Abandoned Car by Mike Baird (Flickr)</li> <li>Abandoned Car by Thomas Brandt (Flickr)</li> <li>Graffiti by striatic (Flickr)</li> <li>River Tree by Astonishing / photocase.com</li> </ul>',
    'slider_1',
  );

  foreach ($nodes as $node_data) {
    $node = new stdClass();
    $node->type = "page";
    node_object_prepare($node);
    $nid = db_query("SELECT nid FROM {node} ORDER BY nid DESC LIMIT 1")->fetchField();
    $nid++;

    $node->title = $node_data[0];
    $node->language = LANGUAGE_NONE;
    $node->uid = 1;
    $node->nid = $nid;
    $node->language = LANGUAGE_NONE;
    $node->body[$node->language][0]['value'] = $node_data[1];
    $node->body[$node->language][0]['format'] = 'full_html';

    $node->is_new = TRUE;
    $node->promote = 0;
    $filename = 'image_' . $node_data[2] . '.jpg';

    $image = file_get_contents('profiles/markaspot/themes/mas/images/' . $node_data[2] . '.jpg');
    $file = file_save_data($image, 'public://' . $filename, FILE_EXISTS_RENAME);
    $node->field_image = array(LANGUAGE_NONE => array('0' => (array) $file));

    if ($node = node_submit($node)) {
      node_save($node);
      // drupal_set_message(t('Created Page node "%node."', array('%node' => $node->title)));

    }
  }
  $context['message'] = t('@operation', array('@operation' => $operation));

}

/**
 * BatchAPI callback. Building Blocks with enabled modules.
 */
function _markaspot_build_blocks($operation, &$context) {
  _markaspot_activate_block('views', 'report_log-report_log', 'content', 'mas', 'node/*', '1', '5');
  _markaspot_activate_block('markaspot_logic', 'taxonomy_category', 'footer', 'mas', 'map', '1', '0');
  _markaspot_activate_block('markaspot_logic', 'taxonomy_status', 'footer', 'mas', 'map', '1', '0');
  _markaspot_activate_block('markaspot_logic', 'markaspot_map', 'sidebar_second', 'mas', 'map', '1', '0');
  _markaspot_activate_block('system', 'navigation', 'sidebar_second', 'mas', '<front>' . "\n" . 'node/7' . "\n" . 'node/8', '1', '0');
  _markaspot_activate_block('menu', 'menu-secondary-navigation', 'footer', 'mas', 'admin' . "\n" . 'admin/*', '0', '0');
  _markaspot_activate_block('user', 'login', 'sidebar_second', 'mas', '<front>' . "\n" . 'node/7' . "\n" . 'node/8', '1', '0');
  _markaspot_activate_block('markaspot_default_content', 'welcome', 'content', 'mas', '<front>', '1', '0');
  _markaspot_activate_block('markaspot_default_content', 'search_id', 'sidebar_second', 'mas', '<front>' . "\n" . 'node/7' . "\n" . 'node/8', '1', '0');
  _markaspot_activate_block('markaspot_stats', 'markaspot_stats', 'sidebar_second', 'mas', '<front>' . "\n" . 'node/7' . "\n" . 'node/8', '1', '0');
  _markaspot_activate_block('comment', 'recent', 'sidebar_second', 'mas', '<front>', '1', '0');
  _markaspot_activate_block('user', 'new', 'sidebar_second', 'mas', '<front>', '1', '0');
  $context['message'] = t('@operation', array('@operation' => $operation));

}
/**
 * BatchAPI callback. Activate blocks.
 */
function _markaspot_activate_block($module, $block, $region, $theme, $pages, $visibility, $weight) {
  // drupal_set_message("Activating block $module:$block\n");
  db_merge('block')
    ->key(array('theme' => $theme, 'delta' => $block, 'module' => $module))
    ->fields(array(
      'region' => ($region == BLOCK_REGION_NONE ? '' : $region),
      'pages' => $pages,
      'status' => (int) ($region != BLOCK_REGION_NONE),
      'visibility' => $visibility,
      'weight' => $weight,
    ))
    ->execute();
  if($block == "report_log-report_log"){
    $query = db_insert('block_node_type')->fields(array('type', 'module', 'delta'));
    $query->values(
      array(
        'type' => 'report',
        'module' => 'views',
        'delta' => 'report_log-report_log',
      )
    );
    $query->execute();
  }
}
/**
 * BatchAPI callback. Delete dummies after installation.
 *
 */
function _markaspot_delete_dummies($operation, &$context) {
  // Get the vocabulary ID.
  $vid = db_query("SELECT vid FROM {taxonomy_vocabulary} WHERE machine_name = 'status'")->fetchField();
  $taxonomies = taxonomy_get_tree($vid, $parent = 0, $max_depth = 1, $load_entities = TRUE);

  foreach ($taxonomies as $term) {
    if ($term->name == 'dummy') {
      taxonomy_term_delete($term->tid);
    }
  }
  $context['message'] = t('@operation', array('@operation' => $operation));
}

/**
 * BatchAPI callback. Enable module callback
 *
 */
function _markaspot_enable_module($module, $module_name, &$context) {
  // Increase maximum function nesting level to prevent install errors.
  $max_nesting_level = ini_get('xdebug.max_nesting_level');
  if ($max_nesting_level > 0 && $max_nesting_level <= '200') {
    ini_set('xdebug.max_nesting_level', 200);
  }
  ini_set('max_execution_time', '300');
  module_enable(array($module), FALSE);
  $context['message'] = st('Installed %module module.', array('%module' => $module_name));
}

/**
 * BatchAPI callback. Flash all cashes.
 *
 */
function _markaspot_flush_cashes(&$context) {
  // Clear the caches in case the data has been updated.
  drupal_flush_all_caches();;
  $context['message'] = st('Flushed cashes.');
}
