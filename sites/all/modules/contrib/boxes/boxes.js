// $Id: boxes.js,v 1.2 2010/02/19 19:21:52 yhahn Exp $
Drupal.behaviors.boxes = function(context) {
  // Unwrap the AHAH call contents as ahah.js replaces the *contents* of the
  // wrapper, not the wrapper itself.
  if ($(context).is('div') && $('div.block-boxes', context).size() > 0) {
    $(context).replaceWith($('div.block-boxes', context).children());
  }
  $('div.boxes-box-controls a:not(.boxes-processed)')
    .addClass('boxes-processed')
    .click(function() {
      $(this).parents('.boxes-box-inline').toggleClass('boxes-box-editing');
      return false;
    });
  // AHAH processing for boxes forms.
  $('input.boxes-box-submit:not(.boxes-processed), input.boxes-box-delete:not(.boxes-processed)')
    .addClass('boxes-processed')
    .each(function() {
      var base = $(this).attr('id');
      // Nothing to see here folks, it's been processed (or will be processed)
      // properly by ahah.js.
      if (Drupal.settings.ahah && Drupal.settings.ahah[base]) {
        return false;
      }
      // Train wreck. We need to wipe ahah.js's.
      else {
        var settings = {
          button: {op: 'Save'},
          effect: 'fade',
          element: $(this),
          event: 'mousedown',
          keypress: true,
          method: 'replace',
          progress: {type: 'throbber'},
          selector: '#' + $(this).attr('id'),
          url: $(this).is('.boxes-box-submit') ? Drupal.settings.boxes.ajaxSubmit : Drupal.settings.boxes.ajaxDelete,
          wrapper: $(this).parents('div.block').attr('id')
        };
        $(settings.selector).each(function() {
          settings.element = this;
          var ahah = new Drupal.ahah(base, settings);
        });
      }
    });
};
