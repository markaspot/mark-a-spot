<?php
/**
 * Example Drush configuration file for a Platform.sh Drupal site.
 */

if (PHP_SAPI === 'cli' && isset($_ENV['PLATFORM_ROUTES']) && isset($_ENV['PLATFORM_APPLICATION_NAME'])) {
  $routes = json_decode(base64_decode($_ENV['PLATFORM_ROUTES']), TRUE);
  $expected_route_urls = [
    'https://{default}/',
    'https://www.{default}/',
    'http://{default}/',
    'http://www.{default}/',
  ];
  foreach ($routes as $url => $route) {
    if ($route['type'] === 'upstream'
    	&& $route['upstream'] === $_ENV['PLATFORM_APPLICATION_NAME']
        && in_array($route['original_url'], $expected_route_urls)) {
      $options['uri'] = $url;
      break;
    }
  }
}
