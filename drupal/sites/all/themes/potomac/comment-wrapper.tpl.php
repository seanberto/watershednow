<?php
// $Id: comment-wrapper.tpl.php,v 1.1 2009/06/26 13:55:36 kiterminal Exp $
?>

<?php if (isset($comments_count)): ?>
  <h3 id="comment-response"><?php print $comments_count ?></h3>
<?php endif; ?>
<div id="comments">
  <?php print $content; ?>
</div>