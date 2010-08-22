// $Id: wysiwyg_imagefield.js,v 1.2 2010/08/08 03:18:59 deciphered Exp $

(function ($) {
  // Make sure WYSIWYG ImageField objects are defined.
  Drupal.wysiwygImageField = Drupal.wysiwygImageField || {};
  Drupal.wysiwygImageField.hookSetPosition = Drupal.wysiwygImageField.hookSetPosition || {};

  Drupal.wysiwyg.plugins['wysiwyg_imagefield'] = {
    /**
     * Return whether the passed node belongs to this plugin.
     */
    isNode: function(node) {
      return false;
    },

    /**
     * Execute the button.
     */
    invoke: function(data, settings, instanceId) {
      $('#wysiwyg_imagefield-wrapper').css('display', 'block').dialog('open');
      Drupal.wysiwygImageField.focus();
      Drupal.wysiwygImageField.setPosition();
      $($('#wysiwyg_imagefield-wrapper').children().get(0)).css('display', 'none');
      if ($('#wysiwyg_imagefield-wrapper').children().length == 1) {
        $('#wysiwyg_imagefield-wrapper table .filefield-element:last').parent().parent().appendTo($('#wysiwyg_imagefield-wrapper'));
      }
    },

    /**
     * Create wysiwyg_imagefield dialog window.
     */
    attach: function(content, settings, instanceId) {
      $('#wysiwyg_imagefield-wrapper').dialog({
        autoOpen: false,
        height: 'inherit',
        title: Drupal.t('WYSIWYG ImageField')
      });
      Drupal.wysiwygImageField.init();
      return content;
    },

    /**
     * Replace images with <!--break--> tags in content upon detaching editor.
     */
    detach: function(content, settings, instanceId) {
      return content;
    }
  };

  Drupal.wysiwygImageField = {
    init: function() {
      $('#wysiwyg_imagefield-wrapper').parents('.ui-dialog').attr('id', 'wysiwyg_imagefield-dialog');

      // Move dialog back inside form.
      $('#wysiwyg_imagefield-dialog').bind('focus', function() {
        Drupal.wysiwygImageField.focus();
      });
    },

    focus: function() {
      if ($('#wysiwyg_imagefield-dialog').parent() !== $('#' + Drupal.wysiwyg.activeId + '-wrapper')) {
        $('#wysiwyg_imagefield-dialog').prependTo($('#' + Drupal.wysiwyg.activeId + '-wrapper'));
      }
    },

    getId: function() {
      // Use tallest field as target.
      height = 0;
      id = null;
      $('#' + Drupal.wysiwyg.activeId + '-wrapper').children().each(function() {
        if ($(this).css('display') !== 'none' && $(this).height() > height && $(this).attr('id') != 'wysiwyg_imagefield-dialog')  {
          height = $(this).height();
          if (!$(this).attr('id')) {
            $(this).attr('id', 'wysiwyg_imagefield-' + (new Date().getTime()) + Math.ceil(Math.random() * 100));
          }
          id = '#' + $(this).attr('id');
        }
      });
      return id;
    },

    setPosition: function() {
      id = Drupal.wysiwygImageField.getId();

      // Position dialog.
      $('#wysiwyg_imagefield-dialog').css({
        left: $(id).position().left + 35,
        top: $(id).position().top + 50,
        width: $(id).width() - 100 > 475 ? $(id).width() - 100 : 475
      });
      $('#wysiwyg_imagefield-wrapper').css('width', 'inherit');

      // Invoke Drupal.wysiwygImageField.hookSetPosition().
      if (Drupal.wysiwygImageField.hookSetPosition != undefined) {
        $.each(Drupal.wysiwygImageField.hookSetPosition, function() {
          if ($.isFunction(this)) {
            this();
          }
        });
      }
    }
  }

  Drupal.behaviors.wysiwygImageField = function(context) {
    $('#wysiwyg_imagefield-wrapper .insert-button').click(function() {
      $('#wysiwyg_imagefield-wrapper').dialog('close');
      $('#wysiwyg_imagefield-wrapper .content-add-more input:submit').mousedown();
      $($('#wysiwyg_imagefield-wrapper').children().get(1)).remove();
    });
  }
})(jQuery);
