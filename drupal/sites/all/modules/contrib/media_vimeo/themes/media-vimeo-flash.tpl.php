<?php
// $Id: media-vimeo-flash.tpl.php,v 1.1 2010/09/17 16:16:34 aaron Exp $

/**
 * @file media_vimeo/themes/media-vimeo-universal.tpl.php
 *
 * The template file for theme('media_vimeo_universal').
 *
 * This will display an iFrame including an HTML5 version of the video.
 *
 * Available variables:
 *  $data => The URL for the data flash parameter.
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
<?php if ($video_code) : ?>
  <object type="application/x-shockwave-flash" width="<?php print $width; ?>" height="<?php print $height; ?>" data="<?php print $data; ?>">
    <param name="quality" value="best" />
    <param name="wmode" value="transparent" />
    <param name="allowfullscreen" value="<?php print ($fullscreen ? 'true' : 'false'); ?>" />
    <param name="scale" value="showAll" />
    <param name="movie" value="<?php print $data; ?>" /></object>
<?php endif; ?>
