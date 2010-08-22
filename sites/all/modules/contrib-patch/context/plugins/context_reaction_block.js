// $Id: context_reaction_block.js,v 1.1.2.16 2010/04/26 14:45:02 yhahn Exp $

Drupal.behaviors.contextReactionBlock = function(context) {
  $('form.context-editor:not(.context-block-processed)')
    .addClass('context-block-processed')
    .each(function() {
      $(this).bind('init.pageEditor', function(event) {
        Drupal.contextBlockEditor = new DrupalContextBlockEditor($(this));
      });
      $(this).bind('start.pageEditor', function(event, context) {
        // Fallback to first context if param is empty.
        if (!context) {
          context = $(this).data('defaultContext');
        }
        Drupal.contextBlockEditor.editStart($(this), context);
      });
      $(this).bind('end.pageEditor', function(event) {
        Drupal.contextBlockEditor.editFinish();
      });
    });

  //
  // Editor ===========================================================
  //
  // Attach handlers to editable blocks.
  // This lives outside the block editor class as it may needs to be
  // called each time Drupal.attachBehaviors() is called.
  $('div.context-block:not(.processed)').each(function() {
    $('a.remove', $(this)).click(function() {
      $(this).parents('div.context-block').remove();
      Drupal.contextBlockEditor.updateBlocks();
      return false;
    });
  });

  //
  // Admin Form =======================================================
  //
  // ContextBlockForm: Init.
  $('#context-blockform:not(.processed)').each(function() {
    $(this).addClass('processed');
    Drupal.contextBlockForm = new DrupalContextBlockForm($(this));
    Drupal.contextBlockForm.setState();
  });

  // ContextBlockForm: Attach block removal handlers.
  // Lives in behaviors as it may be required for attachment to new DOM elements.
  $('#context-blockform a.remove:not(.processed)').each(function() {
    $(this).addClass('processed');
    $(this).click(function() {
      $(this).parents('tr').eq(0).remove();
      Drupal.contextBlockForm.setState();
      return false;
    });
  });
};

/**
 * Context block form. Default form for editing context block reactions.
 */
function DrupalContextBlockForm(blockForm) {
  this.state = {};

  this.setState = function() {
    $('table.context-blockform-region', blockForm).each(function() {
      var region = $(this).attr('id').split('context-blockform-region-')[1];
      var blocks = [];
      $('tr', $(this)).each(function() {
        var bid = $(this).attr('id');
        blocks.push(bid);
      });
      Drupal.contextBlockForm.state[region] = blocks;
    });

    // Serialize here and set form element value.
    $('form input.context-blockform-state').val(JSON.stringify(this.state));

    // Hide enabled blocks from selector that are used
    $('table.context-blockform-region tr').each(function() {
      var bid = $(this).attr('id');
      $('div.context-blockform-selector input[value='+bid+']').parents('div.form-item').eq(0).hide();
    });
    // Show blocks in selector that are unused
    $('div.context-blockform-selector input').each(function() {
      var bid = $(this).val();
      if ($('table.context-blockform-region tr#'+bid).size() === 0) {
        $(this).parents('div.form-item').eq(0).show();
      }
    });
  };

  // Tabledrag
  // Add additional handlers to update our blocks.
  for (var base in Drupal.settings.tableDrag) {
    var table = $('#' + base + ':not(.processed)', blockForm);
    if (table && table.is('.context-blockform-region')) {
      table.addClass('processed');
      table.bind('mouseup', function(event) {
        Drupal.contextBlockForm.setState();
        return;
      });
    }
  }

  // Add blocks to a region
  $('td.blocks a', blockForm).each(function() {
    $(this).click(function() {
      var region = $(this).attr('href').split('#')[1];
      var selected = $("div.context-blockform-selector input:checked");
      if (selected.size() > 0) {
        selected.each(function() {
          // create new block markup
          var block = document.createElement('tr');
          var text = $(this).parents('div.form-item').eq(0).hide().children('label').text();
          $(block).attr('id', $(this).attr('value')).addClass('draggable');
          $(block).html("<td>"+ text + "<input class='block-weight' /></td><td><a href='' class='remove'>X</a></td>");

          // add block item to region
          var base = "context-blockform-region-"+ region;
          Drupal.tableDrag[base].makeDraggable(block);
          $('table#'+base).append(block);
          Drupal.attachBehaviors($('table#'+base));

          Drupal.contextBlockForm.setState();
          $(this).removeAttr('checked');
        });
      }
      return false;
    });
  });
}

/**
 * Context block editor. AHAH editor for live block reaction editing.
 */
function DrupalContextBlockEditor(editor) {
  this.state = {};

  /**
   * Update UI to match the current block states.
   */
  this.updateBlocks = function() {
    var browser = $('div.context-block-browser');

    // For all enabled blocks, mark corresponding addables as having been added.
    $('div.block, div.admin-block').each(function() {
      var bid = $(this).attr('id').split('block-')[1]; // Ugh.
      $('#context-block-addable-'+bid, browser).draggable('disable').addClass('context-block-added').removeClass('context-block-addable');
    });
    // For all hidden addables with no corresponding blocks, mark as addable.
    $('.context-block-item', browser).each(function() {
      var bid = $(this).attr('id').split('context-block-addable-')[1];
      if ($('#block-'+bid).size() === 0) {
        $(this).draggable('enable').removeClass('context-block-added').addClass('context-block-addable');
      }
    });

    // Clean up after jQuery UI. Sometimes addables get left -- not good.
    $('.context-block-item.ui-sortable-helper').remove();

    // Mark empty regions.
    $('.context-block-region').each(function() {
      if ($('div.context-block', this).size() > 0) {
        $(this).removeClass('context-block-region-empty');
      }
      else {
        $(this).addClass('context-block-region-empty');
      }
    });

    // Mark any blocks that have forms as draggable by handle only.
    $('.context-block-region > div.context-block:has(form)').addClass('context-block-handleonly');
  };

  /**
   * Remove script elements while dragging & dropping.
   */
  this.scriptFix = function(event, ui, editor, context) {
    if ($('script', ui.item)) {
      var placeholder = $(Drupal.settings.contextBlockEditor.scriptPlaceholder);
      var label = $('div.handle label', ui.item).text();
      placeholder.children('strong').html(label);
      $('script', ui.item).parent().empty().append(placeholder);
    }
  };

  /**
   * Add a block to a region through an AHAH load of the block contents.
   */
  this.addBlock = function(event, ui, editor, context) {
    // Remove empty regionism early.
    editor.removeClass('context-block-region-empty');

    if (ui.item.is('.context-block-addable')) {
      var bid = ui.item.attr('id').split('context-block-addable-')[1];
      var params = {
        'path': Drupal.settings.contextBlockEditor.path,
        'bid': bid,
        'context': context
      };
      $.getJSON(Drupal.settings.contextBlockEditor.ajax, params, function(data) {
        if (data.status) {
          var newBlock = $(data.block);
          newBlock.addClass('draggable');
          if ($('script', newBlock)) {
            $('script', newBlock).remove();
          }
          
          newBlock = ui.item.replaceWith(newBlock);

          $.each(data.css, function(k, v){
            var cssfile = Drupal.settings.basePath + v;
            if ($('head link[href $='+cssfile+']').length === 0 ) {
              $('head').append('<link type="text/css" rel="stylesheet" media="all" href="' + cssfile + " />'");
            }
          });

          Drupal.contextBlockEditor.updateBlocks();
          Drupal.attachBehaviors();
        }
        else {
          ui.item.remove();
        }
      });
    }
    else if (ui.item.is('.context-block')) {
      Drupal.contextBlockEditor.updateBlocks();
    }
  };

  /**
   * Update form hidden field with JSON representation of current block visibility states.
   */
  this.setState = function() {
    $('div.context-block-region').each(function() {
      var region = $(this).attr('id').split('context-block-region-')[1];
      var blocks = [];
      $('div.context-block', $(this)).each(function() {
        if ($(this).attr('class').indexOf('edit-') != -1) {
          var bid = $(this).attr('id').split('context-block-')[1];
          var context = $(this).attr('class').split('edit-')[1].split(' ')[0];
          context = context ? context : 0;
          var block = {'bid': bid, 'context': context};
          blocks.push(block);
        }
      });
      Drupal.contextBlockEditor.state[region] = blocks;
    });

    // Serialize here and set form element value.
    $('form.context-editor input.context-block-editor-state').val(JSON.stringify(this.state));
  };

  /**
   * Disable text selection.
   */
  this.disableTextSelect = function() {
    if ($.browser.safari) {
      $('div.context-block:not(:has(input,textarea))').css('WebkitUserSelect','none');
    }
    else if ($.browser.mozilla) {
      $('div.context-block:not(:has(input,textarea))').css('MozUserSelect','none');
    }
    else if ($.browser.msie) {
      $('div.context-block:not(:has(input,textarea))').bind('selectstart.contextBlockEditor', function() { return false; });
    }
    else {
      $(this).bind('mousedown.contextBlockEditor', function() { return false; });
    }
  };

  /**
   * Enable text selection.
   */
  this.enableTextSelect = function() {
    if ($.browser.safari) {
      $('*').css('WebkitUserSelect','');
    }
    else if ($.browser.mozilla) {
      $('*').css('MozUserSelect','');
    }
    else if ($.browser.msie) {
      $('*').unbind('selectstart.contextBlockEditor');
    }
    else {
      $(this).unbind('mousedown.contextBlockEditor');
    }
  };

  /**
   * Start editing. Attach handlers, begin draggable/sortables.
   */
  this.editStart = function(editor, context) {
    // This is redundant to the start handler found in context_ui.js.
    // However it's necessary that we trigger this class addition before
    // we call .sortable() as the empty regions need to be visible.
    $(document.body).addClass('context-editing');

    this.disableTextSelect();

    $('div.context-block-region > div.edit-'+context).addClass('draggable');

    // First pass, enable sortables on all regions.
    var params = {
      revert: true,
      dropOnEmpty: true,
      placeholder: 'draggable-placeholder',
      forcePlaceholderSize: true,
      start: function(event, ui) { Drupal.contextBlockEditor.scriptFix(event, ui, editor, context); },
      stop: function(event, ui) { Drupal.contextBlockEditor.addBlock(event, ui, editor, context); },
      items: '> div.editable',
      handle: 'div.handle'
    };
    $('div.context-block-region').sortable(params);

    // Second pass, hook up all regions via connectWith to each other.
    $('div.context-block-region').each(function() {
      $(this).sortable('option', 'connectWith', ['div.context-block-region']);
    });

    // Terrible, terrible workaround for parentoffset issue in Safari.
    // The proper fix for this issue has been committed to jQuery UI, but was
    // not included in the 1.6 release. Therefore, we do a browser agent hack
    // to ensure that Safari users are covered by the offset fix found here:
    // http://dev.jqueryui.com/changeset/2073.
    if ($.ui.version === '1.6' && $.browser.safari) {
      $.browser.mozilla = true;
    }
  };

  /**
   * Finish editing. Remove handlers.
   */
  this.editFinish = function() {
    this.enableTextSelect();

    $('div.context-block-region > div.draggable').removeClass('draggable');
    $('div.context-block-region').sortable('destroy');
    this.setState();

    // Unhack the user agent.
    if ($.ui.version === '1.6' && $.browser.safari) {
      $.browser.mozilla = false;
    }
  };

  // Category selector handler.
  // Also set to "Choose a category" option as browsers can retain
  // form values from previous page load.
  $('select.context-block-browser-categories', editor).val(0).change(function() {
    var category = $(this).val();
    $('div.category').hide();
    $('div.category-'+category).show();
  });

  // Add draggable handler for addables.
  var options = {
    appendTo: 'body',
    helper: 'clone',
    zIndex: '2700',
    connectToSortable: ($.ui.version === '1.6') ? ['div.context-block-region'] : 'div.context-block-region',
    start: function(event, ui) { $(document.body).addClass('context-block-adding'); },
    stop: function(event, ui) { $(document.body).removeClass('context-block-adding'); }
  };
  $('div.context-block-addable', editor).draggable(options);

  // Set the block states.
  this.updateBlocks();
}
