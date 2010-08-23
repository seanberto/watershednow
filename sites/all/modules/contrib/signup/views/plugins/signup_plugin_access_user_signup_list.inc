<?php
// $Id: signup_plugin_access_user_signup_list.inc,v 1.1 2009/07/22 21:47:01 dww Exp $

/**
 * Validate whether an argument is a user who has permission to view signups.
 */
class signup_plugin_access_user_signup_list extends views_plugin_access {

  /**
   * See if the given user should have access outside of the menu system.
   *
   * This is used for block displays, for example. If the given account has
   * the 'view all signups' permission, we always grant access.  Otherwise, we
   * only grant access if the user the view is listing (from a given argument)
   * matches the given account.
   */
  function access($account) {
    if (user_access('view all signups', $account)) {
      return TRUE;
    }
    $this->view->set_display($this->display->id);
    $this->view->init_handlers();
    $user_arg = $this->options['signup_user_argument'];
    $argument = $this->view->argument[$user_arg];
    return $account->uid == $argument->get_value();
  }

  function get_access_callback() {
    return array('signup_view_user_list_access', array($this->view->name, $this->display->id, $this->options['signup_user_argument']));
  }

  function summary_title() {
    return t('View signup user list');
  }

  function option_defaults(&$options) {
    $options['signup_user_argument'] = '';
  }
  
  function options_form(&$form, &$form_state) {
    $arguments = array();
    foreach ($this->view->display_handler->get_handlers('argument') as $id => $handler) {
      $arguments[$id] = $handler->definition['title'];
    }
    $form['signup_user_argument'] = array(
      '#type' => 'select',
      '#options' => $arguments,
      '#title' => t('Signup user argument'),
      '#description' => t('You must select which argument to this view represents the user who signed up.'),
      '#default_value' => $this->options['signup_user_argument'],
    );
  }

}

