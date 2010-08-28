<?php
// $Id: signup_plugin_argument_validate_signup_status.inc,v 1.1 2008/11/12 01:00:58 dww Exp $

/**
 * Validate whether an argument is a signup-enabled node.
 *
 * Optionally filters on if signups are open or closed.
 */
class signup_plugin_argument_validate_signup_status extends views_plugin_argument_validate {
  var $option_name = 'validate_argument_signup_status';

  function validate_form(&$form, &$form_state) {
    $form[$this->option_name] = array(
      '#type' => 'select',
      '#title' => t('Signup status'),
      '#options' => array(
        'any' => t('Signups enabled (either open or closed)'),
        'open' => t('Signups open'),
        'closed' => t('Signups closed'),
        'none' => t('Signups disabled'),
      ),
      '#description' => t('Validate if the current node is signup-enabled and if signups are open or closed'),
      '#default_value' => isset($this->argument->options['validate_argument_signup_status']) ? $this->argument->options['validate_argument_signup_status'] : 'any',
      '#process' => array('views_process_dependency'),
      '#dependency' => array('edit-options-validate-type' => array($this->id)),
    );

    $form['validate_argument_signup_node_access'] = array(
      '#type' => 'checkbox',
      '#title' => t('Validate user has access to the node'),
      '#default_value' => !empty($this->argument->options['validate_argument_node_access']),
      '#process' => array('views_process_dependency'),
      '#dependency' => array('edit-options-validate-type' => array($this->id)),
    );
  }

  function validate_argument($argument) {
    if (!is_numeric($argument)) {
      return FALSE;
    }
    $node = node_load($argument);
    if (!$node) {
      return FALSE;
    }

    if (!empty($this->argument->options['validate_argument_signup_node_access'])) {
      if (!node_access('view', $node)) {
        return FALSE;
      }
    }

    $status = isset($this->argument->options['validate_argument_signup_status']) ? $this->argument->options['validate_argument_signup_status'] : 'any';
    switch ($status) {
      case 'any':
        return !empty($node->signup);

      case 'open':
        return !empty($node->signup_status);

      case 'closed':
        return !empty($node->signup) && empty($node->signup_status);

      case 'none':
        return empty($node->signup);

    }
  }
}

