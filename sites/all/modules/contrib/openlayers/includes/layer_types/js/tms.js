// $Id: tms.js,v 1.1.2.1 2010/05/30 21:46:15 zzolo Exp $

/**
 * @file
 * Layer handler for TMS layers
 */

/**
 * Openlayer layer handler for TMS layer
 */
Drupal.openlayers.layer.tms = function (title, map, options) {
  var styleMap = Drupal.openlayers.getStyleMap(map, options.drupalID);
    if (options.maxExtent !== undefined) {
      options.maxExtent = new OpenLayers.Bounds.fromArray(options.maxExtent);
    }
    options.projection = 'EPSG:'+options.projection;
    var layer = new OpenLayers.Layer.TMS(title, options.base_url, options);
    layer.styleMap = styleMap;
    return layer;
};
