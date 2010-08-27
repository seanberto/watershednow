<?php
// $Id: openlayers_views_plugin_display_openlayers.inc,v 1.1.2.3 2010/03/25 14:40:56 tmcw Exp $

/**
 * @file
 * Display plugin for OpenLayers Data
 *
 * @ingroup openlayers
 */

class openlayers_views_plugin_display_openlayers extends views_plugin_display {
  function uses_breadcrumb() { return FALSE; }
  function get_style_type() { return 'openlayers'; }

  /**
   * OpenLayers data features are used directly in the map object.
   */
  function execute() {
    return $this->view->render();
  }

  function preview() {
    return '<pre>' . check_plain($this->view->render()) . '</pre>';
  }

  /**
   * Instead of going through the standard views_view.tpl.php, delegate this
   * to the style handler.
   */
  function render() {
    return $this->view->style_plugin->render($this->view->result);
  }

  function option_definition() {
    $options = parent::option_definition();

    // Prevent the title from being translated at the view level
    // so that it can be translated at the OpenLayers level
    $options['title']['translatable'] = 0;

    $options['displays'] = array('default' => array());

    // Overrides for standard stuff:
    $options['style_plugin']['default'] = 'openlayers_data';
    $options['row_plugin']['default'] = '';
    $options['defaults']['default']['row_plugin'] = FALSE;
    $options['defaults']['default']['row_options'] = FALSE;
    return $options;
  }

  function options_summary(&$categories, &$options) {
    parent::options_summary($categories, $options);
    unset($options['header']);
    unset($options['footer']);
    unset($options['empty']);
    unset($options['exposed_block']);
    unset($options['analyze-theme']);
  }
}
