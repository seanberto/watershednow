// $Id: openlayers_behavior_argparser.js,v 1.1.2.1 2010/05/30 19:02:01 tmcw Exp $

/**
 * @file
 * JS Implementation of OpenLayers behavior.
 */

/**
 * ArgParser Behavior
 */
Drupal.behaviors.openlayers_behavior_argparser = function(context) {
  var data = $(context).data('openlayers');
  if (data && data.map.behaviors['openlayers_behavior_argparser']) {
    // Add control
    var control = new OpenLayers.Control.ArgParser();
    data.openlayers.addControl(control);
    control.activate();
  }
}
