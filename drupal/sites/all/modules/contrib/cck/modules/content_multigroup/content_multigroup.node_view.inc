<?php
// $Id: content_multigroup.node_view.inc,v 1.1.2.4 2009/11/01 10:44:16 markuspetrux Exp $

/**
 * @file
 * Implementation of node view functions for content multigroup.
 */

/**
 * Implementation of hook_fieldgroup_view().
 */
function _content_multigroup_fieldgroup_view(&$node, &$element, $group, $context) {
  drupal_add_css(drupal_get_path('module', 'content_multigroup') .'/content_multigroup.css');

  $node_copy = drupal_clone($node);
  $group_name = $group['group_name'];
  $group_class = 'content-multigroup-'. str_replace('_', '-', $group_name);
  $group_multiple = (int)$group['settings']['multigroup']['multiple'];
  $subgroup_settings = isset($group['settings']['multigroup']['subgroup']) ? $group['settings']['multigroup']['subgroup'] : array();
  $show_label = isset($subgroup_settings['label']) ? $subgroup_settings['label'] : 'above';
  $subgroup_labels = isset($group['settings']['multigroup']['labels']) ? $group['settings']['multigroup']['labels'] : array();
  $subgroup_format = isset($subgroup_settings[$context]['format']) ? $subgroup_settings[$context]['format'] : 'fieldset';
  $content_type = content_types($group['type_name']);

  // Build list of accessible fields in this group.
  $group_fields = array();
  foreach ($content_type['fields'] as $field_name => $field) {
    if (isset($group['fields'][$field_name]) && isset($element[$field_name])) {
      if (!isset($element[$field_name]['#access']) || $element[$field_name]['#access']) {
        $group_fields[$field_name] = $field;
      }
    }
  }

  switch ($group_multiple) {
    case 0:
      $group_deltas = array(0);
      break;

    case 1:
      // Compute unique deltas from all deltas used by fields in this multigroup.
      $group_deltas = array();
      foreach (array_keys($group_fields) as $field_name) {
        if (is_array($node->content[$field_name]['field']['items'])) {
          foreach (array_keys($node->content[$field_name]['field']['items']) as $delta) {
            $group_deltas[$delta] = $delta;
          }
        }
      }
      sort($group_deltas);
      break;

    default:
      $group_deltas = range(0, $group_multiple - 1);
      break;
  }

  foreach ($group_deltas as $index => $delta) {
    $element[$delta] = array(
      '#title' => ($show_label == 'above' && !empty($subgroup_labels[$index]) ? check_plain(t($subgroup_labels[$index])) : ''),
      '#attributes' => array('class' => 'content-multigroup-wrapper content-multigroup-'. $index),
      '#weight' => $delta,
    );

    // Create a pseudo node that only has the value we want in this group and
    // pass it to the formatter.
    // Default implementation of content-field.tpl.php uses a different CSS
    // class for inline labels when delta is zero, but this is not needed in
    // the context of multigroup, so we place the field into index 1 of the
    // item list. Note that CSS class "field-label-inline" is overridden in the
    // multigroup stylesheet because here labels should always be visible.
    foreach (array_keys($group_fields) as $field_name) {
      if (isset($node->content[$field_name])) {
        $node_copy->content[$field_name]['field']['items'] = array(
          1 => isset($node->content[$field_name]['field']['items'][$delta]) ? $node->content[$field_name]['field']['items'][$delta] : NULL,
        );
        $element[$delta][$field_name] = $node_copy->content[$field_name];
        $element[$delta][$field_name]['#delta'] = $delta;
      }
    }

    switch ($subgroup_format) {
      case 'simple':
        $element['#attributes']['class'] = $group_class;
        $element[$delta]['#theme'] = 'content_multigroup_display_simple';
        $element[$delta]['#fields'] = $group_fields;
        break;
      case 'fieldset_collapsed':
        $element[$delta]['#collapsed'] = TRUE;
      case 'fieldset_collapsible':
        $element[$delta]['#collapsible'] = TRUE;
      case 'fieldset':
        $element['#attributes']['class'] = $group_class;
        $element[$delta]['#type'] = 'content_multigroup_display_fieldset';
        $element[$delta]['#fields'] = $group_fields;
        break;
      case 'hr':
        $element['#attributes']['class'] = $group_class;
        $element[$delta]['#theme'] = 'content_multigroup_display_hr';
        $element[$delta]['#fields'] = $group_fields;
        break;
      case 'table-single':
        $element['#theme'] = 'content_multigroup_display_table_single';
        $element['#attributes']['class'] = $group_class .' content-multigroup-display-table-single-column';
        $element['#fields'] = $group_fields;
        break;
      case 'table-multiple':
        $element['#theme'] = 'content_multigroup_display_table_multiple';
        $element['#attributes']['class'] = $group_class .' content-multigroup-display-table-multiple-columns';
        $element['#fields'] = $group_fields;
        break;
    }
  }

  // Unset the original group field values now that we've moved them.
  foreach (array_keys($group_fields) as $field_name) {
    if (isset($element[$field_name])) {
      unset($element[$field_name]);
    }
  }
}

/**
 * Theme a subgroup of fields in 'simple' format.
 *
 * No output is generated if all fields are empty.
 */
function theme_content_multigroup_display_simple($element) {
  $children = $output = '';
  foreach (element_children($element) as $key) {
    $children .= drupal_render($element[$key]);
  }
  if (!empty($children)) {
    $output .= '<div'. drupal_attributes($element['#attributes']) .'>';
    if (!empty($element['#title'])) {
      $output .= '<label class="content-multigroup">'. $element['#title'] .':</label>';
    }
    $output .= $children .'</div>';
  }
  return $output;
}

/**
 * Theme a subgroup of fields in 'fieldset' format.
 *
 * No output is generated if all fields are empty.
 */
function theme_content_multigroup_display_fieldset($element) {
  if (empty($element['#children']) && empty($element['#value'])) {
    return '';
  }
  return theme('fieldset', $element);
}

/**
 * Theme a subgroup of fields in 'hr' format.
 *
 * No output is generated if all fields are empty.
 */
function theme_content_multigroup_display_hr($element) {
  $children = $output = '';
  foreach (element_children($element) as $key) {
    $children .= drupal_render($element[$key]);
  }
  if (!empty($children)) {
    $output .= '<div'. drupal_attributes($element['#attributes']) .'><hr class="content-multigroup" />';
    if (!empty($element['#title'])) {
      $output .= '<label class="content-multigroup">'. $element['#title'] .':</label>';
    }
    $output .= $children .'</div>';
  }
  return $output;
}

/**
 * Theme a multigroup in 'table-single' format.
 *
 * Each subgroup has its own table row with a single cell for all fields.
 * No output is generated if all fields are empty.
 */
function theme_content_multigroup_display_table_single($element) {
  $headers = array();
  $rows = array();
  foreach (element_children($element) as $delta) {
    $items = array();
    foreach ($element['#fields'] as $field_name => $field) {
      $item = drupal_render($element[$delta][$field_name]);
      if (!empty($item)) {
        $items[] = $item;
      }
    }
    if (!empty($items)) {
      if (!empty($element[$delta]['#title'])) {
        array_unshift($items, '<label class="content-multigroup">'. $element[$delta]['#title'] .':</label>');
      }
      $rows[] = array('data' => array(implode("\n", $items)), 'class' => $element[$delta]['#attributes']['class']);
    }
  }
  return count($rows) ? theme('table', $headers, $rows, $element['#attributes']) : '';
}

/**
 * Theme a multigroup in 'table-multiple' format.
 *
 * Each subgroup has its own table row with a separate cell for each field.
 * No output is generated if all fields are empty.
 */
function theme_content_multigroup_display_table_multiple($element) {
  $headers = array();
  foreach ($element['#fields'] as $field_name => $field) {
    $label_display = isset($field['display_settings']['label']['format']) ? $field['display_settings']['label']['format'] : 'above';
    $headers[] = array(
      'data' => ($label_display != 'hidden' ? check_plain(t($field['widget']['label'])) : ''),
      'class' => 'content-multigroup-cell-'. str_replace('_', '-', $field_name),
    );
  }
  $rows = array();
  foreach (element_children($element) as $delta) {
    $cells = array();
    $empty = TRUE;
    foreach ($element['#fields'] as $field_name => $field) {
      $item = drupal_render($element[$delta][$field_name]);
      $cells[] = array(
        'data' => $item,
        'class' => $element[$delta]['#attributes']['class'] .' content-multigroup-cell-'. str_replace('_', '-', $field_name),
      );
      if (!empty($item)) {
        $empty = FALSE;
      }
    }
    // Get the row only if there is at least one non-empty field.
    if (!$empty) {
      $rows[] = $cells;
    }
  }
  return count($rows) ? theme('table', $headers, $rows, $element['#attributes']) : '';
}
