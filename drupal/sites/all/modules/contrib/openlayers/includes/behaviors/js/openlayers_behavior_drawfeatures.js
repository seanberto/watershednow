// $Id: openlayers_behavior_drawfeatures.js,v 1.1.2.8 2010/06/06 19:54:28 zzolo Exp $

/**
 * @file
 * DrawFeatures Behavior
 */

/**
 * Update function for features
 *
 */
function openlayers_behavior_drawfeatures_update(features) {
  WktWriter = new OpenLayers.Format.WKT();
  var features_copy = features.object.clone();
  for(var i in features_copy.features) {
    features_copy.features[i].geometry.transform(
      features.object.map.projection,
      new OpenLayers.Projection("EPSG:4326")
    );
  }
  wkt_value = WktWriter.write(features_copy.features);
  this.val(wkt_value);
}

/**
 * Behavior for Draw Features
 */
Drupal.behaviors.openlayers_behavior_drawfeatures = function(context) {
  var data = $(context).data('openlayers');
  if (data && data.map.behaviors['openlayers_behavior_drawfeatures']) {
    var feature_types = data.map.behaviors['openlayers_behavior_drawfeatures'].feature_types;
      
    // Add control
    var openlayers_drawfeature_element = 
      $("#" + data.map.behaviors['openlayers_behavior_drawfeatures'].element_id);

    // Create options
    var options = {
      projection: new OpenLayers.Projection('EPSG:4326'),
      drupalID: 'openlayers_drawfeatures_layer'
    };
    var styleMap = Drupal.openlayers.getStyleMap(data.map, options.drupalID);
    var dataLayer = new OpenLayers.Layer.Vector(Drupal.t("Feature Layer"), options);
    dataLayer.styleMap = styleMap;
    data.openlayers.addLayer(dataLayer);

    if (openlayers_drawfeature_element.text() != '') {
      var wktFormat = new OpenLayers.Format.WKT();
      var wkt = openlayers_drawfeature_element.text();
      var features = wktFormat.read(wkt);
      for(var i in features) {
        features[i].geometry = features[i].geometry.transform(
          new OpenLayers.Projection('EPSG:4326'),
          data.openlayers.projection
        );
      }
      dataLayer.addFeatures(features);
    }

    // registering events late, because adding data
    // would result in a reprojection loop
    dataLayer.events.register('featureadded', openlayers_drawfeature_element,
      openlayers_behavior_drawfeatures_update);
    dataLayer.events.register('featureremoved', openlayers_drawfeature_element,
      openlayers_behavior_drawfeatures_update);
    dataLayer.events.register('featuremodified', openlayers_drawfeature_element,
      openlayers_behavior_drawfeatures_update);
    
    var control = new OpenLayers.Control.EditingToolbar(dataLayer);
    data.openlayers.addControl(control);
    control.activate();

    var class_names = {
      'point': 'OpenLayers.Handler.Point',
      'path': 'OpenLayers.Handler.Path',
      'polygon': 'OpenLayers.Handler.Polygon'
    };

    var c = [];
    for(var i in control.controls) {
      for(var j in feature_types) {
        // don't judge the navigation control
        if(control.controls[i].handler !== null) {
          if(control.controls[i].handler.CLASS_NAME == 
          class_names[feature_types[j]]) {
            c.push(control.controls[i]);
          }
        }
        else {
          c.push(control.controls[i]);
        }
      }
    }
    control.controls = c;
    control.redraw();

    var mcontrol = new OpenLayers.Control.ModifyFeature(
      dataLayer, {
        displayClass: 'olControlModifyFeature',
        deleteCodes: [46, 68, 100],
        handleKeypress: function(evt){                              
          if (this.feature && $.inArray(evt.keyCode, this.deleteCodes) > -1) {
            // We must unselect the feature before we delete it 
            var feature_to_delete = this.feature;
            this.selectControl.unselectAll();
            this.layer.removeFeatures([feature_to_delete]);
          }
        }
      }
    );
    control.addControls(mcontrol);
  }
};
