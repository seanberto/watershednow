// $Id: buildmodes.js,v 1.1.2.2 2010/01/28 14:47:28 swentel Exp $

/**
 * Toggle all buildmodes at once.
 */
Drupal.behaviors.ds_buildmodes = function(context) {

	$('.block-all').bind('click', function() {
    var excluder = this;
    $(excluder).parents('tr').find('.checkbox-instance').not('.block-all').each(function() {
      if(excluder.checked) {
        $(this).attr('disabled', 'disabled');
      }
      else {
        $(this).attr('disabled', '');
      }
    });
  });
}