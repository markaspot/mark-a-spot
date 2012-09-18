/**
 * @file
 * Javascript for transforming geolocation address field.
 */


jQuery(document).ready(function(){
	$ = jQuery;
  
  // setting a located/unlocated report, 
  // assuming unchanged lat will be general
  $('#report-node-form').submit(function(){

    init_lat = Drupal.settings.mas.markaspot_ini_lat;
    is_lat = $('#edit-field-geo-und-0-latitem > span').text();

    if (init_lat == is_lat){
      $('#edit-field-common-und').attr('checked','checked');
    } else {
      $('#edit-field-common-und').attr('checked',"");
    }
  })
});
