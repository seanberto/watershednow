// $Id: ds.js,v 1.1.2.27 2010/07/03 09:31:48 swentel Exp $

Drupal.DisplaySuite = Drupal.DisplaySuite || {};
Drupal.DisplaySuite.fieldopened = '';

/**
 * Move a field in the fields table from one region to another via select list.
 *
 * This behavior is dependent on the tableDrag behavior, since it uses the
 * objects initialized in that behavior to update the row.
 *
 * Based on nodeform cols.
 */
Drupal.behaviors.fieldDrag = function(context) {
  var table = $('table#fields');
  var tableDrag = Drupal.tableDrag.fields; // Get the fields tableDrag object.

  // Add a handler for when a row is swapped, update empty regions.
  tableDrag.row.prototype.onSwap = function(swappedRow) {
    checkEmptyRegions(table, this);
  };

  // A custom message for the fields page specifically.
  Drupal.theme.tableDragChangedWarning = function () {
    return '<div class="warning">' + Drupal.theme('tableDragChangedMarker') + ' ' + Drupal.t("The changes to these fields will not be saved until the <em>Save</em> button is clicked.") + '</div>';
  };

  // Add a handler so when a row is dropped, update fields dropped into new regions.
  tableDrag.onDrop = function() {
    dragObject = this;

    var regionRow = $(dragObject.rowObject.element).prevAll('tr.region').get(0);
    var regionName = regionRow.className.replace(/([^ ]+[ ]+)*region-([^ ]+)([ ]+[^ ]+)*/, '$2');
    var regionField = $('select.field-region-select', dragObject.rowObject.element);
    var weightField = $('.field-weight', dragObject.rowObject.element);
    var parent_id = $('.ds-parent-id', dragObject.rowObject.element).val();
    var field_id = $('.ds-field-id', dragObject.rowObject.element).val();
    var oldRegionName = weightField[0].className.replace(/([^ ]+[ ]+)*field-weight-([^ ]+)([ ]+[^ ]+)*/, '$2');

    if (!regionField.is('.field-region-'+ regionName)) {
      regionField.removeClass('field-region-' + oldRegionName).addClass('field-region-' + regionName);
      weightField.removeClass('field-weight-' + oldRegionName).addClass('field-weight-' + regionName);
      regionField.val(regionName);
      checkRegionSelect(regionField, parent_id);

      // Leafs.
      $(dragObject.rowObject.element).nextAll('.tabledrag-leaf').each(function() {
        if ($(this).find('.ds-parent-id').val() == field_id) {
          var regionField = $('select.field-region-select', this);
          regionField.val(regionName);
          regionField.addClass('ds-hidden');
        }
        else {
          return false;
        }
      });
    }
    else {
      checkRegionSelect(regionField, parent_id);
    }
  
    // Manage classes to make it look disabled
    if(regionName == 'disabled') {
      $(dragObject.rowObject.element).addClass('region-css-disabled');
    }
    else {
      $(dragObject.rowObject.element).removeClass('region-css-disabled');
    }
  };

  // Add the behavior to each region select list.
  $('select.field-region-select:not(.fieldregionselect-processed)', context).each(function() {
    $(this).change(function(event) {
      // Make our new row and select field.
      var row = $(this).parents('tr:first');
      var select = $(this);
      tableDrag.rowObject = new tableDrag.row(row);
      
      // Manage classes to make it look disabled
      if(select[0].value == 'disabled') {
        $(row).addClass('region-css-disabled');
      }
      else {
        $(row).removeClass('region-css-disabled');
      }
      
      // Find the correct region and insert the row as the first in the region.
      $('tr.region-message', table).each(function() {
        if ($(this).is('.region-' + select[0].value + '-message')) {
          // Add the new row and remove the old one.
          $(this).after(row);
          // Manually update weights and restripe.
          tableDrag.updateFields(row.get(0));
          tableDrag.rowObject.changed = true;
          if (tableDrag.oldRowElement) {
            $(tableDrag.oldRowElement).removeClass('drag-previous');
          }
          tableDrag.oldRowElement = row.get(0);
          tableDrag.restripeTable();
          tableDrag.rowObject.markChanged();
          tableDrag.oldRowElement = row;
          $(row).addClass('drag-previous');
        }
      });
      
      var field_id = row.find('.ds-field-id').val();
      $('.tabledrag-leaf').each(function() {
        if ($(this).find('.ds-parent-id').val() == field_id) {
          var regionField= $(this).find('.field-region-select');
          $(row).after($(this));
          regionField.val(select[0].value);
        }
      });      
      
      // Modify empty regions with added or removed fields.
      checkEmptyRegions(table, row);
      // Remove focus from selectbox.
      select.get(0).blur();
    });
    $(this).addClass('fieldregionselect-processed');
  });

  // Check if region select must be hidden or not.
  var checkRegionSelect = function(regionField, parent_id) {
    if (parent_id == '') {
      regionField.removeClass('ds-hidden');
    }
    else {
      regionField.addClass('ds-hidden');
    }	  
  };
  
  // Check the emptyness of regions.
  var checkEmptyRegions = function(table, rowObject) {

	$('tr.region-message', table).each(function() {

      // This region has become empty
      if (($(this).next('tr').is(':not(.draggable)') || $(this).next('tr').size() == 0) && $(this).prev('tr').is(':not(.draggable)')) {
        $(this).removeClass('region-populated').addClass('region-empty');
      }
      // This region has become populated.
      else if ($(this).is('.region-empty')) {
        $(this).removeClass('region-empty').addClass('region-populated');
      }
    });

    var regionRow = $(rowObject.element).prevAll('tr.region').get(0);
    var regionName = regionRow.className.replace(/([^ ]+[ ]+)*region-([^ ]+)([ ]+[^ ]+)m*/, '$2');
    $('.region-' + regionName + '-message').addClass('region-populated');	  
  };
};

/**
 * Change label format on fieldgroups.
 */
Drupal.behaviors.fieldgroupFormat = function(context) {
  $('select.fieldgroup-format').each(function() {
    $(this).change(function(event) {
      var field_group_value = $(this).val();
  	  var label_format = $(this).attr('id').replace('format', 'label-format');
      if (field_group_value.substr(0, 17) == 'ds_group_fieldset' || field_group_value.substr(0, 7) == 'ds_tabs') {
        $('#'+ label_format +'-wrapper').addClass('ds-hidden');
      }
      else {
        $('#'+ label_format +'-wrapper').removeClass('ds-hidden');
      }
    });
  });
};

/**
 * Show / hide fields or plugins content.
 */
Drupal.DisplaySuite.toggleDisplayTab = function(element) {
  $('#ds-tabs .tab').each(function() {
    var tab_id = $(this).attr('id');
    var content_id = tab_id.replace('-tab', '-content');
	if (tab_id == element) {
	  // Tabs.
      $(this).addClass('selected');
      $(this).removeClass('not-selected');
      // Content.
      $('#'+ content_id).show();
    }
    else {
      // Tabs.
      $(this).addClass('not-selected');
      $(this).removeClass('selected');
      // Content.
      $('#'+ content_id).hide();
    }
  });	
}

/**
 * Show / hide settings for fields.
 */
Drupal.behaviors.settingsToggle = function(context) {
  // remove click from link
  $('.settings-tab-toggle').click(function(e){
    e.preventDefault();
  });
  
  // Add click event to entire td
  $('.settings-tab-toggle').click(function(){
    var settings = $(this).siblings('.settings-tab');
    if (Drupal.DisplaySuite.fieldopened != '' && Drupal.DisplaySuite.fieldopened != settings.attr('id')) {
      $('#' + Drupal.DisplaySuite.fieldopened).hide();
    }

    if (settings.is(':visible')) {
      settings.hide();
    }
    else {
      settings.slideDown('normal');
    }
    // Store the opened setting.
    Drupal.DisplaySuite.fieldopened = settings.attr('id');
  });
}

/**
 * Change the info about the label format.
 */
Drupal.behaviors.labelChange = function(context) {
  $('.ds-label-change').change(function(){
    var label_info = $(this).val();
    $(this).parents('td').find('.label-info').text('Label: '+ label_info);
  });
}

/**
 * Change the info about the field format.
 */
Drupal.behaviors.formatChange = function(context) {
  $('.ds-format-change, .fieldgroup-format').change(function(){
	var options = new Array();
    $('#'+ $(this).attr('id') +' option:selected').each(function(i, selected) {
       options[i] = $(selected).text();    	
    });
    $(this).parents('td').find('.format-info').text('Format: '+ options.join(', '));
  });
}

/**
 * Change the info about the styles for fields or regions.
 */
Drupal.behaviors.StyleChange = function(context) {
  $('.ds-style-change').change(function(){
    var options = new Array();
    $('#'+ $(this).attr('id') +' option:selected').each(function(i, selected) {
      options[i] = $(selected).text();
    });
    if (options != '') {
    	if ($(this).attr('id').substr(0, 18) == 'edit-region-styles') {
    	  var separator = ''
    	}
    	else {
    	  var separator = ' - ';
    	}
        var info = separator + 'Styles: '+ options.join(', ');
      }
      else {
        var info = '';
      }
    $(this).parents('td').find('.style-info').text(info);
  });
}

