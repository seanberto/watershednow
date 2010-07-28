// $Id: jquery_ui_dialog_child.js,v 1.1.2.12 2010/07/01 14:31:20 eugenmayer Exp $
(function ($) {
  Drupal.jqui_dialogChild = Drupal.jqui_dialogChild || {
    processed: false
  };

  /**
   * Child dialog behavior.
   */
  Drupal.jqui_dialogChild.attach = function (context) {
    var self = Drupal.jqui_dialogChild;
    var settings = Drupal.settings.jqui_dialogChild || {};

    // If we cannot reach the parent window, then we have nothing else todo here.
    if (!self.isObject(parent.Drupal) || !self.isObject(parent.Drupal.jqui_dialog)) {
      return;
    }

    // Shortcuts to parent objects.
    self.$pWindow = parent.jQuery(parent);
    self.pJQui_dialog = parent.Drupal.jqui_dialog;

    // Update title
    self.pJQui_dialog.container.dialog( 'option', 'title' , $('title').html());
    // Make sure this behavior is not processed more than once.
    if (!self.processed) {
      self.processed = true;
      Drupal.jqui_dialogChild.handleOptions(settings);
      Drupal.jqui_dialogChild.recheckSize();
    }
  };

  Drupal.jqui_dialogChild.recheckSize = function () {
    var self = Drupal.jqui_dialogChild;
    self.pJQui_dialog = parent.Drupal.jqui_dialog;
    // we need this timeout otherwise the DOM might not be loaded fully yet. This would result in wrong
    // calculation of width / height
    setTimeout(function () {
      var height = $('body').outerHeight();
      self.pJQui_dialog.chilDocumentSize = {
        width: $(window).width(),
        height: height + 33
      };
      self.pJQui_dialog.resize();
    }, 100);
  };

  /**
  * Handle options
  */
  Drupal.jqui_dialogChild.handleOptions = function(opt) {
    var self = Drupal.jqui_dialogChild;
    if('width' in opt) {
     self.pJQui_dialog.set_childWidth(opt.width);
    }
  };

  /**
   * Check if the given variable is an object.
   */
  Drupal.jqui_dialogChild.isObject = function (something) {
    return (something != null && typeof something === 'object');
  };

  $(document).ready(function () {
    $('body').ajaxComplete(function () {
      Drupal.jqui_dialogChild.recheckSize();
    });
  });

  Drupal.behaviors.jqui_dialogChild = Drupal.jqui_dialogChild.attach;

})(jQuery);