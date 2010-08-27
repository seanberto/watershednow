// $Id: openlayers_behavior_layerswitcher.js,v 1.1.2.3 2009/09/28 01:11:41 zzolo Exp $

/**
 * @file
 * JS Implementation of OpenLayers behavior.
 */

/**
 * Layer Switcher Behavior
 */
Drupal.behaviors.openlayers_behavior_layerswitcher = function(context) {
  var data = $(context).data('openlayers');
  if (data && data.map.behaviors['openlayers_behavior_layerswitcher']) {
    // Add control
    var control = new OpenLayers.Control.LayerSwitcher();
    data.openlayers.addControl(control);
    control.activate();
  }
}