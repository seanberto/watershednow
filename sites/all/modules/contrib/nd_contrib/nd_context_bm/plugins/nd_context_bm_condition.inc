<?php
// $Id: nd_context_bm_condition.inc,v 1.1.2.1 2010/07/27 19:11:52 swentel Exp $

/**
 * @file
 * context plugin that allows a build mode of a node in its full page view to act as a condition for a context
 */

class nd_context_bm_condition extends context_condition {
  /**
   * Generate a list of available build modes
   */
  function condition_values() {
    $values = array();
    $build_modes = ds_get_build_modes('nd');
    foreach ($build_modes as $key => $bm) {
      $values[$key] = $bm['title'];
    }
    return $values;
  }

  function options_form($context) {
    // @todo: expose options like: overrule views node views etc ...
    return array();
  }

  function execute($node, $op) {
    foreach ($this->get_contexts($node->build_mode) as $context) {
      $this->condition_met($context, $node->build_mode);
    }
  }
}
