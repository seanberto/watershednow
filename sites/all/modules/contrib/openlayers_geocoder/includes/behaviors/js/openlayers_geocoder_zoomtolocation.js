// $Id: openlayers_geocoder_zoomtolocation.js,v 1.1.2.1 2010/04/26 08:58:48 antoniodemarco Exp $

/**
 * @file
 * JS Implementation of OpenLayers behavior.
 */

/**
 * Zoom to location
 */
Drupal.behaviors.openlayers_geocoder_zoomtolocation = function(context) {
  var data = $(context).data('openlayers');
  if (data && data.map.behaviors['openlayers_geocoder_zoomtolocation']) {
	var bounds = data.map.behaviors['openlayers_geocoder_zoomtolocation'];
	// After geocoding perform zoom.
	if (!data.map.displayProjection) {
		data.map.displayProjection = 4326;
	}
    var displayProjection = new OpenLayers.Projection('EPSG:' + data.map.displayProjection);
    var projection = new OpenLayers.Projection('EPSG:' + data.map.projection);
    data.openlayers.zoomToExtent(new OpenLayers.Bounds(bounds.southwest.lng, bounds.southwest.lat, bounds.northeast.lng, bounds.northeast.lat).transform(displayProjection, projection));
  }
}