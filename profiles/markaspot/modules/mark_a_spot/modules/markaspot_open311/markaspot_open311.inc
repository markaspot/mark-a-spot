<?php
// markaspot_open311.inc

/**
 * Callback for the georeport service discovery.
 *
 * @return array $discovery
 */


function _markaspot_open311_discovery(){
  
  // toDo:  make these key/value available via administration interface

  $discovery['changeset'] = "2012-11-05 00:18";
  $discovery['contact'] = "You can email or call for assistance app@bonn.de +49 (0) 228-773699";
  $discovery['key_service'] = "You can request a key here: not yet";
  $discovery['endpoints']['specification'] = "http://wiki.open311.org/GeoReport_v2";
  $discovery['endpoints']['endpoint']['url'] = "http://anliegen.bonn.de/georeport/v2";
  $discovery['endpoints']['endpoint']['changeset'] = "2012-11-05 00:18";
  $discovery['endpoints']['endpoint']['type'] = "production";
  $discovery['endpoints']['endpoint']['formats'] = array('text/xml','application/json');

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

function _markaspot_open311_requests_retrieve($nid, $parameters) {
   // added optional parameters achiving this:
   // http://lists.open311.org/groups/discuss/messages/topic/AaxmrRLUHToO7jCQdjAXy
   $node = node_load($nid);
   $request['service_requests'][] = _node_map_request($node, $status=false, $parameters);
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

  $query = db_select('node', 'n');
  $query = $query->extend('PagerDefault')->extend('TableSort');

  if(count($parameters) >= 1) {
     // var_dump($parameters);
     // limit param
    $limit = (isset($parameters['limit'])) ? $parameters['limit']:null;
    if ($limit) {
      $query->limit($limit);
    } else {
      $query->limit(50);
    }
    // start_date param or travel back to 1970
    $start_timestamp = (isset($parameters['start_date']) && $parameters['start_date'] != '') ? strtotime($parameters['start_date']) : strtotime('01-01-1970');

    // end_date param or create a timestamp now
    $end_timestamp = (isset($parameters['end_date']) && $parameters['end_date'] != '') ? strtotime($parameters['end_date']) : time();

    // create conditions with AND
    $anded = db_and();

    $anded
      ->condition('created', $start_timestamp, '>=')
      ->condition('created', $end_timestamp, '<=');
      // ->condition('type', 'report');

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
  $requests = false;

  // now exploring nodes matching other parameters
  foreach ($nodes as $node) {
    // mapping node to request
    if ($node->type == 'report') {
      $found = false;

      // Mapping Category to Service-Code (only to filter)
      $term['category'] = taxonomy_term_load($node->field_category['und'][0]["tid"]);
      // echo $node->title." ".$node->field_category['und'][0]["tid"];
      if (isset($parameters['service_code'])){

        // comma separated service_code parameters to array
        $service_codes_filtered = explode(',', $parameters['service_code']);
        
        // get the service of the current node
        $service = $term['category']->field_category_id['und'][0]['value'];
        // var_dump($service_codes_filtered);
        foreach ($service_codes_filtered as $service_code) {
          if ($service_code == $service) {
            $found['service_code'] = true;
          }
        }

      }
      // Map taxonomy to status (solved = closed, archive = closed)
      $status = _tax_map_status($node->field_status['und'][0]["tid"]);

      if (isset($parameters['status'])) {
        if ($parameters['status'] == $status) {
            $found['status'] = true;
        }

        // now mapping nodes to request
        if (isset($parameters['service_code'])) {
          if (isset($found['service_code']) && isset($found['status'])) {
            $requests[] = _node_map_request($node, $status, $parameters);
          }
        }
        if (!isset($parameters['service_code'])) {
          if (isset($found['status'])) {
            $requests[] = _node_map_request($node, $status, $parameters);
          }
        }
      }
      if (!isset($parameters['status']) && !isset($parameters['service_code'])) {
        $requests[] = _node_map_request($node, $status, $parameters);
      }
    }
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
    $services[] = _tax_map_service($taxonomy_term);
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
  $taxonomy_term =  taxonomy_term_load($service_code);
  $service['service_definition'][] = _tax_map_service($taxonomy_term);
  return $service;
}



/**
 * Mapping node to georeport request.
 * (needed when creating via Method GET)
 *
 * @param object $node
 *  The node object.
 * @return object $request
 *  single request
 */

function _node_map_request($node, $status, $parameters){
  //print_r($node,$parameters);
  $request['service_request_id'] = $node->nid;
 
  $term['category'] = taxonomy_term_load($node->field_category['und'][0]["tid"]);
  
  // defining content of the status-field
  $status = _tax_map_status($node->field_status['und'][0]["tid"]);

  if (isset($term['category'])) {
    $request['service_code'] = $term['category']->field_category_id['und'][0]['value'];
    $request['service_name'] = $term['category']->name;
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

  $request['lat'] = $node->field_geo['und'][0]['lat'];
  $request['long'] = $node->field_geo['und'][0]['lng'];
  $request['status'] = $status;

  // Media Url
  if(isset($node->field_image['und'])){
    $image_uri = image_style_url("thumbnail", file_build_uri($node->field_image['und'][0]['filename']));
    $request['media_url'] = $image_uri;
  }

  // Non Open311 compliant fields can be added here
  if(isset($parameters['extension'])) {
    $request['status_code'] = $node->field_status['und'][0]["tid"];
    $term['status_sub'] = taxonomy_term_load($node->field_status['und'][0]["tid"]);
    $request['status_sub'] = $term['status_sub']->name;

  }

  return $request;
}

/**
 * Mapping a georeport service request to drupal node.
 * needed when importing via method POST
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
  
  // mapping the requested service code to taxonomy id
  $node->field_category[$node->language][0]['tid'] = _service_map_tax($request->service_code);

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

  // File Handling 

  if ($request->media_url != ""){
    $managed = TRUE; // Whether or not to create a Drupal file record
    // define('FILE_EXISTS_REPLACE', 1)
    $file = system_retrieve_file($request->media_url, 'public://', $managed, FILE_EXISTS_REPLACE);
    $node->field_image[$node->language] = array(
      0 => array('fid' => $file->fid, 'display' => 1, 'description' => $service_code->name)
    );
  }


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
  $service['metadata'] = "false";
  $service['type'] = 'realtime';
  $service['description'] = $taxonomy_term->description;
  $service['keywords'] = $taxonomy_term->field_hash['und'][0]['value'];

  return $service;
}


/**
* Mapping taxonomy to status
* geoReport v2 has only open and closed status
*
*/
function _tax_map_status($taxonomyId) {
  // Mapping Status to Open311 Status (open/closed)
  $term['status'] = taxonomy_term_load($taxonomyId);
  if (isset($term['status'])) {
    // declare all taxonomy-ids for status "open" 
    // (could be "acknowledged", "edited", "in progress")
    $status_open = array(1, 2);

    if (in_array($term['status']->tid, $status_open)) {
      $status = 'open';
    } else {
      $status = 'closed';
    }
  }
  //print_r($status);
  return $status;
}

/**
 * Mapping requested service_code to drupal taxonomy.
 *
 * @param string $service_code
 *  Open311 Service code (can be Code0001).
 * @return int $tid
 *  The TaxonomyId
 */

function _service_map_tax($service_code) {

  $query = new EntityFieldQuery();

  $query->entityCondition('entity_type', 'taxonomy_term')
    ->entityCondition('bundle', 'category')
    ->fieldCondition('field_category_id', 'value', $service_code, '=')
    ->addMetaData('account', user_load(1)); // Run the query as user 1.

  $result = $query->execute();
  foreach ($result['taxonomy_term'] as $term) {
    $tid = $term->tid;
  }
  return $tid;
}
?>