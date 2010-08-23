<?php
// $Id: signup_handler_argument_signup_user_uid.inc,v 1.1 2008/11/12 01:00:58 dww Exp $

/**
 * Argument handler display nodes the given UID has not signed up for.
 */
class signup_handler_argument_signup_user_uid extends views_handler_argument_numeric {
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    // Remove the 'not' and 'break_phrase' (which lets you select multiple
    // values) form elements, since neither one makes any sense for this
    // argument.
    unset($form['not']);
    unset($form['break_phrase']);
  }
  function query() {
    $join = $this->get_join();
    $join->extra[] = array(
      'field' => $this->real_field,
      'value' => $this->argument,
    );
    $alias = $this->query->add_table($this->table, $this->relationship, $join);
    $this->query->add_where(0, "$alias.$this->real_field IS NULL");
  }
}

