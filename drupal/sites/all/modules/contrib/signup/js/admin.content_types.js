/* $Id: admin.content_types.js,v 1.3.2.1 2009/04/15 22:25:44 dww Exp $ */

/**
 * Conditionally show or hide the signup date field setting.
 *
 * On a node type settings form, if the "Allow signups" radios set
 * set to 0 ('Disabled'), or the node type is event-enabled, then
 * hide the date field setting.  Otherwise, show it.
 */
Drupal.behaviors.signupShowDateField = function () {
  $('div.signup-node-default-state-radios input[type=radio], div.event-nodeapi-radios input[type=radio]').click(function () {
    var eventEnabled = false;
    var signupDisabled = true;
    $('div.event-nodeapi-radios input.form-radio').each(function() {
      if (this.checked && this.value != 'never') {
        eventEnabled = true;
      }
    });
    $('div.signup-node-default-state-radios input.form-radio').each(function() {
      if (this.checked && this.value != 'disabled') {
        signupDisabled = false;
      }
    });
    if (signupDisabled || eventEnabled) {
      $('div.signup-date-field-setting').hide();
    }
    else {
      $('div.signup-date-field-setting').show();
    }
  });
};
