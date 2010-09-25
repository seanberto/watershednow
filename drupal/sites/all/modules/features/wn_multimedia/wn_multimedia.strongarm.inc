<?php

/**
 * Implementation of hook_strongarm().
 */
function wn_multimedia_strongarm() {
  $export = array();
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_anonymous_multimedia';
  $strongarm->value = 0;

  $export['comment_anonymous_multimedia'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_controls_multimedia';
  $strongarm->value = '3';

  $export['comment_controls_multimedia'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_mode_multimedia';
  $strongarm->value = '4';

  $export['comment_default_mode_multimedia'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_order_multimedia';
  $strongarm->value = '1';

  $export['comment_default_order_multimedia'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_per_page_multimedia';
  $strongarm->value = '50';

  $export['comment_default_per_page_multimedia'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_form_location_multimedia';
  $strongarm->value = '0';

  $export['comment_form_location_multimedia'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_multimedia';
  $strongarm->value = '0';

  $export['comment_multimedia'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_preview_multimedia';
  $strongarm->value = '1';

  $export['comment_preview_multimedia'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_subject_field_multimedia';
  $strongarm->value = '1';

  $export['comment_subject_field_multimedia'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'content_extra_weights_multimedia';
  $strongarm->value = array(
    'title' => '-5',
    'revision_information' => '2',
    'author' => '1',
    'options' => '3',
    'comment_settings' => '4',
    'menu' => '0',
    'path' => '5',
  );

  $export['content_extra_weights_multimedia'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'emfield_emvideo_allow_archive';
  $strongarm->value = 0;

  $export['emfield_emvideo_allow_archive'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'emfield_emvideo_allow_bliptv';
  $strongarm->value = 0;

  $export['emfield_emvideo_allow_bliptv'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'emfield_emvideo_allow_dailymotion';
  $strongarm->value = 0;

  $export['emfield_emvideo_allow_dailymotion'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'emfield_emvideo_allow_flickr_sets';
  $strongarm->value = 1;

  $export['emfield_emvideo_allow_flickr_sets'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'emfield_emvideo_allow_google';
  $strongarm->value = 1;

  $export['emfield_emvideo_allow_google'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'emfield_emvideo_allow_guba';
  $strongarm->value = 0;

  $export['emfield_emvideo_allow_guba'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'emfield_emvideo_allow_imeem';
  $strongarm->value = 0;

  $export['emfield_emvideo_allow_imeem'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'emfield_emvideo_allow_lastfm';
  $strongarm->value = 0;

  $export['emfield_emvideo_allow_lastfm'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'emfield_emvideo_allow_livevideo';
  $strongarm->value = 0;

  $export['emfield_emvideo_allow_livevideo'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'emfield_emvideo_allow_metacafe';
  $strongarm->value = 0;

  $export['emfield_emvideo_allow_metacafe'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'emfield_emvideo_allow_myspace';
  $strongarm->value = 0;

  $export['emfield_emvideo_allow_myspace'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'emfield_emvideo_allow_revver';
  $strongarm->value = 0;

  $export['emfield_emvideo_allow_revver'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'emfield_emvideo_allow_sevenload';
  $strongarm->value = 0;

  $export['emfield_emvideo_allow_sevenload'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'emfield_emvideo_allow_spike';
  $strongarm->value = 0;

  $export['emfield_emvideo_allow_spike'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'emfield_emvideo_allow_tudou';
  $strongarm->value = 0;

  $export['emfield_emvideo_allow_tudou'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'emfield_emvideo_allow_twistage';
  $strongarm->value = 0;

  $export['emfield_emvideo_allow_twistage'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'emfield_emvideo_allow_ustream';
  $strongarm->value = 0;

  $export['emfield_emvideo_allow_ustream'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'emfield_emvideo_allow_ustreamlive';
  $strongarm->value = 0;

  $export['emfield_emvideo_allow_ustreamlive'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'emfield_emvideo_allow_vimeo';
  $strongarm->value = 1;

  $export['emfield_emvideo_allow_vimeo'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'emfield_emvideo_allow_voicethread';
  $strongarm->value = 0;

  $export['emfield_emvideo_allow_voicethread'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'emfield_emvideo_allow_yahoomusic';
  $strongarm->value = 0;

  $export['emfield_emvideo_allow_yahoomusic'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'emfield_emvideo_allow_zzz_custom_url';
  $strongarm->value = 0;

  $export['emfield_emvideo_allow_zzz_custom_url'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'emfield_imagerotator_url';
  $strongarm->value = '';

  $export['emfield_imagerotator_url'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'emfield_swfobject';
  $strongarm->value = 0;

  $export['emfield_swfobject'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'emfield_swfobject_location';
  $strongarm->value = '';

  $export['emfield_swfobject_location'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'emfield_thumbnail_overlay';
  $strongarm->value = 1;

  $export['emfield_thumbnail_overlay'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'emimage_flickr_api_key';
  $strongarm->value = '19067dc9fcab21d3a025dbe40ee59bc9';

  $export['emimage_flickr_api_key'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'emimage_flickr_api_secret';
  $strongarm->value = 'ee95152578315654';

  $export['emimage_flickr_api_secret'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'emvideo_bliptv_rss_format';
  $strongarm->value = 'flv';

  $export['emvideo_bliptv_rss_format'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'emvideo_bliptv_rss_tag';
  $strongarm->value = 'flv';

  $export['emvideo_bliptv_rss_tag'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'emvideo_google_domain';
  $strongarm->value = 'com';

  $export['emvideo_google_domain'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'enable_revisions_page_multimedia';
  $strongarm->value = 0;

  $export['enable_revisions_page_multimedia'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'media_flickr_max_saves';
  $strongarm->value = '10';

  $export['media_flickr_max_saves'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'media_flickr_store_local';
  $strongarm->value = 0;

  $export['media_flickr_store_local'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_multimedia';
  $strongarm->value = array(
    0 => 'status',
  );

  $export['node_options_multimedia'] = $strongarm;
  return $export;
}
