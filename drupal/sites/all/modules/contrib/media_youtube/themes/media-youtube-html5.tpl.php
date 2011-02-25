<?php
// $Id:

/**
 * @file media_youtube/themes/media-youtube-html5.tpl.php
 *
 * Template file for Media: YouTube's theme('media_youtube_html5').
 *
 * This will display YouTube's HTML5 video in an iframe.
 *
 */
?>
<iframe id="media-youtube-html5-<?php print $id; ?>" title="<?php print $iframe_title; ?>" class="<?php print $class; ?>" type="text/html" width="<?php print $width; ?>" height="<?php print $height; ?>" src="<?php echo $url; ?>?hd=1" frameborder="0"<?php if ($fullscreen_value == 'true') { print ' allowFullScreen'; } ?>></iframe>
