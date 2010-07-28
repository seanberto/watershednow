<?php
// $Id: content.inc,v 1.1.2.2.2.1 2009/06/01 21:41:07 jamesandres Exp $

/**
 * @file
 * CRUD functions for creation of CCK fields and widgets.
 */

/**
 * Create a new field for an existing content type.
 *
 * NOTE: Use install_add_existing_field() to add an existing field.
 *
 * TIP: You can call print_r(install_get_widget_types()); to display a list of
 *      available $field_widget_type options.
 *
 * @param $content_type
 *   Required. Existing content type (text id).
 * @param $field_name
 *   Required. A field name using only alpha-numeric and underscores.
 * @param $field_widget_type
 *   Required. A special name that uniquely identifies both the field type and
 *   widget type. Each option item on the first page of the 'Add Field' form.
 * @param $properties
 *   Optional. An array of additional properties for the field.
 */
function install_create_field($content_type, $field_name, $field_widget_type, $label, $properties = array()) {
  $field_type = $properties['type'];

  include_once(drupal_get_path('module', 'content') .'/includes/content.admin.inc');

  // Manage weight, so that fields are ordered as they are created.
  // Pass 'weight' => ##, on the properties array to reset.
  static $weight = 1;
  $weight = isset($properties['weight']) ? $properties['weight'] : $weight + 1;

  $widget_types = install_get_widget_types();
  $_field_widget_type_name = $field_type . '-' . $field_widget_type;
  if (!array_key_exists($_field_widget_type_name, $widget_types)) {
  //if (!array_key_exists($field_widget_type, $widget_types)) {
    drupal_set_message("'$field_widget_type' is not an available field/widget.");
    return;
  }

  // First step of creating the field - create the basic field.
  // Corresponds to first page of the Add field UI.
  $default_field_add = array(
    'type_name' => $content_type,
    'field_name' => $field_name,
    'type' => $field_type,
    'widget_type' => $field_widget_type,
    'label' => $label,
    'module' => $properties['module'],
    'widget_module' => $properties['module'],
  );
  //'widget' => array(),
  
  content_field_instance_create($default_field_add);

  // Second step, update the field with our custom properties.
  // Corresponds to the second page (general config page) of content type creation.
  // We define defaults for a number of different field/widget types (eg. "referenceable_types"
  // is applicable to nodereference. These are ignored in cck when not applicable.
  $default_field_edit = array(
    'type_name' => $content_type,
    'field_name' => $field_name,
    'label' => $label,
    'weight' => $weight,
    'description' => '',
    'default_value_php' => '',
    'group' => 0,
    'required' => 0,
    'multiple' => FALSE,
    'text_processing' => 0,
    'max_length' => '',
    'allowed_values' => '',
    'allowed_values_php' => '',
    'rows' => 1,
    'op' => 'Save field settings',
    'submit' => 'Save field settings',
    'form_id' => '_content_admin_field',
    'referenceable_types' => array(),  // Used in nodereference fields and userreference.
  );

  // Add any field specific properties to the default global properties.
  foreach($properties AS $key => $value) {
    $default_field_edit[$key] = $properties[$key];
  }

  // Derive the widget and type from the input type.
  $widget_parts = explode('-', $field_widget_type);
  $default_field_edit['field_type'] = $widget_parts[0];
  $default_field_edit['widget_type'] = $widget_parts[1];
  //$default_field_edit['module'] = $widget_parts[0] .', optionwidgets';
  $default_field_edit['module'] = $properties['module'];

  $form_state = array();
  $form_state['values'] = $default_field_edit;
  content_field_edit_form_submit('_content_admin_field_add_new', $form_state);

}

/**
 * Returns an array of available fieldtype-widgettype arguments to
 * the $field_widget_type parameter of install_create_field()
 */
function install_get_widget_types() {
  $field_types = _content_field_types();
  $widget_types = _content_widget_types();
  $field_type_options = array();
  foreach ($field_types as $field_name => $field_type) {
    foreach ($widget_types as $widget_name => $widget_type) {
      if (in_array($field_name, $widget_type['field types'])) {
        $field_type_options[$field_name .'-'. $widget_name] = $field_type['label'] .': '. $widget_type['label'];
      }
    }
  }
  return $field_type_options;
}

/**
 * Add an existing field to another content type.
 *
 * @param $content_type       Required. Existing content type (text id).
 * @param $field_name         Required. Existing field name (text id).
 */
function install_add_existing_field($content_type, $field_name, $field_widget_type) {

  include_once(drupal_get_path('module', 'content') .'/includes/content.admin.inc');

  // Defaults for passing to submit function.
  $form_values = array(
    'field_name' => $field_name,
    'type_name' => $content_type,
    'widget_type' => $field_widget_type,
    'op' => 'Add field',
    'submit' => 'Add field',
  );

  module_load_include('inc', 'content', 'includes/content.crud');
  content_field_instance_create($form_values);
}

/**
 * Create a new field group for an existing content type.
 *
 * @param $content_type       Required. Existing content type (text id).
 * @param $group_name         An alpha-numeric/underscore group name.
 * @param $label              Required. A nice text name for your group.
 * @param $settings           Optional. An array of settings, see the default array
 *                            defined in the function for an example.
 * @param $weight             Optional, a standard weight integer.
 *
 */
function install_create_field_group($content_type, $group_name, $label, $settings = array(), $weight = 0) {
  if (!is_array($settings)) {
    // Default settings array.
    $settings = array(
      'form' => array('style' => 'fieldset', 'description' => ''),
      'display' => array('description' => '', 'teaser' => NULL, 'label' => NULL),
    );
  }
  db_query("INSERT INTO {content_group} (type_name, group_name, label, settings, weight)
    VALUES ('%s', '%s', '%s', '%s', %d)",
    $content_type, $group_name, $label, serialize($settings), $weight);
}

/**
 * Assign fields to a field group.
 *
 * @param $content_type   Required. Existing content type (text id).
 * @param $group_name     Required. Existing group name (text id).
 * @param $fields         Required. An array of field names.
 */
function install_fields_in_group($content_type, $group_name, $fields) {
  if (!is_array($fields)) {
    $fields = array($fields);
  }
  foreach ($fields AS $field_name) {
    db_query("INSERT INTO {content_group_fields} (type_name, group_name, field_name)
      VALUES ('%s', '%s', '%s')",
      $content_type, $group_name, $field_name);
  }
}