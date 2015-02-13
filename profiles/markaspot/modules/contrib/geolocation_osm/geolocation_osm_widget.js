/**
 * @file
 * Javascript for osm Leaflet Map widget of Geolocation field.
 */

(function ($) {
  var geocoder;
  markerLayer = new L.LayerGroup();

  Drupal.geolocation = Drupal.geolocation || {};
  Drupal.geolocation.maps = Drupal.geolocation.maps || {};
  Drupal.geolocation.markers = Drupal.geolocation.markers || {};
  Drupal.geolocation.settings = Drupal.geolocation.settings || {};
  /**
   * Set the latitude and longitude values to the input fields
   * And optionaly update the address field
   *
   * @param latLng
   *   a location (latLng) object from google maps api
   * @param i
   *   the index from the maps array we are working on
   * @param op
   *   the op that was performed
   */
  Drupal.geolocation.codeLatLng = function (latLng, i, op) {
    //Update the lat and lng input fields
    $('#geolocation-lat-' + i + ' input').attr('value', latLng.lat);
    $('#geolocation-lat-item-' + i + ' .geolocation-lat-item-value').html(latLng.lat);
    $('#geolocation-lng-' + i + ' input').attr('value', latLng.lng);
    $('#geolocation-lng-item-' + i + ' .geolocation-lng-item-value').html(latLng.lng);

    // Update the address field
    if ((op == 'marker' || op == 'geocoder')) {

      var nominatimEmail = Drupal.settings.geolocation.settings.nominatimEmail;
      L.Icon.Default.imagePath = Drupal.settings.geolocation.settings.leafletImagePath;

      var url = Drupal.settings.geolocation.settings.geocode_service + '/reverse';
      $.getJSON(url + '?email=' + nominatimEmail + '&lat=' + latLng.lat + '&lon=' + latLng.lng + '&json_callback=?', {
        format: 'json'
      }).done(function (result) {
        if (result.address) {
          var street  = result.address.construction ? result.address.construction : "";
          street      = street ? street : result.address.path;
          street      = street ? street : result.address.cycleway;
          street      = street ? street : result.address.road;
          street      = street ? street : result.address.pedestrian;
          street      = street ? street : result.address.footway;

          var city = result.address.city ? result.address.city : "";
          city = city ? city : result.address.village;
          city = city ? city : result.address.town;
          city = city ? city : result.address.hamlet;
          city = city ? city : result.address.state_district;

          street      = result.address.house_number ? street + ' ' + result.address.house_number : street;
          var postcode    = result.address.postcode ? result.address.postcode : "";

          // Need Adress switch for geocoding
          var address = street + ', ' + postcode + ' ' + city;

          $('#edit-field-geo-und-0-address-field').val(address);
          // Drupal.geolocation.maps[i].setView(new L.LatLng(result.lat, result.lon));
          // Drupal.geolocation.setMapMarker(new L.LatLng(result.lat , result.lon),i);
        }
        else {
          alert(Drupal.t("Sorry an address cannot be found for this point"));
        }
      });
    }
  };

  /**
   * Get the location from the address field
   *
   * @param i
   *   the index from the maps array we are working on
   */

  Drupal.geolocation.codeAddress = function (i) {
    // Nominatim
    // http://wiki.openstreetmap.org/wiki/Nominatim_usage_policy

    settings = Drupal.settings.geolocation.settings;

    var address = $('#edit-field-geo-und-0-address-field').val();
    var nominatimEmail = settings.nominatimEmail;
    var url = settings.geocode_service + '/search';
    $.getJSON(url + '?email=' + nominatimEmail + '&q=' + address + '&bounded=1&addressdetails=1&viewbox=' + settings.bbox + '&json_callback=?', {
      format: 'json'
    }).done(function (result) {
      if (result.length > 0) {
        result.address = result[0].address;
        Drupal.geolocation.setMapMarker(new L.LatLng(result[0].lat, result[0].lon), i);

        var street  = result.address.construction ? result.address.construction : "";
        street      = street ? street : result.address.path;
        street      = street ? street : result.address.locality;
        street      = street ? street : result.address.cycleway;
        street      = street ? street : result.address.road;
        street      = street ? street : result.address.pedestrian;
        street      = street ? street : result.address.footway;
        street = street ? street : Drupal.t('City Center');

        street = result[0].address.house_number ? street + ' ' + result[0].address.house_number : street;

        var city = result[0].address.city ? result[0].address.city : "";
        city = city ? city : result[0].address.town;
        city = city ? city : result[0].address.village;
        city = city ? city : result[0].address.hamlet;
        city = city ? city : result[0].address.state_district;

        postcode = result[0].address.postcode ? result[0].address.postcode + " " : "";
        var address = street + ', ' + postcode + city;

        $('#edit-field-geo-und-0-address-field').val(address);
        $('#geolocation-lat-' + i + ' input').attr('value', result[0].lat);
        $('#geolocation-lat-item-' + i + ' .geolocation-lat-item-value').html(result[0].lat);
        $('#geolocation-lng-' + i + ' input').attr('value', result[0].lon);
        $('#geolocation-lng-item-' + i + ' .geolocation-lng-item-value').html(result[0].lon);

      }
      else {
        alert(Drupal.t("Sorry that address cannot be found"));
      }
    });
  };

  /**
   * Set zoom level depending on accuracy (location_type)
   *
   * @param location_type
   *   location type as provided by google maps after geocoding a location
   */
  Drupal.geolocation.setZoom = function (i, location_type) {
    if (location_type == 'APPROXIMATE') {
      Drupal.geolocation.maps[i].setZoom(10);
    }
    else if (location_type == 'GEOMETRIC_CENTER') {
      Drupal.geolocation.maps[i].setZoom(12);
    }
    else if (location_type == 'RANGE_INTERPOLATED' || location_type == 'ROOFTOP') {
      Drupal.geolocation.maps[i].setZoom(16);
    }
  };


  /**
   * Set/Update a marker on a map
   *
   * @param latLng
   *   a location (latLng) object from google maps api
   * @param i
   *   the index from the maps array we are working on
   */
  Drupal.geolocation.setMapMarker = function (latLng, i) {

    // remove old marker
    if (Drupal.geolocation.maps[i].hasLayer(markerLayer) == true) {
      //console.log(Drupal.geolocation.maps[i].hasLayer(markerLayer));
      markerLayer.clearLayers();
    }

    var markerLocation = latLng;
    Drupal.geolocation.maps[i].setView(latLng, 16)
    var marker = new L.Marker(latLng, {draggable: true});
    // add marker to markerLayer
    markerLayer.addLayer(marker);
    Drupal.geolocation.maps[i].addLayer(markerLayer);

    marker.on('dragend', function (event) {
      Drupal.geolocation.codeLatLng(marker.getLatLng(), i, 'marker');
    });

    return false;
  };

  /**
   * Get the current user location if one is given
   * @return
   *   Formatted location
   */
  Drupal.geolocation.getFormattedLocation = function () {
    if (google.loader.ClientLocation.address.country_code == "US" &&
      google.loader.ClientLocation.address.region) {
      return google.loader.ClientLocation.address.city + ", " + google.loader.ClientLocation.address.region.toUpperCase();
    }
    else {
      return  google.loader.ClientLocation.address.city + ", " + google.loader.ClientLocation.address.country_code;
    }
  };

  /**
   * Clear/Remove the values and the marker
   *
   * @param i
   *   the index from the maps array we are working on
   */
  Drupal.geolocation.clearLocation = function (i) {
    $('#geolocation-lat-' + i + ' input').attr('value', '');
    $('#geolocation-lat-item-' + i + ' .geolocation-lat-item-value').html('');
    $('#geolocation-lng-' + i + ' input').attr('value', '');
    $('#geolocation-lng-item-' + i + ' .geolocation-lat-item-value').html('');
    $('#geolocation-address-' + i + ' input').attr('value', '');
    Drupal.geolocation.markers[i].setMap();
  };

  /**
   * Do something when no location can be found
   *
   * @param supportFlag
   *   Whether the browser supports geolocation or not
   * @param i
   *   the index from the maps array we are working on
   */
  Drupal.geolocation.handleNoGeolocation = function (supportFlag, i) {
    var siberia = new L.LatLng(60, 105);
    var newyork = new L.LatLng(40.69847032728747, -73.9514422416687);
    if (supportFlag == true) {
      alert(Drupal.t("Geolocation service failed. We've placed you in NewYork."));
      initialLocation = newyork;
    }
    else {
      alert(Drupal.t("Your browser doesn't support geolocation. We've placed you in Siberia."));
      initialLocation = siberia;
    }
    Drupal.geolocation.maps[i].setView(initialLocation, 16);
    Drupal.geolocation.setMapMarker(initialLocation, i);
  }

  Drupal.behaviors.geolocationOsm = {
    attach: function (context, settings) {

      var lat;
      var lng;
      var latLng;
      var mapOptions;
      var browserSupportFlag = new Boolean();
      var singleClick;

      // Work on each map
      $.each(Drupal.settings.geolocation.defaults, function (i, mapDefaults) {
        // Only make this once ;)
        $("#geolocation-map-" + i).once('geolocation-osm', function () {

          // Attach listeners
          $('#geolocation-address-' + i + ' input').keypress(function (ev) {
            if (ev.which == 13) {
              ev.preventDefault();
              Drupal.geolocation.codeAddress(i);
            }
          });
          $('#geolocation-address-geocode-' + i).click(function (e) {
            Drupal.geolocation.codeAddress(i);
          });

          $('#geolocation-remove-' + i).click(function (e) {
            Drupal.geolocation.clearLocation(i);
          });

          // START: Autodetect clientlocation.
          // First use browser geolocation.
          if (navigator.geolocation) {
            browserSupportFlag = true;
            $('#geolocation-help-' + i + ':not(.geolocation-osm-osm-processed)').addClass('geolocation-osm-osm-processed').append(Drupal.t(', or use your browser geolocation system by clicking this link') + ': <span id="geolocation-client-location-' + i + '" class="geolocation-client-location">' + Drupal.t('My Location') + '</span>');
            // Set current user location, if available
            $('#geolocation-client-location-' + i + ':not(.geolocation-osm-osm-processed)').addClass('geolocation-osm-osm-processed').click(function () {
              navigator.geolocation.getCurrentPosition(function (position) {
                latLng = new L.LatLng(position.coords.latitude, position.coords.longitude);
                // Drupal.geolocation.maps[i].setView(latLng);
                Drupal.geolocation.setMapMarker(latLng, i);
                Drupal.geolocation.codeLatLng(latLng, i, 'geocoder');
              }, function () {
                Drupal.geolocation.handleNoGeolocation(browserSupportFlag, i);
              });
            });
          }

          lat = $('#geolocation-lat-' + i + ' input').attr('value') == false ? mapDefaults.lat : $('#geolocation-lat-' + i + ' input').attr('value');
          lng = $('#geolocation-lng-' + i + ' input').attr('value') == false ? mapDefaults.lng : $('#geolocation-lng-' + i + ' input').attr('value');
          latLng = new L.LatLng(lat, lng);

          // Create map
          var tile_server = Drupal.settings.geolocation.settings.tile_server_dynamic;

          Drupal.geolocation.maps[i] = new L.Map(document.getElementById("geolocation-map-" + i));
          var osmUrl = tile_server,
            osmAttribution = 'Map data &copy; 2014 OpenStreetMap contributors, Imagery &copy; 2014 osm',
            osm = new L.TileLayer(osmUrl, {maxZoom: 18, attribution: osmAttribution});
          Drupal.geolocation.maps[i].setZoom(Drupal.settings.geolocation.settings.map_zoomlevel);

          Drupal.geolocation.maps[i].setView(latLng).addLayer(osm);

          if (lat && lng) {
            // Set initial marker
            Drupal.geolocation.codeLatLng(latLng, i, 'geocoder');
            Drupal.geolocation.setMapMarker(latLng, i);
          }


          Drupal.geolocation.maps[i].on('click', function (e) {
            // console.log(e);
            Drupal.geolocation.codeLatLng(e.latlng, i, 'geocoder');
            Drupal.geolocation.setMapMarker(e.latlng, i);
          });
        });
      });
    }
  };
})(jQuery);
