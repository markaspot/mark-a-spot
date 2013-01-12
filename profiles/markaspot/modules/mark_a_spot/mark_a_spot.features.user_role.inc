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

  // Exported role: Citizen.
  $roles['citizen'] = array(
    'name' => 'Citizen',
    'weight' => '6',
  );

  // Exported role: Verwaltung.
  $roles['management'] = array(
    'name' => 'Management',
    'weight' => '5',
  );

  return $roles;
}
