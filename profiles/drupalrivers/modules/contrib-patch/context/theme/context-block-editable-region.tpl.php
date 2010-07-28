<?php if ($editable && (!empty($blocks) || $show_always)): ?>
  <div class='context-block-region <?php print $class ?>' id='context-block-region-<?php print $region ?>'>
    <div class='target'><?php print $region_description ?></div>
    <?php foreach ($blocks as $block): ?>
      <?php print theme('context_block_editable_block', $block); ?>
    <?php endforeach; ?>
  </div>
<?php else: ?>
<?php /* When themes check to see if there is any content is the region any
         whitespace will make the them think it's got content. Consequently
         we don't nest this following code. */ ?>
<?php foreach ($blocks as $block): ?>
  <?php print theme('block', $block); ?>
<?php endforeach; ?>
<?php endif; ?>
