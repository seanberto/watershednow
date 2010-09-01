// $Id: openlayers_behavior_fullscreen.js,v 1.1.2.3 2010/04/06 18:56:43 tmcw Exp $

/**
 * @file
 * JS Implementation of OpenLayers behavior.
 */

/**
 * Attribution Behavior
 */
Drupal.behaviors.openlayers_behavior_fullscreen = function(context) {
  var fullscreen_panel, data = $(context).data('openlayers');
  if (data && data.map.behaviors['openlayers_behavior_fullscreen']) {
    fullscreen_panel = new OpenLayers.Control.Panel(
      {
        displayClass: "openlayers_behavior_fullscreen_button_panel"
      }
    );
    data.openlayers.addControl(fullscreen_panel);
    var button = new OpenLayers.Control.Button({
      displayClass: "openlayers_behavior_fullscreen_button", 
      title: "Fullscreen", 
      trigger: openlayers_behavior_fullscreen_toggle
    });
    fullscreen_panel.addControls([button]);
  }
}

openlayers_behavior_fullscreen_toggle = function(context) {
  $(this.map.div).parent().toggleClass('openlayers_map_fullscreen');
  $(this.map.div).toggleClass('openlayers_map_fullscreen');
  $(this.map.div).data('openlayers').openlayers.updateSize();
}
