/**
 * @file
 * Javascript for transforming geolocation address field.
 */


jQuery(document).ready(function(){
	$ = jQuery;

  // setting a located/unlocated report, 
  // assuming unchanged lat will be general

  $('#edit-field-common-und').click(function() {
    if ($('#edit-field-common-und:checked')) {
      $('#edit-field-geo-und-0-latitem > span').html(Drupal.settings.mas.markaspot_ini_lat);
      $('#edit-field-geo-und-0-lngitem > span').html(Drupal.settings.mas.markaspot_ini_lng);
      $('#edit-field-geo-und-0-address-field').val(Drupal.settings.mas.markaspot_address);
      $('#edit-field-address-und-0-value').val(Drupal.settings.mas.markaspot_address);
    } 
   });

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