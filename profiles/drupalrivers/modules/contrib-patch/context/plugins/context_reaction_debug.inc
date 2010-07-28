<?php
// $Id: context_reaction_debug.inc,v 1.1.2.1 2009/12/14 22:34:05 yhahn Exp $

/**
 * Output context debug information.
 */
class context_reaction_debug extends context_reaction {
  function options_form() {
    return array('debug' => array('#type' => 'value', '#value' => TRUE));
  }

  function options_form_submit($values) {
    return array('debug' => 1);
  }

  /**
   * Output a list of active contexts.
   */
  function execute() {
    $contexts = context_active_contexts();
    foreach ($contexts as $context) {
      if (!empty($context->reactions['debug'])) {
        drupal_set_message(t("Active context: @name", array('@name' => $context->name)));
      }
    }
  }
}
