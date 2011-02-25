<?php
// $Id: webform-submission-page.tpl.php,v 1.2.2.3 2011/02/17 04:08:55 quicksketch Exp $

/**
 * @file
 * Customize the navigation shown when editing or viewing submissions.
 *
 * Available variables:
 * - $node: The node object for this webform.
 * - $submission: The Webform submission array.
 * - $submission_content: The contents of the webform submission.
 * - $submission_actions: The actions that can be performed on this submission.
 * - $submission_navigation: Links to the previous and next submission.
 * - $submission_information: Information on who made this submission.
 */
?>

<?php if ($submission_actions || $submission_navigation): ?>
  <div class="clear-block">
    <?php print $submission_actions; ?>
    <?php print $submission_navigation; ?>
  </div>
<?php endif; ?>

<?php print $submission_information; ?>

<div class="webform-submission">
  <?php print $submission_content; ?>
</div>

<?php if ($submission_navigation): ?>
  <div class="clear-block">
    <?php print $submission_navigation; ?>
  </div>
<?php endif; ?>