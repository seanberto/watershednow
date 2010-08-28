// $Id: fields.js,v 1.1.2.1 2010/01/28 14:47:28 swentel Exp $

/**
 * Toggle all exclude at once.
 */
Drupal.behaviors.field_excludes = function(context) {

  $('.select-all').bind('click', function() {
    var excluder = this;
    $(excluder).parents('div').parents('div').find('.exclude-types').not('.exclude-all').each(function() {
      if(excluder.checked) {
        $(this).attr('checked', 'checked');
      }
      else {
        $(this).attr('checked', '');
      }
    });
  });
}