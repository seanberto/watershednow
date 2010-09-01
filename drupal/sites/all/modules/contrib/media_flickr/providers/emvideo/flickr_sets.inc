<?php
// $Id: flickr_sets.inc,v 1.1.2.26 2010/02/01 20:01:19 aaron Exp $

/**
 * @file
 *   This include processes flickr.com photosets for use by emfield/emvideo.module.
 */

/**
 *  This defines the version of the content data array that we serialize
 *  in emvideo_flickr_sets_data(). If we change the expected keys of that array,
 *  we must increment this value, which will allow older content to be updated
 *  to the new version automatically.
 */
define('EMVIDEO_FLICKR_SETS_DATA_VERSION', 1);

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
function emvideo_flickr_sets_info() {
  $features = array(
    array(t('Autoplay'), t('No'), ''),
    array(t('RSS Attachment'), t('No'), ''),
    array(t('Thumbnails'), t('Yes'), t('')),
    array(t('Full screen mode'), t('Yes'), t('Full screen mode is enabled for this player, but may not be disabled.')),
  );
  return array(
    'provider' => 'flickr_sets',
    'name' => t('Flickr Photosets'),
    'url' => MEDIA_FLICKR_MAIN_URL,
    'settings_description' => t('These settings specifically affect slideshows displayed from !flickr_sets.', array('!flickr_sets' => l(t('Flickr.com'), MEDIA_FLICKR_MAIN_URL))),
    'supported_features' => $features,
  );
}

/**
 * hook emvideo_PROVIDER_settings
 * this should return a subform to be added to the emvideo_settings() admin settings page.
 * note that a form field will already be provided, at $form['PROVIDER'] (such as $form['flickr_sets'])
 * so if you want specific provider settings within that field, you can add the elements to that form field.
 */
function emvideo_flickr_sets_settings() {
  if (!function_exists('emfield_imagerotator_url')) {
    drupal_set_message(t('You should upgrade to a newer version of !emfield if you wish support for the !player or !imagerotator.', array('!emfield' => l(t('Embedded Media Field'), 'http://drupal.org/project/emfield'), '!player' => l(t('JW Flash Media Player'), 'http://www.longtailvideo.com/players/jw-flv-player/'), '!imagerotator' => l(t('JW Image Rotator'), 'http://www.longtailvideo.com/players/jw-image-rotator/'))), 'error');
  }
  $form['flickr_sets']['media_flickr_store_local'] = array(
    '#type' => 'checkbox',
    '#title' => t('Store images locally'),
    '#description' => t('If checked, then images from Flickr will be stored locally.'),
    '#default_value' => variable_get('media_flickr_store_local', FALSE),
  );
  $form['flickr_sets']['media_flickr_max_saves'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum local saves per page load'),
    '#description' => t('This will limit the number of remote files that will be stored locally from Flickr per page load when storing a photoset locally, causing the rest in that specific view to be displayed from the remote location. If you set this to 0, then all such files will be stored on the initial slideshow view. Note that setting this to 0 or to an arbitrarily large number can cause the initial view to take a very long time, or even cause the browser to time out. Also note that this setting has no effect if the images are not to be stored locally.'),
    '#default_value' => variable_get('media_flickr_max_saves', 10),
  );
  if (function_exists('emimage_flickr_settings')) {
    $form['flickr_sets']['api'] = array(
      '#type' => 'fieldset',
      '#title' => t('Flickr API'),
      '#description' => t('You will first need to apply for an API Developer Key from the <a href="@flickr" target="_blank">Flickr Developer Profile page</a>.', array('@flickr' => MEDIA_FLICKR_API_APPLICATION_URL)),
      '#collapsible' => TRUE,
      '#collapsed' => (variable_get('emimage_flickr_api_key', '') != ''),
    );
    $form['flickr_sets']['api']['notice'] = array(
      '#type' => 'item',
      '#value' => t('Please enter your Flickr Developer Key in the fieldset for Flickr, in the Embedded Image Field field set above.'),
    );
  }
  else {
    $form['flickr_sets']['api'] = array(
      '#type' => 'fieldset',
      '#title' => t('Flickr API'),
      '#description' => t('You will first need to apply for an API Developer Key from the <a href="@flickr" target="_blank">Flickr Developer Profile page</a>.', array('@flickr' => MEDIA_FLICKR_API_APPLICATION_URL)),
      '#collapsible' => TRUE,
      '#collapsed' => (variable_get('emimage_flickr_api_key', '') != ''),
    );
    $form['flickr_sets']['api']['emimage_flickr_api_key'] = array(
      '#type' => 'textfield',
      '#title' => t('Flickr API Key'),
      '#default_value' => variable_get('emimage_flickr_api_key', ''),
      '#description' => t('Please enter your Flickr Developer Key here.'),
    );
    $form['flickr_sets']['api']['emimage_flickr_api_secret'] = array(
      '#type' => 'textfield',
      '#title' => t('Flickr API Secret'),
      '#default_value' => variable_get('emimage_flickr_api_secret', ''),
      '#description' => t('If you have a secret for the Flickr API, enter it here.'),
    );
  }
  return $form;
}

/**
 *  Implement hook emvideo_PROVIDER_data_version().
 */
function emvideo_flickr_sets_data_version() {
  return EMVIDEO_FLICKR_SETS_DATA_VERSION;
}

/**
 * hook emfield_PROVIDER_data
 *
 * provides an array to be serialised and made available with $item elsewhere
 */
function emvideo_flickr_sets_data($field, $item) {
  $data = array();
  $xml = media_flickr_sets_request('flickr.photosets.getPhotos', array('photoset_id' => $item['value'], 'per_page' => 1));
  $data['owner'] = $xml['photoset']['owner'];
  $data['first_photo'] = $xml['photoset']['photo'][0]['id'];
  $data['emvideo_data_version'] = EMVIDEO_FLICKR_SETS_DATA_VERSION;
  return $data;
}

/**
 *
 */
function emvideo_flickr_sets_rss($item, $teaser = NULL) {
}

function emvideo_flickr_sets_validate($value, $error_field) {
  if ($value == 'FLICKR_SETS_ERROR_SEARCH') {
    form_set_error($error_field, t("Flickr search-based slide shows are not currently supported; only slide shows of a specific photo set will be accepted."));
  }
  else if ($value == 'FLICKR_SETS_ERROR_USER') {
    form_set_error($error_field, t("Flickr user slide shows are not currently supported; only slide shows of a specific photo set will be accepted."));
  }
  else if ($value == 'FLICKR_SETS_ERROR_GROUPS') {
    form_set_error($error_field, t("Flickr groups slide shows are not currently supported; only slide shows of a specific photo set will be accepted."));
  }
}


/**
 * hook emvideo_PROVIDER_extract
 * this is called to extract the video code from a pasted URL or embed code.
 * @param $embed
 *   an optional string with the pasted URL or embed code
 * @return
 *   either an array of regex expressions to be tested, or a string with the video code to be used
 *   if the hook tests the code itself, it should return either the string of the video code (if matched), or an empty array.
 *   otherwise, the calling function will handle testing the embed code against each regex string in the returned array.
 */
function emvideo_flickr_sets_extract($embed = '') {
  // http://www.flickr_sets.com/watch/1404/saturday-night-live-snl-digital-short-natalie-raps
  // http://www.flickr.com/search/?q=Paris+City&l=comm&ss=0&ct=0&mt=photos&w=all&adv=1
  // http://www.flickr.com/search/show/?q=Paris+City&l=comm&ss=0&ct=0&mt=photos&adv=1
  // http://www.flickr.com/photos/petemorris/show/
  // http://www.flickr.com/groups/streamsimages/pool/show/
  if (preg_match('@\.flickr\.com/search.+?\?q=([^\&]+)@i', $embed, $matches)) {
    return 'FLICKR_SETS_ERROR_SEARCH';
  }
  else if (!($set = preg_match('@\.flickr\.com/.+?/.+?/sets/([^/\?]+)@i', $embed, $matches)) && preg_match('@\.flickr\.com/photos/([^/\?\#]+)@i', $embed, $matches)) {
    return 'FLICKR_SETS_ERROR_USER';
  }
  else if (!$set && preg_match('@\.flickr\.com/groups/([^/\?\#]+)@i', $embed, $matches)) {
    return 'FLICKR_SETS_ERROR_GROUPS';
  }
  return array(
    '@\.flickr\.com/.+?/.+?/sets/([^/\?]+)@i',
    '@sets%2F(.+?)%2Fshow.+?flickr\.com/apps/slideshow/show\.swf@i',
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
function emvideo_flickr_sets_embedded_link($video_code, $data = array()) {
  if (empty($data)) {
    $data = emvideo_flickr_sets_data(NULL, array('value' => $video_code));
  }
  return 'http://www.flickr.com/photos/'. $data['owner'] .'/sets/'. $video_code;
}

/**
 * The embedded flash displaying the flickr_sets video.
 */
function theme_emvideo_flickr_sets_flash($item, $width, $height, $autoplay, $options = array()) {
  $output = '';
  if ($item['value']) {
    $value = check_plain($item['value']);
    $embed = drupal_urlencode('photos/'. $item['data']['owner'] .'/sets/'. $value);
    $output = <<<EMBED
<object type="application/x-shockwave-flash" data="http://www.flickr.com/apps/slideshow/show.swf?v=71649" width="$width" height="$height">
  <param name="movie" value="http://www.flickr.com/apps/slideshow/show.swf?v=71649" />
  <param name="flashvars" value="offsite=true&amp;lang=en-us&amp;page_show_url=/$embed/show/&amp;page_show_back_url=/$embed&amp;set_id={$value}&amp;jump_to=" />
  <param name="allowFullScreen" value="true" />
</object>
EMBED;
  }
  return $output;
}

/**
 *  Outputs a custom player, such as JW Flash Player, and pulls in the XML
 *  feed for the videos.
 */
function theme_emvideo_flickr_sets_flvmediaplayer($item, $width, $height, $autoplay, $options = array()) {
  global $base_path;

  $formatter = isset($options['formatter']) ? $options['formatter'] : 'video';
  $delay = isset($options['delay']) ? $options['delay'] : (isset($options['field']['widget']['media_flickr_delay_'. $formatter]) ? $options['field']['widget']['media_flickr_delay_'. $formatter] : variable_get('media_flickr_options_delay', 10));

  // Currently, for best performance, we need the
  // Media: Flickr -- XSPF Playlist module to continue.
  if (module_exists('media_flickr_xspf')) {
    $captions = isset($options['captions']) ? $options['captions'] : (isset($options['field']['widget']['media_flickr_captions_'. $formatter]) ? $options['field']['widget']['media_flickr_captions_'. $formatter] : variable_get('media_flickr_xspf_caption_type', MEDIA_FLICKR_XSPF_CAPTION_TYPE_TITLE));
    $flashvars['file'] = url('media/flickr/photoset/'. $item['value'] .'/xspf/'. $delay .'/'. $width .'/'. $height .'/'. $captions);
  }
  else {
    // Grab a playlist from Flickr for this. However, this has a limit of
    // 20 items in the feed, and has no duration.
    $flashvars['file'] = url('media/flickr/remote/'. $item['value']);
  }

  $params['width'] = $width;
  $params['height'] = $height;
  $params['allowfullscreen'] = isset($options['allowfullscreen']) ? $options['allowfullscreen'] : (isset($options['field']['widget']['media_flickr_allowfullscreen_'. $formatter]) ? $options['field']['widget']['media_flickr_allowfullscreen_'. $formatter] : variable_get('media_flickr_options_allowfullscreen', TRUE));
  $params['allowfullscreen'] = $options['allowfullscreen'] ? 'true' : 'false';
  $params['allowscriptaccess'] = isset($options['allowscriptaccess']) ? $options['allowscriptaccess'] : (isset($options['field']['widget']['media_flickr_allowscriptaccess_'. $formatter]) ? $options['field']['widget']['media_flickr_allowscriptaccess_'. $formatter] : variable_get('media_flickr_options_allowscriptaccess', 'always'));

  $flashvars['autostart'] = $autoplay ? 'true' : 'false';
  $flashvars['playlist'] = isset($options['playlist']) ? $options['playlist'] : (isset($options['field']['widget']['media_flickr_playlist_'. $formatter]) ? $options['field']['widget']['media_flickr_playlist_'. $formatter] : variable_get('media_flickr_options_playlist', 'right'));
  $flashvars['playlistsize'] = isset($options['playlistsize']) ? $options['playlistsize'] : (isset($options['field']['widget']['media_flickr_playlistsize_'. $formatter]) ? $options['field']['widget']['media_flickr_playlistsize_'. $formatter] : variable_get('media_flickr_options_playlistsize', 86));
  $flashvars['repeat'] = isset($options['repeat']) ? $options['repeat'] : (isset($options['field']['widget']['media_flickr_repeat_'. $formatter]) ? $options['field']['widget']['media_flickr_repeat_'. $formatter] : variable_get('media_flickr_options_repeat', 'list'));
  $flashvars['controlbar'] = isset($options['controlbar']) ? $options['controlbar'] : (isset($options['field']['widget']['media_flickr_controlbar_'. $formatter]) ? $options['field']['widget']['media_flickr_controlbar_'. $formatter] : variable_get('media_flickr_options_controlbar', 'none'));

  $logo = isset($options['logo']) ? $options['logo'] : (isset($options['field']['widget']['media_flickr_logo_'. $formatter]) ? $options['field']['widget']['media_flickr_logo_'. $formatter] : variable_get('media_flickr_options_logo', ''));
  if ($logo) {
    $flashvars['logo'] = url($logo);
  }

  if (function_exists('emfield_flvmediaplayer_url')) {
    $path = emfield_flvmediaplayer_url();
  }
  if (module_exists('flvmediaplayer')) {
    if (!$path) {
      $path = variable_get('flvmediaplayer_path', drupal_get_path('module', 'flvmediaplayer') .'/mediaplayer.swf');
    }
    return theme('flvmediaplayer_render_player', $base_path . $path, $params, $flashvars);
  }
  else if ($path && module_exists('swfobject_api')) {
    return theme('swfobject_api', $base_path . $path, $params, $flashvars);
  }

  // Fallback if things fail from here. @TODO: Just embed the sucker.
  return theme('emvideo_flickr_sets_flash', $item, $width, $height, $autoplay, $options);
}

/**
 *  Outputs a custom player, such as JW Image Rotator, and pulls in the XML
 *  feed for the videos.
 */
function theme_emvideo_flickr_sets_imagerotator($item, $width, $height, $autoplay, $options = array()) {
  global $base_path;

  $formatter = isset($options['formatter']) ? $options['formatter'] : 'video';
  $delay = isset($options['rotatetime']) ? $options['rotatetime'] : (isset($options['field']['widget']['media_flickr_rotatetime_'. $formatter]) ? $options['field']['widget']['media_flickr_rotatetime_'. $formatter] : variable_get('media_flickr_options_rotatetime', 5));

  // Currently, for best performance, we need the
  // Media: Flickr -- XSPF Playlist module to continue.
  if (module_exists('media_flickr')) {
    $captions = isset($options['captions']) ? $options['captions'] : (isset($options['field']['widget']['media_flickr_ir_captions_'. $formatter]) ? $options['field']['widget']['media_flickr_ir_captions_'. $formatter] : variable_get('media_flickr_xspf_caption_type', MEDIA_FLICKR_XSPF_CAPTION_TYPE_TITLE));
    $flashvars['file'] = url('media/flickr/photoset/'. $item['value'] .'/xspf/'. $delay .'/'. $width .'/'. $height .'/'. $captions, array('absolute' => TRUE));
  }
  else {
    // Grab a playlist from Flickr for this. However, this has a limit of
    // 20 items in the feed, and has no duration.
    $flashvars['file'] = url('media/flickr/remote/'. $item['value'], array('absolute' => TRUE));
  }

  $params['width'] = $width;
  $params['height'] = $height;
  $params['allowfullscreen'] = isset($options['allowfullscreen']) ? $options['allowfullscreen'] : (isset($options['field']['widget']['media_flickr_ir_allowfullscreen_'. $formatter]) ? $options['field']['widget']['media_flickr_ir_allowfullscreen_'. $formatter] : variable_get('media_flickr_options_ir_allowfullscreen', TRUE));
  $params['allowfullscreen'] = $params['allowfullscreen'] ? 'true' : 'false';
  $params['allowscriptaccess'] = isset($options['allowscriptaccess']) ? $options['allowscriptaccess'] : (isset($options['field']['widget']['media_flickr_ir_allowscriptaccess_'. $formatter]) ? $options['field']['widget']['media_flickr_ir_allowscriptaccess_'. $formatter] : variable_get('media_flickr_options_ir_allowscriptaccess', 'always'));

  // The following behaviors are defined by the Image Rotator flash itself.
  $flashvars['usefullscreen'] = $params['allowfullscreen'];
  $flashvars['autostart'] = $autoplay ? 'true' : 'false';
  $flashvars['repeat'] = isset($options['repeat']) ? $options['repeat'] : (isset($options['field']['widget']['media_flickr_ir_repeat_'. $formatter]) ? $options['field']['widget']['media_flickr_ir_repeat_'. $formatter] : variable_get('media_flickr_options_ir_repeat', 'list'));
  $flashvars['shownavigation'] = isset($options['shownavigation']) ? $options['shownavigation'] : (isset($options['field']['widget']['media_flickr_ir_shownavigation_'. $formatter]) ? $options['field']['widget']['media_flickr_ir_shownavigation_'. $formatter] : variable_get('media_flickr_options_ir_shownavigation', TRUE));
  $flashvars['shownavigation'] = $flashvars['shownavigation'] ? 'true' : 'false';
  $flashvars['showicons'] = isset($options['showicons']) ? $options['showicons'] : (isset($options['field']['widget']['media_flickr_ir_showicons_'. $formatter]) ? $options['field']['widget']['media_flickr_ir_showicons_'. $formatter] : variable_get('media_flickr_options_ir_showicons', TRUE));
  $flashvars['showicons'] = $flashvars['showicons'] ? 'true' : 'false';
  $flashvars['shuffle'] = isset($options['shuffle']) ? $options['shuffle'] : (isset($options['field']['widget']['media_flickr_ir_shuffle_'. $formatter]) ? $options['field']['widget']['media_flickr_ir_shuffle_'. $formatter] : variable_get('media_flickr_options_ir_shuffle', FALSE));
  $flashvars['shuffle'] = $flashvars['shuffle'] ? 'true' : 'false';
  $flashvars['transition'] = isset($options['transition']) ? $options['transition'] : (isset($options['field']['widget']['media_flickr_ir_transition_'. $formatter]) ? $options['field']['widget']['media_flickr_ir_transition_'. $formatter] : variable_get('media_flickr_options_ir_transition', 'random'));

  $logo = isset($options['logo']) ? $options['logo'] : (isset($options['field']['widget']['media_flickr_ir_logo_'. $formatter]) ? $options['field']['widget']['media_flickr_ir_logo_'. $formatter] : variable_get('media_flickr_options_ir_logo', ''));
  if ($logo) {
    $flashvars['logo'] = url($logo);
  }

  // Comply with http://www.w3.org/TR/WCAG10-HTML-TECHS/#applet-text-equivalent
  // and add a text element to the object when embedding directly.
  $accessibility_title = isset($options['title']) ? $options['title'] : variable_get('media_flickr_sets_title_default', t('Flickr Photoset'));

  if (function_exists('emfield_imagerotator_url')) {
    $path = emfield_imagerotator_url();
  }
  if ($path && variable_get('emfield_swfobject', FALSE) && module_exists('swfobject_api')) {
    return theme('swfobject_api', $base_path . $path, $params, $flashvars);
  }
  else if ($path) {
    static $count;
    $id = 'emvideo-flickr-'. $count++;
    $path = $base_path . $path;
    // Width & Height need to be set explicity in the flashvars, or else it
    // doesn't display properly with wmode transparent.
    $flashvars['width'] = $width;
    $flashvars['height'] = $height;
    $flashvars = drupal_query_string_encode($flashvars);

    // Fix IE security popup; see http://jira.atlassian.com/browse/CONF-6284.
    $base_root = (isset($_SERVER['HTTPS']) && $_SERVER['HTTPS'] == 'on') ? 'https' : 'http';

    $output = <<<EMBED
    <div id="$id">
      <!--[if !IE]> -->
        <object type="application/x-shockwave-flash" data="$path" name="rotator" width="$width" height="$height">
          $accessibility_title
      <!-- <![endif]-->

      <!--[if IE]>
        <object classid="clsid:D27CDB6E-AE6D-11cf-96B8-444553540000"
        codebase="$base_root://download.macromedia.com/pub/shockwave/cabs/flash/swflash.cab#version=6,0,0,0"
        width="$width" height="$height">
        <param name="movie" value="$path" />
        <!--><!--dgx-->
        <param name="allowfullscreen" value="true" />
        <param name="wmode" value="transparent" />
        <param name="flashvars" value="$flashvars" />
        </object>
      <!-- <![endif]-->
    </div>
EMBED;
    return $output;
  }

  // Fallback if things fail from here.
  return theme('emvideo_flickr_sets_flash', $item, $width, $height, $autoplay, $options);
}

function theme_emvideo_flickr_sets_custom($item, $width, $height, $autoplay, $options = array()) {
  return theme('emvideo_flickr_sets_flvmediaplayer', $item, $width, $height, $autoplay, $options);
}

/**
 * hook emvideo_PROVIDER_thumbnail
 * returns the external url for a thumbnail of a specific video
 * TODO: make the args: ($embed, $field, $item), with $field/$item provided if we need it, but otherwise simplifying things
 *  @param $field
 *    the field of the requesting node
 *  @param $item
 *    the actual content of the field from the requesting node
 *  @return
 *    a URL pointing to the thumbnail
 */
function emvideo_flickr_sets_thumbnail($field, $item, $formatter, $node, $width, $height, $options = array()) {
  if ($item['value']) {
    return media_flickr_photo_url($item['data']['first_photo'], $width, $height);
  }
}

/**
 * hook emvideo_PROVIDER_video
 * This actually displays the full/normal-sized video we want, usually on the default page view
 *  @param $embed
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
function emvideo_flickr_sets_video($embed, $width, $height, $field, $item, &$node, $autoplay, $options = array()) {
  $options['node'] = $options['node'] ? $options['node'] : $node;
  $options['field'] = $options['field'] ? $options['field'] : $field;
  $options['player'] = $options['player'] ? $options['player'] : ($field['widget']['media_flickr_player_video'] ? $field['widget']['media_flickr_player_video'] : variable_get('media_flickr_player', 'flash'));
  $options['formatter'] = $options['formatter'] ? $options['formatter'] : 'video';
  $output = theme('emvideo_flickr_sets_'. $options['player'], $item, $width, $height, $autoplay, $options);
  return $output;
}

/**
 * hook emvideo_PROVIDER_video
 * This actually displays the preview-sized video we want, commonly for the teaser
 *  @param $embed
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
function emvideo_flickr_sets_preview($embed, $width, $height, $field, $item, &$node, $autoplay, $options = array()) {
  $options['node'] = $options['node'] ? $options['node'] : $node;
  $options['field'] = $options['field'] ? $options['field'] : $field;
  $options['player'] = $options['player'] ? $options['player'] : ($field['widget']['media_flickr_player_preview'] ? $field['widget']['media_flickr_player_preview'] : variable_get('media_flickr_player', 'flash'));
  $options['formatter'] = $options['formatter'] ? $options['formatter'] : 'preview';
  $output = theme('emvideo_flickr_sets_'. $options['player'], $item, $width, $height, $autoplay, $options);
  return $output;
}

/**
 * Implementation of hook_emfield_subtheme.
 */
function emvideo_flickr_sets_emfield_subtheme() {
  $themes = array(
    'emvideo_flickr_sets_flash'  => array(
        'arguments' => array('item' => NULL, 'width' => NULL, 'height' => NULL, 'autoplay' => NULL, 'options' => array()),
        'file' => 'providers/emvideo/flickr_sets.inc',
        'path' => drupal_get_path('module', 'media_flickr'),
    ),
    'emvideo_flickr_sets_imagerotator' => array(
        'arguments' => array('item' => NULL, 'width' => NULL, 'height' => NULL, 'autoplay' => NULL, 'options' => array()),
        'file' => 'providers/emvideo/flickr_sets.inc',
        'path' => drupal_get_path('module', 'media_flickr'),
    ),
    'emvideo_flickr_sets_flvmediaplayer' => array(
        'arguments' => array('item' => NULL, 'width' => NULL, 'height' => NULL, 'autoplay' => NULL, 'options' => array()),
        'file' => 'providers/emvideo/flickr_sets.inc',
        'path' => drupal_get_path('module', 'media_flickr'),
    ),
    'emvideo_flickr_sets_custom' => array(
        'arguments' => array('item' => NULL, 'width' => NULL, 'height' => NULL, 'autoplay' => NULL, 'options' => array()),
        'file' => 'providers/emvideo/flickr_sets.inc',
        'path' => drupal_get_path('module', 'media_flickr'),
    ),
  );
  return $themes;
}
