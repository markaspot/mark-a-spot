# Mark-a-Spot

Mark-a-Spot is a full packaged Drupal distribution for public civic issue tracking, including an Open311 GeoReport v2 server.

You are allowed to and *please* do say that your product is based on Mark-a-Spot, inspired by Mark-a-Spot, but you can’t call your product or service Mark-a-Spot. 

This software is based on Drupal 7. 
It's source-code is licensed and made available under the GNU General Public License (GPL) version 2. 

## Demo
* [MaS-City Demo-Site](http://mas-city.com)
* [Installation Video](https://vimeo.com/43443940)

## Initial configuration

1. Start with changing the field settings of the field_geo field by choosing a starting position.
http://yourserver/admin/structure/types/manage/report/fields
2. Copy and paste the lat/lon values into the settings of the Mark-a-Spot configuration screen
http://yourserver/admin/config/system/mark_a_spot
3. Make sure that clean urls are supported and active: http://yourserver/?q=admin/config/search/clean-urls

In case you want to run the system with OSM, you may choose the Cloudmade Geolocation Widget and switch to Cloudmade's map tiles in the Mark-a-Spot configuration screen. You will need an Cloudmade API Key for that. 

Make sure to get a Bing Maps API Key to use geocoding services for OSM. Of course you may choose any other service for that.

## Open311 GeoReport Resources 

http://yourserver/georeport/services.format (xml/json)
http://yourserver/georeport/requests.format (xml/json)
http://yourserver/georeport/discovery.format (xml/json)

## Contact
Holger Kreis | @markaspot | http://mark-a-spot.org