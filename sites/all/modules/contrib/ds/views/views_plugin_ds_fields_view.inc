<?php
// $Id: views_plugin_ds_fields_view.inc,v 1.1.2.15 2010/07/08 15:50:38 swentel Exp $

/**
 * @file
 * Contains the ds views fields style plugin.
 */

class views_plugin_ds_fields_view extends views_plugin_row {

  function option_definition() {
    $options = parent::option_definition();
    return $options;
  }

  /**
   * Provide a form for setting options.
   */
  function options_form(&$form, &$form_state) {

    if (!module_exists('ds_ui')) {
      $form['no_ui'] = array(
        '#type' => 'markup',
        '#value' => '&nbsp;&nbsp;'. t('The Display Suite UI module is not enabled, enable the module if you want to change this layout.'),
      );
      return;
    }

    $fields = array();
    $display_settings = array();
    foreach ($this->display->handler->get_handlers('field') as $field => $handler) {

      $type = ($handler->definition['handler'] == 'ds_handler_field_fieldgroup') ? DS_FIELD_TYPE_GROUP : DS_FIELD_TYPE_PREPROCESS;

      $fields[$field] = array(
        'title' => ($handler->label() == '') ? $handler->ui_name() : $handler->label(),
        'status' => DS_FIELD_STATUS_STATIC,
        'type' => $type,
      );

      // Build the display settings.
      $display_settings['views']['fields'][$field]['region'] = isset($this->options[$field]['region']) ? $this->options[$field]['region'] : 'disabled';
      $display_settings['views']['fields'][$field]['weight'] = isset($this->options[$field]['weight']) ? $this->options[$field]['weight'] : 0;
      $display_settings['views']['fields'][$field]['css-class'] = isset($this->options[$field]['css-class']) ? $this->options[$field]['css-class'] : '';
      $display_settings['views']['fields'][$field]['labelformat'] = isset($this->options[$field]['labelformat']) ? $this->options[$field]['labelformat'] : 'default';
      $display_settings['views']['fields'][$field]['parent'] = isset($this->options[$field]['parent']) ? $this->options[$field]['parent'] : '';
    }

    $form['#module'] = 'ds';
    $form['#build_mode'] = 'views';
    $form['#type_name'] = 'views_object';
    $form['#extra'] = array();
    $form['#regions'] = ds_regions();

    // Load the drag and drop form.
    module_load_include('inc', 'ds', 'includes/ds.display');
    ds_fields_display_form($form, $display_settings, $fields);

    // Add our javascript and css.
    $path = drupal_get_path('module', 'ds');
    $form['#prefix'] = '<script type="text/javascript" src="'. base_path() . $path .'/js/ds.js?d"></script>' .
        '<link type="text/css" rel="stylesheet" media="all" href="'. base_path() . $path .'/css/ds.css" />';

    // Special js stuff for drag and drop.
    $form['#js']['tableDrag']['fields']['ds-parent-id'][0] = array(
      'target' => 'ds-parent-id',
      'source' => 'ds-field-id',
      'group' => 'ds-parent-id',
      'subgroup' => 'ds-parent-id',
      'relationship' => 'parent',
      'action' => 'match',
      'hidden' => FALSE,
      'limit' => 1,
    );
    $form['#js']['tableDrag']['fields']['field-weight'][0] = array(
      'target' => 'field-weight',
      'source' => NULL,
      'relationship' => 'sibling',
      'action' => 'order',
      'hidden' => TRUE,
      'limit' => 0,
    );

    // We're using a different theme.
    $form['#theme'] = 'ds_display_overview_views_form';

    views_ui_standard_form_buttons($form, $form_state, 'views_ui_ds_form');
  }

  /**
   * Make the row_options a bit smaller :)
   */
  function options_submit($form, &$form_state) {
    foreach ($form_state['values']['row_options'] as $field => $value) {
      if ($field != 'buttons') {
        $form_state['values']['row_options'][$field] = array(
          'region' => $value['views']['region'],
          'weight' => $value['ds_weight'],
          'css-class' => $value['views']['css-class'],
          'labelformat' => $value['views']['label']['format'],
        );

        // Parent
        if (!empty($value['views']['parent_id'])) {
          $form_state['values']['row_options'][$field]['parent'] = $value['views']['parent_id'];
        }

        $form_state['values']['row_options']['regions'][$value['views']['region']][$field] = $value['ds_weight'];
      }
    }
    $form_state['values']['row_options']['module'] = $this->ds_views_base_table('module');
    $form_state['values']['row_options']['object_type'] = $this->ds_views_base_table('object_type');
  }

  /**
   * Store the base module.
   */
  function ds_views_base_table($key) {
    $views_base_table = $this->view->base_table;
    $ds_views_fields_modules = array(
      'node' => array(
        'module' => 'nd',
        'object_type' => 'node',
      ),
      'users' => array(
        'module' => 'ud',
        'object_type' => 'user'
      ),
      'comments' => array(
        'module' => 'cd',
        'object_type' => 'comment',
      ),
      'heartbeat_activity' => array(
        'module' => 'hds',
        'object_type' => 'heartbeatactivity',
      ),
    );

    if (isset($ds_views_fields_modules[$views_base_table])) {
      return $ds_views_fields_modules[$views_base_table][$key];
    }
    return 'ds-views';
  }

  /**
   * Render the fields.
   *
   * @param array $fields Collection of fields for display.
   */
  function ds_fields_render($fields) {

    $object = new stdClass;
    $object->type = 'all';
    $ds_fields = array();
    $ds_groups = array();
    $object->build_mode = 'ds_views';
    $object_type = $this->view->style_plugin->row_plugin->options['object_type'];
    $vars[$object_type] = $object_type;
    $regions = $this->view->style_plugin->row_plugin->options['regions'];
    $module = $this->view->style_plugin->row_plugin->options['module'];

    if (empty($object_type) || $object_type == 'ds-views') {
      return;
    }

    // Loop through views fields, see if they are a field or a group.
    foreach ($fields as $key => $field) {

      $region = $this->view->style_plugin->row_plugin->options[$key]['region'];
      if ($region == 'disabled') {
        continue;
      }
      $weight = $this->view->style_plugin->row_plugin->options[$key]['weight'];
      $labelformat = $this->view->style_plugin->row_plugin->options[$key]['labelformat'];
      $parent = (isset($this->view->style_plugin->row_plugin->options[$key]['parent'])) ? $this->view->style_plugin->row_plugin->options[$key]['parent'] : FALSE;

      // Class.
      $class = $field->class;
      if (!empty($this->view->style_plugin->row_plugin->options[$key]['css-class'])) {
        $class .= ' '. implode(' ', $this->view->style_plugin->row_plugin->options[$key]['css-class']);
      }

      // Build field key in ds_fields.
      $ds_fields[$key]['key'] = $key;
      $ds_fields[$key]['title'] = $field->label;
      $ds_fields[$key]['labelformat'] = $labelformat;
      $ds_fields[$key]['class'] = $class;
      $ds_fields[$key]['type'] = 'ds';

      if ($parent != FALSE) {

        // Remove the field from the $regions array.
        unset($regions[$region][$key]);

        // Get the form of the group and create a theme key.
        $parent_field = $fields[$parent];
        $format = (isset($parent_field->handler->options['ds_format'])) ? $parent_field->handler->options['ds_format'] : 'ds_group_fieldset';
        $ds_fields[$parent]['theme'] = $format;
        $ds_groups[$parent][$key] = $weight;
      }

      // Field content.
      $vars[$key] = $field->content;

      // Add key to preprocess fields.
      $object->preprocess_fields[$key] = DS_FIELD_TYPE_PREPROCESS;
    }

    // Vars.
    $object->ds_fields = $ds_fields;
    $object->ds_groups = $ds_groups;
    $vars['regions'] = $regions;

    // Return the row.
    return ds_render_content($object, $module, $vars);
  }

}
