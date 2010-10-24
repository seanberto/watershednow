<?php
// $Id: vimeo.inc,v 1.1.2.3 2010/09/23 21:09:42 aaron Exp $

/**
 * @file media_vimeo/includes/providers/vimeo.inc
 *   Provide support for the Vimeo provider to the emfield.module.
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
function emvideo_vimeo_info() {
  $features = array(
    array(t('Custom player color'), t('Yes'), t("You may customize the player's skin by choosing your own color.")),
    array(t('Thumbnails'), t('Yes'), t('You may select the size of thumbnail to request from Vimeo.')),
    array(t('Full screen mode'), t('Yes'), t('You may customize the player to enable or disable full screen playback. Full screen mode is enabled by default.')),
    array(t('Duration'), t('Yes'), t("A video's duration can be displayed using view formatters.'")),
  );
  return array(
    'provider' => 'vimeo',
    'name' => t('Vimeo'),
    'url' => MEDIA_VIMEO_MAIN_URL,
    'settings_description' => t('These settings specifically affect videos displayed from <a href="@provider" target="_blank">Vimeo</a>. You can learn more about its <a href="@api" target="_blank">API</a> here. (You no longer need a key for any current supported features.)', array('@provider' => MEDIA_VIMEO_MAIN_URL, '@api' => MEDIA_VIMEO_API_INFO)),
    'supported_features' => $features,
  );
}

/**
 * hook emvideo_PROVIDER_settings
 * this should return a subform to be added to the emvideo_settings() admin settings page.
 * note that a form field will already be provided, at $form['PROVIDER'] (such as $form['vimeo'])
 * so if you want specific provider settings within that field, you can add the elements to that form field.
 */
function emvideo_vimeo_settings() {
  module_load_include('inc', 'media_vimeo', 'includes/media_vimeo.admin');
  return media_vimeo_admin_form();
}

/**
 * hook emvideo_PROVIDER_extract
 * this is called to extract the video code from a pasted URL or embed code.
 * @param $video_code
 *   an optional string with the pasted URL or embed code
 * @return
 *   either an array of regex expressions to be tested, or a string with the video code to be used
 *   if the hook tests the code itself, it should return either the string of the video code (if matched), or an empty array.
 *   otherwise, the calling function will handle testing the embed code against each regex string in the returned array.
 */
function emvideo_vimeo_extract($video_code = '') {
  // http://vimeo.com/123456
  // http://www.vimeo.com/123456
  // http://vimeo.com/channels/hd#6912147
  //<object width="400" height="270"><param name="allowfullscreen" value="true" /><param name="allowscriptaccess" value="always" /><param name="movie" value="http://vimeo.com/moogaloop.swf?clip_id=4106135&amp;server=vimeo.com&amp;show_title=1&amp;show_byline=1&amp;show_portrait=0&amp;color=&amp;fullscreen=1" /><embed src="http://vimeo.com/moogaloop.swf?clip_id=4106135&amp;server=vimeo.com&amp;show_title=1&amp;show_byline=1&amp;show_portrait=0&amp;color=&amp;fullscreen=1" type="application/x-shockwave-flash" allowfullscreen="true" allowscriptaccess="always" width="400" height="270"></embed></object><br /><a href="http://vimeo.com/4106135">Two Knives One Motive</a> from <a href="http://vimeo.com/x351">Tyler Capehart</a> on <a href="http://vimeo.com">Vimeo</a>.
  return array(
    '@vimeo\.com/moogaloop\.swf\?clip_id\=([^\"\&]+)@i',
    '@vimeo\.com/[^\"\&\d]*([^\"\&]+)@i',
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
function emvideo_vimeo_embedded_link($video_code) {
  return media_vimeo_video_url($video_code);
}

/**
 * hook emvideo_PROVIDER_video
 * this actually displays the full/normal-sized video we want, usually on the default page view
 *  @param $video_code
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
function emvideo_vimeo_video($video_code, $width, $height, $field, $item, $node, $autoplay, $options = array()) {
  $options['width'] = isset($options['width']) ? $options['width'] : $width;
  $options['height'] = isset($options['height']) ? $options['height'] : $height;
  $options['autoplay'] = isset($options['autoplay']) ? $options['autoplay'] : $autoplay;
  return theme('media_vimeo', $video_code, $options);
}

/**
 * hook emvideo_PROVIDER_video
 * this actually displays the preview-sized video we want, commonly for the teaser
 *  @param $video_code
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
function emvideo_vimeo_preview($video_code, $width, $height, $field, $item, $node, $autoplay, $options = array()) {
  $options['width'] = isset($options['width']) ? $options['width'] : $width;
  $options['height'] = isset($options['height']) ? $options['height'] : $height;
  $options['autoplay'] = isset($options['autoplay']) ? $options['autoplay'] : $autoplay;
  return theme('media_vimeo', $video_code, $options);
}


/**
 * hook emvideo_PROVIDER_thumbnail
 * returns the external url for a thumbnail of a specific video
 * TODO: make the args: ($video_code, $field, $item), with $field/$item provided if we need it, but otherwise simplifying things
 *  @param $field
 *    the field of the requesting node
 *  @param $item
 *    the actual content of the field from the requesting node
 *  @return
 *    a URL pointing to the thumbnail
 */
function emvideo_vimeo_thumbnail($field, $item, $formatter, $node, $width, $height) {
  return media_vimeo_thumbnail_url($item['value']);
}

/**
 *  Implement hook_emvideo_PROVIDER_content_generate().
 */
function emvideo_vimeo_content_generate() {
  return array(
    'http://vimeo.com/1408463',
    'http://vimeo.com/1040532',
    'http://vimeo.com/709081',
    'http://vimeo.com/5565562',
  );
}

/**
 *  Implement hook emvideo_PROVIDER_data_version().
 */
function emvideo_vimeo_data_version() {
  return MEDIA_VIMEO_DATA_VERSION;
}

/**
 * Implement hook_emvideo_PROVIDER_data().
 */
function emvideo_vimeo_data($field, $item) {
  return media_vimeo_data($item['value']);
}

/**
 * hook emvideo_PROVIDER_duration($item)
 * Returns the duration of the video in seconds.
 *  @param $item
 *    The video item itself, which needs the $data array.
 *  @return
 *    The duration of the video in seconds.
 */
function emvideo_vimeo_duration($item) {
  return media_vimeo_duration($item['value']);
}
