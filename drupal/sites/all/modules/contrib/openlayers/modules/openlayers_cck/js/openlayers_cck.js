// $Id: openlayers_cck.js,v 1.22.2.3 2010/08/27 11:50:45 zzolo Exp $

/**
 * @file
 * Interface enhancements for the OpenLayers CCK module.
 */

/**
 * Implements Drupal Behaviors
 */
Drupal.behaviors.openlayers_cck_wkt_hide = function(context) {
  // Hide WKT field and allow to toggle visibility.
  if (typeof Drupal.settings.openlayers_cck.wkt_hide != 'undefined') {
    var textShow = Drupal.settings.openlayers_cck.wkt_hide.text_show;
    var textHide = Drupal.settings.openlayers_cck.wkt_hide.text_hide;

    for (field in Drupal.settings.openlayers_cck.wkt_hide.fields) {
      var fieldID = '#edit-' + field + '-openlayers-wkt-wrapper';
      
      $(fieldID + ':not(.openlayers-cck-processed)').each(function() {
        var $thisField = $(this);
        
        $thisField.addClass('openlayers-cck-processed');
        // Add a link to be able to hide and show, and hide by default.
        var link = $('<a href="" id="' + field + '-toggle" class="openlayers-cck-hide-link">' + textShow + '</a>')
          .data('fieldID', fieldID);
        $thisField.before(link)
          .hide();
        $('#' + field + '-toggle').toggle(
          function () {
            var $thisLink = $(this);
            var localFieldID = $thisLink.data('fieldID');
            $(localFieldID).slideDown();
            $thisLink.text(textHide);
          },
          function () {
            var $thisLink = $(this);
            var localFieldID = $thisLink.data('fieldID');
            $(localFieldID).slideUp();
            $thisLink.text(textShow);
          }
        );
      });
    }
  }
};