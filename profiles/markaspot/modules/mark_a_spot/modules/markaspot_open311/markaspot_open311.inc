<?php
/**
 * @file
 * Several methods for mapping drupal entities and georeport spedification.
 */

/**
 * Process errors with http status codes.
 *
 * @param string $message
 *   The error message
 *
 * @param int $code
 *   The http status/error code
 */
function markaspot_open311_process_services_error($message, $code) {
  $error[] = array('code' => $code, 'description' => $message);
  services_error($message, $code, $error);
}

/**
 * Callback for showing service discovery.
 *
 * @return array
 *   $discovery
 *   The Service discovery for this endpoint
 */
function markaspot_open311_discovery() {
  global $base_url;

  $discovery['changeset'] = variable_get('markaspot_logic_open311_changeset');;
  $discovery['contact'] = variable_get('markaspot_logic_open311_contact');
  $discovery['key_service'] = variable_get('markaspot_logic_open311_key_service');
  $discovery['endpoints']['specification'] = "http://wiki.open311.org/GeoReport_v2";
  $discovery['endpoints']['endpoint']['url'] = $base_url . "/georeport/v2";
  $discovery['endpoints']['endpoint']['changeset'] = variable_get('markaspot_logic_open311_changeset');
  $discovery['endpoints']['endpoint']['type'] = variable_get('markaspot_logic_open311_type');;
  $discovery['endpoints']['endpoint']['formats'] = array('format' => array('text/xml', 'application/json'));

  return $discovery;
}

/**
 * Creating Requests.
 *
 * @param array $data
 *   Data posted via request
 *
 * @return array
 *   The service request response
 */
function markaspot_open311_requests_create($data) {

  $data = (object) $data;
  $service_request = array();
  // Could be more validation here:
  if (!isset($data->email)) {
    markaspot_open311_process_services_error(t('Please provide a valid email address.'), 400);
  }

  if (!isset($data->lat) || !isset($data->long)) {
    markaspot_open311_process_services_error(t('Missing position parameters lat and long'), 400);
  }
  else {
    $lat = $data->lat;
    $lon = $data->long;
    $nw = FALSE;
    $se = FALSE;
    $bbox = array(
      'nw' => array('lat' => variable_get('markaspot_logic_bbox_nw_lat'), 'lng' => variable_get('markaspot_logic_bbox_nw_lng')),
      'se' => array('lat' => variable_get('markaspot_logic_bbox_se_lat'), 'lng' => variable_get('markaspot_logic_bbox_se_lng')),
    );

    if (($bbox['nw']['lat'] >= $lat) && ($bbox['nw']['lng'] <= $lon)) {
      $nw = TRUE;
    }
    if (($bbox['se']['lat'] <= $lat) && ($bbox['se']['lng'] >= $lon)) {
      $se = TRUE;
    }

    if ($se == FALSE || $nw == FALSE) {
      markaspot_open311_process_services_error(t('Your position is not located in our range of activity'), 400);
    }
    if (!isset($data->service_code)) {
      markaspot_open311_process_services_error(t('Service_code wrong, missing or empty.'), 404);
    }

    // Check if submitted service code is in ServiceList:
    $services = markaspot_open311_services_index($args = NULL);
    $services_available = array();
    foreach ($services as $service) {
      $services_available[] = $service['service_code'];
    }

    if (!in_array($data->service_code, $services_available)) {
      markaspot_open311_process_services_error('Service_code was not found', 404);
    }

    // Map request to node object;
    $node = markaspot_open311_request_map_node($request_id = NULL, $data);
    // Then write to DB:
    markaspot_open311_write_request($node);

    // We have the node-id
    if (module_exists('markaspot_uuid')) {
      $service_request[0]['service_request_id'] = $node->uuid;
    }
    else {
      $service_request[0]['service_request_id'] = $node->id;
    }
    $service_request[0]['service_notice'] = "";
  }
  return $service_request;
}


/**
 * Update Requests.
 *
 * @param string $request_id
 *   the ID of the service request
 *
 * @param array $data
 *   Data posted via request
 *
 * @return array
 *   The service request response
 */
function markaspot_open311_requests_update($request_id, $data) {
  global $user;
  $data = (object) $data;
  if (!isset($data->service_code)) {
    return markaspot_open311_process_services_error(t('Wrong or missing service_code or malformed request.'), 404);
  }

  // Check if submitted service code is in ServiceList:
  $services = markaspot_open311_services_index($args = NULL);
  foreach ($services as $service) {
    $services_available[] = $service['service_code'];
  }
  if (!in_array($data->service_code, $services_available)) {
    return services_error('Service_code was not found', 404);
  }

  // map request to node object (with $uuid);
  $node = markaspot_open311_request_map_node($request_id, $data);

  // then write to DB (update this time)
  markaspot_open311_write_request($node);

  // we have the node-id
  $service_request[0]['service_request_id'] = $node->uuid;
  $service_request[0]['service_notice'] = "";
  return $service_request;
}

/**
 * Callback for retrieving a request resource.
 *
 * @param int $id
 *   The request id
 *
 * @param array $parameters
 *   The parameters passed
 *
 * @return array
 *   Service Request
 */
function markaspot_open311_requests_retrieve($request_id, $parameters) {
  /*
   * Added optional parameters achiving this:
   * lists.open311.org/groups/discuss/messages/topic/AaxmrRLUHToO7jCQdjAXy
   *
   * Using entity_load is slower than node_load()
   *
   * $entity = _uuid_services_entity_retrieve('node', $uuid);
   */

  if (module_exists('markaspot_uuid')) {
    // Get node->nid by uuid:
    $service_request_id = entity_get_id_by_uuid('node', array($request_id));

    if (!empty($service_request_id[$request_id])) {
      // Load node with translated nid:
      $node = node_load($service_request_id[$request_id]);
    }
    else {
      markaspot_open311_process_services_error(t('Service request not found'), 404);
    }
  }
  else {
    $node = node_load($request_id);
    $node->language = LANGUAGE_NONE;

  }

  if ($node == TRUE) {
    $node->language = LANGUAGE_NONE;
    $status = markaspot_open311_tax_map_status($node->field_status[$node->language][0]["tid"]);
  }
  $service_requests[] = markaspot_open311_node_map_request($node, $status, $parameters);
  return $service_requests;
}


/**
 * Provides a list of accepted 311 service requests.
 *
 * These request types can be unique to the city/jurisdiction.
 *
 * @param int $page
 *   The page of the service requests list
 *   (not implemented yet)
 *
 * @param array $parameters
 *   Additional parameters
 *
 * @return array
 *   $requests
 *   An array of all requests for this jurisdiction
 */
function markaspot_open311_requests_index($page, $parameters) {
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node')
        ->entityCondition('bundle', 'report')
        ->propertyCondition('status', 1)
        ->addMetaData('account', user_load(1));
  // Run the query as user 1.
  // Checking for a limit parameter:
  $limit = (isset($parameters['limit'])) ? $parameters['limit'] : NULL;
  if ($limit) {
    $query->pager($limit);
  }
  else {
    $query->pager(50);
  }

  // Checking for service_code and map the code with taxonomy terms:
  if (isset($parameters['service_code'])) {
    // Get the service of the current node:
    $tid = markaspot_open311_service_map_tax($parameters['service_code']);
    $query->fieldCondition('field_category', 'tid', $tid);
  }

  $search = (isset($parameters['query'])) ? $parameters['query'] : NULL;
  if ($search) {
    $query->fieldCondition('body', 'value', "%" . $search . "%", "LIKE");
  }

  // Checking for status-parameter and map the code with taxonomy terms:
  if (isset($parameters['status'])) {
    // Get the service of the current node:
    $tids = markaspot_open311_status_map_tax($parameters['status']);
    $query->fieldCondition('field_status', 'tid', $tids);
  }

  // start_date param or travel back to 1970
  $start_timestamp = (isset($parameters['start_date']) && $parameters['start_date'] != '') ? strtotime($parameters['start_date']) : strtotime('01-01-1970');
  $query->propertyCondition('created', $start_timestamp, '>=');

  // End_date param or create a timestamp now:
  $end_timestamp = (isset($parameters['end_date']) && $parameters['end_date'] != '') ? strtotime($parameters['end_date']) : time();
  $query->propertyCondition('created', $end_timestamp, '<=');
  $query->propertyOrderBy('created', $direction = 'DESC');

  $result = $query->execute();

  $requests = FALSE;

  if ($result) {
    $nodes = array_keys($result['node']);
    $nodes = node_load_multiple($nodes);

    // Now exploring nodes matching other parameters:
    foreach ($nodes as $node) {
      $node->language = LANGUAGE_NONE;

      $status = markaspot_open311_tax_map_status($node->field_status[$node->language][0]["tid"]);
      $requests[] = markaspot_open311_node_map_request($node, $status, $parameters);
    }
  }
  else {
    markaspot_open311_process_services_error(t('No service requests found'), 404);

  }

  return $requests;
}


/**
 * Provides a list of acceptable 311 service types.
 *
 * These request types can be unique to the city/jurisdiction.
 *
 * @param array $args
 *   Arguments available for method
 *
 * @return array
 *   $services, All available services
 */
function markaspot_open311_services_index($args) {
  $services = array();
  $taxonomies = taxonomy_get_tree(1, $parent = 0, $max_depth = 1, $load_entities = TRUE);
  foreach ($taxonomies as $taxonomy_term) {
    $services[] = markaspot_open311_tax_map_service($taxonomy_term);
  }
  return $services;
}


/**
 * Callback for retrieving a service defintions.
 *
 * @param string $service_code
 *   $tid = service_code_id
 *
 * @return array
 *   $service
 */
function markaspot_open311_services_retrieve($service_code) {
  $taxonomy_term = taxonomy_term_load($service_code);
  $service['service_definition'][] = markaspot_open311_tax_map_service($taxonomy_term);
  return $service;
}


/**
 * Mapping node to georeport request.
 *
 * (needed when creating via Method GET)
 *
 * @param object $node
 *   The node object.
 *
 * @param string $status
 *   The status of the request (open, closed)
 *
 * @param array $parameters
 *   Parameters
 *
 * @return object
 *   $request as single request
 */
function markaspot_open311_node_map_request($node, $status, $parameters) {
  if (module_exists('markaspot_uuid')) {
    $request['service_request_id'] = $node->uuid;
  }
  else {
    $request['service_request_id'] = $node->nid;
  }

  // Defining content of the status-field:
  $status = markaspot_open311_tax_map_status($node->field_status[$node->language][0]["tid"]);
  $request['status'] = $status;

  if (isset($node->field_statement[$node->language][0]['value'])) {
    $request['status_notes'] = $node->field_statement[$node->language][0]['value'];
  }
  $term['category'] = taxonomy_term_load($node->field_category[$node->language][0]["tid"]);

  if (isset($term['category'])) {
    $request['service_code'] = $term['category']->field_category_id[$node->language][0]['value'];
    $request['service_name'] = $term['category']->name;
  }
  if (isset($node->body[$node->language][0]['value'])) {
    $request['description'] = $node->body[$node->language][0]['value'];
  }
  $request['agency_responsible'] = NULL;
  $request['service_notice'] = NULL;
  $request['address_id'] = NULL;

  $request['requested_datetime'] = date('c', $node->created);
  $request['updated_datetime'] = date('c', $node->changed);

  // $request['expected_datetime'] = date('c',$node->changed);
  // Geo Information including address:
  if (isset($node->field_address[$node->language][0]['value'])) {
    $request['address'] = $node->field_address[$node->language][0]['value'];
  }
  $request['zipcode'] = NULL;

  $request['lat'] = floatval($node->field_geo[$node->language][0]['lat']);
  $request['long'] = floatval($node->field_geo[$node->language][0]['lng']);

  // Media Url:
  if (isset($node->field_image[$node->language])) {
    $image_uri = image_style_url("large", file_build_uri($node->field_image[$node->language][0]['filename']));
    $request['media_url'] = $image_uri;
  }

  // Non Open311 compliant fields can be added here:
  if (isset($parameters['extension'])) {
    $request['status_code'] = $node->field_status[$node->language][0]["tid"];
    $term['status'] = taxonomy_term_load($node->field_status[$node->language][0]["tid"]);
    $request['status_sub'] = isset($term['status']->name) ? $term['status']->name : '';

    // Check for permitted roles to consume protected fields (e-mail)
    global $user;
    $permitted_roles = array( 'administrator', 'City Admin', 'Management');

    if (array_intersect($permitted_roles, $user->roles)) {
      $request['email'] = $node->field_e_mail[$node->language][0]['value'];
    }
  }

  return $request;
}

/**
 * Mapping a georeport service request to drupal node.
 *
 * Needed when importing via method POST
 *
 * @param object $request
 *   The request object
 *
 * @return object
 *   The node object
 */
function markaspot_open311_request_map_node($request_id, $request) {
  global $user;
  /*
  * Added optional parameters achiving this:
  * lists.open311.org/groups/discuss/messages/topic/AaxmrRLUHToO7jCQdjAXy
  *
  * Using entity_load is slower than node_load()
  * $entity = _uuid_services_entity_retrieve('node', $uuid);
  */
  if (isset($request_id)) {
    if (module_exists('markaspot_uuid')) {
      // Get node->nid by uuid:
      $service_request_id = entity_get_id_by_uuid('node', array($request_id));

      if (!empty($service_request_id[$request_id])) {
        // Load node with translated nid:
        $node = node_load($service_request_id[$request_id]);
      }
      else {
        markaspot_open311_process_services_error(t('Service request not found'), 404);
      }
    }
    else {
      $node = node_load($request_id);
    }

    // Look for query params;
    $parameters = drupal_get_query_parameters();

    // Check for permitted roles to update protected fields (status_notes, status_sub).
    $permitted_roles = array( 'administrator', 'City Admin', 'Management');
    if (array_intersect($permitted_roles, $user->roles) && array_key_exists('api_key', $parameters) && isset($parameters['extension'])) {

      if($request->status_sub != ''){
        $status_tid = markaspot_open311_custom_status_map_tax($request->status_sub);
        $node->field_status[$node->language][0]['tid'] = $status_tid;
      }
      $node->field_notify_user[$node->language][0]['value'] = isset($request->notify) ? 1 : 0;

      // Update Statement field
      $node->field_statement[$node->language][0]['value'] = ($request->status_notes != '') ? $request->status_notes : '';
    } else {
      markaspot_open311_process_services_error(t('Missing or wrong API Key'), 403);
    }

    // Get the first status tid (of open, closed);
    //$status_tids = markaspot_open311_status_map_tax($request->status);
    //$node->field_status[$node->language][0]['tid'] = $status_tids[0];



  } else {

    $node = new stdClass();
    $node->language = LANGUAGE_NONE;

    $node->uid = $user->uid;
    $node->created = time();
    $node->is_new = TRUE;
    // All incoming requests get status "open"
    $node->field_status[$node->language][0]['tid'] = array_values(variable_get('markaspot_logic_open311_status_open_start', markaspot_logic_defaults('markaspot_logic_open311_status_open_start')));
  }

  // Modified date set to time of request.
  $node->modified = time();

  // Set status for published/unpublished.
  $node_options = variable_get('markaspot_logic_open311_node_options_status');
  $node->status = $node_options;

  // Mapping the requested service code to taxonomy id:
  $category_tid = markaspot_open311_service_map_tax($request->service_code);

  // Applying term-name to title and it's taxonomyId to field_category
  $node->field_category[$node->language][0]['tid'] = $category_tid;
  // Loading the taxonomy term by requested service_code

  $node->body[$node->language][0]['value'] = $request->description;
  $node->body[$node->language][0]['format'] = 'filtered_html';
  if (isset($request->address_string)) {
    $node->field_address[$node->language][0]['value'] = $request->address_string;
  }
  $node->field_geo[$node->language][0]['lat'] = $request->lat;
  $node->field_geo[$node->language][0]['lng'] = $request->long;

  $node->field_e_mail[$node->language][0]['value'] = isset($request->email) ? $request->email : '';

  // File Handling:
  if (isset($request->media_url) && strstr($request->media_url, "http")) {
    $managed = TRUE;
    // Whether or not to create a Drupal file record:
    // define('FILE_EXISTS_REPLACE', 1)
    $file = system_retrieve_file($request->media_url, 'public://', $managed, FILE_EXISTS_REPLACE);
    $node->field_image[$node->language] = array(
      0 => array(
        'fid' => $file->fid, 'display' => 1,
        'description' => isset($node->title) ? $node->title : '',
      ),
    );
  }

  $node->type = 'report';

  return $node;
}


/**
 * Mapping taxonomies to services.
 *
 * @param object $taxonomy_term
 *   The taxonomy term.
 *
 * @return object
 *   $service: The service object
 */
function markaspot_open311_tax_map_service($taxonomy_term) {
  // $result=field_view_field('taxonomy_term',$term,'field_desired_field');
  $lang = LANGUAGE_NONE;

  $service['service_code'] = $taxonomy_term->field_category_id[$lang][0]['value'];
  $service['service_name'] = $taxonomy_term->name;
  $service['metadata'] = "false";
  $service['type'] = 'realtime';
  $service['description'] = $taxonomy_term->description;
  if (isset($taxonomy_term->field_hash[$lang][0]['value'])){
    $service['keywords'] = $taxonomy_term->field_hash[$lang][0]['value'];
  } else {
    $service['keywords'] = "";
  }
  return $service;
}


/**
 * Mapping taxonomy to status.
 *
 * geoReport v2 has only open and closed status
 */
function markaspot_open311_tax_map_status($taxonomy_id) {
  // Mapping Status to Open311 Status (open/closed)
  $term['status'] = taxonomy_term_load($taxonomy_id);

  if (isset($term['status'])) {
    $status_open = array_values(variable_get('markaspot_logic_open311_status_open', markaspot_logic_defaults('markaspot_logic_open311_status_open')));

    if (in_array($term['status']->tid, $status_open)) {
      $status = 'open';
    }
    else {
      $status = 'closed';
    }
  }
  return $status;
}


/**
 * Mapping requested status to drupal taxonomy.
 *
 * @param string $status
 *   Open311 Service status (can be open, closed).
 *
 * @return array
 *   The array of status term ids
 */
function markaspot_open311_status_map_tax($status) {
  if ($status == "open") {
    $tids = array_values(variable_get('markaspot_logic_open311_status_open', markaspot_logic_defaults('markaspot_logic_open311_status_open')));
  }
  else {
    $tids = array_values(variable_get('markaspot_logic_open311_status_closed', markaspot_logic_defaults('markaspot_logic_open311_status_closed')));
  }

  return $tids;
}

/**
 * Mapping requested status to drupal taxonomy.
 *
 * @param string $status_sub
 *   Custom Service status (can be open, closed, or foreign translated term name).
 *
 * @return int
 *   The tid
 */
function markaspot_open311_custom_status_map_tax($status_sub) {
  $taxonomy = taxonomy_get_tree(2, $parent = 0, $max_depth = 1, $load_entities = TRUE);
  foreach ($taxonomy as $taxonomy_term) {
    if ($taxonomy_term->name == $status_sub)
      $tid = $taxonomy_term->tid;
  }
  if (isset($tid)){
    return $tid;
  } else {
    markaspot_open311_process_services_error(t('Missing or unknown status_sub'), 400);
  }
}


/**
 * Mapping requested service_code to drupal taxonomy.
 *
 * @param string $service_code
 *   Open311 Service code (can be Code0001)
 *
 * @return int
 *   The TaxonomyId
 */
function markaspot_open311_service_map_tax($service_code) {

  $query = new EntityFieldQuery();

  $query->entityCondition('entity_type', 'taxonomy_term')
        ->entityCondition('bundle', 'category')
        ->fieldCondition('field_category_id', 'value', $service_code, '=')
        ->addMetaData('account', user_load(1));

  $result = $query->execute();
  foreach ($result['taxonomy_term'] as $term) {
    $tid = $term->tid;
  }
  return $tid;
}
