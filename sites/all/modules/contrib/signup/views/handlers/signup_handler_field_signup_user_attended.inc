<?php
// $Id: signup_handler_field_signup_user_attended.inc,v 1.1.2.2 2008/12/19 01:00:36 dww Exp $

/**
 * Field handler to display the attendance for a signed up user.
 */
class signup_handler_field_signup_user_attended extends views_handler_field {
  function render($values) {
    return theme('signup_attended_text', $values->{$this->field_alias});
  }
}

