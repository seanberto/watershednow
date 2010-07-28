// $Id:$
// Copyright (c) 2010 KontextWork GbR
// Author: Eugen Mayer

(function($) {
  $.fn.imguploadOuterHTML = function (s) {
    return (s) ? this.before(s).remove() : jQuery("<p>").append(this.eq(0).clone()).html();
  };

  Drupal.wysiwyg.plugins.imgupload = {
    /**
    * Return whether the passed node belongs to this plugin.
    */
    isNode: function (node) {
      $node = this.getRepresentitiveNode(node);
      return $node.is('img.wysiwyg_imageupload');
    },

    /* We need this due all the special cases in the editors */
    getRepresentitiveNode: function(node) {
      if (node != null && typeof(node) != 'undefined' && '$' in node) {
        // This case is for the CKeditor, where
        // $(node) != $(node.$)
        return $(node.$);
      }
      // else
      // This would be for the TinyMCE and hopefully others
      return $(node)
    },

    /**
    * Execute the button.
    */
    invoke: function (data, settings, instanceId) {
      if(data == null && typeof(data) == 'undefined') {
        return;
      }

      if (data.format == 'html') {
        // Default
        var options = {
          id: instanceId,
          action: 'insert',
          // we need that for changing the mode in the edit form to create a new iid if revisioned
          revisions: Drupal.settings.wysiwyg_imageupload.revisions
        };
        var $node = null;
        if ('node' in data) {
          $node = this.getRepresentitiveNode(data.node);
        }

        if ($node != null && $node.is('img') && $node.hasClass('wysiwyg_imageupload')) {
          $n = $(data.node);
          options.iid = decodeURIComponent(data.node.getAttribute('alt'));
          options.action = 'update';
        }
      }
      else {
        // @todo Plain text support.
      }
      // Add or update.
      if (options.action == 'insert') {
        Drupal.wysiwyg.plugins.imgupload.add_form(data, settings, instanceId);
      }
      else if (options.action == 'update') {
        Drupal.wysiwyg.plugins.imgupload.update_form(data, settings, instanceId, options);
      }
    },

    /*
    * Open a dialog and present the add-image form.
    */
    add_form: function (data, settings, instanceId) {
      // Create the buttons
      dialogIframe = Drupal.jqui_dialog.iframeSelector();
      btns = {};
      btns[Drupal.t('Insert')] = function () {
        // well lets test if an image has been selected
        if ($(dialogIframe).contents().find('form#wysiwyg-imageupload-upload-form').find('input#edit-wysiwyg-imageupload-file').size() > 0) {
          alert(Drupal.t("Please select an image to upload first"));
          return;
        }
        // else
        var iid = 0;
        form = $(dialogIframe).contents().find('form#wysiwyg-imageupload-edit-form');
        form.ajaxSubmit({
          dataType : 'json',
          method: 'post',
          async: false,
          data: { 'submitimagedetails' : 'JSinsert' },
          success : function(data,status,xhr,jq) {
              iid = data.data.iid;
              if(!iid) {
                return;
              }
            }
          }
        );
        Drupal.wysiwyg.plugins.imgupload.createImageInContent(iid,instanceId);
        $(this).dialog("close");
      };

      btns[Drupal.t('Cancel')] = function () {
        $(this).dialog("close");
      };
      var parent_build_id = Drupal.settings.wysiwyg_imageupload.current_form;
      // Location, where to fetch the dialog.
      var aurl = Drupal.settings.basePath + 'index.php?q=wysiwy_imageupload/upload/' + parent_build_id;
      // Open the dialog, load the form.
      Drupal.jqui_dialog.open({
        url: aurl,
        buttons: btns,
        width: 540
      });
    },

    /*
    * Open a image-details dialog, prefilled with the current settings of the
    * selected image.
    */
    update_form: function (data, settings, instanceId, options) {
      // Create buttons.
      dialogIframe = Drupal.jqui_dialog.iframeSelector();
      btns = {};
      // Update button.
      btns[Drupal.t('Update')] = function () {
        var iid = 0;
        $(dialogIframe).contents().find('form#wysiwyg-imageupload-edit-form').ajaxSubmit({
          dataType : 'json',
          method: 'post',
          data: { revisions: options.revisions, parent_build_id: Drupal.settings.wysiwyg_imageupload.current_form, 'submitimagedetails' : 'JSinsert' },
          async: false,
          success : function(data,status,xhr,jq) {
              iid = data.data.iid;
              if(!iid) {
                return;
              }
            }
          }
        );
        Drupal.wysiwyg.plugins.imgupload.createImageInContent(iid,instanceId);
        $(this).dialog("close");
      };
      // Cancel button
      btns[Drupal.t('Cancel')] = function () {
        $(this).dialog("close");
      };

      // Location, where to fetch the dialog.
      var aurl = Drupal.settings.basePath + 'index.php?q=wysiwyg_imageupload/edit/' + options.iid;
      // Finally open the dialog.
      Drupal.jqui_dialog.open({
        url: aurl,
        buttons: btns,
        width: 540
      });
    },

    /*
    * Fetches the imagecache preset representitive and insert it all th way down into the current editor
    */
    createImageInContent: function (iid,editor_id) {
      Drupal.wysiwyg.plugins.imgupload.insertIntoEditor(this.get_rendered_wysiwyg_image(iid),editor_id);
    },

    /*
    * Thats the most critical part. Call the WYSIWYG API to insert this html into
    * the current editor, no matter what editor it might be
    */
    insertIntoEditor: function (data, editor_id) {
      // This is all the magic
      Drupal.wysiwyg.instances[editor_id].insert(data);
    },

    attach: function(content, pluginSettings, id) {
      var plugin = this;
      var iids = [];
      content = content.replace(/\[\[wysiwyg_imageupload:(\d+):([^\]]*?)\]\]/g, function(orig, match) {
        iids.push(match);
        return orig;
      });

      if (iids.length === 0) {
        return content;
      }

      iids = plugin.uniqueArray(iids);
      var images = plugin.get_rendered_wysiwyg_images(iids);

      content = content.replace(
        /\[\[wysiwyg_imageupload:(\d+):([^\]]*?)\]\]/g,
        function(orig, iid) {
          return images[iid];
        }
      );
      return content;
    },

    detach: function (content, pluginSettings, id)  {
      var plugin = this;
      content = '<div>'+content+'</div>';
      $content = $(content);
      $content.find('img.wysiwyg_imageupload').map(
        function(i, img) {
          var inlineTag = $(img).attr('alt');
          $(img, $content).replaceWith('[[wysiwyg_imageupload:'+inlineTag+':]]');
        }
      );
      content = $content.html();
      $content.remove();
      return content;
    },

    get_rendered_wysiwyg_image: function(iid) {
       result = '';
       $.ajax( {
          url: Drupal.settings.basePath + 'index.php?q=ajax/wysiwyg_imgupl/render_wysiwyg/' + iid,
          async: false,
          success: function (data, status) {
            result = $(data.data).imguploadOuterHTML();
          },
          dataType: 'json'
        }
      );
      return result;
    },

    get_rendered_wysiwyg_images: function(iids) {
      result = [];
      $.ajax( {
          url: Drupal.settings.basePath + 'index.php?q=ajax/wysiwyg_imgupl/render_wysiwyg_images/' + iids.join(','),
          async: false,
          success: function (data, status) {
            result = data.data;
          },
          dataType: 'json'
        }
      );
      return result;
    },

    uniqueArray: function (a) {
      var r = new Array();
      o: for (var i = 0, n = a.length; i < n; i++) {
          for (var x = 0, y = r.length; x < y; x++) {
              if (r[x] == a[i])
                  continue o;
          }
          r[r.length] = a[i];
      }
      return r;
    },
  };
})(jQuery);
