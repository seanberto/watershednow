<?php
// $Id: signup_handler_filter_signup_status.inc,v 1.2.2.2 2008/12/19 01:00:36 dww Exp $

/**
 * Filter nodes based on their signup status (open vs. closed).
 */
class signup_handler_filter_signup_status extends views_handler_filter_boolean_operator {
  function get_value_options() {
    $this->value_options = array(1 => t('Open'), 0 => t('Closed'));
  }
}

