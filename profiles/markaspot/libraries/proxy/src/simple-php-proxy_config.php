<?php
global $base_url, $base_path, $base_root;

$is_https = isset($_SERVER['HTTPS']) && strtolower($_SERVER['HTTPS']) == 'on';


$http_protocol = $is_https ? 'https' : 'http';
$base_root = $http_protocol . '://' . $_SERVER['HTTP_HOST'];

$base_url = $base_root;

// $_SERVER['SCRIPT_NAME'] can, in contrast to $_SERVER['PHP_SELF'], not
// be modified by a visitor.
if ($dir = rtrim(dirname($_SERVER['SCRIPT_NAME']), '\/')) {
  $base_path = $dir;
  $base_url .= $base_path;
  $base_path .= '/';
} else {
  $base_path = '/';
}



/*
Allowed destination host, request URIs are fetched from there

Say your proxy script resides on "http://www.alice.com/folder/simple-php-proxy/src/index.php"
You set
$dest_host = "bob.com";
$proxy_base_url = '/folder/simple-php-proxy/src';

A request to "http://www.alice.com/folder/simple-php-proxy/src/index.php/my_other_uri/that/resides/on/bob"
would now be proxied to
"http://bob.com/my_other_uri/that/resides/on/bob"
and the output returned

replace this to where you want to proxy
*/
$dest_host = "nominatim.openstreetmap.org";

/*
Location of your proxy index script relative to your web root
The first slash is needed the trailing slash is optional

Use '/my_project/simple-php-proxy/src' if you place the whole folder within your project
or for usage at webroot level use '/'
*/
$proxy_base_url = '/profiles/markaspot/libraries/proxy/src';

/*
What headers to proxy from destination host to client
*/
$proxied_headers = array('Set-Cookie', 'Content-Type', 'Cookie', 'Location');
