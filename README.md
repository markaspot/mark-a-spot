# Mark-a-Spot

Mark-a-Spot is a full packaged Drupal distribution for public civic issue tracking, including an Open311 GeoReport v2 server. Basic Drupal knowledge is required to 
customize it for your own needs.

You are allowed to and *please* do say that your product is based on Mark-a-Spot, inspired by Mark-a-Spot, but you can’t call your product or service Mark-a-Spot.

This software is based on Drupal 7. 
It's source-code is licensed and made available under the GNU General Public License (GPL) version 2.

## Demo
* [Install on simplytest.me](http://simplytest.me/project/markaspot/7.x-2.x) latest commit
* [Installation Video](https://vimeo.com/43443940)

## Initial configuration
1. Make sure that clean-urls are supported and active: http://yourserver/?q=admin/config/search/clean-urls

2. Start with changing the field settings of the field_geo field by choosing a starting position.
http://yourserver/admin/structure/types/manage/report/fields

3. Copy and paste the lat/lon values into the settings of the Mark-a-Spot configuration screen
http://yourserver/admin/config/system/mark_a_spot

4. Go to Structure / Taxonomy and configure the terms of category and status vocabularies. There you can just overwrite the default term-name, change colors and icons. 

## Open311 GeoReport Resources
http://yourserver/georeport/v2/services.format (xml/json)
http://yourserver/georeport/v2/requests.format (xml/json)
http://yourserver/georeport/v2/discovery.format (xml/json)

## Updating from 2.5 or 2.6
After cloning the repository run update.php. Beware that the data model of the category and status terms have changed.
Depending on your theme you have to update your templates for displaying status and category colors. Color field module and Icon API modules are required.

## Contact
Holger Kreis | @markaspot | http://mark-a-spot.org

