<?php
// $Id: signup_handler_field_signup_node_link.inc,v 1.1.2.3 2009/09/20 23:12:37 dww Exp $

/**
 * Field handler to generate links to the various signup subtabs on a node.
 */
class signup_handler_field_signup_node_link extends views_handler_field_node_link {
  function construct() {
    parent::construct();
    $this->additional_fields['uid'] = array('table' => 'node', 'field' => 'uid');
    $this->additional_fields['signup_status'] = array('table' => 'signup', 'field' => 'status');
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['tab'] = array(
      '#type' => 'select',
      '#title' => t('Signup tab to link to'),
      '#options' => array(
        'signup' => t('Signup current user form'),
        'list' => t('Signup user list'),
        'admin' => t('Administer signups'),
        'settings' => t('Signup settings'),
        'add' => t('Form to sign up other users'),
        'broadcast' => t('Signup broadcast form'),
      ),
      '#default_value' => $this->options['tab'],
    );
    $form['destination'] = array(
      '#type' => 'checkbox',
      '#title' => t('Set view as return destination'),
      '#description' => t('After submitting a form, do you want to be redirected back to this view, or stay at the other page?'),
      '#default_value' => $this->options['destination'],
    );
  }

  function render($values) {
    $tab = $this->options['tab'];
    $node = new StdClass();
    $node->nid = $values->{$this->aliases['nid']};
    $node->uid = $values->{$this->aliases['uid']};
    $node->signup_status = $values->{$this->aliases['signup_status']};
    $node->signup = isset($values->{$this->aliases['signup_status']});

    // Ensure the current user can access this link for the given node.
    if (!$this->check_access($node, $tab)) {
      return;
    }

    $text = !empty($this->options['text']) ? $this->options['text'] : $this->get_default_link_text($tab);
    $link_options = array();
    $url = $this->get_url($node, $tab, $link_options);
    if ($tab != 'list' && !empty($this->options['destination'])) {
      $link_options['query'] = drupal_get_destination();
    }
    return l($text, $url, $link_options);
  }

  function get_default_link_text($tab) {
    switch ($tab) {
      case 'signup':
        return t('signup form');

      case 'list':
        return t('signup list');

      case 'admin':
        return t('administer signups');

      case 'settings':
        return t('signup settings');

      case 'add':
        return t('signup other users');

      case 'broadcast':
        return t('signup broadcast');

    }
  }

  function check_access($node, $tab) {
    switch ($tab) {    
      case 'signup':
        // Slightly complicated, since _signup_menu_access() is testing if we
        // want the signup tab, but here we just want to know if the user can
        // signup at all.
        return user_access('sign up for content') && _signup_needs_output($node) && variable_get('signup_form_location', 'node') != 'none';

      case 'list':
        // Also can't use _signup_menu_access() since we don't have to assume
        // a tab for this access check.
        $view_all = user_access('view all signups');
        $admin = _signup_menu_access($node, 'admin');
        return (($view_all || $admin) && _signup_needs_output($node));

      case 'settings':
        // There's no separate check for this, it just assumes 'admin' acces.
        return _signup_menu_access($node, 'admin');

      default:
        // All the other tabs can just use _signup_menu_access() directly.
        return _signup_menu_access($node, $tab);

    }
  }

  function get_url($node, $tab, &$link_options) {
    switch ($tab) {
      case 'signup':
        if (variable_get('signup_form_location', 'node') == 'node') {
          $link_options['fragment'] = 'signup';
          return "node/$node->nid";
        }
        break;

      case 'list':
        $user_list = variable_get('signup_display_signup_user_list', 'signup');
        if ($user_list == 'signup' || $user_list == 'embed-view') {
          $link_options['fragment'] = 'signup';
          return "node/$node->nid";
        }
        break;

    }
    // If we didn't already special-case the URL, we know it's just a subtab
    // on the "signups" tab for the node.
    return "node/$node->nid/signups/$tab";
  }

}

