<?php
// $Id: nd_context_bm_reaction.inc,v 1.1.2.1 2010/07/27 19:11:52 swentel Exp $

/**
 * @file
 * context plugin that allows to set the build mode of a node in its full page view to act as a reaction to a context
 */

class nd_context_bm_reaction extends context_reaction {
  function options_form($context) {
    $values = array();
    $build_modes = ds_get_build_modes('nd');
    foreach ($build_modes as $key => $bm) {
      $values[$key] = $bm['title'];
    }
    return array(
      '#title' => $this->title,
      '#description' => $this->description,
      '#options' => $values,
      '#type' => 'select',
      '#default_value' => $this->fetch_from_context($context),
    );
  }

  /**
   * Output a list of active contexts.
   */
  function execute($node) {
    $contexts = $this->get_contexts();
    foreach ($contexts as $context) {
      if (!empty($context->reactions[$this->plugin])) {
        $node->build_mode = $context->reactions[$this->plugin];
      }
    }
  }
}
