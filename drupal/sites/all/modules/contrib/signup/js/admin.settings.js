/* $Id: admin.settings.js,v 1.3.2.3 2009/01/24 08:52:46 dww Exp $ */

/**
 * Conditionally show/hide settings based on the signup form location.
 *
 * If the signup form is being show on the node itself, it's wrapped
 * in a fieldset and there's a setting to control if that fieldset
 * should be collapsed.  Only show this setting if the form is going
 * to be displayed on the node itself.
 */
Drupal.behaviors.signupShowFormLocationSetting = function () {
  $('div.signup-form-location-radios input.form-radio').click(function () {
    // Simple part: Depending on the form location, hide/show the
    // collapsible fieldset setting.
    if (this.value == 'node') {
      $('div.signup-fieldset_collapsed-setting').show();
    }
    else {
      $('div.signup-fieldset_collapsed-setting').hide();
    }
  });
};

/**
 * Conditionally show/hide settings based on the signup user list setting.
 *
 * If the signup user list is going to be an embedded view, show the
 * view-related settings, otherwise, hide them.
 */
Drupal.behaviors.signupShowUserListViewSetting = function () {
  $('div.signup-display-signup-user-list-setting input.form-radio').click(function () {
    if (this.value == 'embed-view' || this.value == 'embed-view-tab') {
      $('div.signup-user-list-view-settings').show();
    }
    else {
      $('div.signup-user-list-view-settings').hide();
    }
  });
};

/**
 * Conditionally show/hide settings based on the signup admin list setting.
 *
 * If the administer signup user list is going to be an embedded view,
 * show the view-related settings, otherwise, hide them.
 */
Drupal.behaviors.signupShowAdminListViewSetting = function () {
  $('div.signup-display-signup-admin-list-setting input.form-radio').click(function () {
    if (this.value == 'embed-view') {
      $('div.signup-admin-list-view-settings').show();
    }
    else {
      $('div.signup-admin-list-view-settings').hide();
    }
  });
};
