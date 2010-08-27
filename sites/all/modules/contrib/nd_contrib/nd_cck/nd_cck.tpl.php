<?php
// $Id: nd_cck.tpl.php,v 1.1.2.3 2010/06/07 10:43:12 swentel Exp $

/**
 * @file
 * Optimized version for content field for ND_CCK
 *
 * Available variables:
 * - $node: The node object.
 * - $field: The field array.
 * - $items: An array of values for each item in the field array.
 * - $teaser: Whether this is displayed as a teaser.
 * - $page: Whether this is displayed as a page.
 * - $field_name: The field name.
 * - $field_type: The field type.
 * - $field_name_css: The css-compatible field name.
 * - $field_type_css: The css-compatible field type.
 * - $label: The item label.
 * - $label_display: Position of label display, inline, above, or hidden.
 * - $field_empty: Whether the field has any valid value.
 *
 * Each $item in $items contains:
 * - 'view' - the themed view for that item
 *
 * @see template_preprocess_field()
 */
?>
<?php if (!$field_empty) : ?>
<div class="field <?php print $field_name_css ?>">
  <?php if ($label_display == 'above') : ?>
    <div class="field-label"><?php print t($label) ?>:&nbsp;</div>
  <?php endif;?>
  <?php if (count($items) > 1): ?>
    <div class="field-items">
      <?php $count = 1;
      foreach ($items as $delta => $item) :
        if (!$item['empty']) : ?>
          <div class="field-item <?php print($count % 2 ? 'odd' : 'even') ?>">
            <?php if ($label_display == 'inline') : ?><div class="field-label-inline<?php print($delta ? '' : '-first')?>"><?php print t($label) ?>:&nbsp;</div><?php endif; ?><?php print $item['view'] ?>
          </div>
        <?php $count++;
        endif;
      endforeach;?>
    </div>
  <?php else : ?>
    <?php if ($label_display == 'inline') : ?><div class="field-label-inline-first"><?php print t($label) ?>:&nbsp;</div><?php endif; ?><?php print $items[0]['view'];?>
  <?php endif; ?>
</div>
<?php endif; ?>