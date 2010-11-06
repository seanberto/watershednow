<div id="block-<?php print $block->module .'-'. $block->delta ?>" class="<?php print $block_classes . ' ' . $block_zebra; ?>">
  <div class="block-inner">
    <?php if (!empty($block->subject)): ?>
      <?php if(($block->region == 'content_top') || ($block->region == 'content_bottom')): ?>
        <h1 class="title block-title"><?php print $block->subject; ?></h1>
      <?php else: ?>
        <h3 class="title block-title"><?php print $block->subject; ?></h3>
      <?php endif; ?>
    <?php endif; ?>

    <div class="content">
      <?php print $block->content; ?>
    </div>

    <?php print $edit_links; ?>

  </div> <!-- /block-inner -->
</div> <!-- /block -->