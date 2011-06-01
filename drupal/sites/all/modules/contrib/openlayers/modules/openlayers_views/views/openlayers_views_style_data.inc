<?php

/**
 * @file
 * This file holds style plugin for OpenLayers Views
 *
 * @ingroup openlayers
 */

/**
 * @class
 * Extension of the Views Plugin Syle for OpenLayers
 *
 * This class extended the default views plugin class to provide
 * a style plugin for the Open Layers module.
 */
class openlayers_views_style_data extends views_plugin_style {
  /**
   * Set default options
   */
  function option_definition() {
    $options = parent::option_definition();
    $options['data_source'] = array('default' => 'openlayers_wkt');
    return $options;
  }

  /**
   * Options form
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $fields = array();

    // Get list of fields in this view & flag available geodata fields
    $handlers = $this->display->handler->get_handlers('field');

    // Check for any fields, as the view needs them
    if (empty($handlers)) {
      $form['error_markup'] = array(
        '#value' => t('You need to enable at least one field before you can configure your field settings'),
        '#prefix' => '<div class="error form-item description">',
        '#suffix' => '</div>',
      );
      return;
    }

    // Go through fields
    foreach ($handlers as $field_id => $handler) {
        if ($handler->content_field['type'] == 'openlayers_wkt' &&
          $handler->options['format'] !== 'openlayers_wkt'
        ) {
          form_set_error('', t('WKT Field %field not available 
            because it is formatted as a map. Set its format to WKT Value to 
            make it available.', array('%field' => $handler->definition['title'])));
        }
        else {
          $fields[$field_id] = $handler->definition['title'];
        }
    }

    // Data Source options
    $form['data_source'] = array(
      '#type' => 'fieldset',
      '#tree' => TRUE,
      '#title' => t('Data Source'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );
    $form['data_source']['value'] = array(
      '#type' => 'select',
      '#title' => t('Map Data Sources'),
      '#description' => t('Choose which sources of data that the map will provide features for.'),
      '#options' => array(
        'other_latlon' => t('Other: Lat/Lon Point'),
        'other_boundingbox' => t('Other: Bounding Box'),
        'openlayers_wkt' => t('OpenLayers WKT')
      ),
      '#default_value' => $this->options['data_source']['value'],
    );
    // Other Lat and Lon data sources
    if (count($fields > 0)) {
      $form['data_source']['other_lat'] = array(
        '#type' => 'select',
        '#title' => t('Latitude Field'),
        '#description' => t('Choose a field for Latitude.  This should be a field that is a decimal or float value.'),
        '#options' => $fields,
        '#default_value' => $this->options['data_source']['other_lat'],
        '#process' => array('views_process_dependency'),
        '#dependency' => array('edit-style-options-data-source-value' => array('other_latlon')),
      );
      $form['data_source']['other_lon'] = array(
        '#type' => 'select',
        '#title' => t('Longitude Field'),
        '#description' => t('Choose a field for Longitude.  This should be a field that is a decimal or float value.'),
        '#options' => $fields,
        '#default_value' => $this->options['data_source']['other_lon'],
        '#process' => array('views_process_dependency'),
        '#dependency' => array('edit-style-options-data-source-value' => array('other_latlon')),
      );

      // Other: WKT
      $form['data_source']['openlayers_wkt'] = array(
        '#type' => 'select',
        '#title' => t('WKT Field'),
        '#description' => t('Choose the OpenLayers WKT field.'),
        '#options' => $fields,
        '#default_value' => $this->options['data_source']['openlayers_wkt'],
        '#process' => array('views_process_dependency'),
        '#dependency' => array('edit-style-options-data-source-value' => array('openlayers_wkt')),
      );

      // Other: Bounding Box
      $form['data_source']['other_top'] = array(
        '#type' => 'select',
        '#title' => t('Top Field'),
        '#description' => t('Choose a field for Top.  This should be a field that is a decimal or float value.'),
        '#options' => $fields,
        '#default_value' => $this->options['data_source']['other_top'],
        '#process' => array('views_process_dependency'),
        '#dependency' => array('edit-style-options-data-source-value' => array('other_boundingbox')),
      );
      $form['data_source']['other_right'] = array(
        '#type' => 'select',
        '#title' => t('Right Field'),
        '#description' => t('Choose a field for Right.  This should be a field that is a decimal or float value.'),
        '#options' => $fields,
        '#default_value' => $this->options['data_source']['other_right'],
        '#process' => array('views_process_dependency'),
        '#dependency' => array('edit-style-options-data-source-value' => array('other_boundingbox')),
      );
      $form['data_source']['other_bottom'] = array(
        '#type' => 'select',
        '#title' => t('Bottom Field'),
        '#description' => t('Choose a field for Bottom.  This should be a field that is a decimal or float value.'),
        '#options' => $fields,
        '#default_value' => $this->options['data_source']['other_bottom'],
        '#process' => array('views_process_dependency'),
        '#dependency' => array('edit-style-options-data-source-value' => array('other_boundingbox')),
      );
      $form['data_source']['other_left'] = array(
        '#type' => 'select',
        '#title' => t('Left Field'),
        '#description' => t('Choose a field for Left.  This should be a field that is a decimal or float value.'),
        '#options' => $fields,
        '#default_value' => $this->options['data_source']['other_left'],
        '#process' => array('views_process_dependency'),
        '#dependency' => array('edit-style-options-data-source-value' => array('other_boundingbox')),
      );
    }
    $form['data_source']['name_field'] = array(
        '#type' => 'select',
        '#title' => t('Title Field'),
        '#description' => t('Choose the field which will appear as a title on tooltips.'),
        '#options' => array_merge(array('' => ''), $fields),
        '#default_value' => $this->options['data_source']['name_field'],
    );
    $form['data_source']['keep_orig_name_field'] = array(
        '#type' => 'checkbox',
        '#title' => t('Keep non-rendered title field attribute'),
        '#description' => t('Check to ensure the title field will still be present as an attribute in its non-rendered form'),
        '#default_value' => $this->options['data_source']['keep_orig_name_field']
    );
    $form['data_source']['description_field'] = array(
        '#type' => 'select',
        '#title' => t('Description Field'),
        '#description' => t('Choose the field which will appear as a description on tooltips.'),
        '#required' => FALSE,
        '#options' => array_merge(array('' => ''), $fields),
        '#default_value' => $this->options['data_source']['description_field'],
    );
    $form['data_source']['keep_orig_desc_field'] = array(
        '#type' => 'checkbox',
        '#title' => t('Keep non-rendered description field attribute'),
        '#description' => t('Check to ensure the description field will still be present as an attribute in its non-rendered form'),
        '#default_value' => $this->options['data_source']['keep_orig_desc_field']
    );

    // Attributes and variable styling description
    $form['attributes'] = array(
      '#type' => 'fieldset',
      '#title' => t('Attributes and Styling'),
      '#description' => t('Attributes are field data attached to each feature.  This can be used with styling to create Variable styling.'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    // Make array of attributes
    $variable_fields = array();
    // Add name and description
    if (!empty($this->options['data_source']['name_field'])) {
      $variable_fields['name'] = '${name}';
    }
    if (!empty($this->options['data_source']['description_field'])) {
      $variable_fields['description'] = '${description}';
    }
    // Go through fields
    foreach ($this->view->display_handler->get_handlers('field') as $field => $handler) {
      if (($field != $this->options['data_source']['name_field']) && ($field != $this->options['data_source']['description_field'])) {
        $variable_fields[$field] = '${' . $field . '}';
        $variable_fields[$field . '_rendered'] = '${' . $field . '_rendered}';
      }
    }
    $form['attributes']['styling'] = array(
      '#type' => 'markup',
      '#value' => '
        <p>' . t('Any fields that you add to this view will be attached to their respective feature (point, line, polygon) as attributes.  These attributes can then be used to add variable styling to your themes.  This is accomplished by using the %syntax syntax in the values for a style.  The following is a list of formatted variables that are currently available; these can be placed right in the style interface.  The %rendered one means that it has been processed by Views and may have a different value.', array('%syntax' => '${field_name}', '%rendered' => '_rendered')) . '</p>
        ' . theme('item_list', $variable_fields) . '
        <p>' . t('Please note that this does not apply to Grouped Displays.') . '</p>
      ',
    );
  }

  /**
   * Map features
   */
  function map_features($records = array()) {
    $features = array();
    $data_source = $this->options['data_source'];

    // Get list of fields in this view
    $handlers = $this->display->handler->get_handlers('field');
    $fields = array();
    foreach ($handlers as $field_id => $handler) {
      $fields[$field_id] = $handler->definition;
      $fields[$field_id]['field_alias'] = $handler->field_alias;
    }
    
    // Small fix to solve backwards compatability and bad commit
    // and release.  This should be in an update function.
    // See: http://drupal.org/node/895082
    $ds = $data_source['value'];       
    if (is_array($ds)) {
      $ds = array_shift(array_keys($data_source['value']));
    } 

    // Build feature. We create one feature per field per row.

    // @TODO: Redo the grouped-version of this style plugin.
    // Separate mapping for grouped views.  Currently only adds name and count.
    if (!empty($this->options['grouping'])) {
      $this->view->row_index = 0;
      foreach ($records as $id => $record) {
        $feature = array();
        $feature['attributes']['name'] = $id;

        switch ($ds) {
          case 'other_latlon':
            $lat_field = $fields[$data_source['other_lat']]['field_alias'];
            $lon_field = $fields[$data_source['other_lon']]['field_alias'];
            $lon = current($record)->$lon_field;
            $lat = current($record)->$lat_field;
            if (isset($lat) && isset($lon)) {
              $feature['wkt'] = 'POINT(' . $lon . ' ' . $lat . ')';
              $feature['projection'] = '4326';
            }
            break;
          case 'openlayers_wkt':
            $wkt_field = $fields[$data_source['openlayers_wkt']]['field_alias'];
            $wkt = current($record)->$wkt_field;
            if ($wkt) {
              $feature['wkt'] = $wkt;
              $feature['projection'] = '4326';
            }
            break;
          case 'other_boundingbox':
            $top_field = $fields[$data_source['other_top']]['field_alias'];
            $right_field = $fields[$data_source['other_right']]['field_alias'];
            $left_field = $fields[$data_source['other_left']]['field_alias'];
            $bottom_field = $fields[$data_source['other_bottom']]['field_alias'];
            $top = current($record)->$top_field;
            $right = current($record)->$right_field;
            $left = current($record)->$left_field;
            $bottom = current($record)->$bottom_field;
            if (isset($lat) && isset($lon)) {
              $feature['wkt'] = 'MULTIPOLYGON(' .
                $left . ' ' . $top . ', ' .
                $right . ' ' . $top . ', ' .
                $right . ' ' . $bottom . ', ' .
                $left . ' ' . $bottom . ')';
              $feature['projection'] = '4326';
            }
            break;
        }

        $descriptions = array();
        $excluded_fields = array();
        foreach ($record as $k => $v) {
          foreach ($handlers as $hid => $handler) {
            $rendered_record[$handler->field_alias] = $handler->advanced_render($v);
            if (!empty($handler->options['exclude'])) {
              $excluded_fields[] = $handler->field_alias;
            }
          }

          // Get aliases name
          $name_field = (!empty($data_source['name_field'])) ? 
            $fields[$data_source['name_field']]['field_alias'] : NULL;
          $description_field = (!empty($data_source['description_field'])) ? 
            $fields[$data_source['description_field']]['field_alias'] : NULL;
          // Create output
          $descriptions[] = theme('openlayers_views_group_display_item', 
            $rendered_record[$name_field], $rendered_record[$description_field]);
        }

        $grouped_description = theme('item_list', $descriptions);

        $feature['attributes']['description'] = $grouped_description;
        $feature['attributes']['count'] = count($record);

        foreach ($fields as $fid => $field) {
          $field_alias = $field['field_alias'];
          if (!in_array($field_alias, $excluded_fields, TRUE)) {
            // Use regular field name as this is easier to understand
            $feature['attributes'][$fid . "_rendered"] = $rendered_record[$field_alias];
          }
        }

        // Only add features with WKT data
        if (!empty($feature['wkt'])) {
          $features[] = theme('openlayers_views_render_feature', $feature, $record);
        }
        $this->view->row_index++;
      }
      unset($this->view->row_index);
      return $features;
    }

    // Separate mapping for non-grouped views.
    else {
      $excluded_fields = array();

      $records = array_shift($records);
      foreach ($records as $id => $record) {
        $feature = array();
        $rendered_record = array();
        // Note that excluded fields are still rendered in case they are used
        // as render tokens in later fields.
        foreach ($handlers as $hid => $handler) {
          $rendered_record[$handler->field_alias] = $handler->advanced_render($record);
          if (!empty($handler->options['exclude'])) {
            $excluded_fields[] = $handler->field_alias;
          }
        }

        switch ($ds) {
          case 'other_latlon':
            $lat_field = $fields[$data_source['other_lat']]['field_alias'];
            $lon_field = $fields[$data_source['other_lon']]['field_alias'];
            $lon = $record->$lon_field;
            $lat = $record->$lat_field;
            if (isset($lat) && isset($lon)) {
              $feature['wkt'] = 'POINT(' . $lon . ' ' . $lat . ')';
              $feature['projection'] = '4326';
            }

            $excluded_fields[] = $lat_field;
            $excluded_fields[] = $lon_field;
            break;
          case 'openlayers_wkt':
            $wkt_field = $fields[$data_source['openlayers_wkt']]['field_alias'];
            $wkt = $record->$wkt_field;
            if ($wkt) {
              $feature['wkt'] = $wkt;
              $feature['projection'] = '4326';
            }

            $excluded_fields[] = $wkt_field;
            break;
          case 'other_boundingbox':
            $top_field = $fields[$data_source['other_top']]['field_alias'];
            $right_field = $fields[$data_source['other_right']]['field_alias'];
            $left_field = $fields[$data_source['other_left']]['field_alias'];
            $bottom_field = $fields[$data_source['other_bottom']]['field_alias'];
            $top = $record->$top_field;
            $right = $record->$right_field;
            $left = $record->$left_field;
            $bottom = $record->$bottom_field;
            if (isset($top) && isset($left)) {
              $feature['wkt'] = 'MULTIPOLYGON(' .
                $left . ' ' . $top . ', ' .
                $right . ' ' . $top . ', ' .
                $right . ' ' . $bottom . ', ' .
                $left . ' ' . $bottom . ')';
              $feature['projection'] = '4326';
            }

            array_push($excluded_fields, $top_field, $right_field,
              $left_field, $bottom_field);
        }

        // Add the name and description attributes
        // as choosen through interface
        $name_field = NULL;
        $description_field = NULL;
        if (is_array($data_source) && $data_source['name_field']) {
          $name_field = $fields[$data_source['name_field']]['field_alias'];
          $feature['attributes']['name'] = $rendered_record[$name_field];
          $excluded_fields[] = $name_field;
          if (!empty($data_source['keep_orig_name_field'])) {
            $feature['attributes'][$data_source['name_field']] = $record->$name_field;
          }
        }
        else {
          $feature['attributes']['name'] = '';
        }
        if (is_array($data_source) && $data_source['description_field']) {
          $description_field = $fields[$data_source['description_field']]['field_alias'];
          $feature['attributes']['description'] = $rendered_record[$description_field];
          $excluded_fields[] = $description_field;
          if (!empty($data_source['keep_orig_desc_field'])) {
            $feature['attributes'][$data_source['description_field']] = $record->$description_field;
          }
        }
        else {
          $feature['attributes']['description'] = '';
        }

        // Fill in attributes that are not:
        // - Coordinate/bounding box fields
        // - Name/description (already processed)
        // - Views "excluded" fields
        foreach ($fields as $fid => $field) {
          $field_alias = $field['field_alias'];
          if (!in_array($field_alias, $excluded_fields, TRUE)) {
            // Use regular field name as this is easier to understand
            $feature['attributes'][$fid] = $record->$field_alias;
            $feature['attributes'][$fid . "_rendered"] = $rendered_record[$field_alias];
          }
        }

        // Only add features with WKT data
        if (!empty($feature['wkt'])) {
          $features[] = theme('openlayers_views_render_feature', $feature, $record);
        }
      }
      return $features;
    }
  }

  /**
   * Render the map features.
   */
  function render($result) {
    // Check for live preview.
    if (!empty($this->view->live_preview)) {
      return t('OpenLayers views are not compatible with live preview.');
    }

    // Group the rows according to the grouping field, if specified.
    $sets = $this->render_grouping($result, $this->options['grouping']);
    return $this->map_features($sets);
  }
}
