<?php
// $Id: media-vimeo-universal.tpl.php,v 1.1 2010/09/17 16:16:34 aaron Exp $

/**
 * @file media_vimeo/themes/media-vimeo-universal.tpl.php
 *
 * The template file for theme('media_vimeo_universal').
 *
 * This will display an iFrame including an HTML5 version of the video.
 *
 * Available variables:
 *  $iframe_url => The URL of the iFrame source.
 *  $video_code => The Vimeo video ID.
 *  $width => The width of the video display.
 *  $height => The height of the video display.
 *  $fullscreen => Either 1 or 0, for full screen availability.
 *  $show_title => Either 1 or 0, to display the video title.
 *  $show_byline => Either 1 or 0, to display the video creator's byline.
 *  $show_portrait => Either 1 or 0, to display the video creator's portrait.
 *  $color => The hexcode of the color to display, without a #.
 *  $autoplay => Either 1 or 0, determining whether to autoplay the video.
 */
?>
<?php if ($iframe_url) : ?>
  <iframe src="<?php print $iframe_url; ?>" width="<?php print $width; ?>" height="<?php print $height; ?>"></iframe>
<?php endif; ?>
