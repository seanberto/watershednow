// $Id: draggableviews.js,v 1.3.2.10 2010/08/27 09:16:56 sevi Exp $
/**
 * @file draggableviews.js
 *
 * The expand and collapse behaviors.
 */

Drupal.DraggableViews = {};

Drupal.behaviors.draggableviewsLoad = function() {
  $("table.views-table").each( function(i) {
    var table_id = $(this).attr('id');

    if (!Drupal.settings.draggableviews[table_id]) return;

    $(this).find("tr.draggable").each( function(i) {
      $(this).find('.draggableviews-collapse,.draggableviews-expand').remove();
      var nid = $(this).find('td > .hidden_nid').attr('value');
      // Append icon only if we find at least one child.
      if ($("#" + table_id + " tr:has(td > ." + Drupal.settings.draggableviews[table_id].parent + "[value=" + nid + "])").size() > 0) {
        $(this).find('td:first').each( function(i) {
          $(this).append('<div class="draggableviews-expand" href="#"></div>').children('.draggableviews-expand').bind('click', function(){Drupal.DraggableViews.draggableviews_collapse(nid, table_id);});
        });
      }

      // Apply collapsed/expanded state.
      if (Drupal.settings.draggableviews[table_id]) {
        if (Drupal.settings.draggableviews[table_id].states) {
          if (Drupal.settings.draggableviews[table_id].states[nid]) {
            // When list should be collapsed..
            if (Drupal.settings.draggableviews[table_id].states[nid] == 1) {
              // ..collapse list.
              Drupal.DraggableViews.draggableviews_collapse(nid, table_id);

              // ..and set hidden field.
              Drupal.DraggableViews.draggableviews_set_state_field(nid, table_id, true);
            }
          }
        }
      }
    });

    // collapse all by default if set
    if( Drupal.settings.draggableviews[table_id].expand_default && Drupal.settings.draggableviews[table_id].expand_default == 1 ) {
      Drupal.DraggableViews.draggableviews_collapse_all(table_id);
    }
  });
};

// Expand recursively.
Drupal.DraggableViews.draggableviews_expand = function(parent_id, table_id, force) {
  if (force || Drupal.DraggableViews.draggableviews_get_state_field(parent_id, table_id)) {
    // show elements
    Drupal.DraggableViews.draggableviews_show(parent_id, table_id);

    // swap link to collapse link
    $("#" + table_id + " tr:has(td .hidden_nid[value="+parent_id+"])")
    .find('.draggableviews-collapse').each( function (i){
      $(this).unbind('click');
      $(this).attr('class', 'draggableviews-expand');
      $(this).bind('click', function(){ Drupal.DraggableViews.draggableviews_collapse(parent_id, table_id); });
      // set state as value of a hidden field
      Drupal.DraggableViews.draggableviews_set_state_field(parent_id, table_id, false);
    });
  }
};

// show recursively
Drupal.DraggableViews.draggableviews_show = function(parent_id, table_id) {
  $("table[id='" + table_id + "'] tr:has(td ." + Drupal.settings.draggableviews[table_id].parent + "[value="+parent_id+"])").each( function (i) {
    $(this).show();
    var nid = $(this).find('td .hidden_nid').attr('value');
    if (nid) {
      Drupal.DraggableViews.draggableviews_expand(nid, table_id, false);
    }
  });
};

Drupal.DraggableViews.draggableviews_collapse = function(parent_id, table_id) {
  // hide elements
  Drupal.DraggableViews.draggableviews_hide(parent_id, table_id);

  // swap link to expand link
  $("#" + table_id + " tr:has(td .hidden_nid[value=" + parent_id + "])")
  .find('.draggableviews-expand').each( function (i){
    $(this).unbind('click');
    $(this).attr('class', 'draggableviews-collapse');
    $(this).bind('click', function(){ Drupal.DraggableViews.draggableviews_expand(parent_id, table_id, true); });

    // set state as value of a hidden field
    Drupal.DraggableViews.draggableviews_set_state_field(parent_id, table_id, true);
  });
};

// hide recursively
Drupal.DraggableViews.draggableviews_hide = function(parent_id, table_id) {
  $("#" + table_id + " tr:has(td ." + Drupal.settings.draggableviews[table_id].parent + "[value=" + parent_id+"])").each( function (i) {
    $(this).hide();
    var nid = $(this).find('td .hidden_nid').attr('value');
    if (nid) {
      Drupal.DraggableViews.draggableviews_hide(nid, table_id, false);
    }
  });
};

Drupal.DraggableViews.draggableviews_collapse_all = function(table_id) {
  // hide elements
  $("#" + table_id + " tr:has(td ." + Drupal.settings.draggableviews[table_id].parent + "[value!=0])").each( function (i) {
    $(this).hide();
  });
  
  // swap links to expand links
  $("#" + table_id + " tr:has(td .hidden_nid)").each( function (i){
    var parent_id = $(this).find('td .hidden_nid').attr('value');
    
    $(this).find('.draggableviews-expand').each( function (i){
      // set new action and class
      $(this).unbind('click');
      $(this).attr('class', 'draggableviews-collapse');
      $(this).bind('click', function() { Drupal.DraggableViews.draggableviews_expand(parent_id, table_id, true); });
      
      // set collapsed/expanded state
      Drupal.DraggableViews.draggableviews_set_state_field(parent_id, table_id, true);
    });
  });
};

// save state of expanded/collapsed fields in a hidden field
Drupal.DraggableViews.draggableviews_set_state_field = function(parent_id, table_id, state) {
  //build field name
  var field_name = 'draggableviews_collapsed_' + parent_id;
  
  $("table[id='" + table_id + "'] input[name='hidden_nid'][value='" + parent_id + "']")
  .parent().each( function(i) {
    var replaced = false;

    // "check" if field already exists (..by just selecting it)
    $(this).find('input[name="' + field_name + '"]').each( function(i) {
      $(this).attr('value', state ? '1' : '0');
      replaced = true;
    });
    // if no field existed yet -> create it
    if (replaced == false) {
      $(this).append('<input type="hidden" name="' + field_name + '" value="' + (state ? '1' : '0') + '" />');
    }
  });

  Drupal.settings.draggableviews[table_id].states[parent_id] = state;
};

// Get state of expanded/collapsed field.
Drupal.DraggableViews.draggableviews_get_state_field = function(parent_id, table_id) {
  //build field name
  var field_name = 'draggableviews_collapsed_' + parent_id;

  var value = $('table[id="' + table_id + '"] input[name="' + field_name + '"]').attr('value');

  if (value == 1) return false;

  return true;
};