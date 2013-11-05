/**
 * @file
 * Javascript for Goole Map Dynamic API Formatter javascript code.
 *
 * Based on Lukasz Klimek http://www.klimek.ws
 */

(function($) {

  Drupal.geolocationCloudmadeOsm = Drupal.geolocationCloudmadeOsm || {};
  Drupal.geolocationCloudmadeOsm.maps = Drupal.geolocationCloudmadeOsm.maps || {};
  Drupal.geolocationCloudmadeOsm.markers = Drupal.geolocationCloudmadeOsm.markers || {};

  Drupal.behaviors.geolocationCloudmadeOsmDynamicFormatter = {

    attach : function(context, settings) {

      var fields = settings.geolocationCloudmadeOsm.formatters;

      // Work on each map
      $.each(fields, function(instance, data) {
        var instanceSettings = data.settings;
        $.each(data.deltas, function(d, delta) {

          id = instance + "-" + d;
          // Only make this once ;)
          $("#geolocation-cloudmade-osm-dynamic-" + id).once('geolocation-cloudmade-osm-dynamic-formatter', function() {

            var map_type;
            var mapOptions;

            var lat = delta.lat;
            var lng = delta.lng;
            var latLng = new L.LatLng(lat, lng);
            // Create map
            Drupal.geolocationCloudmadeOsm.maps[id] = new L.Map(this).setView([lat,lng],14);
            var markerLocation = latLng;
            L.tileLayer('http://{s}.tile.cloudmade.com/'+ instanceSettings.cloudmade_api_key +'/'+  instanceSettings.map_style +'/256/{z}/{x}/{y}.png', {
            // L.tileLayer('http://{s}.tile.cloudmade.com/BC9A493B41014CAABB98F0471D759707/997/256/{z}/{x}/{y}.png', {
              attribution : 'Map data &copy; 2012 OpenStreetMap contributors, Imagery &copy; 2012 CloudMade',
              maxZoom: 18
            }).addTo(Drupal.geolocationCloudmadeOsm.maps[id]);
            L.Icon.Default.imagePath = instanceSettings.leafletImagePath;
            var marker = L.marker(markerLocation).addTo(Drupal.geolocationCloudmadeOsm.maps[id]);
          });
        });
      });
    }
  };
}(jQuery));
