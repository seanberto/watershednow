<?php
// $Id: signup.views.inc,v 1.4.2.9 2011/01/03 20:27:24 ezrag Exp $


/**
 * @file
 * Provides support for Views integration.
 *
 * This exposes signup-related data and declares handlers (fields, filters)
 * and plugins (argument validators).
 */

/**
 * Implementation of hook_views_data().
 */
function signup_views_data() {
  // Define the base group of the signup table.  Fields that don't
  // have a group defined will go into this field by default.
  $data['signup']['table']['group'] = t('Signup');

  $data['signup']['table']['join'] = array(
    'node' => array(
      'left_field' => 'nid',
      'field' => 'nid',
    ),
  );

  $data['signup']['status'] = array(
    'title' => t('Status: Open/Closed'),
    'help' => t('Are signups open or closed for this node?'),
    // Information for displaying the field
    'field' => array(
      'handler' => 'signup_handler_field_signup_status',
      'click sortable' => TRUE,
    ),
    // Information for accepting status as a filter
    'filter' => array(
      'handler' => 'signup_handler_filter_signup_status',
      'help' => t('Filter on if signups are open or closed for each node.'),
      'label' => 'Signup status',
    ),
    // Information for sorting on status
    'sort' => array(
      'handler' => 'views_handler_sort',
      'help' => t('Sort by if signups are closed or open.'),
    ),
  );

  $data['signup']['close_signup_limit'] = array(
    'title' => t('Signup Limit'),
    'help' => t('Maximum number of users who can sign up before signups are automatically closed (set to 0 for no limit).'),
    // Information for displaying the field
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    // Information for accepting close_signup_limit as a filter
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
      'help' => t('Filter by the maximum number of users who can sign up before signups are automatically closed (set to 0 for no limit).'),
      'label' => 'Signup limit',
    ),
  );

  $data['signup']['forwarding_email'] = array(
    'title' => t('Notification: Email Address'),
    'help' => t('Address where notification emails are sent wenever a user signs up.'),
    // Information for displaying the field
    'field' => array(
      'handler' => 'views_handler_field',
    ),
  );

  $data['signup']['send_confirmation'] = array(
    'title' => t('Send Confirmation'),
    'help' => t('Should confirmation email be sent to each user who signs up.'),
    // Information for displaying the field
    'field' => array(
      'handler' => 'signup_handler_field_signup_send_confirmation',
      'click sortable' => TRUE,
    ),
    // Information for accepting send_confirmation as a filter
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
      'help' => t('Filter on if signup confirmation is enabled or disabled.'),
      'label' => t('Send confirmation'),
    ),
  );

  $data['signup']['confirmation_email'] = array(
    'title' => t('Confirmation: Message'),
    'help' => t('The body of the optional confirmation email that can be sent whenever a user signs up.'),
    // Information for displaying the field
    'field' => array(
      'handler' => 'views_handler_field',
    ),
  );

  $data['signup']['send_reminder'] = array(
    'title' => t('Send Reminder'),
    'help' => t('Should a reminder email be automatically sent to all users who signed up. This will be false if either an administrator disabled the feature for a given event, or if the reminder was already sent.'),
    // Information for displaying the field
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
      'help' => t('Filter on if a signup reminder email will be sent.'),
      'label' => t('Send reminder'),
    ),
  );

  $data['signup']['reminder_days_before'] = array(
    'title' => t('Reminder: Days Before Event for Email'),
    'help' => t('How many days before an event will the reminder email be sent.'),
    // Information for displaying the field
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
  );

  $data['signup']['reminder_email'] = array(
    'title' => t('Reminder: Message'),
    'help' => t('The body of the optional reminder email that can be sent a configurable time before an event begins.'),
    // Information for displaying the field
    'field' => array(
      'handler' => 'views_handler_field',
    ),
  );

  $data['signup']['signup_disabled'] = array(
    'title' => t('Enabled/Disabled'),
    'filter' => array(
      'handler' => 'signup_handler_filter_signup_disabled',
      'help' => t('Filter on if signups are enabled or disabled.'),
      'label' => 'Signups',
      ),
  );

  $data['signup']['node_link'] = array(
    'title' => t('Node: Signup link'),
    'field' => array(
      'handler' => 'signup_handler_field_signup_node_link',
      'help' => t('Link to one of the signup-related tabs on each node.'),
    ),
  );

  // Define base group for signup_log table.
  $data['signup_log']['table']['group'] = t('Signup');

  // Advertise this table as a possible base table so that we can have
  // "Signup" views. This is essential for Views Bulk Operations to work,
  // since we need operations on the signups themselves, not the users or
  // nodes they point to.  With proper use of relationships, we can get
  // everything on signup views we'd want from either node or user views.
  $data['signup_log']['table']['base'] = array(
    'field' => 'sid',
    'title' => t('Signup'),
    'help' => t('A signup represents a relationship between a user and a node.'),
  );

  $data['signup_log']['table']['join'] = array(
    'node' => array(
      'left_field' => 'nid',
      'field' => 'nid',
    ),
    'users' => array(
      'left_field' => 'uid',
      'field' => 'uid',
    ),
  );

  $data['signup_log']['sid'] = array(
    'title' => t('Signup ID'),
    'help' => t('The signup entry ID'),
    // Information for displaying the field
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    // Information for accepting sid as a filter
    'filter' => array(
      'label' => t('Signup entry id'),
      'handler' => 'views_handler_filter_numeric',
    ),
    // Information for sorting on sid
    'sort' => array(
      'handler' => 'views_handler_sort',
      'help' => t('Sort by signup ID.'),
    )
  );
  
  $data['signup_log']['nid'] = array(
    'title' => t('Node'),
    'help' => t('The node a user signed up to.'),
    'relationship' => array(
      'base' => 'node',
      'handler' => 'views_handler_relationship',
      'label' => t('Signup node'),
    ),
  );

  $data['signup_log']['uid'] = array(
    'title' => t('User'),
    'help' => t('The user who signed up.'),
    'relationship' => array(
      'base' => 'users',
      'handler' => 'views_handler_relationship',
      'label' => t('Signup user'),
    ),
  );

  $data['signup_log']['uid_no_signups'] = array(
    'title' => t('User available signups'),
    'help' => t('Displays nodes the given user has not signed up for. Note that this argument will not work properly if you add the %signup_user relationship to the view.', array('%signup_user' => t('Signup: User'))),
    'real field' => 'uid',
    'argument' => array(
      'handler' => 'signup_handler_argument_signup_user_uid',
    ),
  );

  $data['signup_log']['email'] = array(
    'title' => t('User: Email Address'),
    'click sortable' => TRUE,
    'field' => array(
      'handler' => 'signup_handler_field_signup_user_email',
      'help' => t('Email address of a user (authenticated or anonymous) who signed up.') .' <strong>'. t('WARNING: only expose this data in a view that is restricted to users whom you can trust with such sensitive information.') .'</strong>',
    ),
  );

  $data['signup_log']['signup_time'] = array(
    'title' => t('User: Signup Time'),
    'help' => t('Time when user signed up.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
      'help' => t('Time when the user signed up.'),
      'label' => t('Signup time'),
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
      'help' => t('Sort by time of sign up.'),
    ),
  );

  $data['signup_log']['attended'] = array(
    'title' => t('User: Attended'),
    'help' => t('Did the user attend or not?'),
    'field' => array(
      'handler' => 'signup_handler_field_signup_user_attended',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'signup_handler_filter_signup_user_attended',
      'help' => t('Filter on users who attended or not.'),
      'label' => 'Signup attendance',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
      'help' => t('Sort by if the user attended or not.'),
    ),
  );

  $data['signup_log']['form_data'] = array(
    'title' => t('User: Additional Signup Info'),
    'help' => t('Enter the additional field name (from theme/signup_form.inc) in the Option column.'),
    'field' => array(
      'handler' => 'signup_handler_field_signup_user_form_data',
      'option' => 'string',
    ),
  );

  $data['signup_log']['signup_user_auth'] = array(
    'title' => t('User: Anonymous/Authenticated'),
    'real field' => 'uid',
    'filter' => array(
      'handler' => 'signup_handler_filter_signup_user_authenticated',
      'help' => t('Filter on if a user who signed up is anonymous, or an authenticated user on the site.'),
      'label' => t('Signup user'),
    ),
  );

  $data['signup_log']['signup_user_current'] = array(
    'title' => t('User: Current User'),
    'real field' => 'uid',
    'filter' => array(
      'handler' => 'views_handler_filter_user_current',
      'help' => t('This allows you to filter by whether or not the node was signed up by the logged in user of the view.'),
    ),
  );

  $data['signup_log']['edit_link'] = array(
    'title' => t('User: Edit signup link'),
    'field' => array(
      'handler' => 'signup_handler_field_signup_edit_link',
      'help' => t('Link to edit this particular signup.'),
    ),
  );

  return $data;
}

/**
 * Implementation of hook_views_handlers().
 *
 * Register information about all of the views handlers provided by the Signup
 * module.
 */
function signup_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'signup') .'/views/handlers',
    ),
    'handlers' => array(
      'signup_handler_argument_signup_user_uid' => array(
        'parent' => 'views_handler_argument_numeric',
      ),
      'signup_handler_field_signup_edit_link' => array(
        'parent' => 'views_handler_field_node_link',
      ),
      'signup_handler_field_signup_node_link' => array(
        'parent' => 'views_handler_field_node_link',
      ),
      'signup_handler_field_signup_send_confirmation' => array(
        'parent' => 'views_handler_field',
      ),
      'signup_handler_field_signup_status' => array(
        'parent' => 'views_handler_field',
      ),
      'signup_handler_field_signup_user_attended' => array(
        'parent' => 'views_handler_field',
      ),
      'signup_handler_field_signup_user_email' => array(
        'parent' => 'views_handler_field',
      ),
      'signup_handler_field_signup_user_form_data' => array(
        'parent' => 'views_handler_field_prerender_list',
      ),
      'signup_handler_filter_signup_disabled' => array(
        'parent' => 'views_handler_filter_boolean_operator',
      ),
      'signup_handler_filter_signup_status' => array(
        'parent' => 'views_handler_filter_boolean_operator',
      ),
      'signup_handler_filter_signup_user_attended' => array(
        'parent' => 'views_handler_filter_boolean_operator',
      ),
      'signup_handler_filter_signup_user_authenticated' => array(
        'parent' => 'views_handler_filter_boolean_operator',
      ),
    ),
  );
}

/**
 * Implementation of hook_views_plugins().
 */
function signup_views_plugins() {
  $path = drupal_get_path('module', 'signup') .'/views/plugins';
  return array(
    'argument validator' => array(
      'signup_status' => array(
        'title' => t('Signup status'),
        'handler' => 'signup_plugin_argument_validate_signup_status',
        'path' => $path,
      ),
    ),
    'access' => array(
      'user_signup_list' => array(
        'title' => t('View per-user signup lists'),
        'help' => t("Access will be granted to users viewing their own signup list or with the 'view all signups' permission."),
        'handler' => 'signup_plugin_access_user_signup_list',
        'uses options' => TRUE,
        'path' => $path,
      ),
    ),
  );
}

