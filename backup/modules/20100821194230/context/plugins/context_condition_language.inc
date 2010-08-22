<?php
// $Id: context_condition_language.inc,v 1.1.2.1 2010/02/09 05:58:07 yhahn Exp $

/**
 * Expose current language as a context condition.
 */
class context_condition_language extends context_condition {
  function condition_values() {
    return module_exists('locale') ? locale_language_list() : array();
  }
}
