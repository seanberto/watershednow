// $Id: openlayers_behavior_boxselect.js,v 1.1.2.4 2010/08/06 17:00:56 tmcw Exp $

/**
 * @file
 * JS Implementation of OpenLayers behavior.
 */
var selections_layer;

/**
 * Box Select Behavior
 */
Drupal.behaviors.openlayers_behavior_boxselect = function(context) {
  function setRestrictedExtent(box) {
    bounding_box = box.geometry.getBounds().toBBOX();
    $('#edit-center-restrict-restrictedExtent').val(bounding_box);
    for(i = 0; i < selections_layer.features.length; i++) {
      if(selections_layer.features[i] != box) {
        selections_layer.features[i].destroy();
      }
    }
  }

  var data = $(context).data('openlayers');
  if (data && data.map.behaviors['openlayers_behavior_boxselect']) {
    selections_layer = new OpenLayers.Layer.Vector('Temporary Box Layer');
    control = new OpenLayers.Control.DrawFeature(selections_layer, OpenLayers.Handler.RegularPolygon, {featureAdded: setRestrictedExtent});
    control.handler.setOptions({
        'keyMask': OpenLayers.Handler.MOD_SHIFT,
        'sides': 4,
        'irregular': true});
    control.events.on({"featureAdded": this.setRestrictedExtent});
    data.openlayers.addLayer(selections_layer);
    data.openlayers.addControl(control);
    if ($('#edit-center-restrict-restrictedExtent').val() != '') {
      bounds = $('#edit-center-restrict-restrictedExtent').val();
      geometry = new OpenLayers.Bounds.fromString(bounds).toGeometry();
      feature = new OpenLayers.Feature.Vector(geometry);
      selections_layer.addFeatures([feature]);
    }
    control.activate();
  }
}
