<?php
// $Id: views_calc_plugin_style_chart.inc,v 1.7 2009/04/20 19:38:42 karens Exp $

/**
 * @file
 * Contains the chart style plugin.
 */

/**
 * Style plugin to render view as a chart.
 *
 * @ingroup views_style_plugins
 */
class views_calc_plugin_style_chart extends views_plugin_style {
  // Set default options.
  function options(&$options) {
    $options['format'] = 'pie2D';
    $options['height'] = 200;
    $options['width'] = 400;
    $options['color'] = 'ffffff';
    $options['aggregation_field'] = '';
    $options['calc_fields'] = array();
    $options['calc'] = 'COUNT';
    $options['precision'] = 2;
  }

  // Generate a form for setting options.
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    
    $form['format'] = array(
      '#type'    => 'select',
      '#title'   => t('Chart format'),
      '#options' => array(
        'line2D'  => t('Line 2D'),
        'hbar2D'  => t('Horizontal Bar 2D'),
        'vbar2D'  => t('Vertical Bar 2D'),
        'pie2D'   => t('Pie 2D'),
        'pie3D'   => t('Pie 3D'),
        'venn'    => t('Venn'),
        'scatter' => t('Scatter Plot')
      ),
      '#default_value' => $this->options['format'],
    );
    $form['height'] = array(
      '#type'          => 'textfield',
      '#title'         => t('Chart height'),
      '#default_value' => $this->options['height'],
      '#required'      => TRUE, // Google charts breaks if it is empty.
      '#description'   => t('An integer value, the number of pixels of height for this chart.'),
    );
    $form['width'] = array(
      '#type'          => 'textfield',
      '#title'         => t('Chart width'),
      '#default_value' => $this->options['width'],
      '#required'      => TRUE, // Google charts breaks if it is empty.
      '#description'   => t('An integer value, the number of pixels of width for this chart.'),
    );
    $form['color'] = array(
      '#type'          => 'textfield',
      '#title'         => t('Background color'),
      '#default_value' => $this->options['color'],
      '#description'   => t('In hexadecimal format (RRGGBB). Do not use the # symbol.'),
      '#required'      => TRUE, // Google charts breaks if it is empty.
    );
    $form['show_legend'] = array(
      '#type'          => 'checkbox',
      '#title'         => t('Show legend'),
      '#default_value' => $this->options['show_legend'],
      '#description'   => t('Display legend next to the chart.'),
    );
      
    $form['aggregation_field'] = array(
      '#type' => 'select',
      '#title' => t('Aggregation field'),
      '#options' => $this->aggregated_field_options(),
      '#default_value' => $this->options['aggregation_field'],
      '#description' => t('Select a field to aggreagate the results on.')
      );
    // TODO Charts module cannot currently handle more than one series, 
    // update Multiple to TRUE if that changes.
    $form['calc_fields'] = array(
      '#type' => 'select',
      '#title' => t('Computation field'),
      '#options' => $this->aggregated_field_options(),
      '#default_value' => $this->calc_fields(),
      '#multiple' => FALSE, 
      '#description' => t('Select field to perform computations on.')
      );
    $form['calc'] = array(
      '#type' => 'select',
      '#title' => t('Computation to perform'),
      '#options' => $this->calc_options(),
      '#default_value' => $this->options['calc'],
      );
    $form['precision'] = array(
      '#type' => 'select',
      '#title' => t('Precision'),
      '#options' => range(0, 4),
      '#default_value' => $this->options['precision'],
      '#description' => t('Decimal points to use in computed values.'),
      );   
  }

  function calc_options() {
    return array(
      '' => t('None'), 
      'SUM' => t('Sum'), 
      'COUNT' => t('Count'), 
      'AVG' => t('Average'), 
      'MIN' => t('Minimum'), 
      'MAX' => t('Maximum'),
      );
  }

  /**
   * Create an options array of available fields from this view.
   */
  function aggregated_field_options() {
    $field_names = array();
    $handlers = $this->display->handler->get_handlers('field');
    foreach ($handlers as $field => $handler) {
      if ($label = $handler->label()) {
        $field_names[$field] = $label;
      }
      else {
        $field_names[$field] = $handler->ui_name();
      }
    }
    return $field_names;    
  }
  
  /**
   * Make sure calc_fields is always an array, even when not multiple.
   */
  function calc_fields() {
    $calc_fields = (array) $this->options['calc_fields'];
    return array_values($calc_fields);
  }
  
  // Define and display a chart from the grouped values.
  function render() {
    // Scan all Views data and insert them into a series.
    $data = array();
    
    // Get values from rows.
    foreach ($this->calc_fields() as $calc) {
      foreach ($this->view->result as $row) {
        foreach ($this->view->field as $key => $field) {
          if ($key == $this->options['aggregation_field']) {
            $legend_field = array_key_exists($calc, $this->view->field) ? $this->view->field[$calc] : NULL;
            $legend = !empty($legend_field->options['label']) ? $legend_field->options['label'] : NULL;
            if ($this->options['show_legend']) {
              $data[$calc]['#legend'] = $legend;
            }
            $value['#label'] = strip_tags(theme_views_view_field($this->view, $this->view->field[$key], $row)); // .': '. $row->$calc;
            $value['#value'] = $row->$calc;
            $data[$calc][] = $value;
          }
        }
      }
    }
      
    // Get chart settings from options form.
    $legend_field = $this->view->field[$this->options['aggregation_field']];
    $chart = array(
      '#type'   => $this->options['format'],
      '#height' => $this->options['height'],
      '#width'  => $this->options['width'],
      '#color'  => $this->options['color'],
    );

    // Use the view title as the chart title.
    $chart['#title'] = $this->view->get_title();
    
    // Insert series into the chart array.
    foreach ($data as $series) {
      $chart[] = $series;
    }
    
    // Print the chart.
    return charts_chart($chart);
  }
  
  function query() {
    parent::query();
        
    // Clear the fields out, we'll replace them with calculated values.
    $this->view->query->clear_fields();
    // Clear out any sorting, it can create unexpected results
    // when Views adds aggregation values for the sorts.
    $this->view->query->orderby = array();
    
    // Add the grouping information to the query.
    // Field setting of array('aggregate' => TRUE) tells Views not to force 
    // another aggregation in for this field.
            
    foreach ($this->view->field as $field) {
      $query_field = substr($field->field, 0, 3) == 'cid' ? $field->definition['calc'] : $field->table .'.'. $field->field;
      $query_alias = $field->field_alias;
      
      // Add the aggregation.
      if ($field->field == $this->options['aggregation_field']) {
        $this->view->query->add_orderby(NULL, NULL, 'asc', $query_alias);
        $this->view->query->add_groupby($query_field);
        if (substr($field->field, 0, 3) == 'cid') {
          $this->view->query->add_field(NULL, $query_field, $field->field, array('aggregate' => TRUE));
        }
        else {
          $this->view->query->add_field($field->table, $field->field, NULL, array('aggregate' => TRUE));
        }
      }
      // Add computed values.
      if (in_array($field->field, $this->calc_fields())) {
        $sql = "ROUND(". $this->options['calc'] ."($query_field), ". $this->options['precision'] .")";
        $this->view->query->add_field(NULL, $sql, $field->field, array('aggregate' => TRUE));
        
        // TODO This part is not relationship-safe, needs additional work
        // to join in the right table if the computation is done
        // on a field that comes from a relationship.
        
        // Make sure the table with the right alias name is available 
        // (it might have been dropped during Views optimizations.)
        $this->view->query->add_table($field->table);
      }
    }
  }
}