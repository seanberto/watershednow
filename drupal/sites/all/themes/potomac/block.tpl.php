<?php
// $Id: block.tpl.php,v 1.1 2009/06/26 13:55:36 kiterminal Exp $
?>

<!-- start block.tpl.php -->
<div id="block-<?php print $block->module .'-'. $block->delta; ?>" class="block block-<?php print $block->module ?> unstyled-block">
  <?php if ($block->subject): ?>
  <h2><?php print $block->subject ?></h2>
  <?php endif;?>

  <div class="content">
    <?php print $block->content ?>
  </div>
</div>
<!-- /end block.tpl.php -->