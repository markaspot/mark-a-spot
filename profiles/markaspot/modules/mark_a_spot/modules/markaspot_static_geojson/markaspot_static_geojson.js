(function ($) {
  Drupal.markaspot_static_geojson =  Drupal.markaspot_static_geojson || {};
  Drupal.behaviors.markaspot_static_geojson = {
    attach: function(context, settings) {
    }
  };
  Drupal.markaspot_static_geojson.getData = function (){
    $.ajax({
      url: "sites/default/files/geojson/reports.json",
      async: false
    })
    .done(function( data ) {
        geoJson = data;
    });
    return geoJson;
  }
})(jQuery)