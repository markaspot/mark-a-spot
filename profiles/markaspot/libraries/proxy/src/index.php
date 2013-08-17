<?php

define('CONFIG_FILE', 'simple-php-proxy_config.php');

/*
    Installation note:

    Copy example.simple-php-proxy_config.php to simple-php-proxy_config.php
    in the same folder as this script or to one or two levels above it.

    Optionally you can change the variables right here and not use the config file.

    For an explanation of the config variables see example.simple-php-proxy_config.php
*/

$dest_host = "bash.org";

$proxy_base_url = '/profiles/markaspot/libraries/proxy';

$proxied_headers = array('Set-Cookie', 'Content-Type', 'Cookie', 'Location');

// Variables you specify in the config file overwrite variables set above.
foreach( array('./', '../', '../../') as $path_rel )
{
    if( file_exists(dirname(__file__)."/$path_rel" . CONFIG_FILE) )
    {
        include(dirname(__file__)."/$path_rel" . CONFIG_FILE);
        break;
    }
}

//canonical trailing slash
$proxy_base_url_canonical = rtrim($proxy_base_url, '/ ') . '/';

//check if valid
if( strpos($_SERVER['REQUEST_URI'], $proxy_base_url) !== 0 )
{
    die("The config paramter \$prox_base_url \"$proxy_base_url\" that you specified
        does not match the beginning of the request URI: ".
        $_SERVER['REQUEST_URI']);
}

//remove base_url and optional index.php from request_uri
$proxy_request_url = substr($_SERVER['REQUEST_URI'], strlen($proxy_base_url_canonical));

if( strpos($proxy_request_url, 'index.php') === 0 )
{
    $proxy_request_url = ltrim(substr($proxy_request_url, strlen('index.php')), '/');
}

//final proxied request url
$proxy_request_url = "http://" . rtrim($dest_host, '/ ') . '/' . $proxy_request_url;

/* Init CURL */
$ch = curl_init();
curl_setopt($ch, CURLOPT_URL, $proxy_request_url);
curl_setopt($ch, CURLOPT_AUTOREFERER, 1);
curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
curl_setopt($ch, CURLOPT_HEADER, 1);
curl_setopt($ch, CURLOPT_USERAGENT, $_SERVER['HTTP_USER_AGENT']);
curl_setopt($ch, CURLOPT_HTTPHEADER, array('Expect:'));
curl_setopt($ch, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_0);

/* Collect and pass client request headers */
if(isset($_SERVER['HTTP_COOKIE']))
{
    $hdrs[]="Cookie: " . $_SERVER['HTTP_COOKIE'];
}

if(isset($_SERVER['HTTP_USER_AGENT']))
{
    $hdrs[]="User-Agent: " . $_SERVER['HTTP_USER_AGENT'];
}

curl_setopt($ch, CURLOPT_HTTPHEADER, $hdrs);

/* pass POST params */
if( sizeof($_POST) > 0 )
{
    curl_setopt($ch, CURLOPT_POSTFIELDS, $_POST);
}

$res = curl_exec($ch);
curl_close($ch);

/* parse response */
list($headers, $body) = explode("\r\n\r\n", $res, 2);

$headers = explode("\r\n", $headers);
$hs = array();

foreach($headers as $header)
{
    if( false !== strpos($header, ':') )
    {
        list($h, $v) = explode(':', $header);
        $hs[$h][] = $v;
    }
    else
    {
        $header1  = $header;
    }
}

/* set headers */
list($proto, $code, $text) = explode(' ', $header1);
header($_SERVER['SERVER_PROTOCOL'] . ' ' . $code . ' ' . $text);

foreach($proxied_headers as $hname)
{
    if( isset($hs[$hname]) )
    {
        foreach( $hs[$hname] as $v )
        {
            if( $hname === 'Set-Cookie' )
            {
                header($hname.": " . $v, false);
            }
            else
            {
                header($hname.": " . $v);
            }
        }
    }
}

die($body);


