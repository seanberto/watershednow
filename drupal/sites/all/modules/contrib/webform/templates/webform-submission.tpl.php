<?php
// $Id: webform-submission.tpl.php,v 1.1.2.1 2010/08/30 17:01:54 quicksketch Exp $

/**
 * @file
 * Customize the display of a webform submission.
 *
 * Available variables:
 * - $node: The node object for this webform.
 * - $submission: The Webform submission array.
 * - $email: If sending this submission in an e-mail, the e-mail configuration
 *   options.
 * - $format: The format of the submission being printed, either "html" or
 *   "text".
 * - $renderable: The renderable submission array, used to print out individual
 *   parts of the submission, just like a $form array.
 */
?>

<?php print drupal_render($renderable); ?>
