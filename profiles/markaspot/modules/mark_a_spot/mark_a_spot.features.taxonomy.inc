<?php
/**
 * @file
 * mark_a_spot.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function mark_a_spot_taxonomy_default_vocabularies() {
  return array(
    'category' => array(
      'name' => 'Category',
      'machine_name' => 'category',
      'description' => NULL,
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
    ),
    'status' => array(
      'name' => 'Status',
      'machine_name' => 'status',
      'description' => NULL,
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
    ),
    'tags' => array(
      'name' => 'Tags',
      'machine_name' => 'tags',
      'description' => 'Use tags to group articles on similar topics into categories.',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
    ),
  );
}
