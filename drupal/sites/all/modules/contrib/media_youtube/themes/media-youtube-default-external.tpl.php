<?php
// $Id: media-youtube-default-external.tpl.php,v 1.1.2.1 2010/11/12 14:37:18 aaron Exp $

/**
 * @file media_youtube/themes/media-youtube-default-external.tpl.php
 *
 * Template file for Media: YouTube's theme('media_youtube_default_external').
 *
 * This will display YouTube's default embedded video.
 *
 *  This is the fallback display, for when we don't have SWF Object or
 *  JW Flash Media Player.
 */
?>
<div id="media-youtube-default-external-<?php print $id; ?>">
  <object type="application/x-shockwave-flash" height="<?php print $height; ?>" width="<?php print $width; ?>" data="<?php print $url; ?>" id="media-youtube-default-external-object-<?php print $id; ?>">
    <param name="movie" value="<?php print $url; ?>" />
    <param name="allowScriptAccess" value="sameDomain"/>
    <param name="quality" value="best"/>
    <param name="allowFullScreen" value="<?php print $fullscreen_value; ?>"/>
    <param name="bgcolor" value="#FFFFFF"/>
    <param name="scale" value="noScale"/>
    <param name="salign" value="TL"/>
    <param name="FlashVars" value="<?php print $flashvars; ?>" />
    <param name="wmode" value="transparent" />
    <!-- Fallback content -->
      <?php print $thumbnail; ?>
  </object>
</div>
