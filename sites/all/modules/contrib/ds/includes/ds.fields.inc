<?php
// $Id: ds.fields.inc,v 1.1.2.41 2010/06/08 00:22:20 swentel Exp $

/**
 * @file
 * Manage fields.
 */

/**
 *
 * @param integer $integer An integer which identifies a constant.
 * @param string $type A string to identify the category.
 */
function ds_human_name($integer, $type) {
  $human_names = array(
    'type' => array(
      DS_FIELD_TYPE_CODE => t('Code field'),
      DS_FIELD_TYPE_BLOCK => t('Block field'),
      DS_FIELD_TYPE_PREPROCESS => t('Preprocess field'),
    ),
    'status' => array(
      DS_FIELD_STATUS_DEFAULT => t('Default'),
      DS_FIELD_STATUS_CUSTOM => t('Custom'),
      DS_FIELD_STATUS_OVERRIDDEN => t('Overridden'),
    ),
  );
  return $human_names[$type][$integer];
}

/**
 * Fields overview.
 */
function ds_fields($module) {
  $output = '';
  $action = arg(5);
  $field = arg(6);

  drupal_add_js(drupal_get_path('module', 'ds') .'/js/fields.js');

  // Get API information.
  $api_info = ds_api_info($module);

  // Extra info.
  $extra = array();

  // Get fields.
  $db_fields = variable_get($module .'_fields', array());
  $all_fields = ds_get_fields($module, NULL, NULL, $extra, TRUE);

  // Compare with db_fields.
  foreach ($all_fields as $key => $ds_field) {
    if ($ds_field['status'] == DS_FIELD_STATUS_DEFAULT && !isset($db_fields[$key])) {
      $db_fields[$key] = array(
        'title' => $ds_field['title'],
        'properties' => $ds_field['properties'],
        'type' => $ds_field['type'],
        'status' => $ds_field['status'],
        'exclude' => isset($ds_field['exclude']) ? $ds_field['exclude'] : array(),
      );
    }
  }

  // Delete form.
  if (in_array($field, array_keys($db_fields)) && $action == 'delete') {
    $output .= drupal_get_form('ds_field_delete_reset_form', $module, $field, $db_fields[$field]);
  }
  // Edit form.
  elseif (in_array($field, array_keys($db_fields)) && $action == 'edit') {
    if ($db_fields[$field]['type'] == DS_FIELD_TYPE_BLOCK) $form_id = 'ds_block_field_form';
    elseif ($db_fields[$field]['type'] == DS_FIELD_TYPE_PREPROCESS) $form_id = 'ds_preprocess_field_form';
    else $form_id = 'ds_code_field_form';
    $output .= drupal_get_form($form_id, $module, $all_fields, $api_info['object'], $field, $db_fields[$field]);
  }
  // Custom fields overview form
  else {
    $rows = array();
    foreach ($db_fields as $key => $value) {
      if ($value['type'] != DS_FIELD_TYPE_GROUP) {
        $row = array();
        $row[] = l($value['title'], 'admin/build/ds/'. $module .'/fields/edit/'. $key);
        $row[] = $key;
        $row[] = ds_human_name($value['type'], 'type');
        $row[] = ds_human_name($value['status'], 'status');
        $operations = l(t('Edit'), 'admin/build/ds/'. $module .'/fields/edit/'. $key);
        if ($value['status'] != DS_FIELD_STATUS_DEFAULT) {
          $text = ($value['status'] == DS_FIELD_STATUS_OVERRIDDEN) ? 'Reset' : 'Delete';
          $operations .= ' - '. l(t($text), 'admin/build/ds/'. $module .'/fields/delete/'. $key);
        }
        $row[] = $operations;
        $rows[] = $row;
      }
    }

    if (!empty($rows)) {
      $header = array(
        t('Title'), t('Key'), t('Type'), t('Status'), t('Operations'),
      );

      $output .= '<p>'. t('This is a list of fields either defined in code or via the UI.') .' ';
      if (!isset($api_info['no_fields_examples'])) {
        $output .= t('You can look at <a href="javascript:;" onClick="window.open(\'!url\', \'mywindow\', \'width=800,height=600,scrollbars=yes,location=1,resizable=1,toolbar=1\'); return false();">properties and preprocess variables</a> for any object type in a separate window.', array('!url' => url('admin/build/ds/'. $module .'/fields-examples'))) .'</p>';
      }
      $output .= theme('table', $header, $rows);
    }
    else {
      $output .= '<p>'. t('You have not defined any fields.') .' ';
      if (!isset($api_info['no_fields_examples'])) {
        $output .= t('You can look at <a href="javascript:;" onClick="window.open(\'!url\', \'mywindow\', \'width=800,height=600,scrollbars=yes,location=1,resizable=1,toolbar=1\'); return false();">properties and preprocess variables</a> for any object type in a separate window.', array('!url' => url('admin/build/ds/'. $module .'/fields-examples'))) .'</p>';
      }
    }

    // New code field form.
    $output .= drupal_get_form('ds_code_field_form', $module, $all_fields, $api_info['object']);
    // New preprocess field form.
    $output .= drupal_get_form('ds_preprocess_field_form', $module, $all_fields, $api_info['object']);
    // Block field form.
    $output .= drupal_get_form('ds_block_field_form', $module, $all_fields);
  }

  return $output;
}

/**
 * Code field form.
 *
 * @param string $module The module the field is for.
 * @param array $all_fields All fields for this module.
 * @param string $object The object name (ie node, user, comment).
 * @param string $key The key of the field.
 * @param array $field The field with title and code keys.
 */
function ds_code_field_form($form_state, $module, $all_fields, $object = '', $key = '', $field = array()) {
  $form = array();

  if (empty($field)) {
    $field = array(
      'title' => '',
      'exclude' => array(),
      'type' => DS_FIELD_TYPE_CODE,
      'status' => DS_FIELD_STATUS_CUSTOM,
      'properties' => array(
        'code' => '',
        'css-class' => '',
        'token' => FALSE
      )
    );
  }

  $form['code_identity'] = array(
    '#type' => 'fieldset',
    '#title' => (empty($key)) ? t('Add new code field'): t('Update code field'),
    '#collapsible' => (empty($key)) ? TRUE : FALSE,
    '#collapsed' => (empty($key)) ? TRUE : FALSE,
  );
  $form['code_identity']['code_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Field key'),
    '#description' => t('The machine-readable name of this field.'),
    '#required' => TRUE,
  );
  if (!empty($key)) {
    $form['code_identity']['code_key']['#disabled'] = TRUE;
    $form['code_identity']['code_key']['#value'] = $key;
    $form['code_identity']['code_key']['#description'] = t('The machine-readable name of this field. Note: you can not edit this key.');
  }
  $form['code_identity']['code_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Field title'),
    '#description' => t('The title to use when rendering the output and on the display administration screen.'),
    '#required' => TRUE,
    '#default_value' => $field['title']
  );
  $form['code_identity']['code_class'] = array(
    '#type' => 'textfield',
    '#title' => t('Field class'),
    '#description' => t('The classes you want to add to this field. This will replace the automatic class that Display Suite generates.'),
    '#default_value' => isset($field['properties']['css-class']) ? $field['properties']['css-class'] : '',
  );

  $api_info = ds_api_info($module);
  if (isset($api_info['types']) && count($api_info['types']()) > 1) {
    $types = array();
    foreach ($api_info['types']() as $tkey => $type) {

      // Views displays is special case.
      if ($module == 'vd') {
        $global_exclude = variable_get($module .'_type_'. $tkey, FALSE);
        if ($global_exclude == TRUE) {
          continue;
        }
      }

      $types[$tkey] = $type->name;
    }
    $form['code_identity']['code_exclude'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Field exclude'),
      '#options' => $types,
      '#default_value' => $field['exclude'],
      '#attributes' => array('class' => 'exclude-types')
    );
    $form['code_identity']['code_exclude_all'] = array(
      '#type' => 'checkbox',
      '#title' => t('Select all'),
      '#description' => t('Toggle types which you don\'t want the field to appear in.'),
      '#attributes' => array('class' => 'select-all')
    );
  }
  else {
    $form['code_identity']['code_exclude'] = array(
      '#type' => 'value',
      '#value' => array(),
    );
  }
  $form['code_identity']['code_code'] = array(
    '#type' => 'textarea',
    '#title' => t('Field code'),
    '#required' => TRUE,
    '#default_value' => $field['properties']['code'],
  );

  _ds_field_object_info($form, $object, $field);

  $form['code_identity']['code_submit'] = array(
    '#type' => 'submit',
    '#submit' => array('ds_code_field_form_submit'),
    '#value' => t('Save code field'),
  );

  $form['#field_status'] = ($field['status'] == DS_FIELD_STATUS_DEFAULT) ? DS_FIELD_STATUS_OVERRIDDEN : (($field['type'] == DS_FIELD_STATUS_OVERRIDDEN) ? DS_FIELD_STATUS_OVERRIDDEN : DS_FIELD_STATUS_CUSTOM);
  $form['#form_type'] = (empty($key)) ? 'insert' : 'update';
  $form['#module'] = $module;
  $form['#all_fields'] = $all_fields;

  return $form;
}

/**
 * Return info about the value field, including token info.
 *
 * @param array $form The current form.
 * @param string $object The object name (ie node, user, comment)
 * @param array $field The field array.
 */
function _ds_field_object_info(&$form, $object, $field) {
  $form['code_identity']['help'] = array(
    '#type' => 'item',
    '#value' => t('Every field will be wrapped in &lt;div class="field field-key"&gt;VALUE&lt;/div&gt;.<br/>Enter PHP code between &lt;?php ?&gt;. If you are using PHP, the variable for the '. $object .' is called $object.<br/>Note that executing incorrect PHP-code can break your Drupal site.'),
  );

  if (module_exists('token')) {
    $form['code_identity']['help']['code_token'] = array(
      '#type' => 'checkbox',
      '#title' => t('Toggle this checkbox if you are using tokens in this field.'),
      '#default_value' => $field['properties']['token'],
    );
    $form['code_identity']['help']['tokens'] = array(
      '#type' => 'fieldset',
      '#title' => t('Placeholder tokens'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form['code_identity']['help']['tokens']['info'] = array(
      '#type' => 'item',
      '#value' => theme('token_help', $object),
    );
  }
  else {
    $form['code_identity']['help']['code_token'] = array(
      '#type' => 'value',
      '#value' => FALSE,
    );
    $form['code_identity']['help']['#value'] .= '<br />'. t("To use dynamic placeholder tokens in your custom fields (the ID or title of the current node, for example), download and install the <a href='@token'>Token module</a> from Drupal.org.", array('@token' => 'http://www.drupal.org/project/token'));
  }
}

/**
 * Validate code field submission, only on insert.
 */
function ds_code_field_form_validate($form, &$form_state) {
  if ($form['#form_type'] == 'insert') {

    $module = $form['#module'];
    $fields = $form['#all_fields'];
    $existing = array_keys($fields);

    if (in_array($form_state['values']['code_key'], $existing)) {
      form_set_error('code_key', t('This field already exists.'));
    }

    if (!preg_match('!^[a-z_0-9]+$!', $form_state['values']['code_key'])) {
      form_set_error('code_key', t('The machine-readable name must contain only lowercase letters, numbers and underscores.'));
    }
  }
}

/**
 * Save code field.
 */
function ds_code_field_form_submit($form, &$form_state) {
  $module = $form['#module'];
  $fields = variable_get($module .'_fields', array());

  $key = $form_state['values']['code_key'];
  $fields[$key] = array(
    'title' => $form_state['values']['code_title'],
    'exclude' => $form_state['values']['code_exclude'],
    'type' => DS_FIELD_TYPE_CODE,
    'status' => $form['#field_status'],
    'properties' => array(
      'formatters' => array(
        'ds_eval_code' => t('Default'),
      ),
      'code' => $form_state['values']['code_code'],
      'token' => $form_state['values']['code_token'],
    ),
  );

  // Code class.
  if (!empty($form_state['values']['code_class'])) {
    $fields[$key]['properties']['css-class'] = $form_state['values']['code_class'];
  }

  // Safe field and invalidate cached fields.
  variable_set($module .'_fields', $fields);
  variable_del('ds_fields_cached');

  // Redirect and confirmation message.
  $form_state['redirect'] = 'admin/build/ds/'. $form['#module'] .'/fields';
  drupal_set_message(t('Field %field has been saved.', array('%field' => $form_state['values']['code_title'])));
}

/**
 * Preprocess field form.
 *
 * @param string $module The module the field is for.
 * @param array $all_fields All fields for this module.
 * @param string $object The object name (ie node, user, comment).
 * @param string $key The key of the field.
 * @param array $field The field with title and code keys.
 */
function ds_preprocess_field_form($form_state, $module, $all_fields, $object = '', $key = '', $field = array()) {
  $form = array();

  if (empty($field)) {
    $field = array(
      'title' => '',
      'exclude' => array(),
      'type' => DS_FIELD_TYPE_PREPROCESS,
      'status' => DS_FIELD_STATUS_CUSTOM,
      'properties' => array(
        'key' => '',
        'css-class' => '',
      )
    );
  }

  $form['preprocess_identity'] = array(
    '#type' => 'fieldset',
    '#title' => (empty($key)) ? t('Add new preprocess field'): t('Update preprocess field'),
    '#description' => t('Create fields from variables available when preprocessing an object. For inspiration, see <a href="!url">!url</a>.', array('!url' => 'http://drupal.org/node/700056')),
    '#collapsible' => (empty($key)) ? TRUE : FALSE,
    '#collapsed' => (empty($key)) ? TRUE : FALSE,
  );
  $form['preprocess_identity']['preprocess_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Field key'),
    '#description' => t('The machine-readable name of this field. This is also the key that DS is searching in the $vars variable.'),
    '#required' => TRUE,
  );
  if (!empty($key)) {
    $form['preprocess_identity']['preprocess_key']['#disabled'] = TRUE;
    $form['preprocess_identity']['preprocess_key']['#value'] = $key;
    $form['preprocess_identity']['preprocess_key']['#description'] = t('The machine-readable name of this field. Note: you can not edit this key.');
  }

  $form['preprocess_identity']['preprocess_array_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Variable key'),
    '#required' => FALSE,
    '#description' => t('Enter a key if the variable is an array. Eg: og_links is an array and the display in that variable is available in the key "view"'),
    '#default_value' => $field['properties']['key'],
  );

  $form['preprocess_identity']['preprocess_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Field title'),
    '#description' => t('The title to use when rendering the output and on the display administration screen.'),
    '#required' => TRUE,
    '#default_value' => $field['title']
  );
  $form['preprocess_identity']['preprocess_class'] = array(
    '#type' => 'textfield',
    '#title' => t('Field class'),
    '#description' => t('The classes you want to add to this field. This will replace the automatic class that Display Suite generates.'),
    '#default_value' => isset($field['properties']['css-class']) ? $field['properties']['css-class'] : '',
  );

  $api_info = ds_api_info($module);
  if (isset($api_info['types']) && count($api_info['types']()) > 1) {
    $types = array();
    foreach ($api_info['types']() as $tkey => $type) {
      $types[$tkey] = $type->name;
    }
    $form['preprocess_identity']['preprocess_exclude'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Field exclude'),
      '#options' => $types,
      '#default_value' => $field['exclude'],
      '#attributes' => array('class' => 'exclude-types')
    );
    $form['preprocess_identity']['preprocess_exclude_all'] = array(
      '#type' => 'checkbox',
      '#title' => t('Select all'),
      '#description' => t('Toggle types which you don\'t want the field to appear in.'),
      '#attributes' => array('class' => 'select-all')
    );
  }
  else {
    $form['preprocess_identity']['preprocess_exclude'] = array(
      '#type' => 'value',
      '#value' => array(),
    );
  }

  $form['preprocess_identity']['preprocess_submit'] = array(
    '#type' => 'submit',
    '#submit' => array('ds_preprocess_field_form_submit'),
    '#value' => t('Save preprocess field'),
  );

  $form['#field_status'] = ($field['status'] == DS_FIELD_STATUS_DEFAULT) ? DS_FIELD_STATUS_OVERRIDDEN : (($field['type'] == DS_FIELD_STATUS_OVERRIDDEN) ? DS_FIELD_STATUS_OVERRIDDEN : DS_FIELD_STATUS_CUSTOM);
  $form['#form_type'] = (empty($key)) ? 'insert' : 'update';
  $form['#module'] = $module;
  $form['#all_fields'] = $all_fields;

  return $form;
}

/**
 * Validate preprocess field submission, only on insert.
 */
function ds_preprocess_field_form_validate($form, &$form_state) {
  if ($form['#form_type'] == 'insert') {

    $module = $form['#module'];
    $fields = $form['#all_fields'];
    $existing = array_keys($fields);

    if (in_array($form_state['values']['preprocess_key'], $existing)) {
      form_set_error('preprocess_key', t('This field already exists.'));
    }

    if (!preg_match('!^[a-z_0-9]+$!', $form_state['values']['preprocess_key'])) {
      form_set_error('preprocess_key', t('The machine-readable name must contain only lowercase letters, numbers and underscores.'));
    }
  }
}

/**
 * Save preprocess field.
 */
function ds_preprocess_field_form_submit($form, &$form_state) {
  $module = $form['#module'];
  $fields = variable_get($module .'_fields', array());

  $key = $form_state['values']['preprocess_key'];
  $fields[$key] = array(
    'title' => $form_state['values']['preprocess_title'],
    'exclude' => $form_state['values']['preprocess_exclude'],
    'type' => DS_FIELD_TYPE_PREPROCESS,
    'status' => $form['#field_status'],
    'properties' => array(
      'key' => $form_state['values']['preprocess_array_key'],
    ),
  );

  // Code class.
  if (!empty($form_state['values']['preprocess_class'])) {
    $fields[$key]['properties']['css-class'] = $form_state['values']['preprocess_class'];
  }

  // Safe field and invalidate cached fields.
  variable_set($module .'_fields', $fields);
  variable_del('ds_fields_cached');

  // Redirect and confirmation message.
  $form_state['redirect'] = 'admin/build/ds/'. $form['#module'] .'/fields';
  drupal_set_message(t('Field %field has been saved.', array('%field' => $form_state['values']['preprocess_title'])));
}

/**
 * Block field form.
 *
 * @param string $module The module the field is for.
 * @param array $all_fields All fields for this module.
 * @param string $object The object name (ie node, user, comment).
 * @param string $key The key of the field.
 * @param array $field The field with title and code keys.
 */
function ds_block_field_form($form_state, $module, $all_fields, $object = '', $key = '', $field = array()) {
  $form = array();

  if (empty($field)) {
    $field = array(
      'title' => '',
      'exclude' => array(),
      'type' => DS_FIELD_TYPE_BLOCK,
      'status' => DS_FIELD_STATUS_CUSTOM,
      'properties' => array(
        'css-class' => '',
        'block' => '',
        'render' => DS_BLOCK_TEMPLATE
      )
    );
  }

  $form['block_identity'] = array(
    '#type' => 'fieldset',
    '#title' => (empty($key)) ? t('Add new block field'): t('Update block field'),
    '#collapsible' => (empty($key)) ? TRUE : FALSE,
    '#collapsed' => (empty($key)) ? TRUE : FALSE,
  );
  $form['block_identity']['block_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Field key'),
    '#description' => t('The machine-readable name of this field.'),
    '#required' => TRUE,
  );
  if (!empty($key)) {
    $form['block_identity']['block_key']['#disabled'] = TRUE;
    $form['block_identity']['block_key']['#value'] = $key;
    $form['block_identity']['block_key']['#description'] = t('The machine-readable name of this field. Note: you can not edit this field.');
  }
  $form['block_identity']['block_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Field title'),
    '#description' => t('The title to use when rendering the output and on the display administration screen.'),
    '#required' => TRUE,
    '#default_value' => $field['title']
  );
  $form['block_identity']['block_class'] = array(
    '#type' => 'textfield',
    '#title' => t('Field class'),
    '#description' => t('The classes you want to add to this field. This will replace the automatic class that Display Suite generates.'),
    '#default_value' => isset($field['properties']['css-class']) ? $field['properties']['css-class'] : '',
  );

  $api_info = ds_api_info($module);
  if (isset($api_info['types']) && count($api_info['types']()) > 1) {
    $types = array();
    foreach ($api_info['types']() as $tkey => $type) {

      // Views displays is special case.
      if ($module == 'vd') {
        $global_exclude = variable_get($module .'_type_'. $tkey, FALSE);
        if ($global_exclude == TRUE) {
          continue;
        }
      }

      $types[$tkey] = $type->name;
    }
    $form['block_identity']['block_exclude'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Field exclude'),
      '#options' => $types,
      '#default_value' => $field['exclude'],
      '#attributes' => array('class' => 'exclude-types')
    );
    $form['block_identity']['block_exclude_all'] = array(
      '#type' => 'checkbox',
      '#title' => t('Select all'),
      '#description' => t('Toggle types which you don\'t want the field to appear in.'),
      '#attributes' => array('class' => 'select-all')
    );
  }
  else {
    $form['code_identity']['block_exclude'] = array(
      '#type' => 'value',
      '#value' => array(),
    );
  }

  $blocks = array();
  foreach (module_list() as $drupal_module) {
    $module_blocks = module_invoke($drupal_module, 'block', 'list');
    if ($module_blocks) {
      foreach ($module_blocks as $module_key => $info) {
        $blocks[ucfirst($drupal_module)][$drupal_module .'|'. $module_key] = $info['info'];
      }
    }
  }
  ksort($blocks);

  $form['block_identity']['block_block'] = array(
    '#type' => 'select',
    '#options' => $blocks,
    '#title' => t('Block'),
    '#required' => TRUE,
    '#default_value' => $field['properties']['block']
  );
  $form['block_identity']['block_render'] = array(
    '#type' => 'select',
    '#options' => array(
      DS_BLOCK_TEMPLATE => t('Render through block template'),
      DS_BLOCK_TITLE_CONTENT => t('Show block title + content'),
      DS_BLOCK_CONTENT => t('Show only block content'),
    ),
    '#title' => t('Block render'),
    '#required' => TRUE,
    '#default_value' => $field['properties']['render'],
  );

  $form['block_identity']['block_submit'] = array(
    '#type' => 'submit',
    '#submit' => array('ds_block_field_form_submit'),
    '#value' => t('Save block field'),
  );

  $form['#field_status'] = ($field['status'] == DS_FIELD_STATUS_DEFAULT) ? DS_FIELD_STATUS_OVERRIDDEN : (($field['type'] == DS_FIELD_STATUS_OVERRIDDEN) ? DS_FIELD_STATUS_OVERRIDDEN : DS_FIELD_STATUS_CUSTOM);
  $form['#form_type'] = (empty($key)) ? 'insert' : 'update';
  $form['#module'] = $module;
  $form['#all_fields'] = $all_fields;

  return $form;
}

/**
 * Validate block field submission, only on insert.
 */
function ds_block_field_form_validate($form, &$form_state) {
  if ($form['#form_type'] == 'insert') {

    $module = $form['#module'];
    $fields = $form['#all_fields'];
    $existing = array_keys($fields);

    if (in_array($form_state['values']['block_key'], $existing)) {
      form_set_error('block_key', t('This field already exists.'));
    }

    if (!preg_match('!^[a-z_0-9]+$!', $form_state['values']['block_key'])) {
      form_set_error('block_key', t('The machine-readable name must contain only lowercase letters, numbers and underscores.'));
    }
  }
}

/**
 * Save block field.
 */
function ds_block_field_form_submit($form, &$form_state) {
  $module = $form['#module'];
  $fields = variable_get($module .'_fields', array());

  $key = $form_state['values']['block_key'];
  $fields[$key] = array(
    'type' => DS_FIELD_TYPE_BLOCK,
    'status' => $form['#field_status'],
    'title' => $form_state['values']['block_title'],
    'exclude' => $form_state['values']['block_exclude'],
    'properties' => array(
      'block' => $form_state['values']['block_block'],
      'render' => $form_state['values']['block_render'],
    ),
  );

  // Block class.
  if (!empty($form_state['values']['block_class'])) {
    $fields[$key]['properties']['css-class'] = $form_state['values']['block_class'];
  }

  // Safe field and invalidate cached fields.
  variable_set($module .'_fields', $fields);
  variable_del('ds_fields_cached');

  // Redirect and confirmation message.
  $form_state['redirect'] = 'admin/build/ds/'. $form['#module'] .'/fields';
  drupal_set_message(t('Field %field has been saved.', array('%field' => $form_state['values']['block_title'])));
}

/**
 * Field delete or reset form.
 *
 * @param string $module The module the fields are for.
 * @param string $object The object name (ie node, user, comment).
 * @param string $key The key of the field.
 * @param array $field The field with title and code keys.
 */
function ds_field_delete_reset_form($form_state, $module, $key, $field) {
  $form = array();

  $action = ($field['status'] == DS_FIELD_STATUS_OVERRIDDEN) ? t('reset') : t('delete');
  $form['question'] = array(
    '#type' => 'markup',
    '#value' => '<p>'. t('Are you sure you want to !action the field %field ?', array('!action' => $action, '%field' => $field['title'])) .'</p>',
  );
  $form['field'] = array(
    '#type' => 'value',
    '#value' => $key,
  );
  $form['button']['submit'] = array(
    '#prefix' => '<div>',
    '#type' => 'submit',
    '#value' => ($field['status'] == DS_FIELD_STATUS_OVERRIDDEN) ? t('Reset') : t('Delete'),
  );
  $form['buttons']['cancel'] = array(
    '#suffix' => '</div>',
    '#type' => 'markup',
    '#value' => l('cancel', 'admin/build/ds/'. $module .'/fields'),
  );
  $form['#module'] = $module;
  return $form;
}

/**
 * Delete or reset a field.
 */
function ds_field_delete_reset_form_submit($form, &$form_state) {
  $module = $form['#module'];
  $fields = variable_get($module .'_fields', array());
  $field = $fields[$form_state['values']['field']];
  $field_title = $field['title'];
  $action = ($field['status'] == DS_FIELD_STATUS_OVERRIDDEN) ? t('reset') : t('deleted');

  // Save and invalidate fields_cached.
  unset($fields[$form_state['values']['field']]);
  variable_set($module .'_fields', $fields);
  variable_del('ds_fields_cached');

  drupal_set_message(t('Field %field has been !action.', array('%field' => $field_title, '!action' => $action)));
  $form_state['redirect'] = 'admin/build/ds/'. $form['#module'] .'/fields';
}

/**
 * Show examples for a field by loading the objects and preprocess variables.
 */
function ds_fields_examples($module) {

  // Check on devel.
  $devel = drupal_get_path('module', 'devel');
  if (empty($devel)) {
    return t('You need the devel module for this on your site, however, it does not have to be enabled. You can download the module from <a href="!url">!url</a>.', array('!url' => 'http://drupal.org/project/devel'));
  }

  // Load it if needed and show the rest.
  _ds_fields_load_devel();

  $output = '';
  $api_info = ds_api_info($module);
  if (isset($api_info['types'])) {
    $types = array();
    foreach ($api_info['types']() as $tkey => $type) {

      $types[$tkey] = array(
        'title' => $type->name,
        'href' => 'admin/build/ds/'. $module .'/fields-examples',
        'query' => array('type' => $tkey),
      );
    }
  }

  $selects = theme('links', $types);
  $output = '<div><h2 style="margin-bottom: 10px;">'. t('Object types') .'</h2>'. $selects .'</div>';
  $output .= '<div style="margin: 10px 0px 10px 0px; padding-top: 10px; border-top: 1px solid #ddd">'. t('Select a type to see properties from it. The output from the objects is usually the object individually (e.g. a full node, comment, user ...). If you pass the parameter id in the URL, you can select an object with that id instead of a random one. Note that variables ending with "_rendered" are not usable when creating preprocess fields.') .'</div>';
  if (isset($_GET['type']) && isset($types[$_GET['type']])) {
    $output .= _ds_fields_examples_render($api_info, $_GET['type'], $output);
  }

  return $output;
}

/**
 * Get the load/render info about an object.
 */
function _ds_fields_examples_render($api_info, $type, &$output) {
  $object = $api_info['object'];
  $module = $api_info['module'];
  $examples_file = drupal_get_path('module', $module) .'/'. $module .'_fields_examples.inc';
  if (file_exists($examples_file)) {
    $output .= '<div style="margin: 10px 0px 10px 0px; padding-top: 10px; border-top: 1px solid #ddd"></div>';
    require_once($examples_file);
  }
  else {
    drupal_set_message('No info found.');
  }
}

/**
 * Helper function to load devel if needed.
 */
function _ds_fields_load_devel() {
  if (!module_exists('devel')) {
    module_load_include('module', 'devel');
  }
}

/**
 * Helper function to run through the preprocess functions.
 */
function _ds_fields_run_preprocess(&$variables, $hook) {
  $hooks = theme_get_registry();
  $info = $hooks[$hook];
  if (isset($info['preprocess functions']) && is_array($info['preprocess functions'])) {
    // This construct ensures that we can keep a reference through
    // call_user_func_array.
    $args = array(&$variables, $hook);
    foreach ($info['preprocess functions'] as $preprocess_function) {
      if (function_exists($preprocess_function)) {
        call_user_func_array($preprocess_function, $args);
      }
    }
  }
}
