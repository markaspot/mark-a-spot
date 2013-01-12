<?php
/**
 * Callback for the georeport service discovery.
 *
 * @return array $discovery
 */


function _markaspot_open311_discovery(){
  
  // toDo:  make these key/value available via administration interface

  $discovery['changeset'] = "2012-11-05 00:18";
  $discovery['contact'] = "You can email or call for assistance ...";
  $discovery['key_service'] = "You can request a key here: not yet";
  $discovery['endpoints']['specification'] = "http://wiki.open311.org/GeoReport_v2";
  $discovery['endpoints']['endpoint']['url'] = "http://yourcity.com/georeport/";
  $discovery['endpoints']['endpoint']['changeset'] = "2012-11-05 00:18";
  $discovery['endpoints']['endpoint']['type'] = "production";
  $discovery['endpoints']['endpoint']['formats'] = array('format' => 'text/xml','format' =>'application/json');

  return $discovery;
}