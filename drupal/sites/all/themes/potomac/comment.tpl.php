<?php
// $Id: comment.tpl.php,v 1.1 2009/06/26 13:55:36 kiterminal Exp $
?>

<!-- start comment.tpl.php -->
<div class="comment<?php print ($comment->new) ? ' comment-new' : ''; print ' '. $status ?>">
  <?php if ($comment->new): ?>
    <span class="new"><?php print $new ?></span>
  <?php endif; ?>
  <h3 class="title"><?php print $title ?></h3>
  <div class="comment-meta">
    <?php print $submitted ?>
  </div>

  <div class="content">
    <?php print $content ?>
    <?php if ($signature): ?>
      <div class="user-signature clear-block">
        <?php print $signature ?>
      </div>
    <?php endif; ?>
  </div>

  <div class="comment-links">
    <?php print $links ?>
  </div>
</div>
<!-- /end comment.tpl.php -->