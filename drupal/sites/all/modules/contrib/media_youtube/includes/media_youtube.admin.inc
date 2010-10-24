<?php
// $Id: media_youtube.admin.inc,v 1.1.2.1 2010/04/08 20:35:49 aaron Exp $

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

  if (!$zend_path) {
    drupal_set_message(t("You need to download the !zend and extract the entire contents of the archive into the %path folder of your server.", array('!zend' => l(t('Zend PHP library'), 'http://framework.zend.com/download/gdata/', array('attributes' => array('target' => '_blank'))), '%path' => 'sites/all/libraries')), 'error');
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
  if ($flv_path && !_emvideo_youtube_check_flv_player_setup()) {
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
    '#title' => t('YouTube Developer Key'),
    '#default_value' => media_youtube_variable_get('api_key'),
    '#description' => t('Please enter your YouTube Developer Key here.'),
  );

  return $form;
}
