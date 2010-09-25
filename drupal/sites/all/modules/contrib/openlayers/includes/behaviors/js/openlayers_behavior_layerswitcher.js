// $Id: openlayers_behavior_layerswitcher.js,v 1.1.2.5 2010/08/31 08:26:28 strk Exp $

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
    var control = new OpenLayers.Control.LayerSwitcher({
      'ascending': !!data.map.behaviors['openlayers_behavior_layerswitcher'].ascending
    });
    data.openlayers.addControl(control);
    control.activate();
  }
}
