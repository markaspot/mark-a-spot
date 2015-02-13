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

## Changelog
For a full changelog please see git log at drupal.org repository

### 2.7
* Add twitter bootstrap icon class
* Move Mark-a-Spot font module to right place
* Apply admin edit style on add-form, too
* Refactor map filter menu with bootstrap group list and leaflet button
* Fix status_sub handling in extension mode
* Update default map to CartoDB
* Specify Icon API Version to see menu_get_item call removed from module
* Add another task for flushing cashes
* Switch install process to BatchAPI
* Close popUp if report form is shown
* Move and increase xdebug max nesting level
* Fix eye icon on front page's default content block
* Fix list styles and usability for smartphone screens
* Add more colors to front page
* Fix parameter for module_enable
* Fix footer height and bg-color
* Include address_string in form
* Enable Icon API at the end of installation tasks
* [#2161375](http://drupal.org/node/2161375): remove patch for testing
* Clean up and increase max_execution_time for install processing
* Add Icon API module as dependency for dstribution
* Fix subdir key for vbo
* [#2178117](http://drupal.org/node/2178117) by [tormi](/u/tormi): Add VBO for bulk operations with reports
* Enable Open311 Client module on install
* Add update methods for changing icon field types
* Add icon bundle for default content creation
* Fix markup for icons within map filters
* [#2421939](http://drupal.org/node/2421939): Update MaS Feature
* [#2421939](http://drupal.org/node/2421939) Update theme, javascript and dependencies for Icon API Integration
* [#2421939](http://drupal.org/node/2421939): Add a new field type (indexable)
* [#2421939](http://drupal.org/node/2421939): Add Mark-a-Spot font as icon bundle for Icon API
* Update filter identifier for map geojson view
* Update Bootstrap version to 3.3.2
* Fix e-mail validation, add role check for updating request
* Adjust and add button options for reporting
* Fix footer in report list by adding a class selector
* Add [#7210](http://drupal.org/node/7210) function to revert feature on update
* Change footer display on default pages
* Let markers load on front page
* Change git url to "get" bootbox release
* Fix libraries dependencies and paths
* [#2416017](http://drupal.org/node/2416017): Add Open311 client module as initial commit
* Refactor node edit form, change base table for use of Search API
* Add some bootstrap list classes, fix concatination
* Disable user notify on loading the edit form
* Issues [#2415987](http://drupal.org/node/2415987), [#2415763](http://drupal.org/node/2415763): Update feature for list view, add modules and libraries for distro
* Add an Issue ID search block, remove install file
* [#2415769](http://drupal.org/node/2415769): Add custom date() pattern for uuid creation
* Add extended support and update method to API
* Prepare default theme concerning list, navigation and map view
* Add update for icon choice widget as options
* Fix logging dependent on Roles
* Fix centering marker on map load
* Check if description field is not empty
* Apply colors to map taxonomy filter block
* Fix colored spots in radar module
* Enable module before checking if module exists
* Updade CKEditor module version (security)
* Remove admin menu toolbar, add dependencies
* Add logo to Mark-a-Spot Navbar menu item
* [#2376787](http://drupal.org/node/2376787): Add CKEditor to list of modules and libraries
* [#2376813](http://drupal.org/node/2376813): Add Navbar module to Distro, prepare Ckeditor addition
* Fix Drupal core download link in make file
* [#2352225](http://drupal.org/node/2352225): Fix hex codes in js code of radar module
* [#2352441](http://drupal.org/node/2352441): Add a rules based feature for basic notification workflow
* [#2374889](http://drupal.org/node/2374889) by [fredguth](/u/fredguth): Remove german terms from admin feature
* Clean up code
* Fix marker rendering hex codes
* Improve uuid creation
* Add "single-leg" oauth module for simple api_key authorization
* Add setting for intitial radar zoom setting on creating new reports
* Add a drush command for the use of crons, experimental stuff
* Check configuration setting at saving new reports
* [#2352225](http://drupal.org/node/2352225): Adding some update hooks for updating existing sites
* Optimize some script code, rearrange popup
* Make MaS Features revertable / status default after installation
* Update core to 7.33
* Add several improvements for configuration
* [#2352225](http://drupal.org/node/2352225): Turn Icon field type to text list
* [#2352225](http://drupal.org/node/2352225): Adding Color Field to setup and feature
* Fix category and status css for map page
* Remove reference to Cloudmade, add some changelog information


### 2.6

* Issue #2326503: Adds an offset setting for new report ids
* Fix git project url for geolocation_osm submodule
* Improved pop up handling, map filtering
* Fix markerColor json object, added iconColor for markers
* Fix location icon
* Issue #2326503: Provide a more flexible way of creating uuids
* Fix colorswitching on different modes (status/categories
* Added reference module, fixed link of core
* Updated make files
* Color changes in accordance to leaflet awesome marker
* Issue #2319147: Added Mark-a-Spot Static GeoJSON file Generator
* Issue #2319149: Refactored Map Visualization as Drupal Behaviour
* Add custom CSS to ember theme
* Issue #2320001: Report Form Tab fixed, added Photo Button
* Issue #2321559 Icon alignment, Hex Changes, Nav ... 
* Decrease map size for admin theme 
* Log entries should be deleted on node_delete() 
* Issue #2255309: Adding Radar module 
* Issue #2222673 by tormi: Added Leaflet Locate Control Plugin for easier initial location (5 months a
* Switched views to geojson for leaflet map type 
* Security Fix Core 7.27 
* Issue #2222167, #2221871 by tormi: Refactoring validation and ui for bounding box definition and mul
* Issue #2221173 by Carlos Miranda Levy: Made icon field mandatory 
* Issue #2220161 by Carlos Miranda Levy: Change link to theme related path 
* Updated twbs to 3.1.1 
* Issue #2217987 by Carlos Miranda Levy: Using taxonomy_term_delete 
* Hide default address on focus geolocation address 
* Added report logging as responsive timeline 
* Added options and configuration to generate UUID titles 
* Set initial map type to OSM 
* Fixed node count for uuid creation 
* Added OSM as default setting (post installation) 

### 2.5

* Reformated less compiled styles, eliminated some more errors 
* Markup* and horizontal scroll fix on smaller devices 
* Small hook_validate() fix 
* Delete google maps components from branch 
* Outsourcing of google maps components 
* Show bootstrap thumbnail only if image is available * Added thumbnail class to node template 
* Logo responsiveness fixes 
* Changed admin theme 
* Renaming of title field with service_name 
* Fix for initial node->status 
* Changed order of form items >
* Form Style changes 
* Fixed status handling 

### 2.4-beta

* Bootstrap 3.0 Theme Update
* Generic Changes to Mark-a-Spot sub theme helps on small screen
* UUID support for GeoReport endpoint and Userinterface / auto path
* Added Geolocation OSM Module to profile and enable this as default after installation
* Fixed display of awesome Markers on retina displays
* Fixed an issue with tab when creating reports
* Some CSS enhancements for geolocation  when creating forms
* Added OSM as default tile serving operator
* Issue #2032227 use site-name and slogan as default content on front-page
* Issue #2140913 Setting of clean_urls during installation
* Issue #2151909 add translation wrapper
* Issue #2141395 Applied Patch for default content image
* Fixed issues with bootstrap 3.x button drop-downs
* Added Chosen library for better select boxes
* Updated Drupal core to 7.24
* Updated spin.js
