<?php
/**
 * @file
 * Features hooks for the uuid_node features component.
 */

/**
 * Implements hook_features_export_options().
 */
function uuid_node_features_export_options() {
  $options = array();

  $types = node_type_get_names();

  $query = 'SELECT n.nid, n.title, n.type, n.uuid
    FROM {node} n ORDER BY n.type, n.title ASC';
  $results = db_query($query);
  foreach ($results as $node) {
    $options[$node->uuid] = t('@type: @title', array(
      '@type' => $types[$node->type],
      '@title' => $node->title,
    ));
  }

  return $options;
}

/**
 * Implements hook_features_export().
 */
function uuid_node_features_export($data, &$export, $module_name = '') {
  $pipe = array();

  $export['dependencies']['uuid_features'] = 'uuid_features';

  uuid_features_load_module_includes();

  $nids = entity_get_id_by_uuid('node', $data);
  foreach ($nids as $uuid => $nid) {
    // Load the existing node, with a fresh cache.
    $node = node_load($nid, NULL, TRUE);

    $export['features']['uuid_node'][$uuid] = $uuid;
    $pipe['node'][$node->type] = $node->type;

    // drupal_alter() normally supports just one byref parameter. Using
    // the __drupal_alter_by_ref key, we can store any additional parameters
    // that need to be altered, and they'll be split out into additional params
    // for the hook_*_alter() implementations.  The hook_alter signature is
    // hook_uuid_node_features_export_alter(&$export, &$pipe, $node)
    $data = &$export;
    $data['__drupal_alter_by_ref'] = array(&$pipe);
    drupal_alter('uuid_node_features_export', $data, $node);
  }

  return $pipe;
}

/**
 * Implements hook_features_export_render().
 */
function uuid_node_features_export_render($module, $data) {
  $translatables = $code = array();

  uuid_features_load_module_includes();

  $code[] = '  $nodes = array();';
  $code[] = '';
  $nids = entity_get_id_by_uuid('node', $data);
  foreach ($nids as $uuid => $nid) {
    // Only export the node if it exists.
    if ($nid === FALSE) {
      continue;
    }
    // Attempt to load the node, using a fresh cache.
    $node = node_load($nid, NULL, TRUE);
    if (empty($node)) {
      continue;
    }
    if (!empty($node->path)) {
      $node->pathauto_perform_alias = FALSE;
    }
    $export = $node;

    // Use date instead of created, in the same format used by node_object_prepare.
    $export->date = format_date($export->created, 'custom', 'Y-m-d H:i:s O');

    // Don't cause conflicts with nid/vid/revision_timestamp/changed fields.
    unset($export->nid);
    unset($export->vid);
    unset($export->revision_timestamp);
    unset($export->changed);
    unset($export->last_comment_timestamp);

    // The hook_alter signature is:
    // hook_uuid_node_features_export_render_alter(&$export, $node, $module);
    drupal_alter('uuid_node_features_export_render', $export, $node, $module);

    $code[] = '  $nodes[] = ' . features_var_export($export) . ';';
  }

  if (!empty($translatables)) {
    $code[] = features_translatables_export($translatables, '  ');
  }

  $code[] = '  return $nodes;';
  $code = implode("\n", $code);
  return array('uuid_features_default_content' => $code);
}

/**
 * Implements hook_features_revert().
 */
function uuid_node_features_revert($module) {
  uuid_node_features_rebuild($module);
}

/**
 * Implements hook_features_rebuild().
 * Rebuilds nodes based on UUID from code defaults.
 */
function uuid_node_features_rebuild($module) {
  $nodes = module_invoke($module, 'uuid_features_default_content');
  if (!empty($nodes)) {
    module_load_include('inc', 'node', 'node.pages');

    foreach ($nodes as $data) {
      $node = (object) $data;
      node_object_prepare($node);

      // Find the matching UUID, with a fresh cache.
      $nids = entity_get_id_by_uuid('node', array($node->uuid));
      if (isset($nids[$node->uuid])) {
          $nid = $nids[$node->uuid];
          $existing = node_load($nid, NULL, TRUE);
          if (!empty($existing)) {
            $node->nid = $existing->nid;
            $node->vid = $existing->vid;
          }
      }

      // The hook_alter signature is:
      // hook_uuid_node_features_rebuild_alter(&node, $module);
      drupal_alter('uuid_node_features_rebuild', $node, $module);

      $node = node_submit($node);
      node_save($node);
    }
  }
}
