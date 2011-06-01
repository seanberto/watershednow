
/**
 * Global variables to help with scope
 *
 * TODO: Move this to a better place, like the map data().
 */
Drupal.openlayers = Drupal.openlayers || {};
Drupal.openlayers.popup = Drupal.openlayers.popup || {};
Drupal.openlayers.popup.popupSelect = Drupal.openlayers.popup.popupSelect || {};
Drupal.openlayers.popup.selectedFeature = Drupal.openlayers.popup.selectedFeature || {};

/**
 * Javascript Drupal Theming function for inside of Popups
 *
 * To override
 *
 * @param feature
 *  OpenLayers feature object
 * @return
 *  Formatted HTML
 */
Drupal.theme.prototype.openlayersPopup = function(feature) {
  var output =
    '<div class="openlayers-popup openlayers-popup-name">' +
      feature.attributes.name +
    '</div>' +
    '<div class="openlayers-popup openlayers-popup-description">' +
      feature.attributes.description +
    '</div>';
  return output;
}

/**
 * OpenLayers Popup Behavior
 */
Drupal.behaviors.openlayers_behavior_popup = function(context) {
  var layers, data = $(context).data('openlayers');
  if (data && data.map.behaviors['openlayers_behavior_popup']) {
    var map = data.openlayers;
    var options = data.map.behaviors['openlayers_behavior_popup'];
    var layers = [];

    // For backwards compatiability, if layers is not
    // defined, then include all vector layers
    if (typeof options.layers == 'undefined' || options.layers.length == 0) {
      layers = map.getLayersByClass('OpenLayers.Layer.Vector');
    }
    else {
      for (var i in options.layers) {
        var selectedLayer = map.getLayersBy('drupalID', options.layers[i]);
        if (typeof selectedLayer[0] != 'undefined') {
          layers.push(selectedLayer[0]);
        }
      }
    }

    popupSelect = new OpenLayers.Control.SelectFeature(layers,
      {
        onSelect: function (feature) {
          // Create FramedCloud popup.
          popup = new OpenLayers.Popup.FramedCloud(
            'popup',
            feature.geometry.getBounds().getCenterLonLat(),
            null,
            Drupal.theme('openlayersPopup', feature),
            null,
            true,
            function (evt) {
              var f = Drupal.openlayers.popup.selectedFeature;
              if (f.layer)
                Drupal.openlayers.popup.popupSelect.unselect(f);
              else {
                // Remove popup if feature was removed in the meantime.
                f.popup.map.removePopup(f.popup);
                f.popup.destroy();
                f.popup = null;
              }
            }
          );
          
          // Assign popup to feature and map.
          feature.popup = popup;
          feature.layer.map.addPopup(popup);
          popup.map = feature.layer.map;
          Drupal.openlayers.popup.selectedFeature = feature;
        },
        onUnselect: function (feature) {
          // Remove popup if feature is unselected.
          feature.layer.map.removePopup(feature.popup);
          feature.popup.destroy();
          feature.popup = null;
        }
      }
    );

    map.addControl(popupSelect);
    popupSelect.activate();
    Drupal.openlayers.popup.popupSelect = popupSelect;
  }
}
