/**
 * @file
 * Javascript for Goole Map widget of Geolocation field.
 */

(function ($) {
  var geocoder;
  markerLayer = new L.LayerGroup();

  Drupal.geolocation = Drupal.geolocation || {};
  Drupal.geolocation.maps = Drupal.geolocation.maps || {};
  Drupal.geolocation.markers = Drupal.geolocation.markers || {};

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
  Drupal.geolocation.codeLatLng = function(latLng, i, op) {
    
    //Update the lat and lng input fields
    $('#geolocation-lat-' + i + ' input').attr('value', latLng.lat);
    $('#geolocation-lat-item-' + i + ' .geolocation-lat-item-value').html(latLng.lat);
    $('#geolocation-lng-' + i + ' input').attr('value', latLng.lng);
    $('#geolocation-lng-item-' + i + ' .geolocation-lng-item-value').html(latLng.lng);
 
    // Update the address field
    if ((op == 'marker' || op == 'geocoder')) {

      var api_key = Drupal.settings.geolocation.settings.bing_api_key;
     
      $.getJSON('http://dev.virtualearth.net/REST/v1/Locations/' + latLng.lat + ', ' + latLng.lng + '?o=json&key=' + api_key + '&jsonp=?', function (result) {
        if (result.resourceSets[0].estimatedTotal > 0) {
            var loc = result.resourceSets[0].resources[0].point.coordinates;
            var address = result.resourceSets[0].resources[0].address.formattedAddress;
            $('#geolocation-address-' + i + ' input').val(address);            
            Drupal.geolocation.maps[i].setView(new L.LatLng(loc[0],loc[1]), i);
            Drupal.geolocation.setMapMarker(new L.LatLng(loc[0],loc[1]), i);
        }
        else {
            alert(Drupal.t("Sorry an address cannot be found"));
        }
      });    
    }
  }
 
  /**
   * Get the location from the address field
   *
   * @param i
   *   the index from the maps array we are working on
   */

  Drupal.geolocation.codeAddress = function(i) {
    var address = $('#geolocation-address-' + i + ' input').val();
    var api_key = Drupal.settings.geolocation.settings.bing_api_key;
    //console.log(Drupal.geolocation.settings);
    $.getJSON('http://dev.virtualearth.net/REST/v1/Locations?query=' + address + '&key=' + api_key + '&jsonp=?', function (result) {
      if (result.resourceSets[0].estimatedTotal > 0) {
          var loc = result.resourceSets[0].resources[0].point.coordinates;
          Drupal.geolocation.maps[i].setView(new L.LatLng(loc[0],loc[1]), i);
          Drupal.geolocation.setMapMarker(new L.LatLng(loc[0],loc[1]), i);
          Drupal.geolocation.codeLatLng(new L.LatLng(loc[0],loc[1]), i);
      }
      else {
          alert(Drupal.t("Sorry that address cannot be found"));
      }
    });
  }

  /**
   * Set zoom level depending on accuracy (location_type)
   *
   * @param location_type
   *   location type as provided by google maps after geocoding a location
   */
   Drupal.geolocation.setZoom = function(i, location_type) {
     if (location_type == 'APPROXIMATE') {
       Drupal.geolocation.maps[i].setZoom(10);
     }
     else if (location_type == 'GEOMETRIC_CENTER') {
       Drupal.geolocation.maps[i].setZoom(12);
     }
     else if (location_type == 'RANGE_INTERPOLATED' || location_type == 'ROOFTOP') {
       Drupal.geolocation.maps[i].setZoom(16);
     }
   }

   
  /**
   * Set/Update a marker on a map
   *
   * @param latLng
   *   a location (latLng) object from google maps api
   * @param i
   *   the index from the maps array we are working on
   */
  Drupal.geolocation.setMapMarker = function(latLng, i) {
    
    
    // remove old marker
    if(Drupal.geolocation.maps[i].hasLayer(markerLayer) == true){
      //console.log(Drupal.geolocation.maps[i].hasLayer(markerLayer));
      markerLayer.clearLayers();
    }

    var markerLocation = latLng;
      Drupal.geolocation.maps[i].setView(latLng, 13)
      var marker = new L.Marker(latLng);
      // add marker to markerLayer
      markerLayer.addLayer(marker);
      Drupal.geolocation.maps[i].addLayer(markerLayer);
   

    // google.maps.event.addListener(Drupal.geolocation.markers[i], 'dragend', function(me) {
    //   Drupal.geolocation.codeLatLng(me.latLng, i, 'marker');
    //   Drupal.geolocation.setMapMarker(me.latLng, i);
    // });

    return false; // if called from <a>-Tag
  }
 
  /**
   * Get the current user location if one is given
   * @return
   *   Formatted location
   */
  Drupal.geolocation.getFormattedLocation = function() {
    if (google.loader.ClientLocation.address.country_code == "US" &&
      google.loader.ClientLocation.address.region) {
      return google.loader.ClientLocation.address.city + ", " 
          + google.loader.ClientLocation.address.region.toUpperCase();
    }
    else {
      return  google.loader.ClientLocation.address.city + ", "
          + google.loader.ClientLocation.address.country_code;
    }
  }
 
  /**
   * Clear/Remove the values and the marker
   *
   * @param i
   *   the index from the maps array we are working on
   */
  Drupal.geolocation.clearLocation = function(i) {
    $('#geolocation-lat-' + i + ' input').attr('value', '');
    $('#geolocation-lat-item-' + i + ' .geolocation-lat-item-value').html('');
    $('#geolocation-lng-' + i + ' input').attr('value', '');
    $('#geolocation-lng-item-' + i + ' .geolocation-lat-item-value').html('');
    $('#geolocation-address-' + i + ' input').attr('value', '');
    Drupal.geolocation.markers[i].setMap();
  }
 
  /**
   * Do something when no location can be found
   *
   * @param supportFlag
   *   Whether the browser supports geolocation or not
   * @param i
   *   the index from the maps array we are working on
   */
  Drupal.geolocation.handleNoGeolocation = function(supportFlag, i) {
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
    Drupal.geolocation.maps[i].setView(initialLocation);
    Drupal.geolocation.setMapMarker(initialLocation, i);
  }

  Drupal.behaviors.geolocationCloudmadeOsm = {
    attach: function(context, settings) {

      var lat;
      var lng;
      var latLng;
      var mapOptions;
      var browserSupportFlag =  new Boolean();
      var singleClick;

      // Work on each map
      $.each(Drupal.settings.geolocation.defaults, function(i, mapDefaults) {
        // Only make this once ;)
        $("#geolocation-map-" + i).once('geolocation-cloudmade-osm', function(){

          // Attach listeners
          $('#geolocation-address-' + i + ' input').keypress(function(ev){
            if(ev.which == 13){
              ev.preventDefault();
              Drupal.geolocation.codeAddress(i);
            }
          });
          $('#geolocation-address-geocode-' + i).click(function(e) {
            Drupal.geolocation.codeAddress(i);
          });

          $('#geolocation-remove-' + i).click(function(e) {
            Drupal.geolocation.clearLocation(i);
          });

          // START: Autodetect clientlocation.
          // First use browser geolocation
          if (navigator.geolocation) {
            browserSupportFlag = true;
            $('#geolocation-help-' + i + ':not(.geolocation-cloudmade-osm-processed)').addClass('geolocation-cloudmade-osm-processed').append(Drupal.t(', or use your browser geolocation system by clicking this link') +': <span id="geolocation-client-location-' + i + '" class="geolocation-client-location">' + Drupal.t('My Location') + '</span>');
            // Set current user location, if available
            $('#geolocation-client-location-' + i + ':not(.geolocation-cloudmade-osm-processed)').addClass('geolocation-cloudmade-osm-processed').click(function() {
              navigator.geolocation.getCurrentPosition(function(position) {
                latLng = new L.LatLng(position.coords.latitude, position.coords.longitude);
                Drupal.geolocation.maps[i].setView(latLng);
                Drupal.geolocation.setMapMarker(latLng, i);
                Drupal.geolocation.codeLatLng(latLng, i, 'geocoder');
              }, function() {
                Drupal.geolocation.handleNoGeolocation(browserSupportFlag, i);
              });
            });
          }
          // If browser geolication is not supoprted, try ip location
          else if (google.loader.ClientLocation) {
            latLng = new L.LatLng(google.loader.ClientLocation.latitude, google.loader.ClientLocation.longitude);
            $('#geolocation-help-' + i + ':not(.geolocation-cloudmade-osm-processed)').addClass('geolocation-cloudmade-osm-processed').append(Drupal.t(', or use the IP-based location by clicking this link') +': <span id="geolocation-client-location-' + i + '" class="geolocation-client-location">' + Drupal.geolocation.getFormattedLocation() + '</span>');

            // Set current user location, if available
            $('#geolocation-client-location-' + i + ':not(.geolocation-cloudmade-osm-processed)').addClass('geolocation-cloudmade-osm-processed').click(function() {
              latLng = new L.LatLng(google.loader.ClientLocation.latitude, google.loader.ClientLocation.longitude);
              Drupal.geolocation.maps[i].setView(latLng);
              Drupal.geolocation.setMapMarker(latLng, i);
              Drupal.geolocation.codeLatLng(latLng, i, 'geocoder');
            });
          }
          // END: Autodetect clientlocation.
          // Get current/default values

          // Get default values
          // This might not be necesarry
          // It can always come from e
          lat = $('#geolocation-lat-' + i + ' input').attr('value') == false ? mapDefaults.lat : $('#geolocation-lat-' + i + ' input').attr('value');
          lng = $('#geolocation-lng-' + i + ' input').attr('value') == false ? mapDefaults.lng : $('#geolocation-lng-' + i + ' input').attr('value');
          latLng = new L.LatLng(lat, lng);
        
          // Set map options
          // mapOptions = {
          //   zoom: 2,
          //   center: latLng,
          //   mapTypeId: google.maps.MapTypeId.ROADMAP,
          //   scrollwheel: (Drupal.settings.geolocation.settings.scrollwheel != undefined) ? Drupal.settings.geolocation.settings.scrollwheel : false
          // }

          // Create map
          // console.log(Drupal.settings.geolocation.settings);

          Drupal.geolocation.maps[i] = new L.Map(document.getElementById("geolocation-map-" + i));
          var cloudmadeUrl = 'http://{s}.tile.cloudmade.com/BC9A493B41014CAABB98F0471D759707/22677/256/{z}/{x}/{y}.png',
          cloudmadeAttribution = 'Map data &copy; 2012 OpenStreetMap contributors, Imagery &copy; 2012 CloudMade',
          cloudmade = new L.TileLayer(cloudmadeUrl, {maxZoom: 18, attribution: cloudmadeAttribution});

          Drupal.geolocation.maps[i].setView(latLng, 13).addLayer(cloudmade);

          if (lat && lng) {
            // Set initial marker
            Drupal.geolocation.codeLatLng(latLng, i, 'geocoder');
            Drupal.geolocation.setMapMarker(latLng, i);
          }

          Drupal.geolocation.maps[i].on('click', function(e){
            console.log(e);
            Drupal.geolocation.codeLatLng(e.latlng, i, 'geocoder');
            Drupal.geolocation.setMapMarker(e.latlng, i);
          });
        })
      });
    }
  };
})(jQuery);
