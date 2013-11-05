# Mark-a-Spot

Mark-a-Spot is a full packaged Drupal distribution for public civic issue tracking, including an Open311 GeoReport v2 server.

You are allowed to and *please* do say that your product is based on Mark-a-Spot, inspired by Mark-a-Spot, but you can’t call your product or service Mark-a-Spot.

This software is based on Drupal 7.
It's source-code is licensed and made available under the GNU General Public License (GPL) version 2.

## Demo
* [Demo-Site](http://markaspot.de/master)
* [Installation Video](https://vimeo.com/43443940)

## Initial configuration

1. Make sure that clean urls are supported and active: http://yourserver/?q=admin/config/search/clean-urls
2. If you choose to keep OSM as map type, get a cloudmade API Key and enter it there.
3. Cang the field settings of the field_geo field by choosing a starting position (Structure > Content Type > Report)
http://yourserver/admin/structure/types/manage/report/fields
3. Copy and paste the lat/lon values into the settings of the Mark-a-Spot configuration screen
http://yourserver/admin/config/system/mark_a_spot


## Open311 GeoReport Resources

http://yourserver/georeport/services.format (xml/json)
http://yourserver/georeport/requests.format (xml/json)
http://yourserver/georeport/discovery.format (xml/json)

All relevant information of the discovery resource can be added:
http://yourserver/admin/config/system/mark_a_spot


## Contact
Holger Kreis | @markaspot | http://mark-a-spot.org
