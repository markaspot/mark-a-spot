<?php
// markaspot_open311.inc

/**
 * Callback for the georeport service discovery.
 *
 * @return array $discovery
 */


function _markaspot_open311_discovery(){
  
  // toDo:  make these key/value available via administration interface

  $discovery['discovery']['changeset'] = "2011-02-03 14:18";
  $discovery['discovery']['contact'] = "You can email or call for assistance api@mycity.org +1 (555) 555-5555";
  $discovery['discovery']['key_service'] = "You can request a key here: http://api.mycity.gov/api_key/request";
  $discovery['discovery']['endpoints']['specification'] = "http://wiki.open311.org/GeoReport_v2";
  $discovery['discovery']['endpoints']['endpoint']['url'] = "path to endpoint";
  $discovery['discovery']['endpoints']['endpoint']['changeset'] = "http://wiki.open311.org/GeoReport_v2";
  $discovery['discovery']['endpoints']['endpoint']['type'] = "test";
  $discovery['discovery']['endpoints']['endpoint']['formats'] = array('xml','json');

  return $discovery;
}


/**
 * Callback for creating request resources.
 *
 * @param object $data
 * @return object $request
 */
function _markaspot_open311_requests_create($data) {
  global $user;
  $data = (object) $data;

  if (!isset($data->service_code)) {
    return services_error(t('Service_code missing or empty.'), 406);
  } 
  if (!isset($data->lat) || !isset($data->long)) {
    return services_error(t('Missing position parameters [lat and long]'), 406);
  }

  // Check if submitted service code is in ServiceList:
  $service_codes = _markaspot_open311_services_index();

  foreach ($service_codes['services'] as $service) {
    $services_available[] = $service['service_code'];
  }
  if (!in_array($data->service_code, $services_available)) {
    return services_error('Service_code was not found', 404);
  }

  // map request to node object;
  $node = _request_map_node($data);
  
  // then write to DB
  markaspot_open311_write_request($node);
  $request->service_request_id = $node->nid;

  return $request;
  
}


/**
 * Callback for retrieving a request resource.
 *
 * @param int $nid
 * $nid = service_request_id
 * @return array $request
 * 
 */

function _markaspot_open311_requests_retrieve($nid) {
   $node = node_load($nid);
   $request['service_requests'][] = _node_map_request($node);
   return $request;

}


/**
 * Provides a list of accepted 311 service requests. 
 * These request types can be unique to the city/jurisdiction.
 * 
 * @return array $requests
 * an array of all requests for this jurisdiction
 */

function _markaspot_open311_requests_index($page, $parameters) {
  global $user;

  $query = db_select('node', 'n')->extend('PagerDefault')->extend('TableSort');

  if(count($parameters) >= 2) {
     // limit param
    $limit = $parameters['limit'];
    if ($limit) {
      $query->limit($limit);
    } else {
      $query->limit(50);
    }
    // start_date param or travel back to 1970
    $start_timestamp = ($parameters['start_date'] || $parameters['start_date'] != '') ? strtotime($parameters['start_date']) : strtotime('01-01-1970');

    // end_date param or create a timestamp now
    $end_timestamp = ($parameters['end_date'] || $parameters['end_date'] != '') ? strtotime($parameters['end_date']) : time();

    // create conditions with AND
    $anded = db_and();
    $anded
      ->condition('created', $start_timestamp, '>=')
      ->condition('created', $end_timestamp, '<=')
      ->condition('type', 'report');

    // add to query object  
    $query->condition($anded);
  }

  // build query
  $nids = $query
  ->fields('n', array('nid'))
  //->condition($anded)
  ->orderBy('created', 'DESC')//ORDER BY created
  ->addTag('node_access')
  ->execute()
  ->fetchCol();

  $nodes = node_load_multiple($nids);
  foreach ($nodes as $node) {
    // mapping node to request
    $requests['service_requests'][] = _node_map_request($node);
  
  }

  return $requests;
}


/**
 * Provides a list of acceptable 311 service types and their associated service codes. 
 * These request types can be unique to the city/jurisdiction.
 *
 * @return array $services
 * All available services
 */

function _markaspot_open311_services_index($args) {
  $taxonomies = taxonomy_get_tree(2, $parent = 0, $max_depth = 1, $load_entities = TRUE);
  
  foreach ($taxonomies as $taxonomy_term) {
    $services['services'][] = _tax_map_service($taxonomy_term);
   }
 
  return $services;
}


/**
 * Callback for retrieving a service code.
 *
 * @param int $nid
 * $tid = service_code_id
 * @return array $request
 * 
 */

function _markaspot_open311_services_retrieve($service_code) {
  var_dump($service_code);
  // needs more work

}

/**
 * Access callback for the request and service resources.
 *
 * @param string $op
 *  The operation that's going to be performed.
 * @param array $args
 *  The arguments that will be passed to the callback.
 * @return bool
 *  Whether access is given or not.
 */

function _markaspot_open311_access($op, $args) {
  global $user;
  $access = FALSE;

  switch ($op) {
    case 'view':
      $request = markaspot_open311_get_requests($args[0]);
      $access = user_access('Mark-a-Spot open311 resource view any request');
      $access = $access || $node->uid == $user->uid && user_access('Mark-a-Spot open311 resource view own nodes');
      break;
    // case 'update':
    //   $request = markaspot_open311_get_requests($args[0]->id);
    //   $access = user_access('Mark-a-Spot open311 resource edit any request');
    //   $access = $access || $node->uid == $user->uid && user_access('Mark-a-Spot open311 resource edit own nodes');
    //   break;
    // case 'delete':
    //   $request = markaspot_open311_get_requests($args[0]);
    //   $access = user_access('Mark-a-Spot open311 resource delete any request');
    //   $access = $access || $node->uid == $user->uid && user_access('Mark-a-Spot open311 resource delete own nodes');
    //   break;
  }
  $access = TRUE;
  
  return $access;
}


/**
 * Mapping node to georeport request.
 *
 * @param object $node
 *  The node object.
 * @return object $request
 *  single request
 */

function _node_map_request($node){
  $request['service_request_id'] = $node->nid;

  $term['category'] = field_view_field('node', $node, 'field_category');
  if ($term['category']){
    $request['service_code'] = $term['category']['#items'][0]['taxonomy_term']->field_category_id['und'][0]['value'];
    $request['service_name'] = $term['category']['#items'][0]['taxonomy_term']->name;
  }

  $term['status'] = field_view_field('node', $node, 'field_status');
  
  if ($term['status']){
    $request['status'] = $term['status']['#items'][0]['taxonomy_term']->name;
  }




  $request['description'] = $node->body['und'][0]['value'];

  // $request['agency_responsible'] = null;
  // $request['service_notice'] = null;
  // $request['expected_datetime'] = null;
  // $request['address_id'] = $null;

  $request['requested_datetime'] = date('c',$node->created);
  $request['updated_datetime'] = date('c',$node->changed);


  // Geo Information including address
  $request['address'] = $node->field_address['und'][0]['value'];

  $request['zipcode'] = $node->field_zip['und'][0]['value'];
  $request['lat'] = $node->field_geo['und'][0]['lat'];
  $request['long'] = $node->field_geo['und'][0]['lng'];

  // Media Url
  if($node->field_image){
    $image_uri = image_style_url("thumbnail", file_build_uri($node->field_image['und'][0]['filename']));
    $request['media_url'] = $image_uri;
  }
  return $request;
}

/**
 * Mapping a georeport service request to drupal node.
 *
 * @param object $request
 *  The request object.
 * @return object $node
 *  The node object
 */

function _request_map_node($request) {
  global $user;
  
  $node->uid = $user->uid;
  $node->created = time();
  $node->modified = time();

  // Loading the taxonomy term by requested service_code
  $service_code = taxonomy_term_load($request->service_code);

  $node->language = 'und'; // language - neutral
  
  // applying it to title and it's taxonomyId to field_category
  $node->title = $service_code->name;
  $node->field_category[$node->language][0]['tid'] = $service_code->tid;

  // All incoming requests get status "open"
  $node->field_status[$node->language][0]['tid'] = "1";

  $node->body[$node->language][0]['value'] = $request->description;
  $node->body[$node->language][0]['format']  = 'filtered_html';

  $node->field_address[$node->language][0]['value'] = $request->adress;
  $node->field_zip[$node->language][0]['value'] =  $request->zipcode;

  // $node->field_zip[$node->language][0]['value'] = variable_get('markaspot_zip');
  // Make Requests valid only in the municiple
  $node->field_city[$node->language][0]['value'] = variable_get('markaspot_city');

  $node->field_geo[$node->language][0]['lat'] = $request->lat;
  $node->field_geo[$node->language][0]['lng'] = $request->long;
  
  $node->field_e_mail[$node->language][0]['value'] = $request->email;

  $node->is_new = true;
  $node->type = 'report';
  
  return $node;
}


/**
 * Mapping taxonomies to services.
 *
 * @param object $taxonomy_term
 *  The taxonomy term.
 * @return object $service
 *  The service object
 */

function _tax_map_service($taxonomy_term) {
  // $result=field_view_field('taxonomy_term',$term,'field_desired_field');
  $service['service_code'] = $taxonomy_term->field_category_id['und'][0]['value'];
  $service['service_name'] = $taxonomy_term->name;
  $service['description'] = $taxonomy_term->description;

  return $service;
}
?>