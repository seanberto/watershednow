<?php
// $Id: draggableviews-view-draggabletable.tpl.php,v 1.6.2.11 2010/08/27 14:08:25 sevi Exp $
/**
 * This file is a modified version of: views-view-table.tpl.php,v 1.8 2009/01/28 00:43:43 merlinofchaos.
 * 
 * @file views-view-table.tpl.php
 * Template to display a view as a table.
 *
 * - $title : The title of this group of rows.  May be empty.
 * - $header: An array of header labels keyed by field id.
 * - $fields: An array of CSS IDs to use for each field id.
 * - $class: A class or classes to apply to the table, based on settings.
 * - $row_classes: An array of classes to apply to each row, indexed by row
 *   number. This matches the index in $rows.
 * - $rows: An array of row items. Each row is an array of content.
 *   $rows are keyed by row number, fields within rows are keyed by field ID.
 *
 * - $tabledrag_tableId: The table id that drupal_add_tabledrag needs
 */
?>
<table class="<?php print $class; ?>" id="<?php print $tabledrag_table_id; ?>">
  <thead>
    <tr>
      <?php foreach ($header as $field => $label): ?>
        <th class="views-field views-field-<?php print $fields[$field]; ?>"<?php if (!empty($style[$field])) print ' style="'. $style[$field] .'"'; ?>>
          <?php print $label; ?>
        </th>
      <?php endforeach ?>
    </tr>
  </thead>
  <tbody>
    <?php foreach ($rows as $count => $row): ?>
      <tr class="draggable <?php print implode(' ', $row_classes[$count]); ?><?php if (!empty($draggableviews_extended[$count])) print ' draggableviews-extended'; ?><?php if (!empty($tabledrag_type[$count])) print ' '. $tabledrag_type[$count]; ?>">
        <?php foreach ($row as $field => $content): ?>
          <td class="views-field views-field-<?php print $fields[$field]; ?>"<?php if (!empty($style[$field])) print ' style="'. $style[$field] .'"'; ?>>
            <?php print $content; ?>
          </td>
        <?php endforeach; ?>
      </tr>
    <?php endforeach; ?>
  </tbody>
</table>
