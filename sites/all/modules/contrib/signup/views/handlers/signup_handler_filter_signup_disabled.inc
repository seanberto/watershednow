<?php
// $Id: signup_handler_filter_signup_disabled.inc,v 1.4.2.2 2008/12/19 01:00:36 dww Exp $

/**
 * Filter nodes based on if signups are disabled or enabled.
 */
class signup_handler_filter_signup_disabled extends views_handler_filter_boolean_operator {
  function get_value_options() {
    $this->value_options = array(1 => t('Enabled'), 0 => t('Disabled'));
  }

  function query() {
    $this->ensure_my_table();
    $this->query->add_where($this->options['group'], "$this->table_alias.nid ". (empty($this->value) ? 'IS' : 'IS NOT') .' NULL');
  }
}

