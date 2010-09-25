<?php
// $Id: openlayers_geocoder.theme.inc,v 1.3.2.3 2010/09/23 09:07:11 antoniodemarco Exp $

/**
 * Theme funciton for OpenLayers Geocoder widget autocompletion setting
 */
function theme_openlayers_geocoder_widget_autocompletion_settings($form) {
  foreach (element_children($form) as $key) {
    if (is_numeric($key)) {
      $row = array();
      foreach (element_children($form[$key]) as $setting) {
        $row[] = drupal_render($form[$key][$setting]);
      }
      $rows[] = $row;
    }
  }
  $header[] = t('Enable');
  $header[] = t('Override');
  $header[] = t('Field');
  $header[] = t('Content');
  return theme('table', $header, $rows) . drupal_render($form);
}