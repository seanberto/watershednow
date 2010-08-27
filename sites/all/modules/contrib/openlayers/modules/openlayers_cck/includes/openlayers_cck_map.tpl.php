<?php
// $Id: openlayers_cck_map.tpl.php,v 1.1.2.2 2010/03/22 23:55:10 zzolo Exp $

/**
 * @file
 * Template file for cck map
 */
?>
<div id="openlayers-cck-map-container-<?php print $map['id'] ?>" class="form-item openlayers-cck-map-container">
<label for="openlayers-cck-map-<?php $map['id'] ?>"><?php print $title ?>:</label>
  <?php print $map['themed'] ?>
  <div class="description openlayers-cck-map-instructions"><?php print $map_instructions ?></div>
  <div class="description openlayers-cck-map-description">
    <?php print $description ?>
  </div>
  <div class="openlayers-cck-actions">
  <a href="#" id="<?php print $map['id'] ?>-wkt-switcher" rel="<?php print $map['id'] ?>"><?php print $show_hide ?></a>
  </div>
</div>
