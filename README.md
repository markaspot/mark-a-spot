# Mark-a-Spot

Mark-a-Spot is a full packaged Drupal distribution for public civic issue tracking, including an Open311 GeoReport v2 server.

You are allowed to and *please* do say that your product is based on Mark-a-Spot, inspired by Mark-a-Spot, but you can’t call your product or service Mark-a-Spot.

This software is based on Drupal 7.
It's source-code is licensed and made available under the GNU General Public License (GPL) version 2.

## Demo
* [MaS-City Demo-Site](http://markaspot.de/master) Mostly new commits
* [Installation Video](https://vimeo.com/43443940)

## Initial configuration

1. Start with changing the field settings of the field_geo field by choosing a starting position.
http://yourserver/admin/structure/types/manage/report/fields
2. Copy and paste the lat/lon values into the settings of the Mark-a-Spot configuration screen
http://yourserver/admin/config/system/mark_a_spot
3. Make sure that clean urls are supported and active: http://yourserver/?q=admin/config/search/clean-urls

In case you want to run the system with Cloudmade OSM, you may choose the Cloudmade Geolocation Widget and switch to Cloudmade's map tiles in the Mark-a-Spot configuration screen. You will need an Cloudmade API Key for that.

## Open311 GeoReport Resources

http://yourserver/georeport/v2/services.format (xml/json)
http://yourserver/georeport/v2/requests.format (xml/json)
http://yourserver/georeport/v2/discovery.format (xml/json)

## Contact
Holger Kreis | @markaspot | http://mark-a-spot.org


## Changelog
### 2.4-beta
- Bootstrap 3.0 Theme Update
- Generic Changes to Mark-a-Spot sub theme helps on small screen
- UUID support for GeoReport endpoint and Userinterface / auto path
- Added Geolocation OSM Module to profile and enable this as default after installation
- Fixed display of awesome Markers on retina displays
- Fixed an issue with tab when creating reports
- Some CSS enhancements for geolocation  when creating forms
- Added OSM as default tile serving operator
- Issue #2032227 use site-name and slogan as default content on front-page
- Issue #2140913 Setting of clean_urls during installation
- Issue #2151909 add translation wrapper
- Issue #2141395 Applied Patch for default content image
- Fixed issues with bootstrap 3.x button drop-downs
- Added Chosen library for better select boxes
- Updated Drupal core to 7.24
- Updated spin.js
