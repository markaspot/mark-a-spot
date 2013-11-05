/**
 * @file
 * Javascript for Goole Map Dynamic API Formatter javascript code.
 * 
 * Many thanks to Lukasz Klimek http://www.klimek.ws for the help
 */

(function($) {

  Drupal.geolocationGooglemaps = Drupal.geolocationGooglemaps || {};
  Drupal.geolocationGooglemaps.maps = Drupal.geolocationGooglemaps.maps || {};
  Drupal.geolocationGooglemaps.markers = Drupal.geolocationGooglemaps.markers || {};

  Drupal.behaviors.geolocationGooglemapsDynamicFormatter = {

    attach : function(context, settings) {

      var fields = settings.geolocationGooglemaps.formatters;

      // Work on each map
      $.each(fields, function(instance, data) {
        var instanceSettings = data.settings;
        $.each(data.deltas, function(d, delta) {

          id = instance + "-" + d;

          // Only make this once ;)
          $("#geolocation-googlemaps-dynamic-" + id).once('geolocation-googlemaps-dynamic-formatter', function() {

            var map_type;
            var mapOptions;

            var lat = delta.lat;
            var lng = delta.lng;
            var latLng = new google.maps.LatLng(lat, lng);

            switch (instanceSettings.map_maptype) {
              case 'satellite':
                map_type = google.maps.MapTypeId.SATELLITE;
                break;

              case 'terrain':
                map_type = google.maps.MapTypeId.TERRAIN;
                break;

              case 'hybrid':
                map_type = google.maps.MapTypeId.HYBRID;
                break;

              default:
                map_type = google.maps.MapTypeId.ROADMAP;
                break;
            }

            mapOptions = {
              zoom : parseInt(instanceSettings.map_zoomlevel),
              center : latLng,
              mapTypeId : map_type,
              scrollwheel : instanceSettings.map_scrollwheel
            };

            // Create map
            Drupal.geolocationGooglemaps.maps[id] = new google.maps.Map(this, mapOptions);

            // Create and place marker
            Drupal.geolocationGooglemaps.markers[id] = new google.maps.Marker({
              map : Drupal.geolocationGooglemaps.maps[id],
              draggable : false,
              icon : instanceSettings.marker_icon,
              position : latLng
            });
          });
        });
      });
    }
  };
}(jQuery));
