// $Id: openlayers_geocoder.js,v 1.5.2.13 2010/09/23 14:55:42 antoniodemarco Exp $

Drupal.behaviors.openlayers_geocoder = function (context) {

  if (Drupal.jsAC) {
    /**
   * Hides the autocomplete suggestions
   */
    Drupal.jsAC.prototype.hidePopup = function (keycode) {
      // Select item if the right key or mousebutton was pressed
      if (this.selected && ((keycode && keycode != 46 && keycode != 8 && keycode != 27) || !keycode)) {
        this.input.value = this.selected.autocompleteValue;
      }
      // Hide popup
      var popup = this.popup;
      if (popup) {
        this.popup = null;
        $(popup).fadeOut('fast', function() {
          $(popup).remove();
        });
        // Add-on for OpenLayer Geocoder module
        if ($(this.input).attr('geoautocomplete')) {
          geocoder = new Drupal.Geocoder(this);
          geocoder.process(this.input.value);
        }
      }
      this.selected = false;
    
    };
  
  }
  
};

/**
 * Geocoder object
 */
Drupal.Geocoder = function (data) {
  this.data = data;
};

/**
 * Performs a search
 */
Drupal.Geocoder.prototype.process = function (query) {
  
  var fieldname = $(this.data.input).attr('fieldname');
  var dashed = $(this.data.input).attr('dashed');
  var formid = $("input[name=form_id]").val();
  var contenttype = formid.replace("_node_form", "");
  
  var data = {
    query:query,
    fieldname:fieldname,
    content_type:contenttype
  };

  $.ajax({
    type: 'POST',
    url: this.data.db.uri + '/process',
    data: data,
    dataType: 'json',
    success: function(point) {

      if (point.longitude && point.latitude) {

        var data = $('#openlayers-cck-widget-map-' + fieldname).data('openlayers');
        if (!data.map.displayProjection) {
          data.map.displayProjection = 4326;
        }
        var displayProjection = new OpenLayers.Projection('EPSG:' + data.map.displayProjection);
        var projection = new OpenLayers.Projection('EPSG:' + data.map.projection);
        var vectorLayer = data.openlayers.getLayersBy('drupalID', "openlayers_drawfeatures_layer");
        var geometry = new OpenLayers.Geometry.Point(point.longitude, point.latitude).transform(displayProjection, projection);
        var bounds = new OpenLayers.Bounds(point.box.west, point.box.south, point.box.east, point.box.north).transform(displayProjection, projection);

        //Remove all points, unless CCK widget settings prevent it.
        if (point.keep_points) {
          data.openlayers.setCenter(new OpenLayers.LonLat(point.longitude, point.latitude).transform(displayProjection, projection));
        }
        else {
          vectorLayer[0].removeFeatures(vectorLayer[0].features);
          data.openlayers.zoomToExtent(bounds);
          // Adding CCK fields autocompletion
          if (point.fields) {
            jQuery.each(point.fields, function () {
              $(this.type + "[name*='" + this.name + "']").attr('value', this.value);
              if (!this.override) {
                $(this.type + "[name*='" + this.name + "']").attr('readonly', 'TRUE').addClass('readonly');
              }
            });
          }
        }
        
        //Add point to map.
        vectorLayer[0].addFeatures([new OpenLayers.Feature.Vector(geometry)]);
      }
    }
  });

}







