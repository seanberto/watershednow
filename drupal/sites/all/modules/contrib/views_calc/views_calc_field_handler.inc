<?php
// $Id: views_calc_field_handler.inc,v 1.3 2009/04/21 12:15:28 karens Exp $
/**
 * @file
 * Copied from the basic 'node' field handler.
 */

/**
 * Field handler to provide simple renderer that allows linking to a node.
 */
class views_calc_field_handler extends views_handler_field {
  /**
   * Constructor to provide additional field to add.
   */
  function construct() {
    parent::construct();
    $this->additional_fields['nid'] = 'nid';
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['link_to_node'] = array('default' => FALSE);
    return $options;
  }

  /**
   * Provide link to node option
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['link_to_node'] = array(
      '#title' => t('Link this field to its node'),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['link_to_node']),
    );
  }

  /**
   * Render whatever the data is as a link to the node.
   *
   * Data should be made XSS safe prior to calling this function.
   */
  function render_link($data, $values) {
    if (!empty($this->options['link_to_node'])) {
      return l($data, "node/" . $values->{$this->aliases['nid']}, array('html' => TRUE));
    }
    else {
      return $data;
    }
  }

  /**
   * Find the right calculation and add it to the query as
   * an aliased field.
   */
  function query() {
    $results = _views_calc_fields();
    while ($calc_field = db_fetch_array($results)) {
      if ($this->definition['cid'] == $calc_field['cid']) {
        foreach (explode(',', $calc_field['tablelist']) as $table) {
          $this->view->query->add_table($table);
        }
        $this->view->query->add_field(NULL, "({$calc_field['calc']})", "cid". $calc_field['cid']);
        return;
      }
    }    
  }
  
  function pre_query() {
    $this->field_alias = "cid{$this->definition['cid']}";
    parent::pre_query();
  }
  
  /**
   * Use the requested format function to render the raw alias value.
   */
  function render($values) {
    $field_alias = "cid{$this->definition['cid']}";
    
    $value = $values->$field_alias;
    $formats = _views_calc_format_options();
    $format = $formats[$this->definition['format']];
    $tmp = explode(':', $format);
    $function = trim($tmp[0]);
    $vars     = count($tmp) == 2 ? $tmp[1] : '';
    if ($function == 'custom') {
      $tmp = explode(':', $this->definition['custom']);
      $function = trim($tmp[0]);
      $vars     = count($tmp) == 2 ? $tmp[1] : '';
    }
    if (empty($function) || $function == 'none') {
      $function = 'check_plain';
    }
    $raw = $function($value, $vars);

    // This needs to be set for the $this->render_link() to work. It would
    // have been set in the query, if we hadn't bypassed the normal query.
    // TODO there may be a better way to do this.
    $this->aliases['nid'] = 'nid';

    return $this->render_link($raw, $values);
  }
}