/**
 * @file
 * Javascript for Goole Map Dynamic API Formatter javascript code.
 *
 * Based on Lukasz Klimek http://www.klimek.ws
 */

(function($) {

  Drupal.geolocationosmOsm = Drupal.geolocationosmOsm || {};
  Drupal.geolocationosmOsm.maps = Drupal.geolocationosmOsm.maps || {};
  Drupal.geolocationosmOsm.markers = Drupal.geolocationosmOsm.markers || {};

  Drupal.behaviors.geolocationosmOsmDynamicFormatter = {

    attach : function(context, settings) {

      var fields = settings.geolocationosmOsm.formatters;

      // Work on each map
      $.each(fields, function(instance, data) {
        var instanceSettings = data.settings;
        $.each(data.deltas, function(d, delta) {

          id = instance + "-" + d;
          // Only make this once ;)
          $("#geolocation-osm-osm-dynamic-" + id).once('geolocation-osm-osm-dynamic-formatter', function() {

            var map_type;
            var mapOptions;

            var lat = delta.lat;
            var lng = delta.lng;
            var latLng = new L.LatLng(lat, lng);
            // Create map
            Drupal.geolocationosmOsm.maps[id] = new L.Map(this).setView([lat,lng],14);
            var markerLocation = latLng;
            L.tileLayer(instanceSettings.tile_server, {
              attribution : 'Map data &copy; 2012 OpenStreetMap contributors, Imagery &copy; 2012 osm',
              maxZoom: 18
            }).addTo(Drupal.geolocationosmOsm.maps[id]);
            L.Icon.Default.imagePath = instanceSettings.leafletImagePath;
            var marker = L.marker(markerLocation).addTo(Drupal.geolocationosmOsm.maps[id]);
          });
        });
      });
    }
  };
}(jQuery));
