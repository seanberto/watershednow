<?php
// $Id: email.inc,v 1.1.2.1 2009/01/07 00:55:05 dww Exp $


/**
 * @file
 * Theme functions for generating signup email messages.
 */

/**
 * Return the value for the %user_signup_info email token for custom signup data.
 *
 * @param $signup_data
 *   Array of custom data for a particular signup.
 *
 * @see theme_signup_user_form()
 * @see theme_signup_custom_data_email()
 */
function theme_signup_email_token_custom_data($signup_data) {
  return t('SIGNUP INFORMATION') ."\n\r". theme('signup_custom_data_email', $signup_data);
}

/**
 * Renders custom signup data into unfiltered output for use in email.
 *
 * WARNING: This theme function is recursive (it calls itself for
 * nested data), so if you override it, be sure not to change the part
 * where it does "call_user_func(__FUNCTION__)".
 *
 * @param $data
 *   Array of custom user signup data.
 *
 * @return
 *   Plain text output with newlines.
 *
 * @see theme_signup_user_form()
 */
function theme_signup_custom_data_email($data) {
  $output = '';
  // Loop through each first level element.
  foreach ($data as $key => $value) {
    if (is_array($value)) {
      // Element is nested, render it recursively.
      // Instead of the overhead of theme(), just call ourself directly.
      $output .= "\n\r". call_user_func(__FUNCTION__, $value) ."\n\r";
    }
    else {
      $output .= theme('signup_custom_data_field_text', $key, $value) ."\n\r";
    }
  }
  return $output;
}

/**
 * Renders a single custom signup form field into unfiltered output.
 *
 * @param $key
 *   Name of the custom signup field (the array key).
 * @param $value
 *   Value of the custom signup field (the array value).
 *
 * @return
 *   Plain text output to display this key/value pair.
 *
 * @see theme_signup_user_form()
 */
function theme_signup_custom_data_field_text($key, $value) {
  // All of the possible array key values should already be translated as
  // string literals in theme_signup_user_form() via the #title attributes, so
  // passing a variable to t() is actually safe here.  However, to avoid
  // warnings when extracting strings, "hide" the call to t() by using a
  // variable to hold the function name.
  $tr = 't';
  return $tr($key) .': '. $value;
}

/**
 * Controls the body of the copy of the broadcast message sent to the sender.
 *
 * @param $raw_message
 *   The raw message typed in by the sender with no tokens replaced.
 * @param $cooked_message
 *   The message after all the tokens have been replaced.
 *
 * @return
 *   The final text to put in the email body sent to the broadcast sender.
 */
function theme_signup_broadcast_sender_copy($raw_message, $cooked_message) {
  $output = t('This is a copy of the signup broadcast you just sent.') ."\n";
  $output .= wordwrap(t('Here is the original text you entered, with none of the tokens replaced:'), 72) ."\n";
  $output .= "----------\n";
  $output .= $raw_message;
  $output .= "\n----------\n\n";
  $output .= wordwrap(t('Here is how the message that was sent to each user looked, with all of the tokens replaced (using your account for the user-related tokens):'), 72) ."\n";
  $output .= "----------\n";
  $output .= $cooked_message;
  $output .= "\n----------\n\n";
  return $output;
}

