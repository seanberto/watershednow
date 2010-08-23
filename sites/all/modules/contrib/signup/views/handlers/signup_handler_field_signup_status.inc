<?php
// $Id: signup_handler_field_signup_status.inc,v 1.1 2008/11/12 01:00:58 dww Exp $

/**
 * Field handler to display the current signup status of a node.
 */
class signup_handler_field_signup_status extends views_handler_field {
  function render($values) {
    return ($values->{$this->field_alias} == 0) ? t('Closed') : t('Open');
  }
}

