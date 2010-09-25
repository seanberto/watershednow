<?php
// $Id: galleryformatter_imagecache.inc,v 1.1 2010/05/10 12:54:58 manuelgarcia Exp $

/**
 * @file
 * Provides default ImageCache presets that can be overridden by site administrators.
 */

/*
 * Implementation of hook_imagecache_default_presets().
 */
function galleryformatter_imagecache_default_presets() {
  $presets = array();
  $presets['galleryformatter_slide'] = array(
    'presetname' => 'galleryformatter_slide',
    'actions' =>
    array(
      0 =>
      array(
        'weight' => '0',
        'module' => 'galleryformatter',
        'action' => 'imagecache_scale_and_crop',
        'data' =>
        array(
          'width' => '500',
          'height' => '312',
        ),
      ),
    ),
  );

  $presets['galleryformatter_thumb'] = array(
  'presetname' => 'galleryformatter_thumb',
    'actions' =>
    array(
      0 =>
      array(
        'weight' => '0',
        'module' => 'galleryformatter',
        'action' => 'imagecache_scale_and_crop',
        'data' =>
        array(
          'width' => '121',
          'height' => '75',
        ),
      ),
    ),
  );

  return $presets;
}
