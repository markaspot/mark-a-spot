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

## Open311 GeoReport Resources

http://yourserver/georeport/v2/services.format (xml/json)
http://yourserver/georeport/v2/requests.format (xml/json)
http://yourserver/georeport/v2/discovery.format (xml/json)

## Contact
Holger Kreis | @markaspot | http://mark-a-spot.org


## Changelog

For a full changelog please see git log at drupal.org repository

### 2.6

- Issue #2326503: Adds an offset setting for new report ids (7 hours ago) <markaspot>
- Fix git project url for geolocation_osm submodule (5 weeks ago) <markaspot>
- Improved pop up handling, map filtering (6 weeks ago) <markaspot>
- Fix markerColor json object, added iconColor for markers (6 weeks ago) <markaspot>
- Fix location icon (6 weeks ago) <markaspot>
- Issue #2326503: Provide a more flexible way of creating uuids (6 weeks ago) <markaspot>
- Fix colorswitching on different modes (status/categories) (6 weeks ago) <markaspot>
- Added reference module, fixed link of core (6 weeks ago) <markaspot>* 83702f2 - Updated make files (6 weeks ago) <markaspot>
- Color changes in accordance to leaflet awesome marker (6 weeks ago) <markaspot>
- Issue #2319147: Added Mark-a-Spot Static GeoJSON file Generator (6 weeks ago) <markaspot>
- Issue #2319149: Refactored Map Visualization as Drupal Behaviour (6 weeks ago) <markaspot>
- Add custom CSS to ember theme (6 weeks ago) <markaspot>
- Issue #2320001: Report Form Tab fixed, added Photo Button (7 weeks ago) <markaspot>
- Issue #2321559 Icon alignment, Hex Changes, Nav ... (7 weeks ago) <markaspot>
- Decrease map size for admin theme (8 weeks ago) <markaspot>
- Log entries should be deleted on node_delete() (5 months ago) <markaspot>
- Issue #2255309: Adding Radar module (5 months ago) <markaspot>
- Issue #2222673 by tormi: Added Leaflet Locate Control Plugin for easier initial location (5 months a
- Switched views to geojson for leaflet map type (5 months ago) <markaspot>
- Security Fix Core 7.27 (6 months ago) <markaspot>
- Issue #2222167, #2221871 by tormi: Refactoring validation and ui for bounding box definition and mul
- Issue #2221173 by Carlos Miranda Levy: Made icon field mandatory (7 months ago) <markaspot>
- Issue #2220161 by Carlos Miranda Levy: Change link to theme related path (7 months ago) <markaspot>
- Updated twbs to 3.1.1 (7 months ago) <markaspot>
- Issue #2217987 by Carlos Miranda Levy: Using taxonomy_term_delete (7 months ago) <markaspot>
- Hide default address on focus geolocation address (7 months ago) <markaspot>
- Added report logging as responsive timeline (7 months ago) <markaspot>
- Added options and configuration to generate UUID titles (7 months ago) <markaspot>
- Set initial map type to OSM (7 months ago) <markaspot>
- Fixed node count for uuid creation (7 months ago) <markaspot>
- Added OSM as default setting (post installation) (8 months ago) <markaspot>

### 2.5

- Reformated less compiled styles, eliminated some more errors (8 months ago) <markaspot>
- Markup- and horizontal scroll fix on smaller devices (8 months ago) <markaspot>
- Small hook_validate() fix (9 months ago) <markaspot>
- Delete google maps components from branch (9 months ago) <markaspot>
- Outsourcing of google maps components (9 months ago) <markaspot>
- Show bootstrap thumbnail only if image is available (9 months ago) <markaspot>- Added thumbnail class to node template (9 months ago) <markaspot>
- Logo responsiveness fixes (9 months ago) <markaspot>
- Changed admin theme (9 months ago) <markaspot>
- Renaming of title field with service_name (9 months ago) <markaspot>
- Fix for initial node->status (9 months ago) <markaspot>
- Changed order of form items (9 months ago) <markaspot>>
- Form Style changes (9 months ago) <markaspot>
- Fixed status handling (9 months ago) <markaspot>

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
