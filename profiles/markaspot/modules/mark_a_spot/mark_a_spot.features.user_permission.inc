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

  // Exported permission: 'create report content'.
  $permissions['create report content'] = array(
    'name' => 'create report content',
    'roles' => array(
      'administrator' => 'administrator',
      'anonymous user' => 'anonymous user',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'node',
  );

  // Exported permission: 'view field_statement'.
  $permissions['view field_statement'] = array(
    'name' => 'view field_statement',
    'roles' => array(
      'administrator' => 'administrator',
      'anonymous user' => 'anonymous user',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'view field_status'.
  $permissions['view field_status'] = array(
    'name' => 'view field_status',
    'roles' => array(
      'administrator' => 'administrator',
      'anonymous user' => 'anonymous user',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'field_permissions',
  );

  return $permissions;
}
