// $Id: jquery_ui_dialog_parent.js,v 1.1.2.12 2010/07/01 14:31:20 eugenmayer Exp $
(function ($) {
  Drupal.jqui_dialog = Drupal.jqui_dialog || {
    options: {},
    chilDocumentSize: {},
    container: undefined
  };
  /**
   * Open a jquery ui dialog
   * @param options
   *   Properties of the modal frame to open:
   *   - url : the URL of the page to open. If not provided, you have to handle the loading yourself
   *   - method(get) : What method should use to load the url (get/post)
   *   - data : Post DATA you can pass, if you use method=post
   *   - for all the other options please refer the jquery ui dialog documentation http://jqueryui.com/demos/dialog/#options
   */
  Drupal.jqui_dialog.open = function (c_options) {
    var self = this;

    // Close callback for the jQuery UI dialog.
    var dialogClose = function () {
      try {
        self.container.dialog('destroy').remove();
      } catch (e) {};
    };

    self.options = {
      // custom
      url: '',
      method: 'get',
      data: {},
      // jquery dialog
      modal: true,
      autoOpen: false,
      closeOnEscape: true,
      resizable: false,
      draggable: false,
      autoresize: true,
      close: dialogClose,
      dialogClass: 'jquery_ui_dialog-dialog',
      title: Drupal.t('Loading...')
    };
    $.extend(self.options, c_options);

    // Create the dialog and related DOM elements.
    // Setting self.container
    self.create(self.options);
    // Open the dialog offscreen where we can set its size, etc.
    self.container.dialog('open');
    if (self.options.url != '') {
      self.loadIframe(self.iframe.get(0), self.options);
    }

    if ('width' in self.options) {
      self.container.dialog('option', {
        width: self.options.width+25
      });
      self.iframe.width(self.options.width);
    }
    if ('height' in self.options) {
      self.container.dialog('option', {
        height: self.options.height
      });
      self.iframe.height(self.options.height-100);
    }

    return true;
  };

  /**
   * Inititalize the dialog.
   */
  Drupal.jqui_dialog.create = function (options) {
    var self = this;

    // Note: We use scrolling="yes" for IE as a workaround to yet another IE bug
    // where the horizontal scrollbar is always rendered, no matter how wide the
    // iframe element is defined.
    var container = $('<div id="jq-ui-dialog-container"/>').append('<iframe frameborder="0" id="jq-ui-dialog-iframe" name="jq-ui-dialog-iframe"' + ($.browser.msie ? ' scrolling="yes"' : '') + '/>');

    $('body').append(container);
    self.container = $('#jq-ui-dialog-container');
    self.iframe = self.container.find('#jq-ui-dialog-iframe');
    // Open the jQuery UI dialog offscreen.
    self.container.dialog(options);
    // We need this for our theme to be in a namespace
    $('.jquery_ui_dialog-dialog').wrap('<div class="jquery_ui_dialog-dialog-wrapper"/>');
  };

  /**
   * Load the given URL into the dialog iframe.
   */
  Drupal.jqui_dialog.loadIframe = function (iframe, options) {
    var self = this;
    // Get the document object of the iframe window.
    // @see http://xkr.us/articles/dom/iframe-document/
    var doc = (iframe.contentWindow || iframe.contentDocument);
    if ('document' in doc) {
      doc = doc.document;
    }

    if (options.method == 'post') {
      $(doc).post(
      options.url, options.data, function (data) {
        $(doc).html(data);
      });
      return;
    }
    //else
    doc.location.replace(options.url);
  };

  Drupal.jqui_dialog.childLoaded = function (iFrameWindow) {
    var self = this;
    var $iFrameWindow = iFrameWindow.jQuery;
    var $iFrameDocument = $iFrameWindow(iFrameWindow.document);

    //$iFrameDocument.attr('tabIndex', -1).css('outline', 0);
    $('.ui-dialog-title').html($iFrameDocument.attr('title'));
    self.resize();
  };

  /**
   * Resize the modal frame based on the current document size.
   *
   * This method may be invoked by:
   * - The parent window resize handler (when the parent window is resized).
   * - The bindChild() method (when the child document is loaded).
   * - The child window resize handler (when the child window is resized).
   */
  Drupal.jqui_dialog.resize = function () {
    var self = this;
    var documentSize = self.chilDocumentSize;
    if(self.options.autoresize === false) {
      return;
    }
    // Compute frame and dialog size based on document size.
    var maxSize = self.sanitizeSize({});
    var titleBarHeight = $('.ui-dialog-titlebar').outerHeight(true);
    // if we have a button pane
    var buttonBarHeight = 0;
    if ('buttons' in self.options) {
      buttonBarHeight = $('.ui-dialog-buttonpane').outerHeight(true);;
    }

    var paddingHeight = 12;
    var paddingWidth = 26;
    var frameSize = self.sanitizeSize(documentSize);
    var dialogSize = $.extend({}, frameSize);

    if ((dialogSize.height + titleBarHeight + buttonBarHeight) <= maxSize.height) {
      dialogSize.height += titleBarHeight + buttonBarHeight + paddingHeight;
    }
    else {
      dialogSize.height = maxSize.height;
      frameSize.height = dialogSize.height - titleBarHeight - buttonBarHeight;
    }

    // Compute dialog position centered on viewport.
    var dialogPosition = self.computeCenterPosition($('.jquery_ui_dialog-dialog'), dialogSize);
    var animationOptions = $.extend({
      width: frameSize.width + paddingWidth,
      height: dialogSize.height
    }, dialogPosition);
    self.iframe.hide();
    self.container.dialog('option', {
      width: frameSize.width + paddingWidth,
      height: dialogSize.height
    });
    self.iframe.width(frameSize.width).height(frameSize.height);
    // Perform the resize animation.
    $('.jquery_ui_dialog-dialog').animate(animationOptions, 'fast', function () {
      // Proceed only if the dialog still exists.
      if (self.isObject(self.container)) {
        self.iframe.fadeIn('slow');
      }
    });
  };

  /**
   * Check if the given variable is an object.
   */
  Drupal.jqui_dialog.isObject = function (something) {
    return (something !== null && typeof something === 'object');
  };

  /**
   * Sanitize dialog size.
   */
  Drupal.jqui_dialog.sanitizeSize = function (size) {
    var width, height;
    var $window = $(window);
    var minWidth = 300,
      maxWidth = $window.width() - 30;
    if (typeof size.width != 'number') {
      width = maxWidth;
    }
    else if (size.width < minWidth || size.width > maxWidth) {
      width = Math.min(maxWidth, Math.max(minWidth, size.width));
    }
    else {
      width = size.width;
    }
    var minHeight = 100,
      maxHeight = $window.height() - 30;
    if (typeof size.height != 'number') {
      height = maxHeight;
    }
    else if (size.height < minHeight || size.height > maxHeight) {
      height = Math.min(maxHeight, Math.max(minHeight, size.height));
    }
    else {
      height = size.height;
    }
    return {
      width: width,
      height: height
    };
  };

  /**
   * Compute the position to center an element with the given size.
   */
  Drupal.jqui_dialog.computeCenterPosition = function ($element, elementSize) {
    var $window = $(window);
    var position = {
      left: Math.max(0, parseInt(($window.width() - elementSize.width) / 2)),
      top: Math.max(0, parseInt(($window.height() - elementSize.height) / 2))
    };
    if ($element.css('position') != 'fixed') {
      var $document = $(document);
      position.left += $document.scrollLeft();
      position.top += $document.scrollTop();
    }
    return position;
  };

  Drupal.jqui_dialog.fixPosition = function ($element, isOpen) {
    var self = this;
    var $window = $(window);
    if ($.browser.msie && parseInt($.browser.version) <= 6) {
      // IE6 does not support position:'fixed'.
      // Lock the window scrollBar instead.
      if (isOpen) {
        var yPos = $window.scrollTop();
        var xPos = $window.scrollLeft();
        $window.bind(self.eventHandlerName('scroll'), function () {
          window.scrollTo(xPos, yPos);
          // Default browser action cannot be prevented here.
        });
      }
      else {
        $window.unbind(self.eventHandlerName('scroll'));
      }
    }
    else {
      // Use CSS to do it on other browsers.
      if (isOpen) {

        var offset = $element.offset();
        $element.css({
          left: (offset.left - $window.scrollLeft()),
          top: (offset.top - $window.scrollTop()),
          position: 'fixed'
        });
      }
    }
  };

  Drupal.jqui_dialog.iframeSelector = function () {
    return '#jq-ui-dialog-iframe';
  };

  Drupal.jqui_dialog.close = function () {
    $('#jq-ui-dialog-iframe').dialog('close');
  };

  Drupal.jqui_dialog.set_childWidth = function (w) {
    var self = this;
    self.options.width = w;
    self.container.dialog('option', {
        width: self.options.width + 26
    });
    self.iframe.width(self.options.width-10);
  };
})(jQuery);