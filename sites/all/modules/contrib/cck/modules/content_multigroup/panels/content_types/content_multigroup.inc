<?php
// $Id: content_multigroup.inc,v 1.1.2.3 2009/11/01 10:44:16 markuspetrux Exp $

/**
 * @file
 * This file provides a CTools content type for multigroups.
 */

/**
 * Callback function to supply a list of content types.
 */
function content_multigroup_content_multigroup_ctools_content_types() {
  return array(
    'title' => t('Content multigroup'),
    'defaults' => array('label' => 'hidden', 'format' => 'simple', 'subgroup' => 'fieldset', 'empty' => ''),
  );
}

/**
 * Return all multigroup content types available.
 */
function content_multigroup_content_multigroup_content_type_content_types() {
  // This will hold all the individual multigroup content types.
  $types = array();

  // The outer loop goes through each node type with groups.
  foreach (fieldgroup_groups() as $node_type_groups) {
    // The inner loop gives us each fieldgroup on each node type with groups.
    foreach ($node_type_groups as $group) {
      // Skip field groups that are not of multigroup type.
      if ($group['group_type'] != 'multigroup') {
        continue;
      }

      // Name the content type a combination of fieldgroup and node type names.
      $content_type_name = $group['type_name'] . ':' . $group['group_name'];

      // Assemble the information about the content type.
      $info = array(
        'category' => t('Node'),
        'icon' => 'icon_cck_field_group.png',
        'title' => t('Multigroup: @group in @type', array(
          '@group' => t($group['label']),
          '@type' => node_get_types('name', $group['type_name']),
        )),
        'description' => t('All fields from this field group on the referenced node.'),
        'required context' => new ctools_context_required(t('Node'), 'node', array('type' => array($group['type_name']))),
      );

      $types[$content_type_name] = $info;
    }
  }

  return $types;
}

/**
 * Output function for the 'multigroup' content type.
 */
function content_multigroup_content_multigroup_content_type_render($subtype, $conf, $panel_args, $context) {
  if (!isset($context->data)) {
    return;
  }
  $node = drupal_clone($context->data);

  // Extract the node type and fieldgroup name from the subtype.
  list($node_type, $group_name) = explode(':', $subtype, 2);

  // Get a list of all fieldgroups for this node type.
  $groups = fieldgroup_groups($node_type);

  if (!isset($groups[$group_name])) {
    return;
  }
  $group = $groups[$group_name];

  // Render the field group.
  $node->build_mode = NODE_BUILD_NORMAL;
  $group['settings']['display']['label'] = $conf['label'] == 'normal' || !empty($conf['override_title']) ? 'hidden' : $conf['label'];
  $group['settings']['display']['full']['format'] = $conf['format'];
  $group['settings']['display']['full']['exclude'] = 0;
  $group['settings']['multigroup']['subgroup']['full']['format'] = $conf['subgroup'];
  $group['settings']['multigroup']['subgroup']['full']['exclude'] = 0;
  $output = fieldgroup_view_group($group, $node);

  $block = new stdClass();
  if ($conf['label'] == 'normal') {
    $block->title = t($group['label']);
  }
  $block->content = !empty($output) ? $output : $conf['empty'];
  return $block;
}

/**
 * Returns a settings form for the custom type.
 */
function content_multigroup_content_multigroup_content_type_edit_form(&$form, &$form_state) {
  $conf = $form_state['conf'];

  $label_options = array(
    'normal' => t('Block title'),
    'above' => t('Above'),
  );
  $form['label'] = array(
    '#type' => 'select',
    '#title' => t('Multigroup label'),
    '#default_value' => !empty($conf['label']) && isset($label_options[$conf['label']]) ? $conf['label'] : 'hidden',
    '#options' => $label_options,
    '#description' => t('Configure how the field group label is going to be displayed. This option takes no effect when "Override title" option is enabled, the specified block title is displayed instead.'),
  );

  $format_options = array(
    'simple' => t('Simple'),
    'fieldset' => t('Fieldset'),
    'fieldset_collapsible' => t('Fieldset - Collapsible'),
    'fieldset_collapsed' => t('Fieldset - Collapsed'),
  );
  $form['format'] = array(
    '#type' => 'select',
    '#title' => t('Multigroup format'),
    '#default_value' => !empty($conf['format']) && isset($format_options[$conf['format']]) ? $conf['format'] : 'simple',
    '#options' => $format_options,
    '#description' => t('This option allows you to configure the field group format.'),
  );

  $subgroup_options = array(
    'simple' => t('Simple'),
    'fieldset' => t('Fieldset'),
    'fieldset_collapsible' => t('Fieldset - collapsible'),
    'fieldset_collapsed' => t('Fieldset - collapsed'),
    'hr' => t('Horizontal line'),
    'table-single' => t('Table - Single column'),
    'table-multiple' => t('Table - Multiple columns'),
  );
  $form['subgroup'] = array(
    '#type' => 'select',
    '#title' => t('Subgroup format'),
    '#default_value' => !empty($conf['subgroup']) && isset($subgroup_options[$conf['subgroup']]) ? $conf['subgroup'] : 'fieldset',
    '#options' => $subgroup_options,
    '#description' => t('This option allows you to configure the format of the subgroups in the multigroup.'),
  );

  $form['empty'] = array(
    '#type' => 'textarea',
    '#title' => t('Empty text'),
    '#description' => t('Text to display if group has no data. Note that title will not display unless overridden.'),
    '#rows' => 5,
    '#default_value' => $conf['empty'],
  );
}

function content_multigroup_content_multigroup_content_type_edit_form_submit(&$form, &$form_state) {
  // Copy everything from our defaults.
  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}

/**
 * Admin title for multigroup content type.
 */
function content_multigroup_content_multigroup_content_type_admin_title($subtype, $conf, $context) {
  // Extract the node type and fieldgroup name from the subtype.
  list($node_type, $group_name) = explode(':', $subtype, 2);

  // Get information about this field group for this node type.
  $groups = fieldgroup_groups($node_type);
  $group = $groups[$group_name];

  return t('"@s" multigroup: @group in @type', array(
    '@s' => $context->identifier,
    '@group' => t($group['label']),
    '@type' => node_get_types('name', $node_type),
  ));
}
