/**
 * @file markaspot_radar.js
 *
 * Provides some more leaflet layers to check reports
 */

(function ($) {
  Drupal.behaviors.markaspotRadar = {

    attach: function (context, settings) {
      Drupal.geolocation.settings = Drupal.geolocation.settings || {};
      Drupal.geolocation = Drupal.geolocation || {};

      $.each(Drupal.settings.geolocation.defaults, function (i, mapDefaults) {
        var map = Drupal.geolocation.maps[i];

        // Initial Loading
        Drupal.markaspotRadar.addReports(map);

        // Load GeoJSON on ZoomEnd and DragEnd
        map.on('zoomend', function (e) {
          Drupal.markaspotRadar.addReports(map);
        });

        map.on('dragend', function (e) {
          Drupal.markaspotRadar.addReports(map);
        });


      });

    }
  };
  Drupal.markaspotRadar = {
    clearMap: function (m) {
      for(i in m._layers) {
        if(m._layers[i]._path != undefined) {
          try {
            m.removeLayer(m._layers[i]);
          }
          catch(e) {
            console.log("problem with " + e + m._layers[i]);
          }
        }
      }
    },
    addReports: function (map) {
      currentBounds = map.getBounds();
      // Only get new geoJSON on certain zoom level
      if (map.getZoom() >= Drupal.settings.mas.markaspot_radar_zoom) {
        Drupal.markaspotRadar.clearMap(map);
        var reportMarkers = Drupal.markaspotRadar.geoJsonLoad(currentBounds);
        var reportMarkerLayer = L.layerGroup(reportMarkers);
        reportMarkerLayer.addTo(map);

      } else {
        Drupal.markaspotRadar.clearMap(map);
      }
    },
    geoJsonLoad: function (currentBounds) {

      nwLat = currentBounds.getNorthWest().lat;
      nwLng = currentBounds.getNorthWest().lng;
      seLat = currentBounds.getSouthEast().lat;
      seLng = currentBounds.getSouthEast().lng;

      url = Drupal.settings.basePath + 'reports/geojson/map?' + 'nwLat=' + nwLat + '&nwLng=' + nwLng + '&seLat=' + seLat + '&seLng=' + seLng;
      var bounds = new L.LatLngBounds();
      $.ajaxSetup({
        async: false
      });
      var reportMarkers = [];
      $.ajax({
        url: url,
        async: false,
        dataType: 'json',
        success: function (data) {
          $.each(data.features, function (key, item) {
            var latlon = new L.LatLng(item.geometry.coordinates[0], item.geometry.coordinates[1]);
            item = item.properties;

            var html = '<div class="marker-title"><h4><a class="infowindow-link" href="' + item.path + '">' + item.name + '</a></h4><span class="meta-info date">' + item.created + '</span></div>';
            if (item.field_address) {
              html += '<div class="marker-address"><p>' + item.field_address + '</p></div>';
              html += '<div><a class="infowindow-link" href="' + item.path + '">' + Drupal.t('read more') + '</a></div>';
            }
            colors = [
              {"color": "red", "hex": "#FF0000"},
              {"color": "darkred", "hex": "#8B0000" },
              {"color": "orange", "hex": "#FFA500" },
              {"color": "green", "hex": "#008000"},
              {"color": "darkgreen", "hex": "#006400"},
              {"color": "blue", "hex": "#0000FF"},
              {"color": "darkblue", "hex": "#00008B"},
              {"color": "purple", "hex": "#800080"},
              {"color": "darkpurple", "hex": "#871F78"},
              {"color": "cadetblue", "hex": "#5F9EA0"}
            ];

            $.each(colors, function (key, element) {
              if (item.field_status_hex == element.hex || item.field_category_hex == element.hex) {
                color = element.hex;
                var marker = new L.circleMarker(latlon, {radius: 8, weight: 4, opacity: 1, color: '#666', fillOpacity: 1, fillColor: color});
                marker.bindPopup(html);
                reportMarkers.push(marker);
                bounds.extend(latlon);
              }
            });
          });
        }
      });
      return reportMarkers;
    }
  };
})(jQuery);
