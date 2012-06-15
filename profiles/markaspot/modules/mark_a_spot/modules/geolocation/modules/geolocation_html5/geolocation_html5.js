
/**
 * @file
 * Javascript for Geolocation Field.
 */
(function ($) {
  function latitudeToMercator(latitude) {
    return Math.log(Math.tan(latitude * Math.PI / 180) + 1 / Math.cos(latitude * Math.PI / 180));
  }

  Drupal.latitudeToPx = function (latitude, topLatitude, bottomLatitude, height) {
    return (latitudeToMercator(latitude) - latitudeToMercator(bottomLatitude)) / (latitudeToMercator(topLatitude) - latitudeToMercator(bottomLatitude)) * height;
  }

  Drupal.longitudeToPx = function (longitude, leftLongitude, width) {
    return (longitude - leftLongitude + 360) / 360 % 1 * width;
  }

  function plot(widgetParentElement) {
    var thisMap = $('.geolocation-html5-map', widgetParentElement);
    var latitude = $('input.geolocation-lat', widgetParentElement).attr('value');
    var longitude = $('input.geolocation-lng', widgetParentElement).attr('value');

    if (latitude == 0 && longitude == 0) {
      return;
    }

    var latPx = Math.round( Drupal.latitudeToPx(latitude, 78, -58, thisMap.height()) );
    var lngPx = Math.round( Drupal.longitudeToPx(longitude, -168, thisMap.width()) );

    // Plot coords on map.
    $('.dot', thisMap).show();
    $('.dot', thisMap).css('bottom', latPx + 'px');
    $('.dot', thisMap).css('left', lngPx + 'px');
  }

  function getLocation(widgetParentElement) {
    var thisMap = $('.geolocation-html5-map', widgetParentElement);
    var messages = $('.geolocation-html5-messages', widgetParentElement);
    var busyMessage = $('.geolocating', messages);
    if ($('.form-type-checkbox input', widgetParentElement).attr('checked')) {
      thisMap.slideDown('fast', function() {
        // Get position
        busyMessage.show();
        navigator.geolocation.getCurrentPosition(function (position) {
          // Save coords to hidden inputs.
          $('input.geolocation-lat', widgetParentElement).attr('value', position.coords.latitude);
          $('input.geolocation-lng', widgetParentElement).attr('value', position.coords.longitude);

          plot(widgetParentElement);
          busyMessage.hide();
        }, function () { // getCurrentPosition error callback
          busyMessage.hide();
          $('.form-type-checkbox input', widgetParentElement).attr('checked', false).change();
          $('.form-type-checkbox input', widgetParentElement).attr('disabled', true);
        },
        {
          maximumAge: Infinity
        });
      });
    }
    else { // Location not checked
      busyMessage.hide();
      $('.dot', thisMap).attr('style', '');
      $('input.geolocation-lat', widgetParentElement).attr('value', '');
      $('input.geolocation-lng', widgetParentElement).attr('value', '');
      thisMap.slideUp('fast');
    }
  }

  Drupal.behaviors.GeolocationHTML5 = {
    attach: function(context, settings) {
      if (navigator.geolocation) {
        $('.field-type-geolocation-latlng .form-type-checkbox input:not(.geolocation-html5-processed)', context)
        .change(function () {
          var widgetParentElement = $(this).parent().parent();
          getLocation(widgetParentElement);
        })
        .each(function () {
          $(this).addClass('geolocation-html5-processed');
          var widgetParentElement = $(this).parent().parent();
          $('.form-type-checkbox .description', widgetParentElement).html(Drupal.t('Your location will be saved and may be shared.'));
          getLocation(widgetParentElement);
        });
      }
      else { // HTML5 Geolocation not supported
        // Hide all map widgets and disable checkboxes.
        $('.geolocation-html5-map').hide();
        $('.field-type-geolocation-latlng .form-type-checkbox input').attr('disabled', true);
      }
    }
  };
})(jQuery);
