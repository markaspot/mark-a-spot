<?php

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
$dest_host = "bash.org";

/*
Location of your proxy index script relative to your web root
The first slash is needed the trailing slash is optional

Use '/my_project/simple-php-proxy/src' if you place the whole folder within your project
or for usage at webroot level use '/'
*/
$proxy_base_url = '/';

/*
What headers to proxy from destination host to client
*/
$proxied_headers = array('Set-Cookie', 'Content-Type', 'Cookie', 'Location');
