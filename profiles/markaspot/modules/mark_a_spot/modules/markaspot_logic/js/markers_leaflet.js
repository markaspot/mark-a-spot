/**
 * Mark-a-Spot marker_leaflet.js
 *
 * Main Map-Application File with Leaflet Maps api
 *
 */

var arg = "";
var markerLayer;
(function ($) {
  $(document).ready(function () {
    if ($('#markers-list-view #map').length != 0) {
      var offset = $("#markers-list-view #map").offset();
      var topPadding = 120;
      $(window).scroll(function () {
        if ($(window).scrollTop() > offset.top) {
          $("#markers-list-view #map").stop().animate({
            marginTop: $(window).scrollTop() - offset.top + topPadding
          });
        } else {
          $("#map").stop().animate({
            marginTop: 0
          });
        }
      });
    }
    var mas = Drupal.settings.mas;

    Drupal.Markaspot = new Object();
    Drupal.Markaspot.maps = new Array();
    Drupal.Markaspot.markers = new Array();

    var pathId = mas.params.q.split('/');

    /**
     * Split URL and read MarkerID
     *
     */

    switch (pathId[0]) {
      case "map":
        readData(1, "All", "All");
        arg = '';
        break;

      case "list":
        readData(2, "All", "All");
        break;

      case "admin":
        return false;
    }

    var initialLatLng = new L.LatLng(mas.markaspot_ini_lat, mas.markaspot_ini_lng);
    var mapType = mas.markaspot_map_type;

    Drupal.Markaspot.maps[0] = new L.Map('map');

    switch (mapType){
      case '0':
        var attribution = mas.osm_custom_attribution;
        var layer = new L.Google('ROADMAP');
        break;

      case '1':
        var tiles = 'https://{s}.tiles.mapbox.com/v3/' + mas.mapbox_map_id + '/{z}/{x}/{y}.png';
        var attribution = '<a href="http://www.mapbox.com/about/maps/" target="_blank">Terms &amp; Feedback</a>';
        var layer = new L.TileLayer(tiles, {maxZoom: 18, attribution: attribution});
        break;

      case '2':
        var tiles = mas.osm_custom_tile_url;
        var attribution = mas.osm_custom_attribution;
        var layer = new L.TileLayer(tiles, {maxZoom: 18, attribution: attribution});

    }

    Drupal.Markaspot.maps[0].setView(new L.LatLng(mas.markaspot_ini_lat, mas.markaspot_ini_lng), 13).addLayer(layer);


    /**Sidebar Marker-functions*/

    $("#block-markaspot-logic-taxonomy-category .map-menue").click(function (e) {
      e.preventDefault();
      hideMarkers();
      readData(1, getTaxId(this.id), "All");
      return false;
    });

    $("#block-markaspot-logic-taxonomy-status .map-menue").click(function (e) {
      e.preventDefault();
      hideMarkers();
      readData(2, "All", getTaxId(this.id));
      return false;
    });

    $("#block-markaspot-logic-taxonomy-category .map-menue-all").click(function (e) {
      e.preventDefault();
      hideMarkers();
      readData(1, "All", "All");
      return false;
    });

    $("#block-markaspot-logic-taxonomy-status .map-menue-all").click(function (e) {
      e.preventDefault();
      hideMarkers();
      readData(2, "All", "All");
      return false;
    });

    function getTaxId(id) {
      id = id.split("-");
      return id[1];
    }

    function readData(getToggle, categoryCond, statusCond) {

      markerLayer = new L.MarkerClusterGroup({disableClusteringAtZoom: 15, maxClusterRadius: 40 });

      uri = mas.uri.split('?');
      url = Drupal.settings.basePath + 'reports/geojson/map?' + uri[1];

      // Spinner injection.
      var target = document.getElementById('map');
      var spinner = new Spinner().spin(target);

      $.getJSON(url).done(function (data) {

        spinner.stop();
        bounds = new L.LatLngBounds(initialLatLng);

        if (!data.features && mas.node_type == 'report') {
          // invoke a message box or something less permanent than an alert box later
          alert(Drupal.t('No reports found for this category/status'));
        }

        $.each(data.features, function (markers, item) {

          var latlon = new L.LatLng(item.geometry.coordinates[0], item.geometry.coordinates[1]);
          item = item.properties;
          var html = '<div class="marker-title"><h4><a class="infowindow-link" href="' + item.path + '">' + item.name + '</a></h4><span class="meta-info date">' + item.created + '</span></div>';
          if (item.field_address) {
            html += '<div class="marker-address"><p>' + item.field_address + '</p></div>';
            html += '<div><a class="infowindow-link" href="' + item.path + '">' + Drupal.t('read more') + '</a></div>';
          }

          if (getToggle == 1) {
            var colors = [
              {"color": "red", "hex": "FF0000"},
              {"color": "darkred", "hex": "8B0000" },
              {"color": "orange", "hex": "FFA500" },
              {"color": "green", "hex": "008000"},
              {"color": "darkgreen", "hex": "006400"},
              {"color": "blue", "hex": "0000FF"},
              {"color": "darkblue", "hex": "00008B"},
              {"color": "purple", "hex": "800080"},
              {"color": "darkpurple", "hex": "871F78"},
              {"color": "cadetblue", "hex": "5F9EA0"}
            ]
          }
          if (getToggle == 2) {
            var colors = [
              {"color": "red", "hex": "cc0000"},
              {"color": "green", "hex": "8fe83b"},
              {"color": "orange", "hex": "ff6600"},
              {"color": "cadetblue", "hex": "5F9EA0"}

            ]
          }

          $.each(colors, function (key, element) {
            if (item.field_status_hex == element.hex || item.field_category_hex == element.hex) {
              var awesomeColor = element.color;
              var awesomeIcon = (getToggle == 1) ? item.field_category_icon : item.field_status_icon;
              var marker = new L.Marker(latlon, {icon: L.AwesomeMarkers.icon({icon: awesomeIcon, prefix: 'icon', markerColor: awesomeColor, spin: false}) });

              marker.bindPopup(html);
              markerLayer.addLayer(marker);
              bounds.extend(latlon);
            }
          });
          var fn = markerClickFn(latlon, html);
          $('#marker_' + item.nid).on('hover', fn);
        });
        Drupal.Markaspot.maps[0].addLayer(markerLayer);
        // Drupal.Markaspot.maps[0].fitBounds(bounds);
      });
     }
  });
})(jQuery);

function hideMarkers() {
  Drupal.Markaspot.maps[0].removeLayer(markerLayer);
}

function markerClickFn(latlon, html) {
  return function () {
    Drupal.Markaspot.maps[0].closePopup();

    popup = new L.Popup();

    popup.setLatLng(latlon);
    popup.setContent(html);

    Drupal.Markaspot.maps[0].panTo(latlon);
    Drupal.Markaspot.maps[0].openPopup(popup);
  };
}
