// $Id: openlayers_behavior_tooltip.js,v 1.1.2.1 2010/02/03 18:13:45 tmcw Exp $

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
function openlayers_behavior_tooltip_popup_content(feature) {
  return "<div class='openlayers-popup'>" + feature.attributes.name +"</div>" +
         "<div class='openlayers-popup'>" + feature.attributes.description +"</div>";
}


/**
 * OpenLayers Tooltip Behavior
 */
Drupal.behaviors.openlayers_behavior_tooltip = function(context) {
  var layers, data = $(context).data('openlayers');
  if (data && data.map.behaviors['openlayers_behavior_tooltip']) {
      map = data.openlayers;

      // TODO: just select layers you want, instead of all vector layers
      layers = map.getLayersByClass('OpenLayers.Layer.Vector');

      popup_select = new OpenLayers.Control.SelectFeature(layers, 
          {
            hover: true,
            clickout: false,
            multiple: false,
            onSelect: function(feature) {
              popup = new OpenLayers.Popup.FramedCloud(
                'tooltip', 
                feature.geometry.getBounds().getCenterLonLat(),
                null,
                openlayers_behavior_tooltip_popup_content(feature),
                null, 
                true);
              feature.popup = popup;
              feature.layer.map.addPopup(popup);
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
  }
}
