/**
 * @file markaspot_open311_client.js
 *
 * Provides some more leaflet layers to add reports
 */


(function ($) {

  Drupal.behaviors.markaspotOpen311Client = {

    attach: function (context, settings) {
      Drupal.Markaspot = Drupal.Markaspot || {};
      var map = Drupal.Markaspot.maps[0];
      Drupal.markaspotOpen311Client.map = Drupal.markaspotOpen311Client.createMap(Drupal.Markaspot.maps[0].getCenter());

      // Define layer:
      addMarkerLayer = new L.LayerGroup();

      map.on('click', function (e) {
        Drupal.markaspotOpen311Client.setMapMarker(e.latlng, 0);
      });

      $('#open311_client').on('shown.bs.modal', function (e) {
        Drupal.markaspotOpen311Client.codeLatLng(Drupal.Markaspot.maps[0].getCenter(), 0, 'marker');
        // We need this to make map load completely.
        setTimeout(function () {
          Drupal.markaspotOpen311Client.map.invalidateSize();
        }, 10);
      });
      $('#open311_client').on('hidden.bs.modal', function (e) {
        $('.main-container').fadeIn();
      });

      // Add Locate Control
      L.control.locate({
        showPopup: true, // display a popup when the user click on the inner marker
        strings: {
          title: Drupal.t("Show me where I am"),  // title of the locate control
          popup: Drupal.t("You are within {distance} {unit} from this point"),  // text to appear if user clicks on circle
          outsideMapBoundsMsg: Drupal.t("You seem located outside the boundaries of the map") // default message for onLocationOutsideMapBounds
        },
        locateOptions: {}  // define location options e.g enableHighAccuracy: true or maxZoom: 10
      }).addTo(map);

      // Add a report button
      L.easyButton('fa fa-edit', 'topright',
        function () {
          if (!localStorage['tour']) {
            bootbox.alert(Drupal.t('Click or drag marker to file an issue at this location. You can then adjust the location in the opening report form'), tourDone);
            function tourDone() {
              localStorage['tour'] = true;
            }
          }
          Drupal.markaspotOpen311Client.setMapMarker(map.getCenter(), 0);
        },
        Drupal.t('Report at map center'),
        map
      );
    }

  };
  Drupal.markaspotOpen311Client = {
    setMapMarker: function (latLng) {

      // Add Layergroup and define ImagePath.
      L.Icon.Default.imagePath = Drupal.settings.markaspotOpen311Client.markaspot_open311_client_leafletImagePath;

      // Remove old marker.
      if (Drupal.Markaspot.maps[0].hasLayer(addMarkerLayer) == true) {
        addMarkerLayer.clearLayers();
      }
      // Remove list items
      $('.main-container').fadeOut();
      Drupal.Markaspot.maps[0].setView(latLng);
      var marker = new L.Marker(latLng, {
        title: Drupal.t('Add report here'),
        bounceOnAdd: true,
        draggable: true
      });

      // Add marker to markerLayer.
      addMarkerLayer.addLayer(marker);
      var map = Drupal.Markaspot.maps[0];
      map.closePopup();
      map.addLayer(addMarkerLayer);

      // Call client for interaction with open311 api
      marker.on('click', function (e) {
        Drupal.markaspotOpen311Client.showForm(marker);
      });
      marker.on('dragend', function (e) {
        Drupal.markaspotOpen311Client.showForm(marker);
      });
      return false;
    },

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
    codeLatLng: function (latLng, i, op) {
      var settings = Drupal.settings.markaspotOpen311Client;
      var nominatimEmail = settings.markaspot_open311_client_nominatimEmail;
      var url = settings.markaspot_open311_client_geocode_service + '/reverse';
      $.getJSON(url + '?email=' + nominatimEmail + '&lat=' + latLng.lat + '&lon=' + latLng.lng + '&json_callback=?', {
        format: 'json'
      }).done(function (result) {
        if (result) {
          var street = result.address.construction ? result.address.construction : "";
          street = street ? street : result.address.path;
          street = street ? street : result.address.cycleway;
          street = street ? street : result.address.road;
          street = street ? street : result.address.pedestrian;
          street = street ? street : result.address.footway;

          var city = result.address.city ? result.address.city : "";
          city = city ? city : result.address.town;
          city = city ? city : result.address.village;
          city = city ? city : result.address.hamlet;
          city = city ? city : result.address.state_district;

          street = result.address.house_number ? street + ' ' + result.address.house_number : street;
          var postcode = result.address.postcode ? result.address.postcode : "";

          // Need Address switch for geocoding
          var address = street + ', ' + postcode + ' ' + city;

          $('#address_string').val(address);
          // Drupal.geolocation.maps[i].setView(new L.LatLng(result.lat, result.lon));
          // Drupal.geolocation.setMapMarker(new L.LatLng(result.lat , result.lon),i);
        }
        else {
          alert(Drupal.t("Sorry an address cannot be found"));
        }
      });
    },

    localStorage: function () {
      // Localstorage if popup needs to be closed;
      $('.form-control').keyup(function () {
        localStorage[$(this).attr('name')] = $(this).val();
      });

      // Localstorage if popup needs to be closed;
      $('#services').change(function () {
        localStorage['services'] = $(this).val();
      });

      if (localStorage['services']) {
        $('#services option[value=' + localStorage['services'] + ']').attr('selected', 'selected');
      }
      if (localStorage['email']) {
        $('#email').val(localStorage['email']);
      }
      if (localStorage['description']) {
        $('#description').val(localStorage['description']);
      }
    },

    success: function () {
      $('textarea').before('<div class="alert alert-success" role="alert">' + Drupal.t('Thank you! This report has been saved.') + '</div>');
      localStorage.clear();
      $('#georeport_submit').prop('disabled', true);
    },
    highlight: function (element, message) {
      $(element).focus();
      $(element).closest('.form-group').addClass('has-error');
      $('.has-error').before('<div class="error">' + message + '</div>');
    },
    unhighlight: function () {
      $('.form-group').removeClass('has-error');
      $('.error, .alert-success').remove();
      $('#georeport_submit').prop('disabled', false);

    },
    createMap: function (latlng) {
      // Create small map

      var mas = Drupal.settings.mas;
      var mapType = mas.markaspot_map_type;
      switch (mapType) {
        case '0':
          var attribution = mas.osm_custom_attribution;
          var layer = new L.Google('ROADMAP');
          break;
        case '1':
          var tiles = 'https://{s}.tiles.mapbox.com/v3/' + mas.mapbox_map_id + '/{z}/{x}/{y}.png';
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


      var zoom_layer = new L.TileLayer(tiles, {
        maxZoom: 18,
        attribution: attribution
      });

      var zoom = L.map('map_small', {
        layers: [zoom_layer],
        center: latlng,
        zoom: 18,
        zoomControl: false
      });

      // Add Locate Control
      L.control.locate({
        showPopup: false, // display a popup when the user click on the inner marker
        position: 'topright',
        strings: {
          title: Drupal.t("Show me where I am"),  // title of the locate control
          popup: Drupal.t("You are within {distance} {unit} from this point"),  // text to appear if user clicks on circle
          outsideMapBoundsMsg: Drupal.t("You seem located outside the boundaries of the map") // default message for onLocationOutsideMapBounds
        },
        locateOptions: {}  // define location options e.g enableHighAccuracy: true or maxZoom: 10
      }).addTo(zoom);


      zoom.sync(Drupal.Markaspot.maps[0]);
      // Update lat lng form value
      $(zoom).on('dragend', function (e) {
        Drupal.markaspotOpen311Client.codeLatLng(Drupal.Markaspot.maps[0].getCenter(), 0, 'marker');
        latlng = Drupal.Markaspot.maps[0].getCenter();
      });

      $(zoom).on('moveend', function (e) {
        Drupal.markaspotOpen311Client.codeLatLng(zoom.getCenter(), 0, 'marker');
      });

      return zoom;
    },
    showForm: function (marker) {
      var map = Drupal.markaspotOpen311Client.map;
      map.setView(marker.getLatLng(), 16);
      var settings = Drupal.settings.markaspotOpen311Client;
      Drupal.markaspotOpen311Client.codeLatLng(marker.getLatLng(), 0, 'marker');

      $('#open311_client').modal('show');
      var latlng = marker.getLatLng();

      var select = $.get(settings.markaspot_open311_client_endpoint + '/services.json', function (services) {
        var options = $('#services');
        options.empty();
        options.append($('<option>', {
            value: '',
            text: Drupal.t('Select')
          }
        ));
        $.each(services, function (id, sv) {
          options.append($('<option>', {
              name: 'service_code',
              value: sv.service_code,
              text: sv.service_name
            }
          ));
        });
        Drupal.markaspotOpen311Client.localStorage();
      });

      $("#open311_client_close").click(function (event) {
        event.preventDefault();
        $('#open311_client').modal('hide');
        $('.main-container').fadeIn();
      });

      $("#dropbox, #multiple").html5_upload({
        fieldName: "image",
        url: settings.markaspot_open311_image_api_url,
        headers: {
          'Authorization': 'Client-ID' + ' ' + settings.markaspot_open311_image_api_key
        },
        onError: function () {
          alert(Drupal.t('Error while uploading file, please check the API-Key.'));
        },
        onProgress: function () {
          var $multiple = $('#multiple');
          $multiple.parent().addClass('btn-warning');
          $multiple.prev().removeAttr('class').addClass('glyphicon glyphicon-cloud-upload glyphicon-cloud-upload-animate');
        },
        onFinishOne: function (event, response) {
          var obj = JSON.parse(response);
          var $multiple = $('#multiple');
          $('#media_url').val(obj.data.link);
          $multiple.parent().removeClass('btn-warning').addClass('btn-success');
          $multiple.prev().removeAttr('class').addClass('glyphicon glyphicon-check').prop('disabled');
          $multiple.prop('disabled', true);
          $('#uploadMessage').text(Drupal.t('File uploaded successfully'));
        },
        onFinish: function (event, total) {

        }
      });

      Drupal.markaspotOpen311Client.codeLatLng(marker.getLatLng(), 0, 'marker');

      $("#open311_client_submit").click(function (e) {

        e.preventDefault();
        var target = document.getElementById('open311_client_form');
        spinner = new Spinner().spin(target);
        Drupal.markaspotOpen311Client.unhighlight();

        $('#lat').val(latlng.lat);
        $('#long').val(latlng.lng);
        $.post(settings.markaspot_open311_client_endpoint + "/requests.json", $("#open311_client_form").serialize())
          .success(function () {
            spinner.stop();
            Drupal.markaspotOpen311Client.success($('textarea'));
          })
          .error(function (response) {
            spinner.stop();

            var obj = JSON.parse(response.responseText);
            var error = obj[0].description;

            if (error.search("email") >= 0) {
              Drupal.markaspotOpen311Client.highlight($('input[name="email"]'), error);
            } else if (error.search("position") >= 0) {
              Drupal.markaspotOpen311Client.highlight($('input[name="address_string"]'), error);
              $('input[name="address"]').removeAttribute('disabled');
            } else if (error.search("service") >= 0) {
              Drupal.markaspotOpen311Client.highlight($('select'), error);
            }
          });
      });
    }
  };
})(jQuery);
