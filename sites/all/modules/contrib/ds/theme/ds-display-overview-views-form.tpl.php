<?php
// $Id: ds-display-overview-views-form.tpl.php,v 1.1.2.5 2010/06/15 09:07:11 swentel Exp $

/**
 * @file
 *   Template file for the display settings overview form for views fields.
 *
 * @var
 * $build_mode String
 *   Current selected display mode
 * $rows Array of field objects
 *   Fields declared in drupal core and custom fields
 *   Properties (human_name, weight, stickyorder, build_mode, class, label_class)
 */

if ($rows):
?>

<div class="description views-override">Some important notes:
  <ul>
    <li>Label editing is only limited to the format.</li>
    <li>Plugins aren't available, so watch out when positioning your fields.</li>
  </ul>
</div>
<div id="ds-display-content">

  <div id="field-content" class="ds-display">

    <table id="fields" class="sticky-enabled">
      <thead>
        <tr>
          <th><?php print t('Field'); ?></th>
          <th></th>
          <th></th>
          <th><?php print t('Region'); ?></th>
          <th><?php print t('Weight'); ?></th>
        </tr>
      </thead>
      <tbody>

      <!-- Node regions -->
      <?php foreach ($regions as $region => $title): ?>
        <tr class="region region-<?php print $region?> tabledrag-leaf">
          <td colspan="5" class="region">
            <?php print $title; ?>
            <input type="hidden" class="ds-field-id" value="" size="2" id="edit-<?php print $region; ?>-full-field-id" name="region_<?php print $region; ?>[full][field_id]" maxlength="128"/>
            <input type="hidden" class="ds-parent-id" value="" size="2" id="edit-<?php print $region; ?>-full-parent-id" name="region_<?php print $region; ?>[full][parent_id]" maxlength="128"/>
          </td>
        </tr>
        <tr class="tabledrag-leaf region-message region-<?php print $region?>-message <?php print empty($rows[$region]) ? 'region-empty' : 'region-populated'; ?>">
          <td colspan="5">
          <em><?php print t('No fields in this region'); ?></em>
            <input type="hidden" class="ds-field-id" value="" size="2" id="edit-<?php print $region; ?>empty-full-field-id" name="empty<?php print $region; ?>[full][field_id]" maxlength="128"/>
            <input type="hidden" class="ds-parent-id" value="" size="2" id="edit-<?php print $region; ?>empty-full-parent-id" name="empty<?php print $region; ?>[full][parent_id]" maxlength="128"/>
          </td>
        </tr>

        <!-- fields -->
        <?php
        if (!empty($rows[$region])):
          $count = 0;
          foreach ($rows[$region] as $row): ?>
            <tr class="<?php print $count % 2 == 0 ? 'odd' : 'even'; ?> <?php print $row->class ?>">
              <td class="ds-label">
              <?php print $row->{$build_mode}->indentation; ?>
              <span class="<?php print $row->label_class; ?>"><?php print $row->human_name; ?></span></td>
              <td><?php print $row->{$build_mode}->label; ?></td>
              <td><?php print $row->{$build_mode}->class . $row->{$build_mode}->field_id . $row->{$build_mode}->parent_id; ?></td>
              <td><?php print $row->{$build_mode}->region; ?></td>
              <td><?php print $row->ds_weight; ?></td>
            </tr>
            <?php
            $count++;
          endforeach;
        endif;
      endforeach;
      ?>
      </tbody>
    </table>
  </div>
</div>
<?php
endif;
