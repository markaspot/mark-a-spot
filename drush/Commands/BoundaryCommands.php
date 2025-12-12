<?php

namespace Drush\Commands;

use Drush\Commands\DrushCommands;
use GuzzleHttp\Exception\GuzzleException;

/**
 * Drush commands for managing jurisdiction boundaries.
 */
class BoundaryCommands extends DrushCommands {

  /**
   * The Nominatim API base URL.
   */
  protected const NOMINATIM_API_URL = 'https://nominatim.openstreetmap.org/search';

  /**
   * User-Agent string for Nominatim API requests.
   */
  protected const USER_AGENT = 'Mark-a-Spot/1.0 (https://markaspot.org)';

  /**
   * Fetches a boundary polygon from Nominatim and stores it in a jur group.
   *
   * @param array $options
   *   Command options.
   *
   * @command markaspot:fetch-boundary
   * @aliases mas-fb, mas-fetch-boundary
   * @bootstrap full
   * @option city The city name to search for (e.g., "Bonn, Germany")
   * @option group The group entity ID to store the boundary in
   * @usage drush markaspot:fetch-boundary --city="Bonn, Germany" --group=15
   *   Fetches the boundary for Bonn and stores it in group 15.
   * @usage drush mas-fb --city="Cologne" --group=14
   *   Short alias version.
   */
  public function fetchBoundary(array $options = ['city' => NULL, 'group' => NULL]): int {
    $city = $options['city'];
    $group_id = $options['group'];

    // Validate required options.
    if (empty($city)) {
      $this->logger()->error('The --city option is required.');
      return 1;
    }

    if (empty($group_id)) {
      $this->logger()->error('The --group option is required.');
      return 1;
    }

    // Load and validate the group entity.
    $group = $this->loadAndValidateGroup($group_id);
    if ($group === NULL) {
      return 1;
    }

    // Fetch boundary from Nominatim.
    $this->logger()->notice('Fetching boundary for "{city}"...', ['city' => $city]);
    $boundary = $this->fetchBoundaryFromNominatim($city);

    if ($boundary === NULL) {
      $this->logger()->error('Failed to fetch boundary for "{city}".', ['city' => $city]);
      return 1;
    }

    // Store the boundary in the group.
    return $this->storeBoundary($group, $boundary, $city);
  }

  /**
   * Lists all jur groups with their boundary status.
   *
   * @command markaspot:list-boundaries
   * @aliases mas-lb, mas-list-boundaries
   * @bootstrap full
   * @usage drush markaspot:list-boundaries
   *   Lists all jurisdiction groups and their boundary status.
   */
  public function listBoundaries(): int {
    $entity_type_manager = \Drupal::entityTypeManager();
    $group_storage = $entity_type_manager->getStorage('group');
    $groups = $group_storage->loadByProperties(['type' => 'jur']);

    if (empty($groups)) {
      $this->logger()->notice('No jurisdiction groups found.');
      return 0;
    }

    $rows = [];
    foreach ($groups as $group) {
      $has_boundary = $group->hasField('field_boundary') && !$group->get('field_boundary')->isEmpty();
      $boundary_status = $has_boundary ? 'Yes' : 'No';

      // Get boundary size if present.
      $boundary_size = '';
      if ($has_boundary) {
        $value = $group->get('field_boundary')->value;
        $boundary_size = $this->formatBytes(strlen($value));
      }

      $rows[] = [
        'id' => $group->id(),
        'label' => $group->label(),
        'boundary' => $boundary_status,
        'size' => $boundary_size,
      ];
    }

    $this->io()->table(
      ['ID', 'Label', 'Has Boundary', 'Size'],
      $rows
    );

    return 0;
  }

  /**
   * Clears the boundary from a jur group.
   *
   * @param array $options
   *   Command options.
   *
   * @command markaspot:clear-boundary
   * @aliases mas-cb, mas-clear-boundary
   * @bootstrap full
   * @option group The group entity ID to clear the boundary from
   * @usage drush markaspot:clear-boundary --group=15
   *   Clears the boundary from group 15.
   */
  public function clearBoundary(array $options = ['group' => NULL]): int {
    $group_id = $options['group'];

    if (empty($group_id)) {
      $this->logger()->error('The --group option is required.');
      return 1;
    }

    $group = $this->loadAndValidateGroup($group_id);
    if ($group === NULL) {
      return 1;
    }

    if (!$group->hasField('field_boundary') || $group->get('field_boundary')->isEmpty()) {
      $this->logger()->notice('Group "{label}" (ID: {id}) has no boundary to clear.', [
        'label' => $group->label(),
        'id' => $group->id(),
      ]);
      return 0;
    }

    $group->set('field_boundary', NULL);
    $group->save();

    $this->logger()->success('Cleared boundary from group "{label}" (ID: {id}).', [
      'label' => $group->label(),
      'id' => $group->id(),
    ]);

    return 0;
  }

  /**
   * Loads and validates a group entity.
   *
   * @param string|int $group_id
   *   The group ID.
   *
   * @return object|null
   *   The group entity or NULL if invalid.
   */
  protected function loadAndValidateGroup(string|int $group_id): ?object {
    $entity_type_manager = \Drupal::entityTypeManager();
    $group_storage = $entity_type_manager->getStorage('group');
    $group = $group_storage->load($group_id);

    if ($group === NULL) {
      $this->logger()->error('Group with ID {id} not found.', ['id' => $group_id]);
      return NULL;
    }

    if ($group->bundle() !== 'jur') {
      $this->logger()->error('Group {id} is not a jurisdiction (jur) group. Found type: {type}', [
        'id' => $group_id,
        'type' => $group->bundle(),
      ]);
      return NULL;
    }

    if (!$group->hasField('field_boundary')) {
      $this->logger()->error('Group {id} does not have a field_boundary field. Please ensure the field is configured.', [
        'id' => $group_id,
      ]);
      return NULL;
    }

    return $group;
  }

  /**
   * Fetches boundary GeoJSON from Nominatim API.
   *
   * @param string $city
   *   The city name to search for.
   *
   * @return array|null
   *   The GeoJSON boundary data or NULL on failure.
   */
  protected function fetchBoundaryFromNominatim(string $city): ?array {
    $query_params = [
      'q' => $city,
      'format' => 'geojson',
      'polygon_geojson' => '1',
      'limit' => '1',
    ];

    $url = self::NOMINATIM_API_URL . '?' . http_build_query($query_params);

    try {
      /** @var \GuzzleHttp\ClientInterface $http_client */
      $http_client = \Drupal::httpClient();

      $response = $http_client->request('GET', $url, [
        'headers' => [
          'User-Agent' => self::USER_AGENT,
          'Accept' => 'application/json',
        ],
        'timeout' => 30,
      ]);

      $body = $response->getBody()->getContents();
      $data = json_decode($body, TRUE);

      if (json_last_error() !== JSON_ERROR_NONE) {
        $this->logger()->error('Invalid JSON response from Nominatim: {error}', [
          'error' => json_last_error_msg(),
        ]);
        return NULL;
      }

      // Check if we got results.
      if (empty($data['features']) || !is_array($data['features'])) {
        $this->logger()->warning('No results found for "{city}".', ['city' => $city]);
        return NULL;
      }

      // Get the first feature.
      $feature = $data['features'][0];

      // Log some info about what we found.
      $display_name = $feature['properties']['display_name'] ?? 'Unknown';
      $osm_type = $feature['properties']['osm_type'] ?? 'Unknown';
      $this->logger()->notice('Found: {name} (OSM type: {type})', [
        'name' => $display_name,
        'type' => $osm_type,
      ]);

      // Validate geometry.
      if (empty($feature['geometry'])) {
        $this->logger()->warning('No geometry found in the result.');
        return NULL;
      }

      $geometry_type = $feature['geometry']['type'] ?? '';
      if (!in_array($geometry_type, ['Polygon', 'MultiPolygon'])) {
        $this->logger()->warning('Unexpected geometry type: {type}. Expected Polygon or MultiPolygon.', [
          'type' => $geometry_type,
        ]);
      }

      // Return the full feature (includes properties and geometry).
      return $feature;

    }
    catch (GuzzleException $e) {
      $this->logger()->error('HTTP request failed: {message}', [
        'message' => $e->getMessage(),
      ]);
      return NULL;
    }
    catch (\Exception $e) {
      $this->logger()->error('Unexpected error: {message}', [
        'message' => $e->getMessage(),
      ]);
      return NULL;
    }
  }

  /**
   * Stores the boundary GeoJSON in a group entity.
   *
   * @param object $group
   *   The group entity.
   * @param array $boundary
   *   The boundary GeoJSON data.
   * @param string $city
   *   The city name (for logging).
   *
   * @return int
   *   Exit code (0 for success, 1 for failure).
   */
  protected function storeBoundary(object $group, array $boundary, string $city): int {
    // Encode the boundary as JSON.
    $json = json_encode($boundary, JSON_UNESCAPED_UNICODE | JSON_UNESCAPED_SLASHES);

    if ($json === FALSE) {
      $this->logger()->error('Failed to encode boundary as JSON: {error}', [
        'error' => json_last_error_msg(),
      ]);
      return 1;
    }

    // Store in the field.
    $group->set('field_boundary', $json);
    $group->save();

    $size = $this->formatBytes(strlen($json));
    $this->logger()->success('Stored boundary for "{city}" in group "{label}" (ID: {id}). Size: {size}', [
      'city' => $city,
      'label' => $group->label(),
      'id' => $group->id(),
      'size' => $size,
    ]);

    return 0;
  }

  /**
   * Formats bytes into human-readable string.
   *
   * @param int $bytes
   *   The number of bytes.
   *
   * @return string
   *   Formatted string (e.g., "1.5 KB").
   */
  protected function formatBytes(int $bytes): string {
    $units = ['B', 'KB', 'MB', 'GB'];
    $i = 0;
    $size = $bytes;

    while ($size >= 1024 && $i < count($units) - 1) {
      $size /= 1024;
      $i++;
    }

    return round($size, 2) . ' ' . $units[$i];
  }

}
