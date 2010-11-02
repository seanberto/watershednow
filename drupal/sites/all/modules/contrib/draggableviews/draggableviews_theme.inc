<?php
// $Id: draggableviews_theme.inc,v 1.6.2.31 2010/08/27 14:10:30 sevi Exp $

/**
 * @file
 * Theme functions.
 */

function template_preprocess_draggableviews_view_draggabletable(&$vars, $style_plugin_name) {
  $view     = $vars['view'];
  
  // COPY BEGIN from theme.inc,v 1.84.2.14 (2010/06/16). lines 304-404.

  // We need the raw data for this grouping, which is passed in as $vars['rows'].
  // However, the template also needs to use for the rendered fields.  We
  // therefore swap the raw data out to a new variable and reset $vars['rows']
  // so that it can get rebuilt.
  // Store rows so that they may be used by further preprocess functions.
  $result   = $vars['result'] = $vars['rows'];
  $vars['rows'] = array();

  $options  = $view->style_plugin->options;
  $handler  = $view->style_plugin;

  $fields   = &$view->field;
  $columns  = $handler->sanitize_columns($options['columns'], $fields);

  $active   = !empty($handler->active) ? $handler->active : '';
  $order    = !empty($handler->order) ? $handler->order : 'asc';

  $query    = tablesort_get_querystring();
  if ($query) {
    $query = '&' . $query;
  }

  // Fields must be rendered in order as of Views 2.3, so we will pre-render
  // everything.
  $renders = $handler->render_fields($result);
  // @Todo: This is a current workaround because render_fields doesn't return any values unless it is called twice.
  $renders = $handler->render_fields($result);

  foreach ($columns as $field => $column) {
    // render the header labels
    if ($field == $column && empty($fields[$field]->options['exclude'])) {
      $label = check_plain(!empty($fields[$field]) ? $fields[$field]->label() : '');
      if (empty($options['info'][$field]['sortable']) || !$fields[$field]->click_sortable()) {
        $vars['header'][$field] = $label;
      }
      else {
        // @todo -- make this a setting
        $initial = 'asc';

        if ($active == $field && $order == 'asc') {
          $initial = 'desc';
        }

        $title = t('sort by @s', array('@s' => $label));
        if ($active == $field) {
          $label .= theme('tablesort_indicator', $initial);
        }
        $link_options = array(
          'html' => true,
          'attributes' => array('title' => $title),
          'query' => 'order=' . urlencode($field) . '&sort=' . $initial . $query,
        );
        $vars['header'][$field] = l($label, $_GET['q'], $link_options);
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
        $field_output = $renders[$num][$field];

        if (!isset($vars['rows'][$num][$column])) {
          $vars['rows'][$num][$column] = '';
        }

        // Don't bother with separators and stuff if the field does not show up.
        if ($field_output === '') {
          continue;
        }

        // Place the field into the column, along with an optional separator.
        if ($vars['rows'][$num][$column] !== '') {
          if (!empty($options['info'][$column]['separator'])) {
            $vars['rows'][$num][$column] .= filter_xss_admin($options['info'][$column]['separator']);
          }
        }

        $vars['rows'][$num][$column] .= $field_output;
      }
    }
  }

  $count = 0;
  foreach ($vars['rows'] as $num => $row) {
    $vars['row_classes'][$num][] = ($count++ % 2 == 0) ? 'odd' : 'even';
  }

  $vars['row_classes'][0][] = 'views-row-first';
  $vars['row_classes'][count($vars['row_classes']) - 1][] = 'views-row-last';

  $vars['class'] = 'views-table';
  if (!empty($options['sticky'])) {
    drupal_add_js('misc/tableheader.js');
    $vars['class'] .= " sticky-enabled";
  }
  $vars['class'] .= ' cols-'. count($vars['rows']);
  
  // COPY END


  // Draggableviews output begin

  $vars['tabledrag_table_id'] = 'draggableview_'. $view->name .'_'. $vars['id'];

  $info = $view->draggableviews_info;
  $form_elements = &$view->form_elements;

  if (!isset($info['order'])) return;

  $vars['tabledrag_type'] = array();

  // loop through all rows the view returned
  foreach ($vars['rows'] as $i => $row) {

    $nid = $result[$i]->{$view->base_field};

    // build indentation (as tabledrag requires)
    $indentation = theme('indentation', $info['nodes'][$nid]['depth']);

    if (strcmp($view->base_table, 'node') == 0) {
      // Get node object.
      $node = node_load(array('nid' => $nid));
      if (isset($info['types'][$node->type])) {
        // set node type specification, if available (e.g. tabledrag-root)
        $vars['tabledrag_type'][$i] = 'tabledrag-'. $info['types'][$node->type];
      }
    }

    // Tabledrag needs all concerned fields to be input elements.
    // The values of the input elements will be changed by tabledrag while
    // dragging the rows. As we want to use these values we need to
    // give them names.
    //
    // Concerned fields are
    //  *) the order field
    //  *) the parent field (if hierarchy is used)

    // This for loop should prevent copy->paste'ing code.
    foreach (array('hierarchy', 'order') AS $modus) {
      if (isset($info[$modus])) {
        if ($modus == 'hierarchy') {
          // get the field that should be rendered
          $field = $info['hierarchy']['field'];
        }
        else {
          // get the field that should be rendered
          $field = $info['order']['field'];
        }

        // Get the form element. The field_ prefix prevents a possible conflict
        // with the hidden_nid field.
        $form_element = &$form_elements[$i]['field_'. $field['field_name']];

        // Render the new form element.
        $vars['rows'][$i][$field['field_name']] = drupal_render($form_element);

        if (!user_access('Allow Reordering') || $info['locked']) {
          // The tabledrag js would hide the input fields (depending on the settings). But the
          // tabledrag js will not be added (either because of missing permissions or because
          // the hierarchy is locked), so we have to hide the input fields on our own.
          if (!$info[$modus]['visible']) {
            $vars['style'][$field['field_name']] = 'display: none;';
          }
        }
      }
    }

    if (isset($info['hierarchy'])) {
      // Put indentation in front of first field and append rendered hidden node id
      // that we need for tabledrag and expand/collapse.
      $hidden_nid = &$form_elements[$i]['hidden_nid'];
      $key = key($vars['rows'][$i]);
      $vars['rows'][$i][$key] = $indentation . $vars['rows'][$i][$key] . drupal_render($hidden_nid);
    }

    // Mark extended nodes.
    $vars['draggableviews_extended'][$i] = !empty($info['nodes'][$nid]['extension']);

    // Let extension modules alter the output.
    foreach (module_implements('draggableviews_row_alter') as $module) {
      $function = $module .'_draggableviews_row_alter';
      $function($vars['rows'][$i], $nid, $info, $view);
    }
  }

  if (user_access('Allow Reordering') && !$info['locked']) {
    // Prepare tabledrag settings for output.
    $tabledrag = array();
    if (isset($info['order'])) {
      $tabledrag[] = array(
        'action'       => 'order',
        'relationship' => 'sibling',
        'source'       => $info['order']['field']['field_name'],
        'group'        => $info['order']['field']['field_name'],
        'subgroup'     => $info['order']['field']['field_name'],
        'hidden'       => !$info['order']['visible'],
        'limit'        => 0,
      );
    }
    if (isset($info['hierarchy'])) {
      $tabledrag[] = array(
        'action'       => 'match',
        'relationship' => 'parent',
        'source'       => 'hidden_nid',
        'group'        => $info['hierarchy']['field']['field_name'],
        'subgroup'     => $info['hierarchy']['field']['field_name'],
        'hidden'       => !$info['hierarchy']['visible'],
        'limit'        => $info['depth_limit'],
      );
    }

    // Add tabledrag behavior.
    foreach ($tabledrag as $drag) {
      drupal_add_tabledrag($vars['tabledrag_table_id'], $drag['action'], $drag['relationship'], $drag['group'], $drag['subgroup'], $drag['source'], $drag['hidden'], $drag['limit']);
    }
  }

  if (isset($info['hierarchy'])) {
    // Fetch expand information from database.
    $uid = $info['expand_links']['by_uid'] ? $user->uid : 0;
    $result = db_query("SELECT parent_nid, collapsed FROM {draggableviews_collapsed} WHERE uid=%d AND view_name='%s'", $uid, $view->name);
    $states = array();
    while ($state = db_fetch_object($result)) {
      $states[$state->parent_nid] = $state->collapsed;
    }

    // Check if "expand" links should be shown.
    if ($info['expand_links']['show']) {
      drupal_add_js(drupal_get_path('module', 'draggableviews') .'/draggableviews.js');

      // Let javascript know about the parent field.
      drupal_add_js(array(
                      'draggableviews' => array(
                        $vars['tabledrag_table_id'] => array(
                          'parent' => $info['hierarchy']['field']['field_name'],
                        ),
                      ),
                    ),
                    'setting');

      if (empty($states)) {
        // let js know whether child nodes should be expanded or not
        drupal_add_js(array(
                        'draggableviews' => array(
                          $vars['tabledrag_table_id'] => array(
                            'expand_default' => $options['tabledrag_expand']['collapsed'] ? 1 : 0,
                            'states' => array(),
                          ),
                        ),
                      ),
                      'setting');
      }
      else {
        drupal_add_js(array(
                        'draggableviews' => array(
                          $vars['tabledrag_table_id'] => array(
                            'states' => $states,
                          ),
                        ),
                      ),
                      'setting');
      }
    }
  }

  drupal_add_css(drupal_get_path('module', 'draggableviews') .'/styles.css');
}

function template_preprocess_draggableviews_view_draggabletable_form($vars) {
  // Get style plugin.
  $style_plugin = $vars['form']['#parameters'][2];
  // Get structured info array.
  $info = $style_plugin->view->draggableviews_info;

  // Keep the already created form elements "in mind". We'll need them later when we theme the view.
  $style_plugin->view->form_elements = &$vars['form']['rows'];
  
  $vars['view'] = theme($style_plugin->theme_functions(), $style_plugin->view, $style_plugin->options, $style_plugin->view->result, '');

  // Don't render the submit form..
  // - if user has no access or
  // - if structure is locked.

  if (user_access('Allow Reordering') && !$info['locked']) {
    $vars['form'] =  drupal_render($vars['form']);
  }
  else {
    $vars['form'] = '';
  }
}

/**
 * Theme the form for the table style plugin
 */
function theme_draggableviews_ui_style_plugin_draggabletable($form) {
  unset($form['order']);
  unset($form['default']);
  
  // The following lines are a modification of: views/includes/admin.inc,v 1.161.2.12 2010/06/17 02:45:36 merlinofchaos. Lines 3060 - 3101. 
  $output = drupal_render($form['description_markup']);

  $header = array(
    t('Field'),
    t('Column'),
    t('Separator'),
    array(
      'data' => t('Sortable'),
      'align' => 'center',
    ),
  );
  $rows = array();
  foreach (element_children($form['columns']) as $id) {
    $row = array();
    $row[] = drupal_render($form['info'][$id]['name']);
    $row[] = drupal_render($form['columns'][$id]);
    $row[] = drupal_render($form['info'][$id]['separator']);
    if (!empty($form['info'][$id]['sortable'])) {
      $row[] = array(
        'data' => drupal_render($form['info'][$id]['sortable']),
        'align' => 'center',
      );
    }
    else {
      $row[] = '';
    }
    $rows[] = $row;
  }
  drupal_render($form['default'][-1]);

  // Add the special 'None' row.
  $rows[] = array(t('None'), '', '', '');

  $output .= theme('table', $header, $rows);
  // Copy/Modification end


  // Render Draggable view settings
  // The following lines add the new output from draggableviews

  // Build header.
  $tabledrag_header = drupal_render($form['tabledrag_header']);
  $description_tabledrag = drupal_render($form['tabledrag_description']);

  // Build order table.
  $tabledrag_order_columns = array();
  $tabledrag_order_columns[] = 'Order Field:';
  foreach (element_children($form['tabledrag_order']) AS $col) {
    $tabledrag_order_columns[] = drupal_render($form['tabledrag_order'][$col]);
  }
  $tabledrag_order_visible = drupal_render($form['tabledrag_order_visible']);
  $description_order = drupal_render($form['tabledrag_description_order']);

  // Build hierarchy table.
  $tabledrag_hierarchy_columns = array();
  $tabledrag_hierarchy_columns[] = 'Parent:';
  foreach (element_children($form['tabledrag_hierarchy']) AS $col) {
    $tabledrag_hierarchy_columns[] = drupal_render($form['tabledrag_hierarchy'][$col]);
  }
  $tabledrag_hierarchy_visible = drupal_render($form['tabledrag_hierarchy_visible']);
  $description_hierarchy = drupal_render($form['tabledrag_description_hierarchy']);

  $draggableviews_depth_limit = drupal_render($form['draggableviews_depth_limit']);

  $draggableviews_repair = drupal_render($form['draggableviews_repair']);

  // build types table
  $tabledrag_types_rows = array();
  foreach (element_children($form['tabledrag_types']) AS $id) {
    $columns = array();
    foreach (element_children($form['tabledrag_types'][$id]) AS $col) {
      $columns[] = drupal_render($form['tabledrag_types'][$id][$col]);
    }
    $tabledrag_types_rows[] = $columns;
  }
  $tabledrag_types_add = drupal_render($form['tabledrag_types_add']);
  $description_types   = drupal_render($form['tabledrag_description_types']);

  $tabledrag_expand = drupal_render($form['tabledrag_expand']);

  $draggableviews_extensions = drupal_render($form['draggableviews_extensions']);
  $description_extensions    = drupal_render($form['draggableviews_description_extensions']);

  $tabledrag_lock = drupal_render($form['tabledrag_lock']);
  $description_tabledrag_lock = drupal_render($form['description_tabledrag_lock']);

  $draggableviews_default_on_top = drupal_render($form['draggableviews_default_on_top']);

  $draggableviews_button_text = drupal_render($form['draggableviews_button_text']);

  // Let extension modules append to the output
  $extensions = array();
  foreach (module_implements('draggableviews_style_plugin_render') as $module) {
    $function = $module .'_draggableviews_style_plugin_render';
    $extensions[] = $function($form);
  }

  // append form data left to output
  // all items that have not been rendered until this point
  // will be rendered now. So we can seperate the settings we
  // inherited from plugin_table from plugin_draggableviews settings.
  $output .= drupal_render($form);

  // Append order settings.
  $output .= $tabledrag_header;
  $output .= $description_tabledrag;
  $output .= theme('table', array('&nbsp;', t('Field'), t('Handler')), array($tabledrag_order_columns));
  $output .= $description_order;
  $output .= $tabledrag_order_visible;
  // Append hierarchy settings.
  $output .= theme('table', array('&nbsp;', t('Field'), t('Handler')), array($tabledrag_hierarchy_columns));
  $output .= $description_hierarchy;
  $output .= $tabledrag_hierarchy_visible;
  // Append depth limit textfield to output.
  $output .= $draggableviews_depth_limit;
  // Append structure options.
  $output .= $draggableviews_repair;
  if (strcmp($form['#base_table'], 'node') == 0) {
    // Append tabledrag types settings.
    $output .= theme('table', array(t('Node type'), t('Type'), '&nbsp;'), $tabledrag_types_rows);
    $output .= $tabledrag_types_add;
    $output .= $description_types;
  }
  // Append expand yes/no checkbox to output.
  $output .= $tabledrag_expand;
  // Append page extensions output.
  $output .= $draggableviews_extensions;
  $output .= $description_extensions;
  // Append lock output.
  $output .= $tabledrag_lock;
  $output .= $description_tabledrag_lock;
  // Append default behaviour radios.
  $output .= $draggableviews_default_on_top;
  // Append button text.
  $output .= $draggableviews_button_text;
  // append extension modules output
  $output .= implode('', $extensions);

  // return output
  return $output;
}
