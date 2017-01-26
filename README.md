# Mark-a-Spot

Mark-a-Spot is a full packaged Drupal distribution for public civic issue tracking and crowd mapping, including an Open311 GeoReport v2 server. Basic Drupal knowledge is required to customize it for your own needs.

This software is based on Drupal 8 and leaflet.js.
It's source-code is licensed and made available under the GNU General Public License (GPL) version 2.

## Open311 GeoReport Resources

After installation point your browser to http://yourserver/open311 to get more info about the GeoReport features. 

```
http://yourserver/georeport/v2/services.format (xml/json)
http://yourserver/georeport/v2/requests.format (xml/json)
http://yourserver/georeport/v2/discovery.format (xml/json)
```

## Support, Hosting
Holger Kreis | @markaspot | http://mark-a-spot.org

## Installation

```
$ git clone -b master-8.x --single-branch https://github.com/markaspot/mark-a-spot.git
$ cd mark-a-spot
$ composer install
```
