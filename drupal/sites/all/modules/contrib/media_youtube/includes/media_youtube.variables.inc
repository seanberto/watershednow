<?php
// $Id: media_youtube.variables.inc,v 1.1.2.12 2011/02/04 17:17:25 aaron Exp $

/**
 * @file Contains the variables and defaults used by Media: YouTube.
 */

/**
 * The variable namespace for Media: YouTube.
 */
define('MEDIA_YOUTUBE_NAMESPACE', 'media_youtube__');

/**
 * This is the main URL for YouTube.
 */
define('MEDIA_YOUTUBE_MAIN_URL', 'http://youtube.com/');

/**
 * This is the URL to the API of YouTube.
 */
define('MEDIA_YOUTUBE_API_INFO', 'http://code.google.com/apis/youtube/getting_started.html');
define('MEDIA_YOUTUBE_API_APPLICATION_URL', 'http://code.google.com/apis/youtube/dashboard/');
define('MEDIA_YOUTUBE_REST_ENDPOINT', 'http://gdata.youtube.com/feeds/api/videos');

/**
 * This defines the version of the content data array that we serialize
 * in emvideo_youtube_data(). If we change the expected keys of that array,
 * we must increment this value, which will allow older content to be updated
 * to the new version automatically.
 */
define('MEDIA_YOUTUBE_DATA_VERSION', 5);

/**
 * These are player defaults. @TODO: Use variable namespace instead.
 */
define('MEDIA_YOUTUBE_COLOR1_DEFAULT', '#FFFFFF');
define('MEDIA_YOUTUBE_COLOR2_DEFAULT', '#CCCCCC');

/**
 * YouTube API required constants.
 */
define('MEDIA_YOUTUBE_AUTH_URL', 'https://www.google.com/youtube/accounts/ClientLogin');
define('MEDIA_YOUTUBE_UPLOAD_URL', 'http://uploads.gdata.youtube.com/feeds/users/default/uploads');
define('MEDIA_YOUTUBE_APP_ID', 'Media: Youtube Uploader');

define('MEDIA_YOUTUBE_STATUS_UPDATE_NONE', 0);
define('MEDIA_YOUTUBE_STATUS_UPDATE_FROM_UNAVAILABLE', 1);
define('MEDIA_YOUTUBE_STATUS_UPDATE_ALWAYS', 2);

define('MEDIA_YOUTUBE_STATUS_UNAVAILABLE', 0x0000);
define('MEDIA_YOUTUBE_STATUS_AVAILABLE', 0x0001);
define('MEDIA_YOUTUBE_STATUS_PRIVATE', 0x0002);

/**
 * Wrapper for variable_get() that uses the Media: YouTube variable registry.
 *
 * @param string $name
 *  The variable name to retrieve. Note that it will be namespaced by
 *  pre-pending MEDIA_YOUTUBE_NAMESPACE, as to avoid variable collisions
 *  with other modules.
 * @param unknown $default
 *  An optional default variable to return if the variable hasn't been set
 *  yet. Note that within this module, all variables should already be set
 *  in the media_youtube_variable_default() function.
 * @return unknown
 *  Returns the stored variable or its default.
 *
 * @see media_youtube_variable_set()
 * @see media_youtube_variable_del()
 * @see media_youtube_variable_default()
 */
function media_youtube_variable_get($name, $default = NULL) {
  // Allow for an override of the default.
  // Useful when a variable is required (like $path), but namespacing still desired.
  if (!isset($default)) {
    $default = media_youtube_variable_default($name);
  }
  // Namespace all variables
  $variable_name = MEDIA_YOUTUBE_NAMESPACE . $name;
  return variable_get($variable_name, $default);
}

/**
 * Wrapper for variable_set() that uses the Media: YouTube variable registry.
 *
 * @param string $name
 *  The variable name to set. Note that it will be namespaced by
 *  pre-pending MEDIA_YOUTUBE_NAMESPACE, as to avoid variable collisions with
 *  other modules.
 * @param unknown $value
 *  The value for which to set the variable.
 * @return unknown
 *  Returns the stored variable after setting.
 *
 * @see media_youtube_variable_get()
 * @see media_youtube_variable_del()
 * @see media_youtube_variable_default()
 */
function media_youtube_variable_set($name, $value) {
  $variable_name = MEDIA_YOUTUBE_NAMESPACE . $name;
  return variable_set($variable_name, $value);
}

/**
 * Wrapper for variable_del() that uses the Media: YouTube variable registry.
 *
 * @param string $name
 *  The variable name to delete. Note that it will be namespaced by
 *  pre-pending MEDIA_YOUTUBE_NAMESPACE, as to avoid variable collisions with
 *  other modules.
 *
 * @see media_youtube_variable_get()
 * @see media_youtube_variable_set()
 * @see media_youtube_variable_default()
 */
function media_youtube_variable_del($name) {
  $variable_name = MEDIA_YOUTUBE_NAMESPACE . $name;
  variable_del($variable_name);
}

/**
 * The default variables within the Media: YouTube namespace.
 *
 * @param string $name
 *  Optional variable name to retrieve the default. Note that it has not yet
 *  been pre-pended with the MEDIA_YOUTUBE_NAMESPACE namespace at this time.
 * @return unknown
 *  The default value of this variable, if it's been set, or NULL, unless
 *  $name is NULL, in which case we return an array of all default values.
 *
 * @see media_youtube_variable_get()
 * @see media_youtube_variable_set()
 * @see media_youtube_variable_del()
 */
function media_youtube_variable_default($name = NULL) {
  static $defaults;

  if (!isset($defaults)) {
    $defaults = array(
      // The YouTube API developer key.
      'api_key' => '',

      // Default YouTube user & password.
      'youtube_username' => '',
      'youtube_password' => '',

      // Player display options.
      'show_related_videos' => 0,
      'show_colors' => FALSE,
      'show_border' => FALSE,
      'colors_color1' => MEDIA_YOUTUBE_COLOR1_DEFAULT,
      'colors_color2' => MEDIA_YOUTUBE_COLOR2_DEFAULT,
      'full_screen' => 1,
      'high_quality' => FALSE,
      'display_info' => FALSE,
      'enablejsapi' => TRUE,

      // Use Longtail/JW FLV Media Player to display videos?
      'use_jw_flv' => FALSE,

      // Zend path.
      'zend_path' => '',

      // Status message to display when a video is unavailable.
      'status_display_'. EMFIELD_STATUS_UNAVAILABLE => '',

      // Whether to update the status on the fly.
      'status_update_frequency' => MEDIA_YOUTUBE_STATUS_UPDATE_NONE,

      // Status message to display for unavailable duplicate videos.
      'status_display_duplicates' => '',
      'check_for_duplicates' => FALSE,

      // Status message to display for rejected videos.
      'status_display_rejected' => '',
      'check_for_rejected' => FALSE,

      // Status message to display for failed video uploads.
      'status_display_upload_failed' => '',
      'check_for_failed' => FALSE,

      'cron_time' => 10,
      'cron_limit' => 50,

      // If TRUE, then store the entire metadata fetched from YouTube.
      'store_raw_metadata' => FALSE,

      // HTML5 Iframe variables.
      'iframe_title' => 'YouTube video player',
      'html5_player' => FALSE,

      // We accidentally destroyed all thumbnails in a bad update.
      // If that's been run improperly, then this will stay FALSE, and
      // we need to self-correct.
      'media_youtube_update_6012' => FALSE,
    );
  }

  if (!isset($name)) {
    return $defaults;
  }

  if (isset($defaults[$name])) {
    return $defaults[$name];
  }
}

/**
 * Return the fully namespace variable name.
 *
 * @param string $name
 *  The variable name to retrieve the namespaced name.
 * @return string
 *  The fully namespace variable name, prepended with
 *  MEDIA_YOUTUBE_NAMESPACE.
 */
function media_youtube_variable_name($name) {
  return MEDIA_YOUTUBE_NAMESPACE . $name;
}
