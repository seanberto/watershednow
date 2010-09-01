// $Id: openlayers_behavior_popup.js,v 1.1.2.2 2010/02/16 03:44:07 tmcw Exp $

/**
 * Helper function for generating content inside popups
 *
 * This function can be overridden by 
 * other implementors just by redefining it.
 *
 * @param feature
 *  OpenLayers feature object
 * @return
 *  Formatted HTML
 */
function openlayers_behavior_popup_popup_content(feature) {
  return "<div class='openlayers-popup'>" + feature.attributes.name +"</div>" +
         "<div class='openlayers-popup'>" + feature.attributes.description +"</div>";
}

var openlayers_behavior_popup_popup_select, openlayers_behavior_popup_selected_feature;

/**
 * OpenLayers Popup Behavior
 */
Drupal.behaviors.openlayers_behavior_popup = function(context) {
  var layers, data = $(context).data('openlayers');
  if (data && data.map.behaviors['openlayers_behavior_popup']) {
      map = data.openlayers;

      // TODO: just select layers you want, instead of all vector layers
      layers = map.getLayersByClass('OpenLayers.Layer.Vector');

      popup_select = new OpenLayers.Control.SelectFeature(layers, 
          {
            onSelect: function(feature) {
              popup = new OpenLayers.Popup.FramedCloud(
                'popup', 
                feature.geometry.getBounds().getCenterLonLat(),
                null,
                openlayers_behavior_popup_popup_content(feature),
                null, 
                true,
                function(evt) {
                  openlayers_behavior_popup_popup_select.unselect(
                    openlayers_behavior_popup_selected_feature
                  );
                }
              );
              feature.popup = popup;
              feature.layer.map.addPopup(popup);
              openlayers_behavior_popup_selected_feature = feature;
          },
          onUnselect: function(feature) {
            feature.layer.map.removePopup(feature.popup);
            feature.popup.destroy();
            feature.popup = null;
          }
        }
      );
    map.addControl(popup_select);
    popup_select.activate();
    openlayers_behavior_popup_popup_select = popup_select;
  }
}
