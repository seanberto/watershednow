<?php
// $Id: nd.tpl.php,v 1.1.2.3 2010/03/15 19:19:48 swentel Exp $

/**
 * @file
 * This template is optimized for use with the Node displays module.
 *
 * Differences with the default node.tpl.php
 *   - Uses only the $content variable.
 *   - Extra check on $sticky because this node might be build with another build mode.
 */
?>

<div class="buildmode-<?php print $node->build_mode; ?>">
  <div class="node node-type-<?php print $node->type; ?> <?php if (isset($node_classes)): print $node_classes; endif; ?><?php if ($sticky && $node->build_mode == 'sticky'): print ' sticky'; endif; ?><?php if (!$status): print ' node-unpublished'; endif; ?> clear-block">
    <?php print $content; ?>
  </div> <!-- /node -->
</div> <!-- /buildmode -->
