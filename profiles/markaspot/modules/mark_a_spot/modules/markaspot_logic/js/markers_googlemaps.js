/**
 * Mark-a-Spot marker_googlemaps.js
 *
 * Main Map-Application File with Google Maps Api
 *
 */

var arg = "";
var markerLayer, queryString;
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
        };
      });
    }
    var mas = Drupal.settings.mas;

    Drupal.Markaspot = new Object();
    Drupal.Markaspot.maps = new Array();
    Drupal.Markaspot.markers = new Array();
    var categoryCond = mas.params.field_category_tid;
    var statusCond = mas.params.field_status_tid;
    var queryString = mas.params.q.split('?');

    var pathId = mas.params.q.split('/');

    switch (pathId[0]) {
      case "map":
        readData(1, arg, "All", "All");
        arg = '';
        break;

      case "list":
        readData(2, "list", "All", "All");
        break;

      case "admin":
      case "overlay":
        return false;
    }

    var initialLatLng = new google.maps.LatLng(mas.markaspot_ini_lat, mas.markaspot_ini_lng);
    var myOptions = {
      center: initialLatLng,
      zoom: 13,
      draggableCursor: 'point',
      mapTypeId: google.maps.MapTypeId.ROADMAP
    };

    Drupal.Markaspot.maps[0] = new google.maps.Map(document.getElementById("map"), myOptions);

    $("#markers-list").append("<ul>");

    /**Sidebar Marker-functions*/

    $("#block-markaspot-logic-taxonomy-category ul li a.map-menue").click(function (e) {
      e.preventDefault();
      hideMarkers();
      readData(1, getMarkerId, getTaxId(this.id), "All");
      return false;
    });

    $("#block-markaspot-logic-taxonomy-status ul li a.map-menue").click(function (e) {
      e.preventDefault();
      hideMarkers();
      readData(2, getMarkerId, "All", getTaxId(this.id));
      return false;
    });

    $("a.map-all.category").click(function (e) {
      e.preventDefault();
      hideMarkers();
      readData(1, getMarkerId, "All", "All");
      return false;
    });

    $("a.map-all.status").click(function (e) {
      e.preventDefault();
      hideMarkers();
      readData(2, getMarkerId, "All", "All");
      return false;
    });

    function getTaxId(id) {
      id = id.split("-");
      return id[1];
    }

    function hideMarkers() {
      if (mc !== null) {
        mc.clearMarkers();
      }
    };

    function readData(getToggle, arg, categoryCond, statusCond) {

      uri = mas.uri.split('?');

      if (mas.node_type == "report") {
        url = Drupal.settings.basePath + 'reports/json/' + arg;
      } else if (uri[0].search('node/') != -1) {
        url = Drupal.settings.basePath + 'reports/json/' + arg;
      } else if (uri[0].search('map') != -1 || uri[0].search('home') != -1) {
        // map view
        url = Drupal.settings.basePath + 'reports/json/map?' + 'field_category_tid=' + categoryCond + '&field_status_tid=' + statusCond;
      } else {
        url = Drupal.settings.basePath + 'reports/json?' + uri[1];
        categoryCond = mas.params.field_category_tid;
        statusCond = mas.params.field_status_tid;
      }

      $("#markersidebar >*").remove();

      $.getJSON(url, function (data) {

        data = data.nodes;
        points = [];

        var bounds = new google.maps.LatLngBounds(initialLatLng);
        var infoWindow = new google.maps.InfoWindow;
        var markers = [];
        if (!data[0] && mas.node_type == 'report') {
          // invoke a message box or something less permanent than an alert box later
          alert(Drupal.t('No Reports found for this category/status'));
        } else if (!data[0] && mas.params.q.indexOf("map") != -1) {
          alert(Drupal.t('No Reports found for this category/status'));
        }
        $.each(data, function (index, item) {
          if (item.node.positionLat && item.node.positionLat != mas.markaspot_ini_lat) {
            item = item.node;
            var latlon = new google.maps.LatLng(item.positionLat, item.positionLng);
            var html = '<div class="marker-title"><h4><a class="infowindow-link" href="' + item.path + '">' + item.title + '</a></h4><span class="meta-info date">' + item.created + '</span></div>';

            if (item.address) {
              html += '<div class="marker-address"><p>' + item.address + '</br>' + '</p></div><div><a class="infowindow-link" href="' + item.path + '">' + Drupal.t('read more') + '</a></span>';
            }

            if (getToggle == 1) {
              var hex = item.categoryHex;
            }
            if (getToggle == 2) {
              var hex = item.statusHex;
            }

            var image = new google.maps.MarkerImage('http://chart.apis.google.com/chart?cht=mm&chs=32x32&chco=ffffff,' + hex + ',333333&ext=.png');
            var shadow = new google.maps.MarkerImage(
              'http://maps.gstatic.com/intl/de_ALL/mapfiles/shadow50.png',
              new google.maps.Size(37, 32),
              new google.maps.Point(0, 0),
              new google.maps.Point(13, 32)
            );

            var GoogleMarker = new google.maps.Marker({
              position: latlon,
              map: Drupal.Markaspot.maps[0],
              icon: image,
              shadow: shadow,
            });

            fn = bindInfoWindow(GoogleMarker, Drupal.Markaspot.maps[0], infoWindow, html, item.nid);

            bounds.extend(latlon);
            markers.push(GoogleMarker);

            img = $('<img class="pull-left thumbnail"/>').attr('src', 'http://maps.googleapis.com/maps/api/staticmap?center=' + item.positionLat + "," + item.positionLng + '&sensor=true&zoom=13&size=100x100');
            $('.body_' + item.nid).before(img);

            if ($("#markersidebar")) {
              var li = document.createElement('li');
              var htmlSidebar = '<a id="marker_' + item.nid + '">' + item.title + "</a>";
              li.innerHTML = htmlSidebar;
              li.style.cursor = 'pointer';
              $("#markersidebar").append(li);
            }
            $('#marker_' + item.nid).hover(function () {
              $(this).css('background-color', '#ccddee');
              $(this).animate({ backgroundColor: "black" }, 1000);
              google.maps.event.trigger(GoogleMarker, 'click');
            });
          }
        });
        // $.each

        var mc = new MarkerClusterer(Drupal.Markaspot.maps[0], markers, {maxZoom: 15, gridSize: 50});

        Drupal.Markaspot.maps[0].fitBounds(bounds);
        var listener = google.maps.event.addListener(Drupal.Markaspot.maps[0], "idle", function () {
          if (Drupal.Markaspot.maps[0].getZoom() > 14) {
            Drupal.Markaspot.maps[0].setZoom(14);
          }
          google.maps.event.removeListener(listener);
        });
      });
    }
  });
})(jQuery);


function hideMarkers() {
  if (mc !== null) {
    mc.clearMarkers();
  }
};

function bindInfoWindow(marker, map, infoWindow, html, nid) {
  google.maps.event.addListener(marker, 'click', function () {
    Drupal.Markaspot.maps[0].setCenter(marker.getPosition());
    infoWindow.setContent(html);
    infoWindow.open(map, marker);
  });
}
