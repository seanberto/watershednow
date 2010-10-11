<?php
// $Id: imagefield-extended-image.tpl.php,v 1.1.2.2.2.1 2010/03/03 02:12:37 aland Exp $

/**
 * This template is used to print a single FileField file using
 * the formatters supplied with imagefield_extended.
 *
 * Variables available:
 * - $image: The rendered $image
 *
 * - $fields: An array of rendered additional fields, indexed by the key as
 *     generated from theme_imagefield_extended_formatter_ife().
 *     By default these are limited to textfields with the field label as the
 *     field titles. This can be changed by overriding the function
 *     theme_imagefield_extended_formatter_ife().
 * - $values: The raw processed values.
 *     Text fields are escaped text, checkbox fields are booleans (TRUE/FALSE).
 * - $output: The processed output that will normally be used.
 *
 */
  $wrapper_class = 'imagefield-wrapper imagefield-'. $field_name .'-wrapper';
?>
<div class="<?php print $wrapper_class ?>">
  <?php print $image ?>
  <?php foreach ($fields as $field) : ?>
    <?php print $field ?>
  <?php endforeach; ?>
</div>
