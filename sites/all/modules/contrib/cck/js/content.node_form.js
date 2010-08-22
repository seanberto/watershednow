// $Id: content.node_form.js,v 1.1.2.1 2009/06/06 23:44:56 markuspetrux Exp $

/**
 * Private namespace for local methods.
 */
Drupal.contentRemoveButtons = Drupal.contentRemoveButtons || {};

/**
 * Manipulation of content remove buttons.
 *
 * TableDrag objects for multiple value fields (and multigroups) are scanned
 * to find 'remove' checkboxes. These checkboxes are hidden when javascript is
 * enabled (using the Global CSS Killswitch, html.js, defined in drupal.js).
 * A new 'remove' button is created here in place of these checkboxes aimed to
 * provide a more user-friendly method to remove items.
 */
Drupal.behaviors.contentRemoveButtons = function(context) {
  var self = Drupal.contentRemoveButtons;

  $('table.content-multiple-table', context).not('.content-remove-buttons-processed').addClass('content-remove-buttons-processed').each(function() {
    var table = this, tableDrag = Drupal.tableDrag[$(table).attr('id')];

    // Replace remove checkboxes with buttons.
    $('input.content-multiple-remove-checkbox', table).each(function() {
      var $checkbox = $(this), $row = $checkbox.parents('tr:first');
      var isRemoved = $checkbox.attr('checked');
      var $button = $(Drupal.theme('contentRemoveButton', tableDrag.getRemoveButtonTitle(isRemoved)));

      // Bind the onClick event to the remove button.
      $button.bind('click', function(event) {
        self.onClick($button, $checkbox, $row, tableDrag);
        return false;
      });

      // Attach the new button to the DOM tree.
      $checkbox.parent().append($button);

      // If the row is removed, then hide the contents of the cells and show
      // the removed warning on the cell next to the drag'n'drop cell.
      if (isRemoved) {
        self.getCellWrappers($row).hide();
        self.showRemovedWarning($row, tableDrag);

        // FAPI not rendering the form on errors - case #1:
        // If the form has been submitted and any error was found, FAPI will
        // send back the same exact form that was submitted to show the error
        // messages, but it will not invoke the rendering engine which is where
        // we actually assign the removed class to the row, so we need to check
        // this situation here and add the class if it is not present.
        if (!$row.hasClass('content-multiple-removed-row')) {
          $row.addClass('content-multiple-removed-row');
        }
      }
      else {
        // FAPI not rendering the form on errors - case #2:
        // Similar issue than #1, but this time caused when user removes an
        // item, previews, FAPI renders the new form with the removed class,
        // then user changes anything in the form that causes an error, and
        // also restores the previously removed item. This time, FAPI will
        // send the form validation error with the item not flagged for removal
        // but having the removed class that was present when the form was
        // rendered in the previous step. So we need to remove this class here,
        // if present, because the item is not really flagged for removal.
        if ($row.hasClass('content-multiple-removed-row')) {
          $row.removeClass('content-multiple-removed-row');
        }
      }
    });
  });
};

/**
 * onClick handler for remove buttons.
 *
 * @param $button
 *   The jQuery object of the remove button.
 * @param $checkbox
 *   The jQuery object of the remove checkbox.
 * @param $row
 *   The jQuery object of the table row.
 * @param tableDrag
 *   The tableDrag object where the row is.
 */
Drupal.contentRemoveButtons.onClick = function($button, $checkbox, $row, tableDrag) {
  var self = Drupal.contentRemoveButtons;

  // Prevent the user from firing this event while another one is still being
  // processed. This flag is (should be) restored at end of animations.
  // Note that this technique is required because the browser may experience
  // delays while performing the animation, for whatever reason, and if this
  // process it fired more than once at the same time for the same row, then
  // it may cause unexpected behavior because the state of the elements being
  // manipulated would be unknown.
  if ($row.animating) {
    return;
  }
  $row.animating = true;

  // Toggle the state of the checkbox.
  var isRemoved = !$checkbox.attr('checked');
  $checkbox.attr('checked', isRemoved);

  // Toggle the row class.
  if (isRemoved) {
    $row.addClass('content-multiple-removed-row');
  }
  else {
    $row.removeClass('content-multiple-removed-row');
  }

  // Toggle the button title.
  $button.attr('title', tableDrag.getRemoveButtonTitle(isRemoved));

  // Get the list of cell wrappers in this row.
  var $cellWrappers = self.getCellWrappers($row);

  // If for whatever reason this row doesn't have cells with elements,
  // then we are done, but we still need to reset the global busy flag
  // and display the tableDrag changed warning.
  if (!$cellWrappers.size()) {
    tableDrag.displayChangedWarning();
    $row.animating = false;
    return;
  }

  // Toggle the visible state of the row cells.
  $cellWrappers.each(function() {
    var $cellWrapper = $(this);

    // Drop the removed warning during restore operation.
    if (!isRemoved) {
      self.hideRemovedWarning($row);
    }

    // Toggle the visibility state of the contents of cells.
    $cellWrapper.animate({opacity: (isRemoved ? 'hide' : 'show')}, 'fast', function() {
      var $cell = $cellWrapper.parent();

      // Show the removed warning during remove operation.
      if (isRemoved && $cell.prev(':first').hasClass('content-multiple-drag')) {
        self.showRemovedWarning($row, tableDrag);
      }

      // Disable the busy flag when animation of last cell has finished.
      if ($cell.next(':first').hasClass('delta-order')) {
        tableDrag.displayChangedWarning();
        $row.animating = false;
      }
    });
  });
};

/**
 * Show the removed warning on the given row.
 *
 * @param $row
 *   The jQuery object of the table row.
 * @param tableDrag
 *   The tableDrag object where the row is.
 */
Drupal.contentRemoveButtons.showRemovedWarning = function($row, tableDrag) {
  $('.content-multiple-drag', $row).next(':first').append(Drupal.theme('contentRemovedWarning', tableDrag.getRemovedWarning()));
};

/**
 * Hide the removed warning from the given row.
 *
 * @param $row
 *   The jQuery object of the table row.
 */
Drupal.contentRemoveButtons.hideRemovedWarning = function($row) {
  if ($('.content-multiple-removed-warning', $row).size()) {
    $('.content-multiple-removed-warning', $row).remove();
  }
};

/**
 * Get cell wrappers for the given row.
 *
 * @param $row
 *   The jQuery object of the table row.
 */
Drupal.contentRemoveButtons.getCellWrappers = function($row) {
  // Create cell wrappers if this row has not already been processed.
  if (!$('.content-multiple-cell-content-wrapper', $row).size()) {
    // Wrap the contents of all cells (except the drag'n'drop, weight and
    // remove button cells) with a dummy block element. This operation makes
    // animations faster because we just need to show/hide a single element
    // per cell, and it also prevents from creating more than one warning
    // element per row.
    $row.children('td:not(.content-multiple-drag):not(.delta-order):not(.content-multiple-remove-cell)').each(function() {
      var $cell = $(this);
      $cell.wrapInner('<div class="content-multiple-cell-content-wrapper"/>');
    });
  }
  return $('.content-multiple-cell-content-wrapper', $row);
};

/**
 * Display table change warning when appropriate.
 */
Drupal.tableDrag.prototype.displayChangedWarning = function() {
  if (this.changed == false) {
    $(Drupal.theme('tableDragChangedWarning')).insertAfter(this.table).hide().fadeIn('slow');
    this.changed = true;
  }
};

/**
 * Get the title of the remove button.
 *
 * This method is an extension of the tableDrag class. This means a separate
 * module can override this method for a particular tableDrag instance. For
 * example, the multigroup module can change the text to read 'Remove this
 * group of items', another module could change it to 'Remove this image',
 * and so on...
 * To override this function:
 *
 * @code
 *  var tableId = $(table).attr('id');
 *  Drupal.tableDrag[tableId].getRemoveButtonTitle = function(isRemoved) {
 *    return (isRemoved ? Drupal.t('Restore this foo') : Drupal.t('Remove this foo'));
 *  };
 * @endcode
 *
 * @param isRemoved
 *   A flag that indicates the state of the button.
 */
Drupal.tableDrag.prototype.getRemoveButtonTitle = function(isRemoved) {
  return (isRemoved ? Drupal.t('Restore this item') : Drupal.t('Remove this item'));
};

/**
 * Get the item removed warning.
 *
 * This method is an extension of the tableDrag class. It can be overridden by
 * a separate module. See getRemoveButtonTitle() for further information.
 */
Drupal.tableDrag.prototype.getRemovedWarning = function() {
  return Drupal.t('Removed');
};

/**
 * Theme the remove button.
 */
Drupal.theme.prototype.contentRemoveButton = function(title) {
  return '<a href="javascript:void(0)" class="content-multiple-remove-button" title="'+ title +'"></a>';
};

/**
 * Theme the item removed warning.
 */
Drupal.theme.prototype.contentRemovedWarning = function(warning) {
  return '<div class="content-multiple-removed-warning">'+ warning +'</div>';
};
