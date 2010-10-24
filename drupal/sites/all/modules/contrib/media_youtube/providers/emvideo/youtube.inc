<?php
// $Id: youtube.inc,v 1.1.2.3 2010/04/08 20:35:48 aaron Exp $

/**
 * @file
 *   This include processes youtube.com media files for use by emfield.module.
 */

/**
 * hook emvideo_PROVIDER_info
 * this returns information relevant to a specific 3rd party video provider
 * @return
 *   an array of strings requested by various admin and other forms
 *   'name' => the translated name of the provider
 *   'url' => the url to the main page for the provider
 *   'settings_description' => a description of the provider that will be posted in the admin settings form
 *   'supported_features' => an array of rows describing the state of certain supported features by the provider.
 *      These will be rendered in a table, with the columns being 'Feature', 'Supported', 'Notes'.
 */
function emvideo_youtube_info() {
  $features = array(
    array(t('Autoplay'), t('Yes'), ''),
    array(t('RSS Attachment'), t('Yes'), ''),
    array(t('Show related videos'), t('Yes'), t('This is embedded in the video itself when enabled; currently not available with other providers. Set the feature above.')),
    array(t('Thumbnails'), t('Yes'), t('')),
    array(t('Duration'), t('Yes'), ''),
    array(t('Custom player colors'), t('Yes'), t("You may customize the player's skin by choosing your own colors, and/or border in that setting field set.")),
    array(t('Full screen mode'), t('Yes'), t('You may customize the player to enable or disable full screen playback. Full screen mode is enabled by default.')),
    array(t('Use JW FLV Media Player'), t('Yes'), t("You may opt to use the !flvplayer to play YouTube videos if it's installed on your server.", array('!flvplayer' => l(t('JW FLV Media Player'), 'http://www.longtailvideo.com/players/jw-flv-player/')))),
  );
  return array(
    'provider' => 'youtube',
    'name' => t('YouTube'),
    'url' => MEDIA_YOUTUBE_MAIN_URL,
    'settings_description' => t('These settings specifically affect videos displayed from <a href="@youtube" target="_blank">YouTube</a>. You can learn more about its <a href="@api" target="_blank">API</a> here.', array('@youtube' => MEDIA_YOUTUBE_MAIN_URL, '@api' => MEDIA_YOUTUBE_API_INFO)),
    'supported_features' => $features,
  );
}

/**
 * hook emvideo_PROVIDER_settings
 * this should return a subform to be added to the emvideo_settings() admin settings page.
 * note that a form field will already be provided, at $form['PROVIDER'] (such as $form['youtube'])
 * so if you want specific provider settings within that field, you can add the elements to that form field.
 */
function emvideo_youtube_settings() {
  module_load_include('inc', 'media_youtube', 'includes/media_youtube.admin');
  return media_youtube_admin_form();
}

/**
 *  Ensure we're able to run YouTube videos from the JW FLV Media Player.
 *  This requires that we both have the player installed, and the included
 *  yt.swf file is located in the same folder.
 *  @return boolean
 *    Returns TRUE if both checks are TRUE.
 */
function _emvideo_youtube_check_flv_player_setup() {
  static $check;

  if (is_null($check)) {
    // We set up a static cache.

    // First check that the JW FLV Player is installed.
    $flv_path = emfield_flvmediaplayer_url();
    if (!$flv_path || !file_exists($flv_path)) {
      // There's no player installed, so yt.swf is moot.
      $check = FALSE;
    }
    else {
      // Now check that the yt.swf file is also present in the same folder.
      $path = dirname($flv_path);
      $check = file_exists($path . '/yt.swf');
    }
  }
  return $check;
}

/**
 *  Implement hook emvideo_PROVIDER_data_version().
 */
function emvideo_youtube_data_version() {
  return MEDIA_YOUTUBE_DATA_VERSION;
}

/**
 * hook emfield_PROVIDER_data
 *
 * provides an array to be serialised and made available with $item elsewhere
 */
function emvideo_youtube_data($field, $item) {
  $data = array();

  // Create some 'field' version control.
  $data['emvideo_youtube_version'] = $data['emvideo_data_version'] = MEDIA_YOUTUBE_DATA_VERSION;

  // Store the raw data from YouTube's API.
  $data['raw'] = emvideo_youtube_request($item['value']);

  // Store the video's duration.
  $data['duration'] = intval($data['raw']['MEDIA:GROUP']['YT:DURATION'][1]['SECONDS']);

  // Get the large thumbnail.
  $data['thumbnail']['url'] = 'http://img.youtube.com/vi/'. $item['value'] .'/0.jpg';

  // Gather info about the item's raw flash video.
  // RSS / MRSS feeds with the item would have enough info.
  // Alternatively try getting the minimum from an HTTP get.
  $url = 'http://youtube.com/v/'. $item['value'];
  $response = emfield_request_header('youtube', $url);

  if ($response->code == 200) {
    // Don't give the 303 path.
    $data['flash']['url'] = $url;
    $data['flash']['size'] = $response->headers['Content-Length'];
    $data['flash']['mime'] = $response->headers['Content-Type'];
  }

  return $data;
}

/**
 *  hook emfield_PROVIDER_rss
 *
 *  Embeds the video in the RSS feed.
 */
function emvideo_youtube_rss($item, $teaser = NULL) {
  if ($item['value']) {
    if (!empty($item['data']['emvideo_youtube_data_version']) && $item['data']['emvideo_youtube_data_version'] >= 1) {
      $data = $item['data'];
    }
    else {
      $data = emvideo_youtube_data(NULL, $item);
    }

    $file = array();
    if (is_array($data['flash'])) {
      $file['filepath'] = $data['flash']['url'];
      $file['filesize'] = $data['flash']['size'];
      $file['filemime'] = $data['flash']['mime'];
    }
    $file['thumbnail']['filepath'] = $data['thumbnail']['url'];

    return $file;
  }
}

/**
 * this is a wrapper for emvideo_request_xml that includes youtube's api key
 */
function emvideo_youtube_request($code, $cached = TRUE) {
  $args['dev_id'] = trim(media_youtube_variable_get('api_key'));
  $args['method'] = $method;

  // if we've got a secret sign the arguments
  // TODO: doesn't seem to matter
//  if ($secret = trim(media_youtube_variable_get('api_secret'))) {
//    $args['api_sig'] = md5($secret . $arghash);
//  }

  $request = module_invoke('emfield', 'request_xml', 'youtube', MEDIA_YOUTUBE_REST_ENDPOINT .'/'. $code, array('video' => $code), $cached);
  return $request;
}

/**
 * hook emvideo_PROVIDER_extract
 * this is called to extract the video code from a pasted URL or embed code.
 * @param $code
 *   an optional string with the pasted URL or embed code
 * @return
 *   either an array of regex expressions to be tested, or a string with the video code to be used
 *   if the hook tests the code itself, it should return either the string of the video code (if matched), or an empty array.
 *   otherwise, the calling function will handle testing the embed code against each regex string in the returned array.
 */
function emvideo_youtube_extract($code = '') {
  // src="http://www.youtube.com/v/nvbQQnvxXDk"
  // http://youtube.com/watch?v=nvbQQnvxXDk
  // http://www.youtube.com/watch?v=YzFCA-xUc8w&feature=dir
  return array(
    '@youtube\.com/v/([^"\& ]+)@i',
    '@youtube\.com/watch\?v=([^"\& ]+)@i',
     '@youtube\.com/\?v=([^"\& ]+)@i',
  );
}

/**
 * hook emvideo_PROVIDER_embedded_link($video_code)
 * returns a link to view the video at the provider's site
 *  @param $video_code
 *    the string containing the video to watch
 *  @return
 *    a string containing the URL to view the video at the original provider's site
 */
function emvideo_youtube_embedded_link($video_code) {
  return media_youtube_video_url($video_code);
}

/**
 * hook emvideo_PROVIDER_duration($item)
 * Returns the duration of the video in seconds.
 *  @param $item
 *    The video item itself, which needs the $data array.
 *  @return
 *    The duration of the video in seconds.
 */
function emvideo_youtube_duration($item) {
  if (!isset($item['data']['emvideo_youtube_version']) || $item['data']['emvideo_youtube_version'] < 3) {
    $item['data'] = emvideo_youtube_data(NULL, $item);
  }
  return isset($item['data']['duration']) ? $item['data']['duration'] : 0;
}

function emvideo_youtube_convert_color($color = NULL) {
  if ($color{0} == '#') {
    return drupal_substr($color, 1);
  }

  return $color;
}

/**
 * The embedded flash displaying the youtube video.
 */
function theme_emvideo_youtube_flash($code, $width, $height, $autoplay, $options = array()) {
  static $count;
  $output = '';
  if ($code = check_plain($code)) {
    // Set the height and width.
    $width = intval($width);
    $height = intval($height);

    // Set the Full Screen option; if TRUE, then allow it.
    $fullscreen = isset($options['fullscreen']) ? $options['fullscreen'] : media_youtube_variable_get('full_screen');
    $fullscreen_value = $fullscreen ? "true" : "false";
    $fs = $fullscreen ? "&fs=$fullscreen" : "";

    // Set the 'related videos' youtube option.
    $related = isset($options['related']) ? $options['related'] : media_youtube_variable_get('show_related_videos');
    $related = "rel=$related";

    // Set the Autoplay option.
    $autoplay = isset($options['autoplay']) ? $options['autoplay'] : $autoplay;
    $autoplay_value = $autoplay ? '&autoplay=1' : '';

    // Set the custom colors.
    $show_colors = isset($options['show_colors']) ? $options['show_colors'] : media_youtube_variable_get('show_colors');
    $colors = '';
    if ($show_colors) {
      $color1 = isset($options['color1']) ? $options['color1'] : media_youtube_variable_get('colors_color1');
      $color2 = isset($options['color2']) ? $options['color2'] : media_youtube_variable_get('colors_color2');
      $colors='&color1=0x'. emvideo_youtube_convert_color($color1) .'&color2=0x'. emvideo_youtube_convert_color($color2);
    }

    // Set the custom border; if TRUE, then display a big border.
    $border = isset($options['border']) ? $options['border'] : media_youtube_variable_get('show_border');
    $border = $border ? '&border=1' : '';

    // Enable JS for playback control.
    $enablejsapi = isset($options['enablejsapi']) ? $options['enablejsapi'] : media_youtube_variable_get('enablejsapi');
    $enablejsapi = $enablejsapi ? '&enablejsapi=1&playerapiid=ytplayer' : '';

    // Set the div ID for CSS.
    $id = check_plain(isset($options['id']) ? $options['id'] : 'emvideo-youtube-flash-'. (++$count));
    $div_id = check_plain(isset($options['div_id']) ? $options['div_id'] : 'emvideo-youtube-flash-wrapper-'. $count);

    // Whether to allow high quality videos or not.
    $high_quality = isset($options['high_quality']) ? $options['high_quality'] : media_youtube_variable_get('high_quality');
    $high_quality = $high_quality ? '&hd=1' : '';

    // Whether to display the video's info from YouTube in the player.
    $display_info = isset($options['display_info']) ? $options['display_info'] : media_youtube_variable_get('display_info');
    $display_info  = $display_info  ? '&showinfo=0' : '';

    // Create a URL from our options.
    $url = check_plain("http://www.youtube.com/v/$code&$related$autoplay_value$colors$border$high_quality$display_info$enablejsapi$fs");

    // Find the path to the JW FLV Media Player.
    $path = emfield_flvmediaplayer_url();

    // If the JW FLV Player has been installed, the yt.swf file is in the same
    // folder, and the server has been configured to use it for YouTube videos,
    // then we'll do so now.
    $use_flv = isset($options['use_flv']) ? $options['use_flv'] : media_youtube_variable_get('use_jw_flv');

    // Set up the FLV Media Player options, assuming we're supposed to,
    // and the player is actually installed on the system.
    if ($use_flv && ($flv_path = emfield_flvmediaplayer_url()) && _emvideo_youtube_check_flv_player_setup()) {
      global $base_path;

      // Build the Thumbnail image for the player.
      $tn_options = array(
        'width' => $width,
        'height' => $height,
        'return_url' => TRUE,
      );

      $flashvars = array();

      // Grab the thumbnail for this video and tell JW FLV Player about it.
      $flashvars['image'] = theme('emvideo_video_thumbnail', NULL, $options['item'], 'emvideo_thumbnail', $options['node'], TRUE, $tn_options);

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

    if ($use_flv && module_exists('flvmediaplayer')) {
      // If we are using the JW FLV Player, defer to the flvmediaplayer module
      // for display.
      $params['width'] = $width;
      $params['height'] = $height;
      $params['div_id'] = $id;
      $params['allowFullScreen'] = $fullscreen_value;

      // If we have been told to use a specific FLV Player Option from
      // that module, then use it here.
      $flv_profile = isset($options['flv_profile']) ? $options['flv_profile'] : variable_get('emfield_flv_profile', '');
      if ($flv_profile) {
        // Get the configuration data for this profile.
        $data = flvmediaplayer_build_player_data($options['node'], $flv_profile, array('file' => $flashvars['file']));
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
    else if (variable_get('emfield_swfobject', FALSE) && (module_exists('swfobject_api') || variable_get('emfield_swfobject_location', ''))) {
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
      $noembed = theme('emvideo_video_thumbnail', NULL, $options['item'], 'emvideo_thumbnail', $options['node'], TRUE, $tn_options);
      $flv_path = url($flv_path);
      $flashvars = check_plain(drupal_query_string_encode($flashvars));
      // Use object tags rather than embed.
      // See http://www.alistapart.com/articles/flashsatay
      $output = <<<FLV
        <div id="$div_id">
          <object
            type="application/x-shockwave-flash" data="$flv_path"
            width="$width" height="$height" id="$id">
            <param name="movie" value="$flv_path" />
            <param name="flashvars" value="$flashvars" />
            <param name="wmode" value="transparent" />
            $noembed
          </object>
        </div>
FLV;
    }
    else {
      // The fallback is to display plain old vanilla youtube.
      $flashvars = check_plain(drupal_query_string_encode(array('playerMode' => 'embedded')));
      $thumbnail = theme('emvideo_video_thumbnail', NULL, $options['item'], 'thumbnail', NULL, FALSE, array('width' => $width, 'height' => $height, 'link_url' => emvideo_youtube_embedded_link($code)));
      $output = theme('emvideo_youtube_default_external', $height, $width, $url, $id, $div_id, $fullscreen_value, $flashvars, $thumbnail);
    }
  }
  return '<div class="emfield-emvideo emfield-emvideo-youtube">'. $output .'</div>';
}

/**
 *  This is the fallback display, for when we don't have SWF Object or
 *  JW Flash Media Player.
 */
function theme_emvideo_youtube_default_external($height, $width, $url, $id, $div_id, $fullscreen_value, $flashvars, $thumbnail = '') {
  $output = <<<FLASH
        <div id="$div_id"><object type="application/x-shockwave-flash" height="$height" width="$width" data="$url" id="$id">
          <param name="movie" value="$url" />
          <param name="allowScriptAccess" value="sameDomain"/>
          <param name="quality" value="best"/>
          <param name="allowFullScreen" value="$fullscreen_value"/>
          <param name="bgcolor" value="#FFFFFF"/>
          <param name="scale" value="noScale"/>
          <param name="salign" value="TL"/>
          <param name="FlashVars" value="$flashvars" />
          <param name="wmode" value="transparent" />
          <!-- Fallback content -->
          $thumbnail
        </object></div>
FLASH;
  return $output;
}

/**
 * hook emvideo_PROVIDER_thumbnail
 * returns the external url for a thumbnail of a specific video
 * TODO: make the args: ($code, $field, $item), with $field/$item provided if we need it, but otherwise simplifying things
 *  @param $field
 *    the field of the requesting node
 *  @param $item
 *    the actual content of the field from the requesting node
 *  @return
 *    a URL pointing to the thumbnail
 */
function emvideo_youtube_thumbnail($field, $item, $formatter, $node, $width, $height, $options = array()) {
  // Always return the larger image, since we're storing images locally.
  $tn = "http://img.youtube.com/vi/{$item['value']}/0.jpg";

  return $tn;
}

/**
 * hook emvideo_PROVIDER_video
 * this actually displays the full/normal-sized video we want, usually on the default page view
 *  @param $code
 *    the video code for the video to embed
 *  @param $width
 *    the width to display the video
 *  @param $height
 *    the height to display the video
 *  @param $field
 *    the field info from the requesting node
 *  @param $item
 *    the actual content from the field
 *  @return
 *    the html of the embedded video
 */
function emvideo_youtube_video($code, $width, $height, $field, $item, $node, $autoplay, $options = array()) {
  $options['item'] = $item;
  $options['node'] = $node;
  $output = theme('emvideo_youtube_flash', $code, $width, $height, $autoplay, $options);
  return $output;
}

/**
 * hook emvideo_PROVIDER_video
 * this actually displays the preview-sized video we want, commonly for the teaser
 *  @param $code
 *    the video code for the video to embed
 *  @param $width
 *    the width to display the video
 *  @param $height
 *    the height to display the video
 *  @param $field
 *    the field info from the requesting node
 *  @param $item
 *    the actual content from the field
 *  @return
 *    the html of the embedded video
 */
function emvideo_youtube_preview($code, $width, $height, $field, $item, $node, $autoplay, $options = array()) {
  $options['item'] = $item;
  $options['node'] = $node;
  $output = theme('emvideo_youtube_flash', $code, $width, $height, $autoplay, $options);
  return $output;
}

/**
 * Implementation of hook_emfield_subtheme.
 */
function emvideo_youtube_emfield_subtheme() {
  return array(
      'emvideo_youtube_flash'  => array(
          'arguments' => array('embed' => NULL, 'width' => NULL, 'height' => NULL, 'autoplay' => NULL, 'options' => array()),
          'file' => 'providers/youtube.inc'
      ),
      'emvideo_youtube_default_external' => array(
        'arguments' => array('height' => NULL, 'width' => NULL, 'url' => NULL, 'id' => NULL, 'div_id' => NULL, 'fullscreen_value' => NULL, 'flashvars' => NULL)
      ),
  );
}

/**
 *  Implement hook_emvideo_PROVIDER_content_generate().
 */
function emvideo_youtube_content_generate() {
  return array(
    'http://www.youtube.com/watch?v=-jubiv7QUco',
    'http://www.youtube.com/watch?v=VG_ss5QT03Y',
    'http://www.youtube.com/watch?v=LrUMJgyQVE8',
    'http://www.youtube.com/watch?v=H2eI02rK9_U',
    'http://www.youtube.com/watch?v=B3qxki6H8Fk',
    'http://www.youtube.com/watch?v=Rsaw5NuFIto',
    'http://www.youtube.com/watch?v=J3CaN-g5mQQ',
    'http://www.youtube.com/watch?v=L2pXcNu4GRo',
  );
}
