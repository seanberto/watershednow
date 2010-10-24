<?php
// $Id: media_vimeo.theme.inc,v 1.1 2010/09/17 16:16:34 aaron Exp $

/**
 * @file media_vimeo/themes/media_vimeo.theme.inc
 * Theme functions for Media: Vimeo.
 */

function template_preprocess_media_vimeo(&$variables) {
  _media_vimeo_options($variables);
  $variables['width'] = isset($variables['width']) ? $variables['width'] : media_vimeo_variable_get('default_width');
  $variables['height'] = isset($variables['height']) ? $variables['height'] : media_vimeo_variable_get('default_height');
  $variables['fullscreen'] = isset($variables['fullscreen']) ? $variables['fullscreen'] : media_vimeo_variable_get('full_screen');
  $variables['on_screen_info'] = isset($variables['on_screen_info']) ? $variables['on_screen_info'] : media_vimeo_variable_get('on_screen_info');
  $variables['show_portrait'] = isset($variables['show_portrait']) ? $variables['show_portrait'] : ($variables['on_screen_info']['portrait'] ? 1 : 0);
  $variables['show_title'] = isset($variables['show_title']) ? $variables['show_title'] : ($variables['on_screen_info']['title'] ? 1 : 0);
  $variables['show_byline'] = isset($variables['show_byline']) ? $variables['show_byline'] : ($variables['on_screen_info']['byline'] ? 1 : 0);
  $variables['color_override'] = isset($variables['color_override']) ? $variables['color_override'] : media_vimeo_variable_get('color_override');
  if ($variables['color_override']) {
    $variables['color'] = isset($variables['color']) ? $variables['color'] : _media_vimeo_convert_color();
  }

  $variables['format'] = media_vimeo_variable_get('universal') ? 'universal' : 'flash';
  $variables['output'] = theme('media_vimeo_' . $variables['format'], $variables['video_code'], $variables);
}

/**
 * The embedded flash displaying the Vimeo video.
 */
function template_preprocess_media_vimeo_flash(&$variables) {
  _media_vimeo_options($variables);
  if ($variables['video_code']) {
    $variables['clip_id'] = $variables['video_code'];
    $variables['query'] = isset($variables['query']) ? $variables['query'] : array(
      'clip_id' => $variables['video_code'],
      'server' => 'www.vimeo.com',
    );
    foreach (array('fullscreen', 'show_title', 'show_byline', 'show_portrait', 'color', 'autoplay') as $key) {
      if (isset($variables[$key])) {
        $variables['query'][$key] = $variables[$key];
      }
    }
    $variables['data'] = url('http://www.vimeo.com/moogaloop.swf', array('query' => $variables['query']));
  }
}

/**
 * Display the video in an iFrame with HTML5.
 */
function template_preprocess_media_vimeo_universal(&$variables) {
  _media_vimeo_options($variables);
  if ($variables['video_code']) {
    $variables['query'] = isset($variables['query']) ? $variables['query'] : array();
    foreach (array('fullscreen', 'show_title', 'show_byline', 'show_portrait', 'color', 'autoplay') as $key) {
      if (isset($variables[$key])) {
        $variables['query'][$key] = $variables[$key];
      }
    }
    $variables['iframe_url'] = url('http://player.vimeo.com/video/' . $variables['video_code'], array('query' => $variables['query']));
  }
}

/**
 * Strip any beginning #'s from the color hex code.
 */
function _media_vimeo_convert_color($color = NULL) {
  if (!isset($color)) {
    $color = media_vimeo_variable_get('color');
  }
  if ($color{0} == '#') {
    return substr($color, 1);
  }
  return $color;
}

function _media_vimeo_options(&$variables) {
  foreach ($variables['options'] as $key => $value) {
    $variables[$key] = $value;
  }
  unset($variables['options']);
}
