<?php
// $Id: youtube.inc,v 1.1.2.23 2011/02/04 17:38:09 aaron Exp $

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
    array(t('Status'), t('Yes'), t('Stores video status (available and unavailable).')),
    array(t('Custom player colors'), t('Yes'), t("You may customize the player's skin by choosing your own colors, and/or border in that setting field set.")),
    array(t('Full screen mode'), t('Yes'), t('You may customize the player to enable or disable full screen playback. Full screen mode is enabled by default.')),
    array(t('Use JW FLV Media Player'), t('Yes'), t("You may opt to use the !flvplayer to play YouTube videos if it's installed on your server.", array('!flvplayer' => l(t('JW FLV Media Player'), 'http://www.longtailvideo.com/players/jw-flv-player/')))),
  );
  return array(
    'module' => 'media_youtube',
    'provides' => array('emvideo'),
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
  return media_youtube_emfield_data($item['value']);
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
 * hook emvideo_PROVIDER_extract
 * this is called to extract the video code from a pasted URL or embed code.
 * @param $video_id
 *   an optional string with the pasted URL or embed code
 * @return
 *   either an array of regex expressions to be tested, or a string with the video code to be used
 *   if the hook tests the code itself, it should return either the string of the video code (if matched), or an empty array.
 *   otherwise, the calling function will handle testing the embed code against each regex string in the returned array.
 */
function emvideo_youtube_extract($video_id = '') {
  // Special extraction for playlists.
  $playlist_regex = array(
    '@youtube\.com/p/([^"\& ]+)@i', // A playlist.
    '@youtube\.com/view_play_list\?p=([^"\& ]+)@i', // A playlist.
  );
  foreach ($playlist_regex as $regex) {
    if (preg_match($regex, trim($video_id), $matches)) {
      return 'PLAYLIST_'. $matches[1];
    }
  }

  // src="http://www.youtube.com/v/nvbQQnvxXDk"
  // http://youtube.com/watch?v=nvbQQnvxXDk
  // http://www.youtube.com/watch?v=YzFCA-xUc8w&feature=dir
  // src="http://www.youtube.com/embed/kge8zlJfwQQ"
  // http://youtu.be/bh-BwFNNAGc
  return array(
    '@youtube\.com/(?:v|embed)/([^"\&\? ]+)@i',
    '@youtube\.com/watch\?v=([^"\& ]+)@i',
    '@youtube\.com/\?v=([^"\& ]+)@i',
    '@youtu.be/([^"\& ]+)@i',
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
 * hook emvideo_PROVIDER_thumbnail
 * returns the external url for a thumbnail of a specific video
 * TODO: make the args: ($video_id, $field, $item), with $field/$item provided if we need it, but otherwise simplifying things
 *  @param $field
 *    the field of the requesting node
 *  @param $item
 *    the actual content of the field from the requesting node
 *  @return
 *    a URL pointing to the thumbnail
 */
function emvideo_youtube_thumbnail($field, $item, $formatter, $node, $width, $height, $options = array()) {
  if (isset($item['data']['thumbnail']['url'])) {
    return $item['data']['thumbnail']['url'];
  }

  // Always return the larger image, since we're storing images locally.
  $tn = "http://img.youtube.com/vi/{$item['value']}/0.jpg";

  return $tn;
}

/**
 * hook emvideo_PROVIDER_video
 * this actually displays the full/normal-sized video we want, usually on the default page view
 *  @param $video_id
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
function emvideo_youtube_video($video_id, $width, $height, $field, $item, $node, $autoplay, $options = array()) {
  $options['video_id'] = isset($options['video_id']) ? $options['video_id'] : $video_id;
  $options['width'] = isset($options['width']) ? $options['width'] : $width;
  $options['height'] = isset($options['height']) ? $options['height'] : $height;
  $options['field'] = isset($options['field']) ? $options['field'] : $field;
  $options['item'] = isset($options['item']) ? $options['item'] : $item;
  $options['node'] = isset($options['node']) ? $options['node'] : $node;
  $options['autoplay'] = isset($options['autoplay']) ? $options['autoplay'] : $autoplay;
  $options['html5'] = isset($options['html5']) ? $options['html5'] : media_youtube_variable_get('html5_player');
  if ($options['html5']) {
    $output = theme('media_youtube_html5', $options);
  } else {
    $output = theme('media_youtube_flash', $options);
  }
  return $output;
}

/**
 * hook emvideo_PROVIDER_video
 * this actually displays the preview-sized video we want, commonly for the teaser
 *  @param $video_id
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
function emvideo_youtube_preview($video_id, $width, $height, $field, $item, $node, $autoplay, $options = array()) {
  $options['video_id'] = isset($options['video_id']) ? $options['video_id'] : $video_id;
  $options['width'] = isset($options['width']) ? $options['width'] : $width;
  $options['height'] = isset($options['height']) ? $options['height'] : $height;
  $options['field'] = isset($options['field']) ? $options['field'] : $field;
  $options['item'] = isset($options['item']) ? $options['item'] : $item;
  $options['node'] = isset($options['node']) ? $options['node'] : $node;
  $options['autoplay'] = isset($options['autoplay']) ? $options['autoplay'] : $autoplay;
  $options['html5'] = isset($options['html5']) ? $options['html5'] : media_youtube_variable_get('html5_player');
  if ($options['html5']) {
    $output = theme('media_youtube_html5', $options);
  } else {
    $output = theme('media_youtube_flash', $options);
  }
  return $output;
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
    'http://www.youtube.com/watch?v=9GaCttVGv70',
  );
}
