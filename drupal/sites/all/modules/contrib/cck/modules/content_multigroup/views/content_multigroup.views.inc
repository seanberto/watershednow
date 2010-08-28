<?php
// $Id: content_multigroup.views.inc,v 1.1.2.2 2009/08/10 03:53:05 markuspetrux Exp $

/**
 * @file
 * Views integration for Content Multigroups.
 */

/**
 * Implementation of hook_views_data_alter().
 */
function content_multigroup_views_data_alter(&$data) {
  // Scan all field groups in the system.
  foreach (fieldgroup_groups() as $type_name => $groups) {
    $type_label = node_get_types('name', $type_name);

    foreach ($groups as $group_name => $group) {
      // Let's focus on multigroups that really accept multiple values.
      if ($group['group_type'] == 'multigroup' && !empty($group['settings']['multigroup']['multiple'])) {

        // Scan all fields in this multigroup.
        $field_labels = array();
        foreach (array_keys($group['fields']) as $field_name) {
          // Load information about the field for this particular content type.
          $field = content_fields($field_name, $type_name);

          // Get the Views table alias.
          $table_alias = content_views_tablename($field);

          // Discard this field if not already exposed to Views.
          if (isset($data[$table_alias])) {
            $field_labels[$field_name] = t($field['widget']['label']);
          }
        }

        if (!empty($field_labels)) {
          // Build the name for this filter.
          // The scope of field groups is the content type itself. You can
          // have more than one group with the same name but different fields
          // in different content types. Therefore, we need the type name and
          // multigroup name.
          $db_field = 'multigroup_'. $type_name .'_'. $group_name;

          // Build the labels for the filter.
          $label_truncated = truncate_utf8(t($group['label']), 10, TRUE);
          $title = t('@group_label multigroup in @type_label', array(
            '@group_label' => t($group['label']),
            '@type_label' => $type_label,
          ));
          $title_short = t('@label-truncated in @type_label', array(
            '@label-truncated' => $label_truncated,
            '@type_label' => $type_label,
          ));
          $help_text = t('Synchronize multiple values for fields in @group_label multigroup, @type_label type. Fields in this group: @fields.', array(
            '@group_label' => t($group['label']),
            '@type_label' => $type_label,
            '@fields' => implode(', ', $field_labels),
          ));

          // Attach the new filter to the node table.
          $data['node'][$db_field] = array(
            'group' => t('Content multigroup'),
            'title' => $title,
            'title short' => $title_short,
            'help' => $help_text,
            'filter' => array(
              'field' => $db_field,
              'table' => 'node',
              'handler' => 'content_multigroup_handler_filter',
              'content_type_name' => $type_name,
              'content_group_name' => $group_name,
              'allow empty' => TRUE,
            ),
          );
        }
      }
    }
  }
}

/**
 * Implementation of hook_views_handlers().
 */
function content_multigroup_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'content_multigroup') . '/views/handlers',
    ),
    'handlers' => array(
      'content_multigroup_handler_filter' => array(
        'parent' => 'views_handler_filter',
      ),
    ),
  );
}
