<?php
// $Id: context_condition_sitewide.inc,v 1.1.2.1 2009/12/14 22:34:05 yhahn Exp $

/**
 * Simple sitewide context, always present.
 */
class context_condition_sitewide extends context_condition {
  function condition_values() {
    return array(1 => t('Always active'));
  }

  function editor_form($context = NULL) {
    $form = parent::editor_form($context);
    $form[1]['#title'] = t('Always active');
    $form['#weight'] = -10;
    return $form;
  }
}
