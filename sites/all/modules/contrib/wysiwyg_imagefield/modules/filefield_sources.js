// $Id: filefield_sources.js,v 1.1 2010/08/06 11:05:26 deciphered Exp $

(function ($) {
  // Make sure WYSIWYG ImageField objects are defined.
  Drupal.wysiwygImageField = Drupal.wysiwygImageField || {};
  Drupal.wysiwygImageField.hookSetPosition = Drupal.wysiwygImageField.hookSetPosition || {};

  // Implements hookSetPosition.
  Drupal.wysiwygImageField.hookSetPosition.fileFieldSources = function() {
    // Set table width.
    $('#wysiwyg_imagefield-wrapper .filefield-element .widget-edit table').css('width', $('#wysiwyg_imagefield-dialog').width() - 131);

    // Set pager width.
    $('#wysiwyg_imagefield-wrapper .filefield-element .widget-edit .view-content + .pager').css({
      'clear': 'both',
      'width': $('#wysiwyg_imagefield-dialog').width() - 10
    });
  }

  Drupal.behaviors.wysiwygImageField_fileFieldSources = function(context) {
    var vars = {};
    getVars = function(element) {
      $(($(element).attr('class')).split(' ')).each(function(id, className) {
        if (className.indexOf('wysiwyg_imagefield-') != -1) {
          args = className.substr(19).split('-');
          vars[args[0]] = args[1];
        }
      });
    }

    // Invoke FileField Sources hookSetPosition.
    Drupal.wysiwygImageField.hookSetPosition.fileFieldSources();

    // Table hover.
    $('#wysiwyg_imagefield-wrapper tbody tr').hover(
      function() {
        getVars(this);
        $(this).addClass('selected').css('cursor', 'pointer');
        $(this).parents('.view-content').find('.imagefield-preview').html(
          $(this).parents('.view-content').find('.wysiwyg_imagefield-thumbnail-' + vars['fid']).html()
        );
      },
      function() {
        getVars(this);
        $(this).removeClass('selected').css('cursor', 'default');
        $(this).parents('.view-content').find('.imagefield-preview').html('');
      }
    );

    // Table click.
    $('#wysiwyg_imagefield-wrapper tbody tr').click(function() {
      getVars(this);
      $(this).parents('.filefield-source-reference').find('.wysiwyg_imagefield-hidden .form-text').val('[fid:' + vars['fid'] + ']');
      $(this).parents('.filefield-source-reference').find('.wysiwyg_imagefield-hidden .form-submit').mousedown();
    });
  }
})(jQuery);
