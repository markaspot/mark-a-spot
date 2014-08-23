/**
 * Mark-a-Spot marker_leaflet.js
 *
 * Main Map-Application File with Leaflet Maps api
 *
 */
(function($) {
  Drupal.behaviors.Markaspot = {
    attach: function() {
      Drupal.settings.mas = Drupal.settings.mas || {};
      var mas = Drupal.settings.mas;
      Drupal.Markaspot = {};
      Drupal.Markaspot.maps = [];
      Drupal.Markaspot.markers = [];
      var mapType = mas.markaspot_map_type;
      Drupal.Markaspot.maps[0] = new L.Map('map');

      // Get Data from static json module
      if (Drupal.markaspot.static_enable() == true) {
        data = Drupal.markaspot_static_geojson.getData();
      }

      var attribution, layer, tiles;
      switch (mapType) {
        case '0':
          attribution = mas.osm_custom_attribution;
          layer = new L.Google('ROADMAP');
          break;
        case '1':
          tiles = 'https://{s}.tiles.mapbox.com/v3/' + mas.mapbox_map_id + '/{z}/{x}/{y}.png';
          attribution = '<a href="http://www.mapbox.com/about/maps/" target="_blank">Terms &amp; Feedback</a>';
          layer = new L.TileLayer(tiles, {
            maxZoom: 18,
            attribution: attribution
          });
          break;
        case '2':
          tiles = mas.osm_custom_tile_url;
          attribution = mas.osm_custom_attribution;
          layer = new L.TileLayer(tiles, {
            maxZoom: 18,
            attribution: attribution
          });
      }
      Drupal.Markaspot.maps[0].setView(new L.LatLng(mas.markaspot_ini_lat, mas.markaspot_ini_lng), 15).addLayer(layer);

      if ($('#markers-list-view').length !== 0) {
        var offset = $("#map").offset();
        var topPadding = 120;
        $(window).scroll(function() {
          if ($(this).scrollTop() > offset.top) {
            $("#map").stop().animate({
              marginTop: $(this).scrollTop() - offset.top + topPadding
            });
          } else {
            $("#map").stop().animate({
              marginTop: 0
            });
          }
        });
      }
      var pathId = mas.params.q.split('/');

      switch (pathId[0]) {
        case "map":
          if (Drupal.markaspot.static_enable() != true) {
            Drupal.markaspot.parse(1, "All", "All", false);
          } else {
            Drupal.markaspot.parse(1, "All", "All", true);
          }
          break;
        case "list":
          Drupal.markaspot.parse(2, "All", "All", false);
          break;
        case "admin":
          return;
      }
      $("#block-markaspot-logic-taxonomy-category .map-menue").click(function(e) {
        e.preventDefault();
        Drupal.markaspot.hideMarkers();
        if (Drupal.markaspot.static_enable() != true) {
          Drupal.markaspot.parse(1, Drupal.markaspot.getTaxId(this.id), "All", false);
        } else {
          Drupal.markaspot.parse(1, Drupal.markaspot.getTaxHex($(this).attr('class')), "All", true);
        }
        return false;
      });
      $("#block-markaspot-logic-taxonomy-status .map-menue").click(function(e) {
        e.preventDefault();
        Drupal.markaspot.hideMarkers();
        if (Drupal.markaspot.static_enable() != true) {
          Drupal.markaspot.parse(2, "All", Drupal.markaspot.getTaxId(this.id), false);
        } else {
          Drupal.markaspot.parse(2, "All", Drupal.markaspot.getTaxHex($(this).attr('class')), true);
        }
        return false;
      });
      $("#block-markaspot-logic-taxonomy-category .map-menue-all").click(function(e) {
        e.preventDefault();
        Drupal.markaspot.hideMarkers();
        if (Drupal.markaspot.static_enable() != true) {
          Drupal.markaspot.parse(1, "All", "All", false);
        } else {
          Drupal.markaspot.parse(1, "All", "All", true);
        }
        return false;
      });
      $("#block-markaspot-logic-taxonomy-status a.map-menue-all").click(function(e) {
        e.preventDefault();
        Drupal.markaspot.hideMarkers();
        if (Drupal.markaspot.static_enable() != true) {
          Drupal.markaspot.parse(2, "All", "All", false);
        } else {
          Drupal.markaspot.parse(2, "All", "All", true);
        }
        return false;
      });
    }
  };


  Drupal.markaspot = {

    /*
     * Hide Layers
     */
    hideMarkers: function() {
      Drupal.Markaspot.maps[0].removeLayer(markerLayer);
    },

    /*
     * Actions on Marker Click and Hover
     */
    markerClickFn: function (latlon, html, id) {
      return function (e) {
        map = Drupal.Markaspot.maps[0];
        map.closePopup();
        var report_url = Drupal.settings.basePath + 'georeport/v2/requests/' + id + '.json';
        $.getJSON(report_url).success(function (data) {
          var request = data[0].media_url ? '<img style="height: 80px; margin: 20px 10px 10px 0" src="' + data[0].media_url + '" class="map img-thumbnail pull-left"><p>' + data[0].description + '</p>' : '<p>' + data[0].description + '</p>';

          L.popup({autoPanPadding: new L.Point(15,120)})
            .setLatLng(latlon)
            .setContent(html + request + '</div>')
            .openOn(map);
        });

        map.on('popupopen', function (e) {
          if ($(window).width() >= 1000) {
            $('.map.img-thumbnail').popover({
              html: true,
              trigger: 'hover',
              placement: 'left',
              content: function () {
                return '<img class="img-thumbnail" style="float:right;width:320px;max-width:320px;" src="' + $(this)[0].src + '" />';
              }
            });
          }
        });
      };
    },

    /*
     * Show
     */
    showData: function(getToggle, dataset) {
      var mas = Drupal.settings.mas;

      markerLayer = new L.MarkerClusterGroup({
        disableClusteringAtZoom: 15,
        maxClusterRadius: 40
      });

      var initialLatLng = new L.LatLng(mas.markaspot_ini_lat, mas.markaspot_ini_lng);
      // bounds = new L.LatLngBounds(initialLatLng);

      if (!dataset && mas.node_type == 'report') {
        alert(Drupal.t('No reports found for this category/status'));
      }

      $.each(dataset, function(markers, item) {
        var latlon = new L.LatLng(item.geometry.coordinates[0], item.geometry.coordinates[1]);
        item = item.properties;

        var html = '<div class="popover-report"><div class="marker-title"><h4>' + item.name + '</h4><span class="meta-info date">' + item.created + '</span><p>' + item.description + '</p></div>';
        if (item.field_address) {
          html += '<div class="marker-address"><p><i class="icon-li icon-location "></i> ' + item.field_address + '</p></div>';
          html += '<div><a class="infowindow-link" href="' + item.path + '">' + Drupal.t('read more') + '</a></div>';
        }



        var colors = [{
          "color": "red", "hex": "FF0000"
        }, {
          "color": "darkred", "hex": "8B0000"
        }, {
          "color": "orange", "hex": "FFA500"
        }, {
          "color": "green", "hex": "008000"
        }, {
          "color": "darkgreen", "hex": "006400"
        }, {
          "color": "blue", "hex": "0000FF"
        }, {
          "color": "darkblue", "hex": "00008B"
        }, {
          "color": "purple", "hex": "800080"
        }, {
          "color": "darkpurple", "hex": "871F78"
        }, {
          "color": "cadetblue", "hex": "5F9EA0",
        }, {
          "color": "lightgray", "hex": "D3D3D3",
        }, {
          "color": "gray", "hex": "808080",
        }, {
          "color": "black", "hex": "000000",
        }, {
          "color": "beige", "hex": "F5F5DC",
        }, {
          "color": "white", "hex": "5F9EA0",
        }];

        var colorswitch = (getToggle == 1) ? String(item.field_category_hex) : String(item.field_status_hex);

        $.each(colors, function(key, element) {
          if (colorswitch == element.hex) {
            var awesomeColor = element.color;
            var awesomeIcon = (getToggle == 1) ? item.field_category_icon : item.field_status_icon;
            var marker = new L.Marker(latlon, {
              icon: L.AwesomeMarkers.icon({
                icon: awesomeIcon,
                prefix: 'icon',
                markerColor: awesomeColor,
                spin: false
              })
            });
            markerLayer.addLayer(marker);
            marker.on('click', Drupal.markaspot.markerClickFn(latlon, html, item.uuid));
          }

        });
        var fn = Drupal.markaspot.markerClickFn(latlon, html, item.uuid);
        $('#marker_' + item.nid).on('hover', fn);
        Drupal.Markaspot.maps[0].addLayer(markerLayer);
      });
    },


    parse: function(getToggle, categoryCond, statusCond, moduleStatic) {

      var mas     = Drupal.settings.mas;

      var target  = document.getElementById('map');
      var spinner = new Spinner().spin(target);

      //Mark-a-Spot Static Module detected, get all data via JSON in sites/default/files/geojson.
      if (moduleStatic) {
        if (categoryCond == "All" && statusCond == "All") {
          var dataset = data.features;
          Drupal.markaspot.showData(getToggle, dataset);
        }
        if (categoryCond != "All") {
          var filter = data.features.filter(function(i) {
            return i.properties.field_category_hex == categoryCond;
          });
          Drupal.markaspot.showData(getToggle, filter);
        } else if (statusCond != "All") {
          var filter = data.features.filter(function(i) {
            return i.properties.field_status_hex == statusCond;
          });
          Drupal.markaspot.showData(getToggle, filter);
        }
      } else {
        //No Static module detected, get all data via Drupal geojson display.
        uri = mas.uri.split('?');
        if (uri[0].search('map') != -1 || uri[0].search('home') != -1) {
          // map view
          url = Drupal.settings.basePath + 'reports/geojson/map?' + 'field_category_tid=' + categoryCond + '&field_status_tid=' + statusCond;
        } else {
          url = Drupal.settings.basePath + 'reports/geojson?' + uri[1];
        }
        $.getJSON(url).done(function(data) {
          var dataset = data.features;
          Drupal.markaspot.showData(getToggle, dataset);
        });
      }
      spinner.stop();

    },
    static_enable: function() {
      return (Drupal.settings.markaspot_static_geojson) ? Drupal.settings.markaspot_static_geojson.enable : false;
    },

    getTaxHex: function(id) {
      id = id.split(" ");
      id = id[2].split("-");
      return id[1];
    },
    getTaxId: function(id) {
      id = id.split("-");
      return id[1];
    }

  }
})(jQuery);