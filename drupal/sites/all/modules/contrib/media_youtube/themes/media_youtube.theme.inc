<?php
// $Id: media_youtube.theme.inc,v 1.1.2.8 2011/02/04 17:29:53 aaron Exp $

/**
 * @file
 * Theme functions for Media: YouTube.
 */

function theme_media_youtube_video_unavailable($message, $status = array()) {
  $message = token_replace($message, 'media_youtube_status', $status);
  $message = filter_xss($message);
  return "<div class='media-youtube-status-unavailable'>$message</div>";
}

function theme_media_youtube_video_duplicate($message, $status = array()) {
  $message = token_replace($message, 'media_youtube_status', $status);
  $message = filter_xss($message);
  return "<div class='media-youtube-status-duplicate'>$message</div>";
}

function theme_media_youtube_video_rejected($message, $status = array()) {
  $message = token_replace($message, 'media_youtube_status', $status);
  $message = filter_xss($message);
  return "<div class='media-youtube-status-rejected'>$message</div>";
}

function theme_media_youtube_video_upload_failed($message, $status = array()) {
  $message = token_replace($message, 'media_youtube_status', $status);
  $message = filter_xss($message);
  return "<div class='media-youtube-status-upload-failed'>$message</div>";
}

function template_preprocess_media_youtube_html5(&$variables) {
  // Merge in our default variables.
  $variables = array_merge($variables, $variables['variables']);

  $video_id = $variables['video_id'];
  $variables['url'] = isset($variables['url']) ? $variables['url'] : "http://www.youtube.com/v/$video_id";
  $variables['url'] = check_plain($variables['url']);
  $variables['width'] = check_plain($variables['width']);
  $variables['height'] = check_plain($variables['height']);
  $variables['fullscreen_value'] = check_plain($variables['fullscreen_value']);
  $variables['iframe_title'] = isset($variables['iframe_title']) ? check_plain($variables['iframe_title']) : t('@title', array('@title' => media_youtube_variable_get('iframe_title')));
  $variables['classes'] = isset($variables['classes']) ? $variables['classes'] : array('media-youtube-html5');
  $variables['class'] = check_plain(implode(' ', $variables['classes']));
}

function template_preprocess_media_youtube_default_external(&$variables) {
  // Merge in our default variables.
  $variables = array_merge($variables, $variables['variables']);

  $variables['flashvars'] = drupal_query_string_encode(array('playerMode' => 'embedded'));
  $video_id = $variables['video_id'];
  $variables['url'] = isset($variables['url']) ? $variables['url'] : "http://www.youtube.com/v/$video_id";
  $variables['url'] = check_plain($variables['url']);
  $variables['width'] = check_plain($variables['width']);
  $variables['height'] = check_plain($variables['height']);
  $variables['fullscreen_value'] = check_plain($variables['fullscreen_value']);
  $variables['thumbnail'] = isset($variables['thumbnail']) ? $variables['thumbnail'] : theme('emvideo_video_thumbnail', $variables['field'], array('value' => $video_id, 'provider' => 'youtube'), 'thumbnail', $variables['node'], FALSE, array('width' => $variables['width'], 'height' => $variables['height'], 'link_url' => media_youtube_video_url($video_id)));
}

/**
 * The embedded flash displaying the youtube video.
 *
 * @TODO: Break this down; it's too big!!!
 */
function template_preprocess_media_youtube_flash(&$variables) {
  static $count;
  // Merge in our default variables.
  $variables = array_merge($variables, $variables['variables']);

  // Set up internal variables.
  $output = '';
  $video_id = isset($variables['video_id']) ? $variables['video_id'] : $variables['item']['value'];
  $width = $variables['width'];
  $height = $variables['height'];
  $autoplay = $variables['autoplay'];
  if ($video_id = check_plain($video_id)) {
    // Display a message if a video is unavailable.
    if (($variables['item']['status'] == EMFIELD_STATUS_UNAVAILABLE) && !$variables['no_unavailable_message']) {
      $status = media_youtube_video_full_status($video_id);
      if (media_youtube_variable_get('check_for_duplicates') && ($status_msg = media_youtube_variable_get('status_display_duplicates')) && media_youtube_video_is_duplicate($video_id)) {
        $variables['output'] = theme('media_youtube_video_duplicate', $status_msg, $status);
        return;
      }
      if (media_youtube_variable_get('check_for_rejected') && ($status_msg = media_youtube_variable_get('status_display_rejected')) && media_youtube_video_was_rejected($video_id)) {
        $variables['output'] = theme('media_youtube_video_rejected', $status_msg, $status);
        return;
      }
      if (media_youtube_variable_get('check_for_failed') && ($status_msg = media_youtube_variable_get('status_display_upload_failed')) && media_youtube_video_upload_failed($video_id)) {
        $variables['output'] = theme('media_youtube_video_upload_failed', $status_msg, $status);
        return;
      }
      if ($status_msg = media_youtube_variable_get('status_display_'. EMFIELD_STATUS_UNAVAILABLE)) {
        $variables['output'] = theme('media_youtube_video_unavailable', $status_msg, $status);
        return;
      }
    }

    // Set the height and width.
    $width = intval($width);
    $height = intval($height);

    // Build the Thumbnail image for the player.
    // First, grab the URL for the thumbnail link.
    $variables['thumbnail_link_url'] = isset($variables['thumbnail_link_url']) ? $variables['thumbnail_link_url'] : media_youtube_video_url($video_id);

    // Now set the thumbnail w/h options.
    $tn_options = array(
      'width' => $width,
      'height' => $height,
      'absolute' => $variables['absolute'],
      'link_url' => $variables['thumbnail_link_url'],
    );

    // Grab the thumbnail, complete with link.
    $variables['thumbnail'] = isset($variables['thumbnail']) ? $variables['thumbnail'] : theme('emvideo_video_thumbnail', NULL, $variables['item'], 'emvideo_thumbnail', $variables['node'], FALSE, $tn_options);

    // Now just grab the thumnbail path, if needed (for Longtail).
    $tn_options['return_url'] = TRUE;
    $variables['thumbnail_path'] = isset($variables['thumbnail_path']) ? $variables['thumbnail_path'] : theme('emvideo_video_thumbnail', NULL, array('value' => $video_id, 'provider' => 'youtube'), 'emvideo_thumbnail', $variables['node'], TRUE, $tn_options);

    // Set the Full Screen option; if TRUE, then allow it.
    $fullscreen = isset($variables['fullscreen']) ? $variables['fullscreen'] : media_youtube_variable_get('full_screen');
    $fullscreen_value = $fullscreen ? "true" : "false";
    $fs = $fullscreen ? "&fs=$fullscreen" : "";

    // Set the 'related videos' youtube option.
    $related = isset($variables['related']) ? $variables['related'] : media_youtube_variable_get('show_related_videos');
    $related = "rel=$related";

    // Set the Autoplay option.
    $autoplay_value = $autoplay ? '&autoplay=1' : '';

    // Set the custom colors.
    $show_colors = isset($variables['show_colors']) ? $variables['show_colors'] : media_youtube_variable_get('show_colors');
    $colors = '';
    if ($show_colors) {
      $color1 = isset($variables['color1']) ? $variables['color1'] : media_youtube_variable_get('colors_color1');
      $color2 = isset($variables['color2']) ? $variables['color2'] : media_youtube_variable_get('colors_color2');
      $colors='&color1=0x'. emvideo_youtube_convert_color($color1) .'&color2=0x'. emvideo_youtube_convert_color($color2);
    }

    // Set the custom border; if TRUE, then display a big border.
    $border = isset($variables['border']) ? $variables['border'] : media_youtube_variable_get('show_border');
    $border = $border ? '&border=1' : '';

    // Enable JS for playback control.
    $enablejsapi = isset($variables['enablejsapi']) ? $variables['enablejsapi'] : media_youtube_variable_get('enablejsapi');
    $enablejsapi = $enablejsapi ? '&enablejsapi=1&playerapiid=ytplayer' : '';

    // Set the div ID for CSS.
    $id = check_plain(isset($variables['id']) ? $variables['id'] : 'emvideo-youtube-flash-'. (++$count));
    $div_id = check_plain(isset($variables['div_id']) ? $variables['div_id'] : 'emvideo-youtube-flash-wrapper-'. $count);

    // Whether to allow high quality videos or not.
    $high_quality = isset($variables['high_quality']) ? $variables['high_quality'] : media_youtube_variable_get('high_quality');
    $high_quality = $high_quality ? '&hd=1' : '';

    // Whether to display the video's info from YouTube in the player.
    $display_info = isset($variables['display_info']) ? $variables['display_info'] : media_youtube_variable_get('display_info');
    $display_info  = $display_info  ? '&showinfo=0' : '';

    // Create a URL from our options.
    if ($variables['item']['data']['playlist']) {
      $video_id = substr($video_id, 9);
      $url = check_plain("http://www.youtube.com/p/$video_id&$related$autoplay_value$colors$border$high_quality$display_info$enablejsapi$fs");
      // FLV Player doesn't support playlists, so we need to fall back.
      $variables['use_flv'] = FALSE;
    }
    else {
      $url = check_plain("http://www.youtube.com/v/$video_id&$related$autoplay_value$colors$border$high_quality$display_info$enablejsapi$fs");
    }

    // Find the path to the JW FLV Media Player.
    $path = emfield_flvmediaplayer_url();

    // If the JW FLV Player has been installed, the yt.swf file is in the same
    // folder, and the server has been configured to use it for YouTube videos,
    // then we'll do so now.
    $use_flv = isset($variables['use_flv']) ? $variables['use_flv'] : media_youtube_variable_get('use_jw_flv');

    // Set up the FLV Media Player options, assuming we're supposed to,
    // and the player is actually installed on the system.
    if ($use_flv && ($flv_path = emfield_flvmediaplayer_url()) && _media_youtube_check_flv_player_setup()) {
      global $base_path;

      $flashvars = array();

      // Grab the thumbnail for this video and tell JW FLV Player about it.
      $flashvars['image'] = $variables['thumbnail_path'];

      // Tell the FLV Player we're sending a YouTube video.
      $flashvars['type'] = 'youtube';

      // We need to set the file to the original YouTube video.
      $flashvars['file'] = $url;

      // The JW FLV Player uses its own autoplay flashvar.
      $flashvars['autostart'] = $autoplay ? 'true' : 'false';

      // The URL will now be the JW FLV Player.
      $url = $base_path . $flv_path;
    }
    else {
      // We don't have access to FLV Media Player, so reset the option.
      $use_flv = FALSE;
    }

    if ($use_flv && module_exists('flvmediaplayer') && !$variables['no_swf']) {
      // If we are using the JW FLV Player, defer to the flvmediaplayer module
      // for display.
      $params['width'] = $width;
      $params['height'] = $height;
      $params['div_id'] = $id;
      $params['allowFullScreen'] = $fullscreen_value;

      // If we have been told to use a specific FLV Player Option from
      // that module, then use it here.
      $flv_profile = isset($variables['flv_profile']) ? $variables['flv_profile'] : variable_get('emfield_flv_profile', '');
      if ($flv_profile) {
        // Get the configuration data for this profile.
        $data = flvmediaplayer_build_player_data($variables['node'], $flv_profile, array('file' => $flashvars['file']));
        $data['params']['width'] = $width;
        $data['params']['height'] = $height;
        $data['flashvars']['autostart'] = $flashvars['autostart'];
        $data['params']['allowFullScreen'] = $fullscreen_value;
        $data['params']['wmode'] = 'transparent';
        $data['flashvars']['image'] = $flashvars['image'];
        $output = theme('flvmediaplayer_render_player', $url, $data['params'], $data['flashvars']);
      }
      else {
        // Just display the default jw flv media player.
        $output = theme('flvmediaplayer_render_player', $url, $params, $flashvars);
      }
    }
    else if (variable_get('emfield_swfobject', FALSE) && (module_exists('swfobject_api') || variable_get('emfield_swfobject_location', '')) && !$variables['no_swf']) {
      // Use SWFObject API module if it's installed.
      // Note that we're going to try to send the Flv Media player as well,
      // assuming it's been set up and the flvmediaplayer module has not.
      $params['width'] = $width;
      $params['height'] = $height;
      $params['div_id'] = $id;
      $params['allowFullScreen'] = $fullscreen_value;
      $params['wmode'] = 'transparent';
      $output = theme('emfield_swfobject', $url, $params, $flashvars, $id);
    }
    else if ($use_flv) {
      // We need to grab the thumbnail again for the NOEMBED option,
      // for accessibility compliance.
      // See http://www.w3.org/TR/WCAG10-HTML-TECHS/#text-equivs-multimedia.
      unset($tn_options['return_url']);
      $noembed = theme('emvideo_video_thumbnail', NULL, array('value' => $video_id, 'provider' => 'youtube'), 'emvideo_thumbnail', $variables['node'], TRUE, $tn_options);
      $flv_path = url($flv_path, array('absolute' => $variables['absolute']));
      $flashvars = check_plain(drupal_query_string_encode($flashvars));
      // Use object tags rather than embed.
      // See http://www.alistapart.com/articles/flashsatay
      $output = <<<FLV
<div id="$div_id"><object type="application/x-shockwave-flash" data="$flv_path" width="$width" height="$height" id="$id"><param name="movie" value="$flv_path" /><param name="flashvars" value="$flashvars" /><param name="allowFullScreen" value="$fullscreen_value"/><param name="wmode" value="transparent" />$noembed</object></div>
FLV;
    }
    else {
      // The fallback is to display plain old vanilla youtube.
      $output = theme('media_youtube_default_external', array('field' => $variables['field'], 'node' => $variables['node'], 'video_id' => $video_id, 'width' => $width, 'height' => $height, 'url' => $url, 'fullscreen_value' => $fullscreen_value, 'thumbnail' => $variables['thumbnail']));
    }
  }
  $variables['class'] = isset($variables['class']) ? $variables['class'] : array('media-youtube');
  $variables['classes'] = implode(' ', $variables['class']);
  $variables['output'] = $output;
}
