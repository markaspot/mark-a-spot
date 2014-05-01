<?php

/**
 * @file
 *
 */

/**
 * Custom filter to return only points within a provided bounding box.
 */
class views_geojson_bbox_argument extends views_handler_argument {

  /*
   * Filter options definition.
   */
  function option_definition() {
    $options = parent::option_definition();
    $options['arg_id'] = array('default' => 'bbox');
    return $options;
  }

  /**
   *
   */
  function default_argument_form(&$form, &$form_state) {
    parent::default_argument_form($form, $form_state);
    // Clarify this, since we're treating pulling from the query string as
    // different than a normal arg.
    $form['no_argument']['#title'] = 'When the filter value is NOT in the URL <em>as a normal Drupal argument</em>';
  }

  /*
   * Figure out which fields are being used so we can filter on them.
   */
  function _get_fields($data_source) {
    $field_ids = array();
    switch ($data_source['value']) {
      case 'latlon':
        $field_ids[] = $data_source['latitude'];
        $field_ids[] = $data_source['longitude'];
        break;
      case 'geofield':
        $field_ids[] = $data_source['geofield'];
        break;
      case 'wkt':
        $field_ids[] = $data_source['wkt'];
        break;
    }

    return $field_ids;
  }

  /*
   * Pull BBOX string from URL to get coordinates
   *
   * ?bbox=x,x,x,x
   */
  function _get_bbox_coords() {
    if (isset($_GET[$this->options['arg_id']])) {
      $this->argument = $_GET[$this->options['arg_id']];
    }
    return $this->_explode_bbox_coords($this->argument);
  }

  /*
   * Split BBOX string into {left, bottom, right, top}
   */
  function _explode_bbox_coords($bbox_coords_str) {
    // If the GET prefix is still in there (like "?bbox="), remove it.
    // @TODO: this might just handle what we've broken into a special case as
    // the "query string" default arg
    $arg_get_prefix = '?' . $this->options['arg_id'] . '=';
    $bbox_coords_str = str_ireplace($arg_get_prefix, '', $bbox_coords_str);

    $bbox_coords = explode(',', $bbox_coords_str);
    if (count($bbox_coords) == 4) {
      $bbox = array(
        'left' => $bbox_coords[0],
        'bottom' => $bbox_coords[1],
        'right' => $bbox_coords[2],
        'top' => $bbox_coords[3],
      );
      return $bbox;
    }
  }

  /*
   * Use the filter to modify the query.
   */
  function query() {
    $this->ensure_my_table();
    // TODO: this argument should be set in
    // views_plugin_argument_default_bboxquery::get_argument
    // unfortunately that seems broken.
    // @see http://drupal.org/node/1884214
    if (!empty($this->argument) && ($this->argument !== TRUE)) {
      $bbox = $this->_explode_bbox_coords($this->argument);
    } else {
      $bbox = $this->_get_bbox_coords();
    }

    // If bbox not set properly, don't edit the query.
    // @TODO: Move this into argument validation?
    if (!isset($bbox['left']) ||
        !isset ($bbox['bottom']) ||
        !isset ($bbox['right']) ||
        !isset ($bbox['top'])) {
      return;
    }

    if (isset($this->view->display_handler->display->display_options['style_options'])) {
      $data_source = $this->view->display_handler->display->display_options['style_options']['data_source'];
    }
    else {
      $data_source = $this->view->display['default']->display_options['style_options']['data_source'];
    }

    // We handle latlon and geofield data_source types.
    // No support yet for WKT.
    if ($data_source['value'] == 'latlon') {
      $lat_field_obj = $this->view->field[$data_source['latitude']];
      $lon_field_obj = $this->view->field[$data_source['longitude']];
      $lat_field_table = $lat_field_obj->table;
      $lon_field_table = $lon_field_obj->table;
    }
    elseif ($data_source['value'] == 'geofield') {
      // Geofield includes both Lat and Lon fields.
      // We pretend they're distinct fields so we can use same code as for
      // latlon, since we're just going to get lat & lon from geofield anyway.
      $lat_field_obj = $lon_field_obj = $this->view->field[$data_source['geofield']];
      $lat_field_table = $lon_field_table = $lat_field_obj->table;
    } else {
      return;
    }

    // If the Latitude / Longitude fields are really geofields,
    // we need to dig a bit deeper to find field names.
    if (isset($lat_field_obj->field_info) && $lat_field_obj->field_info['type'] == 'geofield') {
      // @TODO: Maybe the field name can really just be "${lat_field_obj}_lat"?
      $lat_field_name = $lat_field_obj->field_info['storage']['details']['sql']['FIELD_LOAD_CURRENT'][$lat_field_table]['lat'];
    } else {
      $lat_field_name = $lat_field_obj->field;
    }
    if (isset($lon_field_obj->field_info) && $lon_field_obj->field_info['type'] == 'geofield') {
      $lon_field_name = $lon_field_obj->field_info['storage']['details']['sql']['FIELD_LOAD_CURRENT'][$lon_field_table]['lon'];
    } else {
      $lon_field_name = $lon_field_obj->field;
    }

    // Add JOIN(s) to query.
    $this->query->add_table($lat_field_table);
    if ($lon_field_table != $lat_field_table) {
      $this->query->add_table($lon_field_table);
    }

    // Add WHERE(s) to query.
    $left = $bbox['left'];
    $right = $bbox['right'];
    // OpenLayers' longitude bbox values can be >180 or <-180 when the map wraps around.
    // We need to turn these into BETWEENs with OR.
    if ($right - $left < 360) {
      $group = $this->query->set_where_group('AND');
      if ($left > -180) {
        $this->query->add_where($group, "$lon_field_table.$lon_field_name", $left, '>=');
      } else {
        $this->query->set_where_group('OR', $group);
        $left = -1 * ($left % 180);
        $this->query->add_where($group, "$lon_field_table.$lon_field_name", array($left, 0), 'BETWEEN');
      }
      if ($right < 180) {
        $this->query->add_where($group, "$lon_field_table.$lon_field_name", $right, '<=');
      } else {
        $this->query->set_where_group('OR', $group);
        $right = -1 * ($right % 180);
        $this->query->add_where($group, "$lon_field_table.$lon_field_name", array($right, 0), 'BETWEEN');
      }
    }

    $bottom = $bbox['bottom'];
    $top = $bbox['top'];
    $group = $this->query->set_where_group('AND');
    $this->query->add_where($group, "$lat_field_table.$lat_field_name", $bottom, '>=');
    $this->query->add_where($group, "$lat_field_table.$lat_field_name", $top, '<=');
  }
}