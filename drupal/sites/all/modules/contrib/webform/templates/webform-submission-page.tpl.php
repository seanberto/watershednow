<?php
// $Id: webform-submission-page.tpl.php,v 1.2.2.2 2011/01/05 03:21:29 quicksketch Exp $

/**
 * @file
 * Customize the navigation shown when editing or viewing submissions.
 *
 * Available variables:
 * - $node: The node object for this webform.
 * - $submission: The Webform submission array.
 * - $submission_content: The contents of the webform submission.
 * - $submission_navigation: The previous submission ID.
 * - $submission_information: The next submission ID.
 */
?>

<?php print $submission_navigation; ?>
<?php print $submission_information; ?>

<div class="webform-submission">
  <?php print $submission_content; ?>
</div>

<?php print $submission_navigation; ?>
