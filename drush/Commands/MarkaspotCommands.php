<?php

namespace Drush\Commands;

use Drush\Drush;
use Symfony\Component\Console\Output\ConsoleOutput;
use Consolidation\AnnotatedCommand\Events\CustomEventAwareInterface;
use Consolidation\AnnotatedCommand\Events\CustomEventAwareTrait;
use Symfony\Component\Console\Input\ArrayInput;

/**
 * Defines Drush commands for Mark-a-Spot.
 *
 * @see http://docs.drush.org/en/master/commands/
 */
class MarkaspotCommands extends DrushCommands implements CustomEventAwareInterface {

  use CustomEventAwareTrait;
  /**
   * The config factory.
   *
   * @var \Drupal\Core\Config\ConfigFactoryInterface
   */
  protected $configFactory;

  /**
   * Initializes the command just after the input has been validated.
   *
   * @throws \Exception
   */
  protected function initialize() {
    if (\Drupal::hasContainer()) {
      $this->configFactory = \Drupal::service('config.factory');
    }
  }


  /**
   * Installs Mark-a-Spot with customizations.
   *
   * @command markaspot:install
   * @aliases mi
   * @option lat Latitude for geolocation field.
   * @option lng Longitude for geolocation field.
   * @option city The city for geolocation field.
   * @option locale The locale for default country setting.
   * @option skip-confirmation Whether to skip confirmation or not.
   */
  public function install($options = ['lat' => '40.73', 'lng' => '-73.93', 'city' => 'New York', 'locale' => 'en_US', 'skip-confirmation' => FALSE]) {
    $lat = $options['lat'];
    $lng = $options['lng'];
    $city = $options['city'];
    list($language, $country) = explode('_', $options['locale']);

    $account_name = 'admin';
    $account_pass = 'admin';
    $account_mail = 'admin@example.com';
    $profile = ['markaspot'];

    // Call the site:install command with the desired options.
    $input = new ArrayInput([
      'command' => 'site:install',
      'profile' => $profile,
      '--account-name' => $account_name,
      '--account-pass' => $account_pass,
      '--account-mail' => $account_mail,
      '--existing-config' => true,
      '--locale' => $options['locale'],
      '-y' => $options['skip-confirmation'],
    ]);

    $output = new ConsoleOutput();
    $exitCode = Drush::getApplication()->run($input, $output);
    if (\Drupal::hasContainer()) {
      $config_factory = \Drupal::service('config.factory');
      if ($options['lat'] && $options['lng']) {
        // Update geolocation field configuration

        $geolocation_config = $config_factory->getEditable('field.field.node.service_request.field_geolocation');

        // Calculate the additional values
        $lat = floatval($options['lat']);
        $lng = floatval($options['lng']);
        $lat_sin = sin(deg2rad($lat));
        $lat_cos = cos(deg2rad($lat));
        $lng_rad = deg2rad($lng);
        $value = "{$lat}, {$lng}";

        // Set the new values
        $geolocation_config->set('default_value.0', [
          'lat' => $lat,
          'lng' => $lng,
          'lat_sin' => $lat_sin,
          'lat_cos' => $lat_cos,
          'lng_rad' => $lng_rad,
          'value' => $value,
        ]);

        $geolocation_config->save();


        $radius = 50; // 5 km radius
        $radius_in_degrees = $radius / 111.32; // Approximately 1 degree is equal to 111.32 km



        $min_lat = $lat - $radius_in_degrees;  // 39.549407810104
        $max_lat = $lat + $radius_in_degrees;  // 40.450592189896
        $min_lng = $lng - $radius_in_degrees;  // -73.546987224066
        $max_lng = $lng + $radius_in_degrees;  // -72.453012775934

        $coords = [
          [$min_lng, $min_lat],
          [$max_lng, $min_lat],
          [$max_lng, $max_lat],
          [$min_lng, $max_lat],
          [$min_lng, $min_lat]
        ];

        $wkt = 'POLYGON((';
        foreach ($coords as $coord) {
          $wkt .= $coord[0] . ' ' . $coord[1] . ',';
        }
        $wkt = rtrim($wkt, ',');
        $wkt .= '))';
        // Update the markaspot_validation.settings.yml file
        $validation_settings = $config_factory->getEditable('markaspot_validation.settings');
        $validation_settings
          ->set('wkt', $wkt)
          ->set('location', [$city])
          ->save();
        $config_factory->getEditable('system.date')->set('country.default', $country)->save();


        // Update form display configurations
        $field_limit_viewbox = "$min_lng,$max_lat,$max_lng,$min_lat";
        $form_display_configurations = [
          'core.entity_form_display.node.service_request.default',
          'core.entity_form_display.node.service_request.management',
        ];

        foreach ($form_display_configurations as $form_display_configuration) {
          $this->updateFormDisplayConfig($config_factory, $form_display_configuration, $lat, $lng, $field_limit_viewbox, $city, $country);
        }

        $this->logger()
          ->success(dt('Geolocation field configuration updated.'));

        // Update map settings.
        $this->markaspotSettingsConfig($config_factory,'markaspot_map.settings', $lat, $lng);

        $this->logger()
          ->success(dt('Markaspot Map module settings updated.'));
      }
      else {
        $this->logger()
          ->error(dt('Both latitude and lngitude options are required.'));
      }
    }
  }

  // Helper function to update form display configurations
  private function updateFormDisplayConfig($config_factory, $config_name, $lat, $lng, $limit_viewbox, $city, $country) {
    $form_display_config = $config_factory->getEditable($config_name);
    $form_display_config->set('content.field_geolocation.settings.limit_viewbox', $limit_viewbox)
      ->set('content.field_geolocation.settings.city', $city)
      ->set('content.field_geolocation.settings.limit_country_code', $country)
      ->set('content.field_geolocation.settings.center_lat', $lat)
      ->set('content.field_geolocation.settings.center_lng', $lng)
      ->save();
  }

  // Helper function to update form display configurations
  private function markaspotSettingsConfig($config_factory, $config_name, $lat, $lng) {
    $markaspot_map = $config_factory->getEditable($config_name);
    $markaspot_map
      ->set('center_lat', $lat)
      ->set('center_lng', $lng)
      ->save();
  }
}

