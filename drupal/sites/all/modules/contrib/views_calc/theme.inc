<?php
// $Id: theme.inc,v 1.9 2009/04/23 18:32:48 karens Exp $

/**
 * @file theme.inc
 *
 * An array of preprocessors to fill variables for templates and helper
 * functions to make theming easier.
 */
/**
 * Theme the form for the table style plugin
 */
function theme_views_calc_ui_table($form) {
  $output = drupal_render($form['description_markup']);

  $header = array(
    t('Field'),
    t('Justification'),
    t('Column calculations'),
    t('Column'),
    t('Separator'),
    array(
      'data' => t('Sortable'),
      'align' => 'center',
    ),
    array(
      'data' => t('Default sort'),
      'align' => 'center',
    ),
  );
  $rows = array();
  foreach (element_children($form['columns']) as $id) {
    $row = array();
    $row[] = drupal_render($form['info'][$id]['name']);
    $row[] = drupal_render($form['info'][$id]['justification']);
    $row[] = drupal_render($form['info'][$id]['has_calc']) . drupal_render($form['info'][$id]['calc']);
    $row[] = drupal_render($form['columns'][$id]);
    $row[] = drupal_render($form['info'][$id]['separator']);
    if (!empty($form['info'][$id]['sortable'])) {
      $row[] = array(
        'data' => drupal_render($form['info'][$id]['sortable']),
        'align' => 'center',
      );
      $row[] = array(
        'data' => drupal_render($form['default'][$id]),
        'align' => 'center',
      );
    }
    else {
      $row[] = '';
      $row[] = '';
    }
    $rows[] = $row;
  }

  // Add the special 'None' row.
  $rows[] = array(t('None'), '', '', '', '', array('align' => 'center', 'data' => drupal_render($form['default'][-1])));

  $output .= theme('table', $header, $rows);
  $output .= drupal_render($form);
  return $output;
}


/**
 * Display a view as a table style.
 */
function template_preprocess_views_calc_table(&$vars) {
  $view     = $vars['view'];
  if (!empty($view->views_calc_calculation)) {
    $vars['rows'] = array();
    return;
  }
  
  drupal_add_css(drupal_get_path('module', 'views_calc') .'/views_calc.css');
  
  // We need the raw data for this grouping, which is passed in as $vars['rows'].
  // However, the template also needs to use for the rendered fields.  We
  // therefore swap the raw data out to a new variable and reset $vars['rows']
  // so that it can get rebuilt.
  $result   = $vars['rows'];
  $vars['rows'] = array();
  $totals   = $view->totals;
  $sub_totals = $view->sub_totals;
    
  $options  = $view->style_plugin->options;
  $handler  = $view->style_plugin;
  $vars['options'] = $options;
  $hide_details = $options['detailed_values'];
  
  $fields   = &$view->field;
  $columns  = $handler->sanitize_columns($options['columns'], $fields);

  $active   = !empty($handler->active) ? $handler->active : '';
  $order    = !empty($handler->order) ? $handler->order : 'asc';

  $query    = tablesort_get_querystring();
  if ($query) {
    $query = '&' . $query;
  }

  foreach ($columns as $field => $column) {
    // render the header labels
    if ($field == $column && empty($fields[$field]->options['exclude'])) {
      $label = check_plain(!empty($fields[$field]) ? $fields[$field]->label() : '');
      if (empty($options['info'][$field]['sortable'])) {
        $vars['header'][$field] = $label;
      }
      else {
        // @todo -- make this a setting
        $initial = 'asc';

        if ($active == $field && $order == 'asc') {
          $initial = 'desc';
        }

        $image = theme('tablesort_indicator', $initial);
        $title = t('sort by @s', array('@s' => $label));
        $link_options = array(
          'html' => true,
          'attributes' => array('title' => $title),
          'query' => 'order=' . urlencode($field) . '&sort=' . $initial . $query,
        );
        $vars['header'][$field] = l($label . $image, $_GET['q'], $link_options);
      }
    }

    // Create a second variable so we can easily find what fields we have and what the
    // CSS classes should be.
    $vars['fields'][$field] = views_css_safe($field);
    if ($active == $field) {
      $vars['fields'][$field] .= ' active';
    }

    // Render each field into its appropriate column.
    foreach ($result as $num => $row) {
      if (!empty($fields[$field]) && empty($fields[$field]->options['exclude'])) {
        $field_output = $fields[$field]->theme($row);

        // Don't bother with separators and stuff if the field does not show up.
        if (!isset($field_output) && isset($vars['rows'][$num][$column])) {
          continue;
        }
        
        if ($hide_details) {
          continue;
        }

        // Place the field into the column, along with an optional separator.
        if (isset($vars['rows'][$num][$column])) {
          if (!empty($options['info'][$column]['separator'])) {
            $vars['rows'][$num][$column] .= filter_xss_admin($options['info'][$column]['separator']);
          }
        }
        else {
          $vars['rows'][$num][$column] = '';
        }

        $vars['rows'][$num][$column] .= $field_output;
      }
    }
  }
  
  // Add totals.
  if($view->total_rows > $view->pager['items_per_page']) {
    $process_available = array('sub_totals', 'totals');  
  }
  else{
    $process_available = array('totals');
    $vars['sub_totals'] = array(); //if don't set, error in template
  }
  foreach ($process_available as $process) {
    $vars[$process] = array();
    $added_label = array();
    foreach ($columns as $field => $column) {
      $field_alias = $fields[$field]->field_alias;
      if ($field == $column && empty($fields[$field]->options['exclude'])) {
        foreach ($$process as $num => $row) {
          $type = '';
          foreach ($row as $item) {
            if (in_array($item, array_keys(_views_calc_calc_options()))) {
              $type = $item;
              break;
            }
          }
          if (!empty($row->$field_alias) || (isset($row->$field_alias) && $row->$field_alias === 0)) {
    
            // COUNT is always a numeric value, no matter what kind of field it is.
            if ($type == 'COUNT') {
              $vars[$process][$type][$column] =  $row->$field_alias;
            }
            // Calculations other than COUNT should run the value through the field's theme.
            // This will allow dates and numeric values to apply the right formatting to the result.
            else {
              $vars[$process][$type][$column] =  $fields[$field]->theme($row);
            }
          }
          elseif (!empty($type)) {
            // Add the calc type label into the first empty column.
            // Identify which is the sub total and which the grand total
            // when both are provided.
            if (empty($added_label[$type])) {
              if ($process == 'sub_totals') {
                $label = t("Page !Calculation", array("!Calculation" => $type));
              }
              else {
                $label = t("Total !Calculation", array("!Calculation" => $type));
              }
              $vars[$process][$type][$column] = $label;
              $added_label[$type] = TRUE;
            }
            else {
              $vars[$process][$type][$column] = '';
            }
          }
        }
      }
    }
  }

  $vars['class'] = 'views-table';
  if (!empty($options['sticky'])) {
    drupal_add_js('misc/tableheader.js');
    $vars['class'] .= " sticky-enabled";
  }
  
  return;
     
}