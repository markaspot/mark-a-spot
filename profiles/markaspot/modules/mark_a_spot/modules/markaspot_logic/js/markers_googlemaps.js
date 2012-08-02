
/**
 * Mark-a-Spot marker_googlemaps.js
 *
 * Main Map-Application File with google Maps api
 * *
 *
 * @copyright  2012 Holger Kreis <holger@markaspot.org>
 * @link       http://mark-a-spot.org/
 * @version    2.0  
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
    
    var initialLatLng = new google.maps.LatLng(mas.markaspot_ini_lat, mas.markaspot_ini_lng);
    var myOptions = {
      center: initialLatLng,
      zoom: 13,
      draggableCursor: 'point',
      mapTypeId: google.maps.MapTypeId.ROADMAP,
      mapTypeControlOptions: {
        mapTypeIds: [google.maps.MapTypeId.ROADMAP,google.maps.MapTypeId.SATELLITE],
        style: google.maps.MapTypeControlStyle.DROPDOWN_MENU
      }
    };
  
    var styles = [[{
      url: '/sites/all/themes/mas/images/google_multi_marker_blue.png',
      height: 66,
      width: 68,
      anchor: [9, 0],
      textColor: '#fff',
      textSize: 13
    }]];
  
    //var mcOptions = {gridSize: 50, maxZoom: 15, styles:style};
    
    Drupal.Geolocation.maps[0] = new google.maps.Map(document.getElementById("map"), myOptions);
    var mc = new MarkerClusterer(Drupal.Geolocation.maps[0], [],{maxZoom: 12, gridSize:10,styles:styles[0]});
    
    
    $("#markers-list").append("<ul>");
  
  
    /**Sidebar Marker-functions*/
     
    $("#block-markaspot-logic-taxonomy-category div div a.map-menue").click(function(e){
      e.preventDefault();
      hideMarkers();
      readData(1, getMarkerId, getTaxId(this.id), "All");
      return false;
    });
    
    $("#block-markaspot-logic-taxonomy-status div div a.map-menue").click(function(e){
      e.preventDefault();
      hideMarkers();
      readData(2, getMarkerId, "All", getTaxId(this.id));
      return false;
    });
    
    function getTaxId(id){
      id = id.split("-");
      return id[1];
    }
    
    function hideMarkers(){
     if(mc !== null) {
       mc.clearMarkers();
     }
     return;
    };

    function readData(getToggle,getMarkerId,categoryCond,statusCond) {
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
  
        var bounds = new google.maps.LatLngBounds(initialLatLng);
        var infoWindow = new google.maps.InfoWindow;

        if (!data[0] && mas.node_type == 'report') {
          // invoke a message box or something less permanent than an alert box later
          alert(Drupal.t('No Reports found for this category/status'));
        }
        $.each(data, function(markers, item){

          if (item.node.positionLat && item.node.positionLat != mas.markaspot_ini_lat){
            item = item.node;
            var latlon = new google.maps.LatLng(item.positionLat,item.positionLng);
            var html = '<div class="marker-title"><h4><a class="infowindow-link" href="' + item.path + '">' + item.title + '</a></h4><span class="meta-info date">' + item.created + '</span></div>';


            if (item.address){
              html += '<div class="marker-address"><p>'+ item.address + '</br>' + item.zip + ' ' + item.city + '</p></div><div><a class="infowindow-link" href="' + item.path + '">mehr lesen</a></span>';
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
                case 0:
                  var image = new google.maps.MarkerImage('https://chart.googleapis.com/chart?chs=150x150&cht=qr&chl=Hello%20world&choe=UTF-8');
                  var shadow = new google.maps.MarkerImage(
                   'http://maps.gstatic.com/intl/de_ALL/mapfiles/shadow50.png', 
                    new google.maps.Size(37, 32),
                    new google.maps.Point(0,0),
                    new google.maps.Point(13, 32)
                  );
                break;
                case 1:
                  var image = new google.maps.MarkerImage('http://chart.apis.google.com/chart?cht=mm&chs=32x32&chco=ffffff,' + item.categoryHex + ',333333&ext=.png');
                  var shadow = new google.maps.MarkerImage(
                   'http://maps.gstatic.com/intl/de_ALL/mapfiles/shadow50.png', 
                    new google.maps.Size(37, 32),
                    new google.maps.Point(0,0),
                    new google.maps.Point(13, 32)
                  );
                break;

                case 2:
                  var image = new google.maps.MarkerImage('http://chart.apis.google.com/chart?cht=mm&chs=32x32&chco=ffffff,' + item.statusHex + ',000000&ext=.png');
                  var shadow = new google.maps.MarkerImage(
                   'http://maps.gstatic.com/intl/de_ALL/mapfiles/shadow50.png', 
                    new google.maps.Size(37, 32),
                    new google.maps.Point(0,0),
                    new google.maps.Point(13, 32)
                  );
                break;

              }
              
              var GoogleMarker = new google.maps.Marker({
                position: latlon,
                map: Drupal.Geolocation.maps[0],
                icon: image,
                shadow: shadow,
                title: 'Klicke für Detail'
              });

              fn = bindInfoWindow(GoogleMarker,  Drupal.Geolocation.maps[0], infoWindow, html);

            }
            mc.addMarker(GoogleMarker);
            bounds.extend(latlon);
           

            if ($("#markersidebar")){
              var li = document.createElement('li');
              var htmlSidebar = '<a id="marker_'+ item.nid +'">'+ item.title +"</a>";
              li.innerHTML = htmlSidebar;
              li.style.cursor = 'pointer';
              $("#markersidebar").append(li);
            }
            $('#marker_'+item.nid).click(function(){
                $(this).css('background-color', '#ccddee')
                $(this).animate({ backgroundColor: "black" }, 1000);
                google.maps.event.trigger(GoogleMarker, 'click');
            });
          } 
        }); // $.each
        Drupal.Geolocation.maps[0].fitBounds(bounds);
        var listener = google.maps.event.addListener(Drupal.Geolocation.maps[0], "idle", function() { 
          if (Drupal.Geolocation.maps[0].getZoom() > 14) Drupal.Geolocation.maps[0].setZoom(14); 
          google.maps.event.removeListener(listener); 
        });
      });
    } 
  });
})(jQuery);



function hideMarkers(){
 if(mc !== null) {
   mc.clearMarkers();
 }
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