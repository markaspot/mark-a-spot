<?php

/**
 * @file
 * Environment driven settings for the public Mark-a-Spot runtime.
 */

use Symfony\Component\HttpFoundation\Request;

$app_root = dirname(dirname(__DIR__));
$site_path = 'sites/default';
$siteIdentifier = getenv('SITE_IDENTIFIER') ?: 'default';
if (!preg_match('/^[A-Za-z0-9_-]{1,64}$/', $siteIdentifier)) {
  throw new RuntimeException('SITE_IDENTIFIER must be a safe path segment of at most 64 characters.');
}
$hashSalt = getenv('DRUPAL_HASH_SALT');
if (!is_string($hashSalt) || strlen($hashSalt) < 32) {
  throw new RuntimeException('DRUPAL_HASH_SALT must contain at least 32 characters.');
}

$settings['hash_salt'] = $hashSalt;
$settings['site_identifier'] = $siteIdentifier;
$settings['config_sync_directory'] = getenv('CONFIG_SYNC_DIR') ?: '../config/sync';
$settings['file_public_path'] = 'sites/default/files';
$settings['file_private_path'] = '../private/' . $siteIdentifier;
$settings['file_temp_path'] = '/tmp/drupal';

foreach (['services.yml', 'runtime.services.yml'] as $servicesFile) {
  if (file_exists($app_root . '/' . $site_path . '/' . $servicesFile)) {
    $settings['container_yamls'][] = $app_root . '/' . $site_path . '/' . $servicesFile;
  }
}

$databases['default']['default'] = [
  'database' => getenv('DB_NAME') ?: 'drupal',
  'username' => getenv('DB_USER') ?: 'drupal',
  'password' => getenv('DB_PASSWORD') ?: '',
  'host' => getenv('DB_HOST') ?: 'database',
  'port' => getenv('DB_PORT') ?: '3306',
  'driver' => 'mysql',
  'prefix' => '',
  'charset' => 'utf8mb4',
  'collation' => 'utf8mb4_unicode_ci',
];

$settings['trusted_host_patterns'] = [
  '^localhost$',
  '^127\.0\.0\.1$',
  '^nginx$',
  '^.+-nginx-\d+$',
];

foreach (['DRUPAL_TRUSTED_HOST', 'FRONTEND_HOSTS'] as $hostVariable) {
  foreach (explode(',', (string) getenv($hostVariable)) as $host) {
    $host = trim($host);
    if ($host !== '') {
      $settings['trusted_host_patterns'][] = '^' . preg_quote($host, '/') . '$';
    }
  }
}

$settings['reverse_proxy'] = TRUE;
$settings['reverse_proxy_addresses'] = [
  '127.0.0.1',
];
$settings['reverse_proxy_trusted_headers'] =
  Request::HEADER_X_FORWARDED_FOR |
  Request::HEADER_X_FORWARDED_PORT |
  Request::HEADER_X_FORWARDED_PROTO;

if ($cookieDomain = getenv('COOKIE_DOMAIN')) {
  $settings['cookie_domain'] = $cookieDomain;
}

$preserveRuntimeApiKeys = getenv('MARKASPOT_PRESERVE_RUNTIME_API_KEYS');
$settings['markaspot_preserve_runtime_api_keys'] =
  $preserveRuntimeApiKeys === FALSE || $preserveRuntimeApiKeys === ''
    ? TRUE
    : (filter_var($preserveRuntimeApiKeys, FILTER_VALIDATE_BOOLEAN, FILTER_NULL_ON_FAILURE) ?? TRUE);

if ($apiKeysJson = getenv('GEOREPORT_API_KEYS')) {
  $apiKeys = json_decode($apiKeysJson, FALSE, 16, JSON_THROW_ON_ERROR);
  if (!is_object($apiKeys)) {
    throw new RuntimeException('GEOREPORT_API_KEYS must be a JSON object.');
  }
  foreach (get_object_vars($apiKeys) as $keyName => $keyValue) {
    if (!preg_match('/^[a-z0-9_]+$/', $keyName) || !is_string($keyValue) || $keyValue === '') {
      throw new RuntimeException('GEOREPORT_API_KEYS contains an invalid key entry.');
    }
    $config['services_api_key_auth.api_key.' . $keyName]['key'] = $keyValue;
  }
}
elseif (($geoReportApiKey = getenv('GEOREPORT_API_KEY')) && $geoReportApiKey !== '*') {
  $keyName = getenv('GEOREPORT_API_KEY_NAME') ?: 'nuxt';
  if (!preg_match('/^[a-z0-9_]+$/', $keyName)) {
    throw new RuntimeException('GEOREPORT_API_KEY_NAME is invalid.');
  }
  $config['services_api_key_auth.api_key.' . $keyName]['key'] = $geoReportApiKey;
  if ($apiUserUuid = getenv('API_USER_UUID')) {
    $config['services_api_key_auth.api_key.' . $keyName]['user_uuid'] = $apiUserUuid;
  }
}

if ($geocoderProvider = getenv('GEOCODER_PROVIDER')) {
  $config['markaspot_geocoder.settings']['provider'] = $geocoderProvider;
}
if ($geocoderLanguage = getenv('GEOCODER_LANGUAGE')) {
  $config['markaspot_geocoder.settings']['language'] = $geocoderLanguage;
}
if ($mapToken = getenv('GEOCODER_API_KEY') ?: getenv('MAPBOX_TOKEN') ?: getenv('MAPBOX_API_KEY')) {
  $config['markaspot_geocoder.settings']['mapbox_token'] = $mapToken;
  $config['markaspot_nuxt.settings']['mapbox_token'] = $mapToken;
  $config['markaspot_map.settings']['mapbox_token'] = $mapToken;
}

$defaultMailBackend = getenv('SMTP_MODULE') ?: 'phpmailer_smtp';
$mailMode = (string) (getenv('MARKASPOT_MAIL_MODE') ?: '');
if (!in_array($mailMode, ['production', 'mailpit'], TRUE)) {
  error_log('MARKASPOT_MAIL_MODE is unset or invalid. Falling back to mailpit.');
  $mailMode = 'mailpit';
}
$settings['markaspot_mail_mode'] = $mailMode;

if ($mailMode === 'production') {
  $config['system.mail']['interface']['default'] = $defaultMailBackend;
  $config['mailsystem.settings']['defaults']['sender'] = $defaultMailBackend;
  $config['mailsystem.settings']['defaults']['formatter'] = $defaultMailBackend;

  if ($smtpHost = getenv('SMTP_HOST')) {
    foreach (['phpmailer_smtp.settings', 'smtp.settings'] as $smtpConfigKey) {
      $config[$smtpConfigKey]['smtp_host'] = $smtpHost;
      $config[$smtpConfigKey]['smtp_port'] = getenv('SMTP_PORT') ?: '587';
      $config[$smtpConfigKey]['smtp_protocol'] = 'tls';
      $config[$smtpConfigKey]['smtp_username'] = getenv('SMTP_USER') ?: '';
      $config[$smtpConfigKey]['smtp_password'] = getenv('SMTP_PASSWORD') ?: '';
    }
    $config['phpmailer_smtp.settings']['smtp_authentication_type'] = 'basic_auth';
    $config['smtp.settings']['smtp_hostbackup'] = '';
  }
}
else {
  $config['mailsystem.settings']['modules'] = [];
  $config['system.mail']['interface']['default'] = $defaultMailBackend;
  $config['mailsystem.settings']['defaults']['sender'] = $defaultMailBackend;
  $config['mailsystem.settings']['defaults']['formatter'] = $defaultMailBackend;
  foreach (['phpmailer_smtp.settings', 'smtp.settings'] as $smtpConfigKey) {
    $config[$smtpConfigKey]['smtp_host'] = 'mailpit';
    $config[$smtpConfigKey]['smtp_port'] = '1025';
    $config[$smtpConfigKey]['smtp_protocol'] = '';
    $config[$smtpConfigKey]['smtp_username'] = '';
    $config[$smtpConfigKey]['smtp_password'] = '';
  }
  $config['phpmailer_smtp.settings']['smtp_authentication_type'] = 'basic_auth';
  $config['smtp.settings']['smtp_hostbackup'] = '';
}

if ($smtpFrom = getenv('SMTP_FROM')) {
  $config['system.site']['mail'] = $smtpFrom;
}

$settings['markaspot_mail_recipient_override'] =
  trim((string) (getenv('MARKASPOT_MAIL_RECIPIENT_OVERRIDE') ?: ''));
$operatingMode = strtolower(trim((string) (getenv('MARKASPOT_OPERATING_MODE') ?: '')));
$settings['markaspot_operating_mode'] = $operatingMode === 'saas' ? 'saas' : 'self_hosted';
