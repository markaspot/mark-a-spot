/**
 * Mark-a-Spot marker_leaflet.js
 *
 * Main Map-Application File with Leaflet Maps api
 * *
 *
 * @copyright  2012 Holger Kreis <holger@markaspot.org>
 * @link       http://mark-a-spot.org/
 * @version    2.1.2  
 */

var getMarkerId ="";
var markerLayer, queryString ;
(function ($) {
  $(document).ready(function () {
    if ($('#markers-list-view #map').length != 0){
      var offset = $("#markers-list-view #map").offset();
      var topPadding = 120;
      $(window).scroll(function() {
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

    Drupal.Geolocation = new Object();
    Drupal.Geolocation.maps = new Array();
    Drupal.Geolocation.markers = new Array();
    var categoryCond = mas.params.field_category_tid;
    var statusCond = mas.params.field_status_tid;

    var queryString =  mas.params.q.split('?');
    
    var pathId = mas.params.q.split('/');


    /**
     * Split URL and read MarkerID
     *
     */
     
    switch (pathId[0]) {
      case "map":
      case "home":
      case "list":
        readData(1, getMarkerId, "All", "All");
        getMarkerId = '';
      break;
      case "node":
        readData(1, pathId[1], categoryCond, statusCond);
        break;
      case "admin":
      case "overlay":
        return false;
      default:
        return false;
      break;
    }
    
    var initialLatLng = new L.LatLng(mas.markaspot_ini_lat, mas.markaspot_ini_lng);

    Drupal.Geolocation.maps[0] = new L.Map('map');
    var cloudmadeUrl = 'http://{s}.tile.cloudmade.com/'+ mas.cloudmade_api_key +'/22677/256/{z}/{x}/{y}.png',
        cloudmadeAttribution = 'Map data &copy; 2012 OpenStreetMap contributors, Imagery &copy; 2012 CloudMade',
        cloudmade = new L.TileLayer(cloudmadeUrl, {maxZoom: 18, attribution: cloudmadeAttribution});

    Drupal.Geolocation.maps[0].setView(new L.LatLng(mas.markaspot_ini_lat, mas.markaspot_ini_lng), 13).addLayer(cloudmade);

    $("#markers-list").append("<ul>");
  
  
    /**Sidebar Marker-functions*/
     
    $("#block-markaspot-logic-taxonomy-category ul li a.map-menue").click(function(e){
      e.preventDefault();
      hideMarkers();
      readData(1, getMarkerId, getTaxId(this.id), "All");
      return false;
    });
    
    $("#block-markaspot-logic-taxonomy-status ul li a.map-menue").click(function(e){
      e.preventDefault();
      hideMarkers();
      readData(2, getMarkerId, "All", getTaxId(this.id));
      return false;
    });
    
    function getTaxId(id){
      id = id.split("-");
      return id[1];
    }
    

    function readData(getToggle,getMarkerId,categoryCond,statusCond) {
      // markerLayer = new L.LayerGroup();
      markerLayer = new L.MarkerClusterGroup();

      uri = mas.uri.split('?');
      
      if (uri[0] == "/node"){
        url = Drupal.settings.basePath + 'reports/json/' + getMarkerId;
      } else if (uri[0].search('node/') != -1){
        url = Drupal.settings.basePath + 'reports/json/' + getMarkerId;
      } else if (uri[0].search('map') != -1 || uri[0].search('home') != -1 ){
        url = Drupal.settings.basePath + 'reports/json?' + 'field_category_tid=' + categoryCond + '&field_status_tid=' + statusCond;
      } else {
        url = Drupal.settings.basePath + 'reports/json?' + uri[1];
        categoryCond = mas.params.field_category_tid;
        statusCond = mas.params.field_status_tid;
      }
      $("#markersidebar >*").remove();
 
      $.getJSON(url, function(data){
        data = data.nodes;

        points = [];
        var bounds = new L.LatLngBounds(initialLatLng);
        //var infoWindow = new google.maps.InfoWindow;

        if (!data[0] && mas.node_type == 'report') {
          // invoke a message box or something less permanent than an alert box later
          alert(Drupal.t('No Reports found for this category/status'));
        }
        $.each(data, function(markers, item){

          if (item.node.positionLat && item.node.positionLat != mas.markaspot_ini_lat){
            item = item.node;
            var latlon = new L.LatLng(item.positionLat,item.positionLng);
            var html = '<div class="marker-title"><h4><a class="infowindow-link" href="' + item.path + '">' + item.title + '</a></h4><span class="meta-info date">' + item.created + '</span></div>';


            if (item.address){
              html += '<div class="marker-address"><p>'+ item.address + '</p></div><div><a class="infowindow-link" href="' + item.path + '">mehr lesen</a></span>';
            }
            /*
            if (item.Value){
              circleRadius = (parseInt(item.Value) +15) * 10;
              console.log(circleRadius);
            }
            */
            var hex = (categoryCond != "All" && statusCond == "All") ? item.categoryHex : item.statusHex;

            if (getToggle != 3) {
              switch (getToggle) {
                // case 0:
                //   var image = new google.maps.MarkerImage('https://chart.googleapis.com/chart?chs=150x150&cht=qr&chl=Hello%20world&choe=UTF-8');
                //   var shadow = new google.maps.MarkerImage(
                //    'http://maps.gstatic.com/intl/de_ALL/mapfiles/shadow50.png', 
                //     new google.maps.Size(37, 32),
                //     new google.maps.Point(0,0),
                //     new google.maps.Point(13, 32)
                //   );
                // break;
                case 1:
                  var LeafIcon = L.Icon.extend({
                    options: {
                      shadowUrl: Drupal.settings.basePath + 'profiles/markaspot/modules/mark_a_spot/modules/markaspot_logic/img/icons/cartosoft/marker_crts_shadow.png',
                      iconSize: new L.Point(32, 32),
                      shadowSize: new L.Point(52, 33),
                      iconAnchor: new L.Point(30, 14)
                    }
                  });
                  var masIcon = new LeafIcon({iconUrl: Drupal.settings.basePath + 'profiles/markaspot/modules/mark_a_spot/modules/markaspot_logic/img/icons/cartosoft/marker_crts_' + item.categoryHex + '.png'});

                break;
                case 2:
                  var LeafIcon = L.Icon.extend({
                    options: {
                      shadowUrl: 'profiles/markaspot/modules/mark_a_spot/modules/markaspot_logic/img/icons/cartosoft/marker_crts_shadow.png',
                      iconSize: new L.Point(32, 32),
                      shadowSize: new L.Point(52, 33),
                      iconAnchor: new L.Point(0, 14)
                    }
                  });
                  var masIcon = new LeafIcon({iconUrl: 'profiles/markaspot/modules/mark_a_spot/modules/markaspot_logic/img/icons/cartosoft/marker_crts_' + item.statusHex + '.png'});

                break;

              }
              
              var marker = new L.Marker(latlon, {icon: masIcon});
              marker.bindPopup(html)
              markerLayer.addLayer(marker);

              //fn = bindInfoWindow(GoogleMarker,  Drupal.Geolocation.maps[0], infoWindow, html);

            }
   
            bounds.extend(latlon);
            
            img = $('<img class="pull-left thumbnail"/>').attr('src', 'http://staticmaps.cloudmade.com/' + mas.cloudmade_api_key + '/staticmap?&amp;zoom=15&amp;size=100x100&amp;format=png&amp;styleid=22677&amp;center=50.8211546%2C6.895930599999929&amp;markers=size%3Amid%7Ccolor%3Ared%7C50.8211546%2C6.895930599999929center=' + item.positionLat + "," + item.positionLng + '&sensor=true&zoom=13&size=100x100');
            $('.body_' + item.nid).before(img);

            var fn  = markerClickFn(latlon, html);

            if ($("#markersidebar")){
              var li = document.createElement('li');
              var htmlSidebar = '<a id="marker_'+ item.nid +'">'+ item.title +"</a>";
              li.innerHTML = htmlSidebar;
              li.style.cursor = 'pointer';
              $("#markersidebar").append(li);
            }
            $('#marker_'+item.nid).click(fn);
          } 
        }); // $.each
        var bounds = new L.LatLngBounds(points);

        Drupal.Geolocation.maps[0].addLayer(markerLayer);
        // Drupal.Geolocation.maps[0].fitBounds(bounds);
      });
    } 
  });
})(jQuery);



function hideMarkers(){
  Drupal.Geolocation.maps[0].removeLayer(markerLayer);
  return;
};

function bindInfoWindow(marker, map, infoWindow, html) {
  google.maps.event.addListener(marker, 'click', function() {
    Drupal.Geolocation.maps[0].setCenter(marker.getPosition());
    Drupal.Geolocation.maps[0].setZoom(15);
    infoWindow.setContent(html);
    infoWindow.open(map, marker);
  });
}

function markerClickFn(latlon, html) {
  return function() {
    Drupal.Geolocation.maps[0].panTo(latlon); 
    popup = new L.Popup();
 
    popup.setLatLng(latlon);
    popup.setContent(html);
  
    Drupal.Geolocation.maps[0].openPopup(popup);
  };
}

















