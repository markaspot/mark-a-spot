<?php
/**
 * @file
 * mark_a_spot.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function mark_a_spot_field_group_info() {
  $export = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_contact|node|report|form';
  $field_group->group_name = 'group_contact';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'report';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_report';
  $field_group->data = array(
    'label' => 'Contact details',
    'weight' => '-2',
    'children' => array(
      0 => 'field_e_mail',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Kontaktangaben',
      'instance_settings' => array(
        'required_fields' => 0,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $export['group_contact|node|report|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_location|node|report|form';
  $field_group->group_name = 'group_location';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'report';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_report_form';
  $field_group->data = array(
    'label' => '1. Location',
    'weight' => '1',
    'children' => array(
      0 => 'field_address',
      1 => 'field_geo',
    ),
    'format_type' => 'bootstrap_fieldgroup_nav_item',
    'format_settings' => array(
      'label' => '1. Location',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => ' group-location field-group-bootstrap_fieldgroup_nav_item',
      ),
    ),
  );
  $export['group_location|node|report|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_multimedia|node|report|form';
  $field_group->group_name = 'group_multimedia';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'report';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_report_form';
  $field_group->data = array(
    'label' => '3. Media',
    'weight' => '3',
    'children' => array(
      0 => 'field_image',
    ),
    'format_type' => 'bootstrap_fieldgroup_nav_item',
    'format_settings' => array(
      'label' => '3. Media',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => ' group-multimedia field-group-bootstrap_fieldgroup_nav_item',
      ),
    ),
  );
  $export['group_multimedia|node|report|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_report_form|node|report|form';
  $field_group->group_name = 'group_report_form';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'report';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'report',
    'weight' => '0',
    'children' => array(
      0 => 'group_location',
      1 => 'group_multimedia',
      2 => 'group_report',
    ),
    'format_type' => 'bootstrap_fieldgroup_nav',
    'format_settings' => array(
      'label' => 'report',
      'instance_settings' => array(
        'bootstrap_nav_type' => 'tabs',
        'bootstrap_stacked' => '0',
        'bootstrap_orientation' => '0',
        'required_fields' => 1,
        'classes' => ' group-report-form field-group-bootstrap_fieldgroup_nav',
      ),
    ),
  );
  $export['group_report_form|node|report|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_report|node|report|form';
  $field_group->group_name = 'group_report';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'report';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_report_form';
  $field_group->data = array(
    'label' => '2. Your report',
    'weight' => '2',
    'children' => array(
      0 => 'body',
      1 => 'field_category',
      2 => 'title',
      3 => 'group_contact',
    ),
    'format_type' => 'bootstrap_fieldgroup_nav_item',
    'format_settings' => array(
      'label' => '2. Your report',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => ' group-report field-group-bootstrap_fieldgroup_nav_item',
      ),
    ),
  );
  $export['group_report|node|report|form'] = $field_group;

  return $export;
}
