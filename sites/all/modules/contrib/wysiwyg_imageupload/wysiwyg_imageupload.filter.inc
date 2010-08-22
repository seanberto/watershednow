<?php
// $Id:$
// Copyright (c) 2010 KontextWork GbR
// Author: Eugen Mayer

/**
* Implementation of hook_filter().
*/
function wysiwyg_imageupload_filter($op, $delta = 0, $format = -1, $text = '') {
  switch ($op) {
    case 'list':
      return array(0 => t('Drupal Wiki inline-images'));

    case 'description':
      return t('Converts Drupal Wiki inline images to HTML.');

    case 'process':
      return _wysiwyg_imageupload_filter_process($text);

    case 'no cache':
          return false;

    case 'settings':
          $form = array();
          return $form;
    default:
      return $text;
  }
}

/**
* Finds all the occurences of a inline image tag
*/
function _wysiwyg_imageupload_filter_process(&$body) {
  // Syntax: [[inlineimage:<iid>:<inlineargs>]]
  $body = preg_replace(array('/\[\[wysiwyg_imageupload:(\d+):([^\]]*?)\]\]/e'), array("_wysiwyg_imageupload_render_nodeview($1,'$2');"), $body);
  return $body;
}

/**
* returns a rendered(html) image for the tag
*/
function _wysiwyg_imageupload_render_nodeview($iid, $arguments) {
  $arguments = unserialize(urldecode($arguments));
  $image_obj = _wysiwyg_imageupload_load_inline_entity($iid);  
  
  return theme('wysiwyg_imageupload_render_image_entity', $image_obj, $arguments);
}
