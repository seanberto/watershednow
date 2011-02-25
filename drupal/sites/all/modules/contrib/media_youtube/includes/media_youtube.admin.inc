<?php
// $Id: media_youtube.admin.inc,v 1.1.2.11 2011/02/04 16:06:40 aaron Exp $

/**
 * @file
 * Administrative functions for Media: YouTube.
 */

/**
 * The administrative settings form for Media: YouTube.
 */
function media_youtube_settings() {
  $form = media_youtube_admin_form();
  $form['settings_info'] = array(
    '#type' => 'item',
    '#value' => t('These settings specifically affect videos displayed from !youtube. You can also read more about its !api.', array('!youtube' => l(t('YouTube.com'), MEDIA_YOUTUBE_MAIN_URL, array('attributes' => array('target' => '_blank'))), '!api' => l(t("developer's API"), MEDIA_YOUTUBE_API_INFO, array('attributes' => array('target' => '_blank'))))),
    '#weight' => -10,
  );
  return system_settings_form($form);
}

/**
 * This form will be displayed both at /admin/settings/media_youtube and
 * admin/content/emfield.
 */
function media_youtube_admin_form() {
  // Find the Zend library.
  $zend_path = media_youtube_zend_path();

  if (!$zend_path && module_exists('media_mover_api')) {
    drupal_set_message(t("For complete video upload capabilities, you need to download the !zend and extract the entire contents of the archive into the %path folder of your server.", array('!zend' => l(t('Zend PHP library'), 'http://framework.zend.com/download/gdata/', array('attributes' => array('target' => '_blank'))), '%path' => 'sites/all/libraries')), 'warning');
  }

  $form = array();

  $form['player'] = array(
    '#type' => 'fieldset',
    '#title' => t('Player settings'),
    '#description' => t('These settings are for the various !players.', array('!players' => l(t('YouTube players'), 'http://www.youtube.com/custom_player', array('attributes' => array('target' => '_blank'))))),
    '#collapsible' => TRUE,
  );
  $form['player'][media_youtube_variable_name('show_related_videos')] = array(
    '#type' => 'checkbox',
    '#title' => t('Show related videos'),
    '#default_value' => media_youtube_variable_get('show_related_videos'),
    '#description' => t('If checked, then when playing a video from YouTube, users may hover over the video to see thumbnails & links to related videos.'),
  );
  $form['player']['colors'] = array(
    '#type' => 'fieldset',
    '#title' => t('YouTube video player Colors'),
    '#description' => t('If allowed, these two colors, in hexidecimal form (#RRGGBB), will be used to skin the YouTube player.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['player']['colors'][media_youtube_variable_name('show_colors')] = array(
    '#type' => 'checkbox',
    '#title' => t('Override player colors'),
    '#default_value' => media_youtube_variable_get('show_colors'),
  );
  $form['player']['colors'][media_youtube_variable_name('show_border')] = array(
    '#type' => 'checkbox',
    '#title' => t('Display border around player'),
    '#default_value' => media_youtube_variable_get('show_border'),
  );
  $form['player']['colors'][media_youtube_variable_name('colors_color1')] = array(
    '#type' => 'textfield',
    '#title' => t('Color 1'),
    '#default_value' => media_youtube_variable_get('colors_color1'),
  );
  $form['player']['colors'][media_youtube_variable_name('colors_color2')] = array(
    '#type' => 'textfield',
    '#title' => t('Color 2'),
    '#default_value' => media_youtube_variable_get('colors_color2'),
  );

  if (module_exists('colorpicker')) {
    $form['player']['colors'][media_youtube_variable_name('colors_color1')]['#type'] = 'colorpicker_textfield';
    $form['player']['colors'][media_youtube_variable_name('colors_color1')]['#colorpicker'] = 'colorpicker_1';
    $form['player']['colors'][media_youtube_variable_name('colors_color2')]['#type'] = 'colorpicker_textfield';
    $form['player']['colors'][media_youtube_variable_name('colors_color2')]['#colorpicker'] = 'colorpicker_2';
    $form['player']['colors']['colorpicker_1'] = array(
      '#type' => 'colorpicker',
      '#title' => t('Color 1 picker'),
      '#description' => t('Click in this textfield to start picking your color'),
    );
    $form['player']['colors']['colorpicker_2'] = array(
      '#type' => 'colorpicker',
      '#title' => t('Color 2 picker'),
      '#description' => t('Click in this textfield to start picking your color'),
    );
  }
  else {
    $form['player']['colors']['#description'] .= ' '. t('The !colorpicker module, if active, gives an easy way to select these colors.', array('!colorpicker' => l(t('Colorpicker'), 'http://drupal.org/project/colorpicker', array('attributes' => array('target' => '_blank')))));
  }

  $form['player']['player_options'] = array(
    '#type' => 'fieldset',
    '#title' => t('YouTube video player options'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['player']['player_options'][media_youtube_variable_name('full_screen')] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow fullscreen'),
    '#default_value' => media_youtube_variable_get('full_screen'),
    '#description' => t('Allow users to view video using the entire computer screen.'),
  );
  $form['player']['player_options'][media_youtube_variable_name('high_quality')] = array(
    '#type' => 'checkbox',
    '#title' => t('Use YouTube high quality content'),
    '#default_value' => media_youtube_variable_get('high_quality'),
    '#description' => t("If checked, then a parameter will be set to request high quality content.  Note: Not all videos from youtube are available in high quality. Those that aren't will play in normal quality."),
  );
   $form['player']['player_options'][media_youtube_variable_name('display_info')] = array(
    '#type' => 'checkbox',
    '#title' => t('Remove titles and ratings on videos'),
    '#default_value' => media_youtube_variable_get('display_info'),
    '#description' => t("If checked, additional info, such as title and rating, will NOT be displayed on the YouTube player."),
  );

  $form['player']['player_options'][media_youtube_variable_name('html5_player')] = array(
    '#type' => 'checkbox',
    '#title' => t('Use the HTML5 player instead of the Flash-based'),
    '#default_value' => media_youtube_variable_get('html5_player'),
    '#description' => t("If checked, the HTML5 capable player will be used instead of the regular flash-based. This is achieved as an iframe."),
  );

  $form['player']['jwflv'] = array(
    '#type' => 'fieldset',
    '#title' => t('JW FLV Media Player Options'),
    '#collapsible' => TRUE,
    '#collapsed' => !($flv_path = emfield_flvmediaplayer_url()),
  );
  $form['player']['jwflv'][media_youtube_variable_name('use_jw_flv')] = array(
    '#type' => 'checkbox',
    '#title' => t('Use JW FLV Media Player'),
    '#description' => t('If checked, and the server is properly configured, YouTube videos will be played using !flvplayer.', array('!flvplayer' => l(t('JW FLV Media Player'), 'http://www.longtailvideo.com/players/jw-flv-player/', array('attributes' => array('target' => '_blank'))))),
    '#default_value' => media_youtube_variable_get('use_jw_flv'),
  );
  if (!$flv_path) {
    $form['player']['jwflv']['#description'] = t('You must first install the !flvplayer, preferably in your /sites/all/plugins folder.', array('!flvplayer' => l(t('JW FLV Media Player'), 'http://www.longtailvideo.com/players/jw-flv-player/')));
  }
  if ($flv_path && !_media_youtube_check_flv_player_setup()) {
    $form['player']['jwflv']['#description'] = t('You must have the yt.swf file installed in the same folder as your !flvplayer.', array('!flvplayer' => l(t('JW FLV Media Player'), 'http://www.longtailvideo.com/players/jw-flv-player/')));
  }
  $form['player']['jwflv']['#description'] = $form['jwflv']['#description'] ? $form['jwflv']['#description'] : t('!flvplayer is properly configured on your server, so may be used to play YouTube videos if desired.', array('!flvplayer' => l(t('JW FLV Media Player'), 'http://www.longtailvideo.com/players/jw-flv-player/', array('attributes' => array('target' => '_blank')))));



  $form['api'] = array(
    '#type' => 'fieldset',
    '#title' => t('YouTube API'),
    '#description' => t('If you wish to allow video uploads via the Media Mover module, you will need to apply for an API Developer Key from the <a href="@youtube" target="_blank">YouTube Developer Profile page</a>. Note that you do not need this key to display YouTube videos or their thumbnails.', array('@youtube' => MEDIA_YOUTUBE_API_APPLICATION_URL)),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['api'][media_youtube_variable_name('api_key')] = array(
    '#type' => 'textfield',
    '#title' => t('YouTube developer key'),
    '#default_value' => media_youtube_variable_get('api_key'),
    '#description' => t('Please enter your YouTube Developer Key here.'),
  );
  $form['api'][media_youtube_variable_name('youtube_username')] = array(
    '#type' => 'textfield',
    '#title' => t('YouTube username'),
    '#default_value' => media_youtube_variable_get('youtube_username'),
    '#description' => t('If you plan to upload videos, you will need to enter the YouTube account username here.'),
  );
  $form['api'][media_youtube_variable_name('youtube_password')] = array(
    '#type' => 'password',
    '#title' => t('YouTube password'),
    '#default_value' => media_youtube_variable_get('youtube_password'),
    '#description' => t('If you plan to upload videos, you will need to enter the YouTube account password here.'),
  );
  $form['api'][media_youtube_variable_name('store_raw_metadata')] = array(
    '#type' => 'checkbox',
    '#title' => t('Store raw metadata'),
    '#description' => t('If checked, then store the full metadata supplied by the YouTube API. Note that the Media: YouTube module does not need this, so only check it if directed to by another module, or if you know you need it for custom purposes.'),
    '#default_value' => media_youtube_variable_get('store_raw_metadata'),
  );

  $form['status'] = array(
    '#type' => 'fieldset',
    '#title' => t('Video status'),
    '#description' => t('Settings concerning the status of a video.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['status'][media_youtube_variable_name('status_display_'. EMFIELD_STATUS_UNAVAILABLE)] = array(
    '#type' => 'textarea',
    '#title' => t('Status unavailable message'),
    '#default_value' => media_youtube_variable_get('status_display_'. EMFIELD_STATUS_UNAVAILABLE),
    '#description' => t('The message to display if a video is unavailable. Note that if this is blank, then no attempt will be made to hide an unavailable video, regardless of the other settings. Note also that this message may contain HTML markup, and may use token replacements.'),
  );
  $form['status']['duplicates'] = array(
    '#type' => 'fieldset',
    '#title' => t('Duplicate status'),
    '#description' => t('Settings concerning duplicate videos.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['status']['duplicates'][media_youtube_variable_name('check_for_duplicates')] = array(
    '#type' => 'checkbox',
    '#title' => t('Check for duplicates'),
    '#description' => t('If checked, then an attempt will be made to discover if an unavailable video was rejected for being duplicate, and if so, display the following message.'),
    '#default_value' => media_youtube_variable_get('check_for_duplicates'),
  );
  $form['status']['duplicates'][media_youtube_variable_name('status_display_duplicates')] = array(
    '#type' => 'textarea',
    '#title' => t('Status duplicate message'),
    '#default_value' => media_youtube_variable_get('status_display_duplicates'),
    '#description' => t('The message to display if a video is unavailable because it is duplicate. Note that if this is blank, then the message will fall back to the unavailable message above. Note also that this message may contain HTML markup, and may use token replacements.'),
  );
  $form['status']['rejected'] = array(
    '#type' => 'fieldset',
    '#title' => t('Rejected status'),
    '#description' => t('Settings concerning duplicate videos.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['status']['rejected'][media_youtube_variable_name('check_for_rejected')] = array(
    '#type' => 'checkbox',
    '#title' => t('Check for rejected uploads'),
    '#description' => t('If checked, then an attempt will be made to discover if an unavailable video was rejected for any other reason, and if so, display the following message.'),
    '#default_value' => media_youtube_variable_get('check_for_rejected'),
  );
  $form['status']['rejected'][media_youtube_variable_name('status_display_rejected')] = array(
    '#type' => 'textarea',
    '#title' => t('Status rejected message'),
    '#default_value' => media_youtube_variable_get('status_display_rejected'),
    '#description' => t('The message to display if a video is unavailable because it is rejected. Note that if this is blank, then the message will fall back to the unavailable message above. Note also that this message may contain HTML markup, and may use token replacements.'),
  );
  $form['status']['failed'] = array(
    '#type' => 'fieldset',
    '#title' => t('Failed status'),
    '#description' => t('Settings concerning duplicate videos.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['status']['failed'][media_youtube_variable_name('check_for_failed')] = array(
    '#type' => 'checkbox',
    '#title' => t('Check for failed uploads'),
    '#description' => t('If checked, then an attempt will be made to discover if the upload of an unavailable video failed for any other reason, and if so, display the following message.'),
    '#default_value' => media_youtube_variable_get('check_for_failed'),
  );
  $form['status']['failed'][media_youtube_variable_name('status_display_upload_failed')] = array(
    '#type' => 'textarea',
    '#title' => t('Status upload failed message'),
    '#default_value' => media_youtube_variable_get('status_display_upload_failed'),
    '#description' => t('The message to display if a video is unavailable because its upload failed. Note that if this is blank, then the message will fall back to the unavailable message above. Note also that this message may contain HTML markup, and may use token replacements.'),
  );

  $form['status']['tokens'] = array(
    '#type' => 'fieldset',
    '#title' => t('Replacement tokens'),
    '#description' => t('You may use any of the following tokens in the status messages above; they will be replaced by the specified values.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['status']['tokens']['token_help'] = array(
    '#type' => 'markup',
    '#value' => theme('token_help', array('media_youtube_status')),
  );

  $form['status']['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced status settings'),
    '#description' => t('Cron status update functionality.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $options = array(
    MEDIA_YOUTUBE_STATUS_UPDATE_NONE => t('Never update'),
    MEDIA_YOUTUBE_STATUS_UPDATE_FROM_UNAVAILABLE => t('Update only from unavailable'),
    MEDIA_YOUTUBE_STATUS_UPDATE_ALWAYS => t('Update always'),
  );
  $form['status']['advanced'][media_youtube_variable_name('status_update_frequency')] = array(
    '#type' => 'select',
    '#title' => t('Status update frequency'),
    '#options' => $options,
    '#default_value' => media_youtube_variable_get('status_update_frequency'),
    '#description' => t('Please select the frequency you wish to update the status of individual videos. If you select %from_unavailable, the status will only be rechecked if a video is unavailable, until it has finally been marked by YouTube as being available. If you select %always, the status will be checked always. Otherwise, the status will never be checked, once set initially. Note that the results are always cached, so the YouTube API is kept happy.', array('%from_unavailable' => t('Update only from unavailable'), '%always' => t('Update always'))),
  );
  $options = array(
    0 => 'Always',
    5 => '5 minutes',
    10 => '10 minutes',
    15 => '15 minutes',
    30 => '30 minutes',
    60 => '1 hour',
    120 => '2 hours',
    240 => '4 hours',
    480 => '8 hours',
    60 * 24 => '1 day',
    60 * 24 * 2 => '2 days',
    60 * 24 * 4 => '4 days',
    60 * 24 * 7 => '1 week',
  );
  $form['status']['advanced'][media_youtube_variable_name('cron_time')] = array(
    '#type' => 'select',
    '#title' => t('Cron time limit'),
    '#options' => $options,
    '#default_value' => media_youtube_variable_get('cron_time'),
    '#description' => t('YouTube videos will only be checked for a new status if at least this much time has passed since they were last checked. If the site has a lot of videos, you might consider raising this value so that the YouTube API gods stay happy.'),
  );
  $options = array(
    0 => 'All',
    1 => 1,
    2 => 2,
    5 => 5,
    10 => 10,
    25 => 25,
    50 => 50,
    100 => 100,
    200 => 200,
    250 => 250,
    500 => 500,
  );
  $form['status']['advanced'][media_youtube_variable_name('cron_limit')] = array(
    '#type' => 'select',
    '#title' => t('Cron video count limit'),
    '#options' => $options,
    '#default_value' => media_youtube_variable_get('cron_limit'),
    '#description' => t('Only up to this many videos will be checked for a new status during each cron run. If you are experiencing cron time outs, you might consider reducing this limit.'),
  );

  $form['#submit'][] = 'media_youtube_settings_submit';

  return $form;
}
