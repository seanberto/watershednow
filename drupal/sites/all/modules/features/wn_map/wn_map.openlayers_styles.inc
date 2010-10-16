<?php

/**
 * Implementation of hook_openlayers_styles().
 */
function wn_map_openlayers_styles() {
  $export = array();
  $openlayers_styles = new stdClass;
  $openlayers_styles->disabled = FALSE; /* Edit this to true to make a default openlayers_styles disabled initially */
  $openlayers_styles->api_version = 1;
  $openlayers_styles->name = 'features';
  $openlayers_styles->title = 'Features';
  $openlayers_styles->description = 'Map features created and maintained by staff';
  $openlayers_styles->data = array(
    'externalGraphic' => '/sites/all/modules/features/map/openlayers_themes/wn/tax_icons/${tid}-tax.png',
    'pointRadius' => 8,
    'fillColor' => '#EE9900',
    'strokeColor' => '#EE9900',
    'strokeWidth' => 1,
    'fillOpacity' => 1,
    'strokeOpacity' => 1,
    'strokeLinecap' => 'round',
    'strokeDashstyle' => 'solid',
    'cursor' => 'pointer',
    'graphicWidth' => 16,
    'graphicHeight' => 21,
    'graphicOpacity' => 1,
    'labelAlign' => 'cm',
  );

  $export['features'] = $openlayers_styles;
  $openlayers_styles = new stdClass;
  $openlayers_styles->disabled = FALSE; /* Edit this to true to make a default openlayers_styles disabled initially */
  $openlayers_styles->api_version = 1;
  $openlayers_styles->name = 'sidebar_features';
  $openlayers_styles->title = 'Sidebar Features';
  $openlayers_styles->description = 'Styling of sidebar feature map points';
  $openlayers_styles->data = array(
    'externalGraphic' => '/sites/all/modules/features/map/openlayers_themes/wn/tax_icons/${tid}-tax.png',
    'pointRadius' => 8,
    'fillColor' => '#EE9900',
    'strokeColor' => '#EE9900',
    'strokeWidth' => 1,
    'fillOpacity' => 1,
    'strokeOpacity' => 1,
    'strokeLinecap' => 'round',
    'strokeDashstyle' => 'solid',
    'graphicWidth' => 16,
    'graphicHeight' => 21,
    'graphicOpacity' => 1,
    'labelAlign' => 'cm',
  );

  $export['sidebar_features'] = $openlayers_styles;
  return $export;
}
