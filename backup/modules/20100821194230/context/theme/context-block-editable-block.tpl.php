<div id='context-block-<?php print $block->module ?>-<?php print $block->delta ?>' class='context-block <?php print $class ?>'>
  <?php if (!empty($block->context)): ?>
    <div class='handle'>
      <?php print $tools ?>
      <label><?php print $label ?></label>
    </div>
  <?php endif; ?>
  <?php print theme('block', $block) ?>
</div>
