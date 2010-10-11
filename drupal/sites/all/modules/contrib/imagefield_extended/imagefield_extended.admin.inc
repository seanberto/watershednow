<?php
// $Id: imagefield_extended.admin.inc,v 1.1.2.2.2.1 2010/03/03 02:12:37 aland Exp $

/**
 * @file
 * Administration related functions.
 */

/**
 * Menu callback.
 * Used to define what field types will be available when
 * creating a file or image field.
 */
function imagefield_extended_admin_settings_form() {
  $form = array();
  $form['imagefield_extended_textfields'] = array(
    '#type' => 'textarea',
    '#title' => t('Additional text fields'),
    '#default_value' => variable_get('imagefield_extended_textfields', ''),
    '#description' => t('A list of new textfields to make available. One textfield form API key / value per line. Key / value pairs must be entered seperated by pipes (|), such as "fapi_safe_key|Some readable option".') .'<br/>'. t('The following are reserved keywords and can not be used: "description", "alt", "title".'),
    '#cols' => 60,
    '#rows' => 5,
    '#element_validate' => array('imagefield_extended_options_validate'),
  );
  $form['imagefield_extended_checkboxes'] = array(
    '#type' => 'textarea',
    '#title' => t('Additional checkboxes fields'),
    '#default_value' => variable_get('imagefield_extended_checkboxes', ''),
    '#description' => t('A list of new checkboxes to make available. One checkbox form API key / value info per line. Key / value pairs must be entered seperated by pipes (|), such as "fapi_safe_key|Some readable option".') .'<br/>'. t('The following are reserved keywords and can not be used: "description", "alt", "title".'),
    '#cols' => 60,
    '#rows' => 5,
    '#element_validate' => array('imagefield_extended_options_validate'),
  );
  return system_settings_form($form);
}

/**
 * To parse a newline selection list into options.
 */
function imagefield_extended_options_validate($element, &$form_state) {
  // Defined as static to preserve state between the two validation callbacks.
  static $used_keys = array();
  $options = array();
  $reserved_keys = array('description', 'alt', 'title');
  $rows = array_filter(explode("\n", trim($element['#value'])));
  foreach ($rows as $option) {
    $option = trim($option);
    if (strlen($option)) {
      if (preg_match('/^([^|]+)\|(.*)$/', $option, $matches)) {
        $matches[1] = trim($matches[1]);
        $matches[2] = trim($matches[2]);
        if (empty($matches[1]) || empty($matches[2])) {
          form_set_error(implode('][', $element['#array_parents']), t('Each option must be a key / value pair seperated by a pipe (|).'));
          return;
        }
        elseif (!preg_match('/^[a-z_]{1}[a-z0-9_]*$/', $matches[1])) {
          form_set_error(implode('][', $element['#array_parents']), t('The specified key contains one or more illegal characters. Spaces or any other special characters except an underscore (_) are not allowed. The first letter can not be a number.'));
          return;
        }
        elseif (in_array($matches[1], $reserved_keys)) {
          form_set_error(implode('][', $element['#array_parents']), t('The specified key %key is a reserved keyword.', array('%key' => $matches[1])));
          return;
        }
        elseif (in_array($matches[1], $used_keys)) {
          form_set_error(implode('][', $element['#array_parents']), t('The specified key %key is already in use. The combined keys for both the checkboxes and text options must be unique.', array('%key' => $matches[1])));
          return;
        }
        $used_keys[] = $matches[1];
        $options[] = $matches[1] .'|'. $matches[2];

      }
      else {
        form_error($element, t('Each option must be a key / value pair seperated by a pipe (|).'));
        return;
      }
    }
  }

  form_set_value($element, implode("\n", $options), $form_state);
}
