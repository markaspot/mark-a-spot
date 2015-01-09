CONTENTS OF THIS FILE
---------------------

* Summary
* Requirements
* Installation
* Usage
  * Bounding Box Support
* To Do
* Credits
* Current Maintainers


SUMMARY
-------

Views GeoJSON is a style plugin for Views to deliver location-specific data in GeoJSON format, (see GeoJSON spec: http://geojson.org/geojson-spec.html).

Each row is output as a GeoJSON "Features" including geospatial data and optional metadata. All features are wrapped in a "Feature Collection" object.


REQUIREMENTS
------------

Drupal modules:
* Views - http://drupal.org/project/views
* geoPHP - http://drupal.org/project/geophp


INSTALLATION
------------

Install the module as usual. See
http://drupal.org/documentation/install/modules-themes/modules-7 for help.


USAGE
-----

1. Create a View with a Feed display on content with geospatial data.
2. Add fields to output a Geofield, longitude & latitude or WKT.
3. Optionally add fields for name and description.
4. Set Format for the display to "GeoJSON Feed".
5. In the "Style options" for this format:
  * Choose Map Data Source (Other: Lat/Lon Point, Geofield, etc.)
  * Assign the fields that represent Latitude and Longitude.
  * Optionally choose fields representing name and description for each point.
  * Optionally set a JSONP prefix, (see http://en.wikipedia.org/wiki/JSONP).
  * Set the Content-type header to be sent. The default is "application/json",
    the standard MIME type for JSON, per http://www.ietf.org/rfc/rfc4627.txt.
6. If using with the OpenLayers module, your new GeoJSON layer will be available
  as an Overlay layer on the "Layers & Styles" tab when editing your map.

Bounding Box Filtering
--------------------
GeoJSON views can accept a bounding box as an argument to return only the
points within that box.

It has been tested with OpenLayers' Bounding Box Strategy but should work
with any mapping tool that requests bounding box coordinates as
"?bbox=left,bottom,right,top" in the query string. Argument ID "bbox" is
default for OpenLayers but can be changed.

OpenLayers 7.x-2.x-dev is currently required for OpenLayers BBOX integration.
See OpenLayers issue http://drupal.org/node/1493344, "Support BBOX strategy in
the GeoJSON layer type".

1. Create a GeoJSON view as above in USAGE.
2. Add a layer to OpenLayers of type GeoJSON, at
   admin/structure/openlayers/layers/add/openlayers_layer_type_geojson,
   specifying the URL to your GeoJSON feed and checking the box for "Use
   Bounding Box Strategy".
3. In your GeoJSON View configuration, add a Contextual Filter of type:
   "Custom: Bounding box".
4. In the Contextual Filter settings, under "When the filter value is NOT in
   the URL as a normal Drupal argument", choose: "Provide default value".
5. In the "Type" dropdown, choose: "Bounding box from query string".
6. For OpenLayers, leave "Query argument ID" as "bbox" and click Apply.


TO DO
-----
* Support addditional GeoJSON feature types like LineString.
* Support an optional altitude coordinate for Point positions.
* Support additional coordinate systems.


CREDITS
-------

This module was originally born from a patch by tmcw to the OpenLayers module
(http://drupal.org/node/889190#comment-3376628) and adapted to model the Views
Datasource module (http://drupal.org/project/views_datasource).

Much of the code is drawn directly from these sources.


CURRENT MAINTAINERS
-------------------

* jeffschuler - http://drupal.org/user/239714
