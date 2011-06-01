<?php

/**
 * @file
 * Customize the navigation shown when editing or viewing submissions.
 *
 * Available variables:
 * - $node: The node object for this webform.
 * - $submission: The contents of the webform submission.
 * - $previous: The previous submission ID.
 * - $next: The next submission ID.
 * - $previous_url: The URL of the previous submission.
 * - $next_url: The URL of the next submission.
 * - $mode: Either "form" or "display". As the navigation is shown in both uses.
 */
?>
<div class="webform-submission-navigation">
  <?php if ($previous): ?>
    <?php print l(t('Previous submission'), $previous_url, array('attributes' => array('class' => 'webform-submission-previous'), 'query' => ($mode == 'form' ? 'destination=' . $previous_url : NULL))); ?>
  <?php else: ?>
    <span class="webform-submission-previous"><?php print t('Previous submission'); ?></span>
  <?php endif; ?>

  <?php if ($next): ?>
    <?php print l(t('Next submission'), $next_url, array('attributes' => array('class' => 'webform-submission-next'), 'query' => ($mode == 'form' ? 'destination=' . $next_url : NULL))); ?>
  <?php else: ?>
    <span class="webform-submission-next"><?php print t('Next submission'); ?></span>
  <?php endif; ?>
</div>
