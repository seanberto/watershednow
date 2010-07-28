<?php
// $Id: context_condition_user.inc,v 1.1.2.5 2010/04/26 14:55:48 yhahn Exp $

/**
 * Expose current user role as a context condition.
 */
class context_condition_user extends context_condition {
  function condition_values() {
    return drupal_map_assoc(user_roles());
  }

  function execute($account) {
    $roles = $account->roles;
    foreach ($roles as $rid => $role) {
      if ($rid == DRUPAL_ANONYMOUS_RID || $rid == DRUPAL_AUTHENTICATED_RID) {
        $role = t($role);
      }
      foreach ($this->get_contexts($role) as $context) {
        $this->condition_met($context, $role);
      }
    }
  }
}
