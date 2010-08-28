<?php
// $Id: ds_handler_field_fieldgroup.inc,v 1.1.2.5 2010/04/22 22:52:05 swentel Exp $

/**
 * @file
 * Contains the display suite fieldgroup handler.
 */

/**
 * Field handler to create a fieldgroup.
 */
class ds_handler_field_fieldgroup extends views_handler_field {

  function query() {
    // do nothing -- to override the parent query.
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['ds_format'] = array('default' => 'ds_field');
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $options = array(
      'ds_field' => 'A simple div',
      'ds_group_fieldset_open' => 'Fieldset open',
      'ds_group_fieldset_collapsible' => 'Fieldset collapsible',
      'ds_group_fieldset_collapsed' => 'Fieldset collapsed',
    );

    if (module_exists('tabs')) {
      $options['tabs'] = 'Tabs';
    }

    $form['ds_format'] = array(
      '#type' => 'select',
      '#title' => t('Format'),
      '#default_value' => $this->options['ds_format'],
      '#options' => $options,
    );

    // Remove all the rest that's normall associated with fields.
    unset($form['alter']);
    unset($form['exclude']);
    unset($form['empty']);
    unset($form['empty_zero']);
    unset($form['hide_empty']);
  }

  function render($values) {
    return;
  }
}
