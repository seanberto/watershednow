<?php
// $Id: media_flickr.cck.inc,v 1.1.2.7 2009/12/03 19:07:28 aaron Exp $

/**
 *  @file
 *  CCK settings functions for Media: Flickr.
 */

function _media_flickr_emfield_widget_settings_extra($op, $widget) {
  if ($op == 'form') {
    if ($widget['module'] == 'emvideo') {
      $form['media_flickr'] = array(
        '#type' => 'fieldset',
        '#title' => t('Media: Flickr settings'),
        '#collapsible' => TRUE,
        '#collapsed' => !$widget['providers']['flickr_sets'],
        '#description' => t('These settings only affect Flickr slideshows.'),
      );
      foreach (array('video' => t('video'), 'preview' => t('preview')) as $formatter => $title) {
        $form['media_flickr'][$formatter] = array(
          '#type' => 'fieldset',
          '#title' => t('Media: Flickr settings for @formatter size', array('@formatter' => $title)),
          '#collapsible' => TRUE,
          '#description' => t('These settings only affect Flickr slideshows when displayed in the @formatter size.', array('@formatter' => $title)),
        );
        $form['media_flickr'][$formatter]['media_flickr_player_'. $formatter] = array(
          '#type' => 'select',
          '#title' => t('Media player (@formatter size)', array('@formatter' => $title)),
          '#options' => array(
            'flash' => t('Default Flickr slideshow (flash)'),
            'imagerotator' => t('JW Image Rotator'),
            'flvmediaplayer' => t('JW Flash Player'),
            'custom' => t('Custom'),
          ),
          '#description' => t('Please select the player to display Flickr photoset slideshows in the @formatter size.', array('@formatter' => $title)),
          '#default_value' => isset($widget['media_flickr_player_'. $formatter]) ? $widget['media_flickr_player_'. $formatter] : 'flash',
        );

        $form['media_flickr'][$formatter]['imagerotator'] = array(
          '#type' => 'fieldset',
          '#title' => t('Media: Flickr settings for JW Image Rotator in @formatter size', array('@formatter' => $title)),
          '#collapsible' => TRUE,
          '#collapsed' => (!($widget['media_flickr_player_'. $formatter] == 'imagerotator')),
          '#description' => t('These settings only affect the slideshow when the JW Image Rotator is selected for the @formatter size in the above select drop-down list.', array('@formatter' => $title)),
        );
        if (module_exists('media_flickr_xspf')) {
          $form['media_flickr'][$formatter]['imagerotator']['media_flickr_ir_captions_'. $formatter] = array(
            '#type' => 'radios',
            '#title' => t('Captions (@formatter size)', array('@formatter' => $title)),
            '#options' => array(
              MEDIA_FLICKR_XSPF_CAPTION_TYPE_TITLE => t('Photo title'),
              MEDIA_FLICKR_XSPF_CAPTION_TYPE_DESCRIPTION => t('Photo description'),
              MEDIA_FLICKR_XSPF_CAPTION_TYPE_BOTH => t('Both photo title and description'),
              MEDIA_FLICKR_XSPF_CAPTION_TYPE_DESCRIPTION_FALLBACK_TO_TITLE => t('Photo description, falling back to title if not present'),
              MEDIA_FLICKR_XSPF_CAPTION_TYPE_NONE => t('No caption'),
            ),
            '#description' => t('Please select what type of caption to display for individual photos. (This information is derived from information associated with each individual photo provided directly by Flickr.)'),
            '#default_value' => isset($widget['media_flickr_ir_captions_'. $formatter]) ? $widget['media_flickr_ir_captions_'. $formatter] : variable_get('media_flickr_xspf_caption_type', MEDIA_FLICKR_XSPF_CAPTION_TYPE_TITLE),
          );
        }
        $form['media_flickr'][$formatter]['imagerotator']['media_flickr_ir_repeat_'. $formatter] = array(
          '#type' => 'select',
          '#title' => t('Playlist repeat (@formatter size)', array('@formatter' => $title)),
          '#options' => array(
            'true' => t('True'),
            'list' => t('List'),
          ),
          '#description' => t('Set to %list to play the entire playlist once, to %always to continously play the photoset slide show, when the JW Image Rotator is chosen in the @formatter size.', array('@formatter' => $title, '%list' => t('list'), '%always' => t('true'))),
          '#default_value' => isset($widget['media_flickr_ir_repeat_'. $formatter]) ? $widget['media_flickr_ir_repeat_'. $formatter] : 'list',
        );
        $form['media_flickr'][$formatter]['imagerotator']['media_flickr_ir_rotatetime_'. $formatter] = array(
          '#type' => 'textfield',
          '#title' => t('Rotate time (@formatter size)', array('@formatter' => $title)),
          '#description' => t('This determines how long to display each image, in seconds, when the JW Image Rotator is chosen in the @formatter size.', array('@formatter' => $title)),
          '#default_value' => isset($widget['media_flickr_ir_rotatetime_'. $formatter]) ? $widget['media_flickr_ir_rotatetime_'. $formatter] : '5',
        );
        $form['media_flickr'][$formatter]['imagerotator']['media_flickr_ir_shownavigation_'. $formatter] = array(
          '#type' => 'checkbox',
          '#title' => t('Show navigation (@formatter size)', array('@formatter' => $title)),
          '#description' => t('Uncheck this box to completely hide the navigation bar when the JW Image Rotator is chosen in the @formatter size.', array('@formatter' => $title)),
          '#default_value' => isset($widget['media_flickr_ir_shownavigation_'. $formatter]) ? $widget['media_flickr_ir_shownavigation_'. $formatter] : TRUE,
        );
        $form['media_flickr'][$formatter]['imagerotator']['media_flickr_ir_showicons_'. $formatter] = array(
          '#type' => 'checkbox',
          '#title' => t('Show icons (@formatter size)', array('@formatter' => $title)),
          '#description' => t('Uncheck this box to hide the activity icon and play button in the middle of the display when the JW Image Rotator is chosen in the @formatter size.', array('@formatter' => $title)),
          '#default_value' => isset($widget['media_flickr_ir_showicons_'. $formatter]) ? $widget['media_flickr_ir_showicons_'. $formatter] : TRUE,
        );
        $form['media_flickr'][$formatter]['imagerotator']['media_flickr_ir_shuffle_'. $formatter] = array(
          '#type' => 'checkbox',
          '#title' => t('Shuffle (@formatter size)', array('@formatter' => $title)),
          '#description' => t('Check this box to playback the images in random order when the JW Image Rotator is chosen in the @formatter size.', array('@formatter' => $title)),
          '#default_value' => isset($widget['media_flickr_ir_shuffle_'. $formatter]) ? $widget['media_flickr_ir_shuffle_'. $formatter] : FALSE,
        );
        $form['media_flickr'][$formatter]['imagerotator']['media_flickr_ir_allowfullscreen_'. $formatter] = array(
          '#type' => 'checkbox',
          '#title' => t('Allow full screen (@formatter size)', array('@formatter' => $title)),
          '#description' => t('If checked, then the user may choose to display the slide show in a full screen mode, when the JW Image Rotator is chosen in the @formatter size.', array('@formatter' => $title)),
          '#default_value' => isset($widget['media_flickr_ir_allowfullscreen_'. $formatter]) ? $widget['media_flickr_ir_allowfullscreen_'. $formatter] : TRUE,
        );
        $form['media_flickr'][$formatter]['imagerotator']['media_flickr_ir_logo_'. $formatter] = array(
          '#type' => 'textfield',
          '#title' => t('Logo (@formatter size)', array('@formatter' => $title)),
          '#description' => t('If set to an image (JPG|GIF|PNG) then this logo will be displayed in the top right corner of the slide show, when the JW Image Rotator is chosen in the @formatter size. Do not enter a leading slash.', array('@formatter' => $title)),
          '#default_value' => isset($widget['media_flickr_ir_logo_'. $formatter]) ? $widget['media_flickr_ir_logo_'. $formatter] : '',
        );
        $form['media_flickr'][$formatter]['imagerotator']['media_flickr_ir_transition_'. $formatter] = array(
          '#type' => 'select',
          '#title' => t('Transition (@formatter size)', array('@formatter' => $title)),
          '#options' => array(
            'random' => t('Random'),
            'fade' => t('Fade'),
            'bgfade' => t('Background fade'),
            'blocks' => t('Blocks'),
            'bubbles' => t('Bubbles'),
            'circles' => t('Circles'),
            'flash' => t('Flash'),
            'fluids' => t('Fluids'),
            'lines' => t('Lines'),
            'slowfade' => t('Slow fade'),
          ),
          '#description' => t('Sets the transition to use between images, when the JW Image Rotator is chosen in the @formatter size. The default, %random, randomly pick a transition. To restrict to a certain transition, select another value.', array('@formatter' => $title, '%random' => t('Random'))),
          '#default_value' => isset($widget['media_flickr_ir_transition_'. $formatter]) ? $widget['media_flickr_ir_transition_'. $formatter] : 'random',
        );

        $form['media_flickr'][$formatter]['flvmediaplayer'] = array(
          '#type' => 'fieldset',
          '#title' => t('Media: Flickr settings for JW Flash Player in @formatter size', array('@formatter' => $title)),
          '#collapsible' => TRUE,
          '#collapsed' => (!($widget['media_flickr_player_'. $formatter] == 'flvmediaplayer')),
          '#description' => t('These settings only affect the slideshow when the JW Flash Player is selected for the @formatter size in the above select drop-down list.', array('@formatter' => $title)),
        );
        $form['media_flickr'][$formatter]['flvmediaplayer']['media_flickr_playlist_'. $formatter] = array(
          '#type' => 'select',
          '#title' => t('Playlist position (@formatter size)', array('@formatter' => $title)),
          '#options' => array(
            'none' => t('None'),
            'bottom' => t('Bottom'),
            'over' => t('Over'),
            'right' => t('Right'),
          ),
          '#description' => t('This determines the position of the playlist icons when the JW Flash Player is chosen in the @formatter size.', array('@formatter' => $title)),
          '#default_value' => isset($widget['media_flickr_playlist_'. $formatter]) ? $widget['media_flickr_playlist_'. $formatter] : 'right',
        );
        $form['media_flickr'][$formatter]['flvmediaplayer']['media_flickr_playlistsize_'. $formatter] = array(
          '#type' => 'textfield',
          '#title' => t('Playlist size (@formatter size)', array('@formatter' => $title)),
          '#description' => t('This determines the width of the playlist icons when the JW Flash Player is chosen in the @formatter size.', array('@formatter' => $title)),
          '#default_value' => isset($widget['media_flickr_playlistsize_'. $formatter]) ? $widget['media_flickr_playlistsize_'. $formatter] : '86',
        );
        if (module_exists('media_flickr_xspf')) {
          $form['media_flickr'][$formatter]['flvmediaplayer']['media_flickr_captions_'. $formatter] = array(
            '#type' => 'radios',
            '#title' => t('Captions (@formatter size)', array('@formatter' => $title)),
            '#options' => array(
              MEDIA_FLICKR_XSPF_CAPTION_TYPE_TITLE => t('Photo title'),
              MEDIA_FLICKR_XSPF_CAPTION_TYPE_DESCRIPTION => t('Photo description'),
              MEDIA_FLICKR_XSPF_CAPTION_TYPE_BOTH => t('Both photo title and description'),
              MEDIA_FLICKR_XSPF_CAPTION_TYPE_DESCRIPTION_FALLBACK_TO_TITLE => t('Photo description, falling back to title if not present'),
              MEDIA_FLICKR_XSPF_CAPTION_TYPE_NONE => t('No caption'),
            ),
            '#description' => t('Please select what type of caption to display for individual photos. (This information is derived from information associated with each individual photo provided directly by Flickr.)'),
            '#default_value' => isset($widget['media_flickr_captions_'. $formatter]) ? $widget['media_flickr_captions_'. $formatter] : variable_get('media_flickr_xspf_caption_type', MEDIA_FLICKR_XSPF_CAPTION_TYPE_TITLE),
          );
        }
        $form['media_flickr'][$formatter]['flvmediaplayer']['media_flickr_repeat_'. $formatter] = array(
          '#type' => 'select',
          '#title' => t('Playlist repeat (@formatter size)', array('@formatter' => $title)),
          '#options' => array(
            'none' => t('None'),
            'list' => t('List'),
            'always' => t('Always'),
            'single' => t('Single'),
          ),
          '#description' => t('Set to %list to play the entire playlist once, to %always to continously play the song/video/playlist and to %single to continue repeating the selected file in a playlist, when the JW Flash Player is chosen in the @formatter size.', array('@formatter' => $title, '%list' => t('list'), '%always' => t('always'), '%single' => t('single'), )),
          '#default_value' => isset($widget['media_flickr_repeat_'. $formatter]) ? $widget['media_flickr_repeat_'. $formatter] : 'list',
        );
        $form['media_flickr'][$formatter]['flvmediaplayer']['media_flickr_delay_'. $formatter] = array(
          '#type' => 'textfield',
          '#title' => t('Playlist delay (@formatter size)', array('@formatter' => $title)),
          '#description' => t('This determines how long to display each image, in seconds, when the JW Flash Player is chosen in the @formatter size.', array('@formatter' => $title)),
          '#default_value' => isset($widget['media_flickr_delay_'. $formatter]) ? $widget['media_flickr_delay_'. $formatter] : '10',
        );
        $form['media_flickr'][$formatter]['flvmediaplayer']['media_flickr_controlbar_'. $formatter] = array(
          '#type' => 'select',
          '#title' => t('Control bar position (@formatter size)', array('@formatter' => $title)),
          '#options' => array(
            'none' => t('None'),
            'bottom' => t('Bottom'),
            'over' => t('Over'),
          ),
          '#description' => t('This controls the position of the control bar when the JW Flash Player is chosen in the @formatter size.', array('@formatter' => $title)),
          '#default_value' => isset($widget['media_flickr_controlbar_'. $formatter]) ? $widget['media_flickr_controlbar_'. $formatter] : 'none',
        );
        $form['media_flickr'][$formatter]['flvmediaplayer']['media_flickr_allowfullscreen_'. $formatter] = array(
          '#type' => 'checkbox',
          '#title' => t('Allow full screen (@formatter size)', array('@formatter' => $title)),
          '#description' => t('If checked, then the user may choose to display the slide show in a full screen mode, when the JW Flash Player is chosen in the @formatter size.', array('@formatter' => $title)),
          '#default_value' => isset($widget['media_flickr_allowfullscreen_'. $formatter]) ? $widget['media_flickr_allowfullscreen_'. $formatter] : TRUE,
        );
        $form['media_flickr'][$formatter]['flvmediaplayer']['media_flickr_logo_'. $formatter] = array(
          '#type' => 'textfield',
          '#title' => t('Logo (@formatter size)', array('@formatter' => $title)),
          '#description' => t('If set to an image (JPG|GIF|PNG) then this logo will be displayed in the top right corner of the slide show, when the JW Flash Player is chosen in the @formatter size. Do not enter a leading slash.', array('@formatter' => $title)),
          '#default_value' => isset($widget['media_flickr_logo_'. $formatter]) ? $widget['media_flickr_logo_'. $formatter] : '',
        );
      }
      return $form;
    }
  }
  else if ($op == 'save') {
    $columns = array();
    if ($widget['module'] == 'emvideo') {
      $columns = array();
      foreach (array('video', 'preview') as $formatter) {
        // JW Image Rotator options.
        $columns[] = 'media_flickr_ir_repeat_'. $formatter;
        $columns[] = 'media_flickr_ir_rotatetime_'. $formatter;
        $columns[] = 'media_flickr_ir_shownavigation_'. $formatter;
        $columns[] = 'media_flickr_ir_showicons_'. $formatter;
        $columns[] = 'media_flickr_ir_shuffle_'. $formatter;
        $columns[] = 'media_flickr_ir_allowfullscreen_'. $formatter;
        $columns[] = 'media_flickr_ir_logo_'. $formatter;
        $columns[] = 'media_flickr_ir_transition_'. $formatter;
        $columns[] = 'media_flickr_ir_captions_'. $formatter;

        // JW Flash Player options.
        $columns[] = 'media_flickr_player_'. $formatter;
        $columns[] = 'media_flickr_playlist_'. $formatter;
        $columns[] = 'media_flickr_playlistsize_'. $formatter;
        $columns[] = 'media_flickr_repeat_'. $formatter;
        $columns[] = 'media_flickr_delay_'. $formatter;
        $columns[] = 'media_flickr_controlbar_'. $formatter;
        $columns[] = 'media_flickr_allowfullscreen_'. $formatter;
        $columns[] = 'media_flickr_logo_'. $formatter;
        $columns[] = 'media_flickr_captions_'. $formatter;
      }
    }
    return $columns;
  }
}
