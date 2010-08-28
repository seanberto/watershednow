/**
 * Drupal behavior for designkit.
 */
Drupal.behaviors.designkit = function (context) {
  $('input.designkit-colorpicker:not(.designkit-processed)')
    .addClass('designkit-processed')
    .each(function() {
      // Add Farbtastic
      var target = $('div#' + $(this).attr('id') + '-colorpicker');
      $.farbtastic(target, $(this));
      $(this)
        .focus(function() { target.show('fast'); })
        .blur(function() { target.hide('fast'); });
  });
};
