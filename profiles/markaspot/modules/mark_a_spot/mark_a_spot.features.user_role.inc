<?php
/**
 * @file
 * mark_a_spot.features.user_role.inc
 */

/**
 * Implements hook_user_default_roles().
 */
function mark_a_spot_user_default_roles() {
  $roles = array();

  // Exported role: Bürger.
  $roles['Bürger'] = array(
    'name' => 'Bürger',
    'weight' => '6',
  );

  // Exported role: Verwaltung.
  $roles['Verwaltung'] = array(
    'name' => 'Verwaltung',
    'weight' => '5',
  );

  return $roles;
}
