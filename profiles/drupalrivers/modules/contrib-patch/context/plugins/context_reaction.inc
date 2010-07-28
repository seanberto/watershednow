<?php
// $Id: context_reaction.inc,v 1.1.2.4 2010/05/25 16:06:07 yhahn Exp $

/**
 * Base class for a context reaction.
 */
class context_reaction {
  var $plugin;
  var $title;
  var $description;

  /**
   * Clone our references when we're being cloned.
   *
   * PHP 5.3 performs 'shallow' clones when clone()'ing objects, meaning that
   * any objects or arrays referenced by this object will not be copied, the
   * cloned object will just reference our objects/arrays instead. By iterating
   * over our properties and serializing and unserializing them, we force PHP to
   * copy them.
   */
  function __clone() {
    foreach ($this as $key => $val) {
      if (is_object($val) || (is_array($val))) {
        $this->{$key} = unserialize(serialize($val));
      }
    }
  }

  /**
   * Constructor. Do not override.
   */
  function __construct($plugin, $info) {
    $this->plugin = $plugin;
    $this->title = isset($info['title']) ? $info['title'] : $plugin;
    $this->description = isset($info['description']) ? $info['description'] : '';
  }

  function options_form($context) {
  }

  /**
   * Options form submit handler.
   */
  function options_form_submit($values) {
    return $values;
  }

  /**
   * Public method that is called from hooks or other integration
   * points with Drupal.
   */
  function execute() {
  }

  /**
   * Retrieve active contexts that have values for this reaction.
   */
  function get_contexts() {
    $contexts = array();
    foreach (context_active_contexts() as $context) {
      if ($this->fetch_from_context($context)) {
        $contexts[$context->name] = $context;
      }
    }
    return $contexts;
  }

  /**
   * Retrieve options from the context provided.
   */
  function fetch_from_context($context) {
    return isset($context->reactions[$this->plugin]) ? $context->reactions[$this->plugin] : array();
  }
}
