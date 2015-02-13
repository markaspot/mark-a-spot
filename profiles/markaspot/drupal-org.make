api = 2
core = 7.x

; Modules
projects[ctools][subdir] = "contrib"
projects[icon][subdir] = "contrib"
projects[icon][version] = 1.x-dev
projects[features][subdir] = "contrib"
projects[entity][subdir] = "contrib"
projects[geolocation][subdir] = "contrib"
projects[field_group][subdir] = "contrib"
projects[field_permissions][subdir] = "contrib"
projects[jquery_update][subdir] = "contrib"
projects[libraries][subdir] = "contrib"
projects[panels][subdir] = "contrib"
projects[services][subdir] = "contrib"
projects[strongarm][subdir] = "contrib"
projects[views][subdir] = "contrib"
projects[views_datasource][subdir] = "contrib"
projects[better_exposed_filters][subdir] = "contrib"
projects[bootstrap_fieldgroup][subdir] = "contrib"
projects[field_formatter_settings][subdir] = "contrib"
projects[field_formatter_class][subdir] = "contrib"
projects[field_formatter_css_class][subdir] = "contrib"
projects[geophp][subdir] = "contrib"
projects[references][subdir] = "contrib"
projects[references_dialog][subdir] = "contrib"
projects[color_field][subdir] = "contrib"
projects[fontawesome][subdir] = "contrib"
projects[views_bulk_operations][subdir] = "contrib"

projects[views_geojson][subdir] = "contrib"
projects[views_geojson][version] = 1.x-dev


projects[twitter][subdir] = "contrib"
projects[twitter][patch][2132231] = "https://drupal.org/files/issues/Twitter-add_geo_and_entities_Twitter_object_2132231-1_0.patch"

projects[pathauto][subdir] = "contrib"
projects[token][subdir] = "contrib"

projects[uuid][subdir] = "contrib"
;projects[uuid][patch][2161375] = https://drupal.org/files/issues/custom_method_of_UUID_creation_2161375_1.patch

projects[oauth][subdir] = "contrib"
projects[workbench][subdir] = "contrib"
projects[rules][subdir] = "contrib"

projects[chosen][subdir] = "contrib"
projects[chosen][version] = 2.x-dev

projects[geolocation_osm][type] = module
projects[geolocation_osm][subdir] = "contrib"
projects[geolocation_osm][download][url] = "http://git.drupal.org/project/geolocation_osm.git"

projects[navbar][type] = module
projects[navbar][version] = 1.4
projects[navbar][subdir] = "contrib"

projects[ckeditor][type] = module
projects[ckeditor][version] = 1.16
projects[ckeditor][subdir] = contrib

projects[search_api][subdir] = contrib
projects[search_api_db][subdir] = contrib

libraries[chosen][type] = libraries
libraries[chosen][download][type] = get
libraries[chosen][download][url] = "https://github.com/harvesthq/chosen/releases/download/1.0.0/chosen_v1.0.0.zip"
libraries[chosen][directory_name] = chosen

libraries[waypoints][type] = libraries
libraries[waypoints][download][type] = git
libraries[waypoints][download][url] = "https://github.com/imakewebthings/waypoints.git"
libraries[waypoints][directory_name] = waypoints

libraries[bootbox][type] = libraries
libraries[bootbox][download][type] = get
libraries[bootbox][download][url] = "https://github.com/makeusabrew/bootbox/releases/download/v4.3.0/bootbox.min.js"
libraries[bootbox][directory_name] = bootbox

libraries[Leaflet.awesome-markers][type] = libraries
libraries[Leaflet.awesome-markers][download][type] = git
libraries[Leaflet.awesome-markers][download][url] = "https://github.com/markaspot/Leaflet.awesome-markers.git"
libraries[Leaflet.awesome-markers][download][branch] = additional-colors
libraries[Leaflet.awesome-markers][directory_name] = Leaflet.awesome-markers

libraries[Leaflet.draw][type] = libraries
libraries[Leaflet.draw][download][type] = git
libraries[Leaflet.draw][download][url] = "https://github.com/Leaflet/Leaflet.draw.git"
libraries[Leaflet.draw][directory_name] = Leaflet.draw

libraries[Leaflet.EasyButton][type] = libraries
libraries[Leaflet.EasyButton][download][type] = git
libraries[Leaflet.EasyButton][download][url] = "https://github.com/markaspot/Leaflet.EasyButton.git"
libraries[Leaflet.EasyButton][directory_name] = Leaflet.EasyButton

libraries[Leaflet.markercluster][type] = libraries
libraries[Leaflet.markercluster][download][type] = git
libraries[Leaflet.markercluster][download][url] = "https://github.com/Leaflet/Leaflet.markercluster.git"
libraries[Leaflet.markercluster][directory_name] = Leaflet.markercluster

libraries[leaflet-locatecontrol][type] = libraries
libraries[leaflet-locatecontrol][download][type] = git
libraries[leaflet-locatecontrol][download][url] = "https://github.com/domoritz/leaflet-locatecontrol.git"
libraries[leaflet-locatecontrol][directory_name] = leaflet-locatecontrol

libraries[Leaflet.Sync][type] = libraries
libraries[Leaflet.Sync][download][type] = git
libraries[Leaflet.Sync][download][url] = "https://github.com/turban/Leaflet.Sync.git"
libraries[Leaflet.Sync][directory_name] = Leaflet.Sync

libraries[leaflet-plugins][type] = libraries
libraries[leaflet-plugins][download][type] = git
libraries[leaflet-plugins][download][url] = "https://github.com/shramov/leaflet-plugins.git"
libraries[leaflet-plugins][directory_name] = leaflet-plugins

libraries[leaflet.bouncemarker][type] = libraries
libraries[leaflet.bouncemarker][download][type] = git
libraries[leaflet.bouncemarker][download][url] = "https://github.com/maximeh/leaflet.bouncemarker.git"
libraries[leaflet.bouncemarker][directory_name] = leaflet.bouncemarker

libraries[Leaflet][type] = libraries
libraries[Leaflet][download][type] = file
libraries[Leaflet][download][url] = "http://leaflet-cdn.s3.amazonaws.com/build/leaflet-0.7.2.zip"
libraries[Leaflet][directory_name] = leaflet

libraries[spin.js][type] = libraries
libraries[spin.js][download][type] = git
libraries[spin.js][download][url] = "https://github.com/fgnass/spin.js.git"
libraries[spin.js][directory_name] = spin.js

libraries[jquery-html5-upload][type] = libraries
libraries[jquery-html5-upload][download][type] = git
libraries[jquery-html5-upload][download][url] = "https://github.com/mihaild/jquery-html5-upload.git"
libraries[jquery-html5-upload][directory_name] = jquery-html5-upload

libraries[markaspot-font][type] = libraries
libraries[markaspot-font][download][type] = git
libraries[markaspot-font][download][url] = "https://github.com/markaspot/markaspot-font.git"
libraries[markaspot-font][directory_name] = markaspot-font

libraries[fontawesome][type] = "libraries"
libraries[fontawesome][download][type] = "get"
libraries[fontawesome][download][url] = "https://github.com/FortAwesome/Font-Awesome/archive/master.zip"
libraries[fontawesome][directory_name] = "fontawesome"
libraries[fontawesome][destination] = "libraries"

libraries[proxy][type] = libraries
libraries[proxy][download][type] = git
libraries[proxy][download][url] = "https://github.com/markaspot/Simple-php-proxy-script.git"
libraries[proxy][download][branch] = osm-nominatim
libraries[proxy][directory_name] = proxy

libraries[bootstrap][download][type] = "get"
libraries[bootstrap][download][url] = "https://github.com/twbs/bootstrap/archive/v3.3.2.zip"
libraries[bootstrap][directory_name] = "bootstrap"
libraries[bootstrap][destination] = "themes/mas"
libraries[bootstrap][overwrite] = TRUE

libraries[underscore][download][type] = get
libraries[underscore][download][url] = https://github.com/jashkenas/underscore/archive/1.5.2.zip

libraries[backbone][download][type] = get
libraries[backbone][download][url] = https://github.com/jashkenas/backbone/archive/1.1.0.zip

libraries[modernizr][download][type] = "get"
libraries[modernizr][download][url] = "https://github.com/Modernizr/Modernizr/archive/v2.7.0.tar.gz"

libraries[ckeditor][download][type] = get
libraries[ckeditor][download][url] = http://download.cksource.com/CKEditor%20for%20Drupal/edit/ckeditor_4.3.2_edit.zip

; Themes
projects[bootstrap][type] = "theme"
projects[ember][type] = "theme"
projects[ember][version] = 2.x-dev
