function vs_webform_update_candidate( $candidate, $selected_candidates )
{
  var $ = jQuery;
  var $t = $candidate;

  var selected_id = 'selected-'+ $t.attr('name').match(/\d+/)[0];
  var label = $.trim($t.parent().text());

  if( $t.filter(':checked').length == 1 )
  {
    $('#vs-webform-no-cand-selected-error').remove();
    $selected_candidate = $('<span id="'+selected_id+'"><a href="#" title="uncheck '+label+'">'+ label +'</a>; </span>');
    $selected_candidate.click(function(){
      $candidate.attr('checked','');
      $(this).remove();
      return false;
    });
    $selected_candidates.children().length == 0
    $selected_candidates.append($selected_candidate);
  }
  else
    $selected_candidates.children('#'+selected_id).trigger('click');

}

function vs_webform_candidate_wrapper( $candidate_wrapper )
{
  var $ = jQuery;
  if( $candidate_wrapper.length < 1 ) return;
  $selected_candidates = $('<div class="vs-webform-selected-candidates" />');
  $selected_candidates_wrapper = $('<div class="form-item">');

  $selected_candidates_wrapper.append('<label>Selected Officials:</label>');
  $selected_candidates_wrapper.append($selected_candidates);

  $candidate_wrapper.after($selected_candidates_wrapper);
  $('input[type=checkbox]').change(function(){
    vs_webform_update_candidate($(this),$selected_candidates);
  }).each(function(){
    vs_webform_update_candidate($(this),$selected_candidates);
  });
}

jQuery(function(){
  var $ = jQuery;
  $components = $('.webform-component-vs-webform');

  $components.each(function(){
    var $component = $(this);
    var $form = $component.parents('form:first');
    var $zip = $component.find('input[name~=zip]');
    var $state = $component.find('select[name~=state]');
    var no_candidates_present = $component.find('.vs-webform-candidate-wrapper').length < 1


    //if the zip field has a value and we don't have any candidates
    //trigger change so we can search for candidates
    if( no_candidates_present )
    {
      if( $zip.length == 1 && $zip.val().match(/\d+/) )
        $zip.trigger('change');
      else if( $state.length == 1 && $state.val().length == 2)
        $state.trigger('change');
    }



    $candidate_wrapper = $component.find('.vs-webform-candidate-wrapper');
    $form.bind('submit',function(){
      $candidate_wrapper = $(this).find('.vs-webform-candidate-wrapper');
      if( $candidate_wrapper.length == 0 ) {
        return TRUE;
      }
      var has_candidate_checked = $candidate_wrapper.find('input:checked').length > 0;
      if ( !has_candidate_checked ) {
        if ( $candidate_wrapper.prev('div.error').length == 0 ) {
        $candidate_wrapper.before('<div id="vs-webform-no-cand-selected-error" class="messages error">Please select at least one official.</div>');
        }
      }
      return has_candidate_checked;
    });

    window.setTimeout(function(){
     vs_webform_candidate_wrapper($candidate_wrapper)
    },500);


    $component.bind('DOMNodeInserted',function(e){
      $new_content = $(e.target);
      $candidate_wrapper = $new_content.find('.vs-webform-candidate-wrapper');
      window.setTimeout(function(){
        vs_webform_candidate_wrapper($candidate_wrapper)
      },500);

    })
  })
});
