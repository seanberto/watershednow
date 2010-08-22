<?php
// $Id: content_multigroup_handler_filter.inc,v 1.1.2.2 2009/10/31 20:31:09 markuspetrux Exp $

/**
 * @file
 * Defines a content_multigroup filter object in views.
 *
 * This filter allows you to associate the deltas in a multigroup so that
 * views doesn't do a cross-join across all values in multivalue fields.
 */
class content_multigroup_handler_filter extends views_handler_filter {
  var $content_multigroup_fields;

  function can_expose() {
    return FALSE;
  }

  function admin_summary() {
    return t('+ delta');
  }

  /**
   * Get information about the multigroup.
   */
  function _get_multigroup() {
    $groups = fieldgroup_groups($this->definition['content_type_name']);
    return $groups[$this->definition['content_group_name']];
  }

  /**
   * Get information about the fields in this multigroup.
   */
  function _get_multigroup_fields() {
    if (!isset($this->content_multigroup_fields)) {
      $group = $this->_get_multigroup();
      $this->content_multigroup_fields = array();
      foreach (array_keys($group['fields']) as $field_name) {
        $field = content_fields($field_name, $this->definition['content_type_name']);
        $table_alias = content_views_tablename($field);
        $this->content_multigroup_fields[$table_alias] = $field;
      }
    }
    return $this->content_multigroup_fields;
  }

  /**
   * Define default value for master field.
   */
  function options(&$options) {
    $multigroup_fields = $this->_get_multigroup_fields();
    // Find the first required field.
    foreach ($multigroup_fields as $table_alias => $field) {
      if ($field['required']) {
        $options['content_multigroup_master_field'] = $table_alias;
        break;
      }
    }
    // Default to first field in the multigroup.
    if (empty($options['content_multigroup_master_field'])) {
      $options['content_multigroup_master_field'] = current(array_keys($multigroup_fields));
    }
  }

  /**
   * Options from to ask the user for a master field.
   */
  function options_form(&$form, &$form_state) {
    $group = $this->_get_multigroup();
    $options = array();
    foreach ($this->_get_multigroup_fields() as $table_alias => $field) {
      $options[$table_alias] = t($field['widget']['label']);
    }
    $form['content_multigroup_master_field'] = array(
      '#title' => t('Available fields in @group_label multigroup', array('@group_label' => t($group['label']))),
      '#type' => 'radios',
      '#options' => $options,
      '#default_value' => $this->options['content_multigroup_master_field'],
      '#description' => t('Select the field in this multigroup that will be used to build the primary join with the content table.'),
    );
  }

  /**
   * Add joins to the query to synchronize the fields in this multigroup.
   */
  function query() {
    // Create the join between the master field table and the node table.
    $base_alias = $this->query->ensure_table($this->options['content_multigroup_master_field'], $this->relationship);

    // Now we want to join the master field table with all other tables
    // related to fields in the same multigroup, but adding the delta
    // key to the join condition. This is what allows us to keep delta
    // values in sync for all fields in the same multigroup.
    foreach ($this->_get_multigroup_fields() as $table_alias => $field) {
      if ($table_alias != $this->options['content_multigroup_master_field']) {
        $alias = $this->query->ensure_table($table_alias, $this->relationship);
        $this->query->table_queue[$alias]['join']->extra = $base_alias  .'.delta = '. $alias .'.delta';
      }
    }
  }
}
