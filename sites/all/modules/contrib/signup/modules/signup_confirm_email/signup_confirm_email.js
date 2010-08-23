/* $Id: signup_confirm_email.js,v 1.1 2009/08/03 20:59:45 dww Exp $ */

/**
 * Conditionally show the "Update e-mail..." checkbox on signup forms.
 *
 * We only want to display this checkbox if the user changes the
 * "E-mail address" field while signing up or editing a signup.
 * Unfortunately, just using change() doesn't work. We need to see
 * when a key is pressed that can modify the value, and use that to
 * trigger revealing the checkbox and its description.
 *
 * However, if there's a validation error regarding the checkbox, we
 * need to ensure it's always visible, even if the e-mail address
 * field is unchanged.
 */
Drupal.behaviors.showSignupConfirmEmailCheckbox = function() {
  if (Drupal.settings.signupConfirmEmailCheckboxError) {
    $('div#signup-confirm-email-checkbox').show();
  }
  else {
    $('#edit-email-address-wrapper input[type=text]').keyup(function (e) {
      switch (e.keyCode) {
        case 16: // shift
        case 17: // ctrl
        case 18: // alt
        case 20: // caps lock
        case 33: // page up
        case 34: // page down
        case 35: // end
        case 36: // home
        case 37: // left arrow
        case 38: // up arrow
        case 39: // right arrow
        case 40: // down arrow
        case 9:  // tab
        case 13: // enter
        case 27: // esc
          return false;
        default:
          $('div#signup-confirm-email-checkbox').show();
      }
    });
  }
}

