<?php
/**
 * @file
 * mark_a_spot.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function mark_a_spot_user_default_permissions() {
  $permissions = array();

  // Exported permission: create report content.
  $permissions['create report content'] = array(
    'name' => 'create report content',
    'roles' => array(
      0 => 'administrator',
      1 => 'anonymous user',
      2 => 'authenticated user',
    ),
    'module' => 'node',
  );

  // Exported permission: view field_status.
  $permissions['view field_status'] = array(
    'name' => 'view field_status',
    'roles' => array(
      0 => 'administrator',
      1 => 'anonymous user',
      2 => 'authenticated user',
      3 => 'council',
    ),
    'module' => 'field_permissions',
  );

  return $permissions;
}
