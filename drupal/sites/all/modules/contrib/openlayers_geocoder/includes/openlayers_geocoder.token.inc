<?php
// $Id: openlayers_geocoder.token.inc,v 1.2.2.5 2010/09/23 13:06:55 antoniodemarco Exp $

/**
 * @file
 * Token module integration.
 *
 * @ingroup token
 */

/**
 * Implementation of hook_token_list()
 */
function openlayers_geocoder_token_list($type = 'all') {
  $tokens = array();

  if ($type == 'openlayers data') {

    $layers = module_invoke('openlayers_views', 'openlayers_layers');
    foreach ($layers as $layer_name => $layer) {
      $list = array();
      $layer = openlayers_layer_load($layer_name);
      $features = $layer->get_features($layer->data['views']['view'], $layer->data['views']['display']);
      foreach ($features as $feature) {
        foreach ($feature['attributes'] as $attribute_name => $attribute) {
          $parts = array();
          $parts[] = $layer_name;
          $parts[] = $attribute_name;
          $key = implode('-', $parts);
          $list[$key] = t('Attriubute: <em>!attribute_name</em>', array('!attribute_name' => $attribute_name));
        }
      }
    }
    $tokens['openlayers data'] = $list;
  }

  if ($type == 'geocoder') {

    $tokens['geocoder']['formatted_address'] = t('Full formatted address.');
    $tokens['geocoder']['street_address'] = t('Indicates a precise street address.');
    $tokens['geocoder']['route'] = t('Indicates a named route (such as "US 101").');
    $tokens['geocoder']['intersection'] = t('Indicates a major intersection, usually of two major roads.');
    $tokens['geocoder']['political'] = t('Indicates a political entity. Usually, this type indicates a polygon of some civil administration.');
    $tokens['geocoder']['country'] = t('Indicates the national political entity, and is typically the highest order type returned by the Geocoder.');
    $tokens['geocoder']['country_code'] = t('Country code (<em>in <a href="!url" target="_blank">ISO 3166-1</a> format</em>).', array('!url' => 'http://www.iso.org/iso/english_country_names_and_code_elements'));
    $tokens['geocoder']['administrative_area_level_1'] = t('Indicates a first-order civil entity below the country level. Within the United States, these administrative levels are states. Not all nations exhibit these administrative levels.');
    $tokens['geocoder']['administrative_area_level_2'] = t('Indicates a second-order civil entity below the country level. Within the United States, these administrative levels are counties. Not all nations exhibit these administrative levels.');
    $tokens['geocoder']['administrative_area_level_3'] = t('Indicates a third-order civil entity below the country level. This type indicates a minor civil division. Not all nations exhibit these administrative levels.');
    $tokens['geocoder']['colloquial_area'] = t('Indicates a commonly-used alternative name for the entity.');
    $tokens['geocoder']['locality'] = t('Indicates an incorporated city or town political entity.');
    $tokens['geocoder']['sublocality'] = t('Indicates an first-order civil entity below a locality');
    $tokens['geocoder']['neighborhood'] = t('Indicates a named neighborhood');
    $tokens['geocoder']['premise'] = t('Indicates a named location, usually a building or collection of buildings with a common name');
    $tokens['geocoder']['subpremise'] = t('Indicates a first-order entity below a named location, usually a singular building within a collection of buildings with a common name');
    $tokens['geocoder']['postal_code'] = t('Indicates a postal code as used to address postal mail within the country.');
    $tokens['geocoder']['natural_feature'] = t('Indicates a prominent natural feature.');
    $tokens['geocoder']['airport'] = t('Indicates an airport.');
    $tokens['geocoder']['park'] = t('Indicates a named park.');
    $tokens['geocoder']['point_of_interest'] = t('Indicates a named point of interest. Typically, these "POI"s are prominent local entities that don\'t easily fit in another category such as "Empire State Building" or "Statue of Liberty."');
    $tokens['geocoder']['post_box'] = t('Indicates a specific postal box.');
    $tokens['geocoder']['street_number'] = t('Indicates the precise street number.');
    $tokens['geocoder']['floor'] = t('Indicates the floor of a building address.');
    $tokens['geocoder']['room'] = t('Indicates the room of a building address.');
    $tokens['geocoder']['latitude'] = t('Latitude');
    $tokens['geocoder']['longitude'] = t('Longitude');

    // Keep v2 tokens for backward compatibility
    $tokens['geocoder']['address'] = t(' <b>Deprecated: use "street_address" instead.</b> Full address');
    $tokens['geocoder']['thoroughfare'] = t(' <b>Deprecated: use "route" or "street_address" instead.</b> Thoroughfare (<em>i.e. street name + number</em>)');
    $tokens['geocoder']['administrative_area'] = t(' <b>Deprecated: use "administrative_area_level_1" instead.</b> Administrative area');
    $tokens['geocoder']['sub_administrative_area'] = t(' <b>Deprecated: use "administrative_area_level_2" instead.</b> Sub-administrative area');
    $tokens['geocoder']['dependent_locality'] = t(' <b>Deprecated: use "administrative_area_level_3" instead.</b> Dependent locality');    
  }

  return $tokens;
}

/**
 * Implementation of hook_token_values()
 */
function openlayers_geocoder_token_values($type, $object = NULL, $options = array()) {

  $tokens = array();
  if ($type == 'openlayers data') {
    foreach ($object['layers'] as $layer_name => $layer) {
      if ($layer['layer_type'] == 'openlayers_views_vector') {
        foreach ($layer['features'] as $feature) {
          foreach ($feature['attributes'] as $attribute_name => $attribute) {
            $parts = array();
            $parts[] = $layer_name;
            $parts[] = $attribute_name;
            $key = implode('-', $parts);
            $tokens[$key] = $attribute;
          }
        }
      }
    }
  }

  if ($type == 'geocoder') {
    $result = module_invoke_all('token_list', 'geocoder');
    foreach ($result[$type] as $name => $value) {
      $tokens[$name] = isset($object['components'][$name]) ? $object['components'][$name] : '';
    }

    $tokens['latitude'] = $object['location']['lat'];
    $tokens['longitude'] = $object['location']['lng'];
    $tokens['formatted_address'] = $object['address'];
    
    // Keep v2 tokens for backward compatibility
    $tokens['address'] = $object['components']['street_address'] ? $tokens['street_address'] : '';
    $tokens['thoroughfare'] = $object['components']['route'] ? ($tokens['route'] .' '. $tokens['street_number']) : '';
    $tokens['administrative_area'] = $object['components']['administrative_area_level_1'] ? $tokens['administrative_area_level_1'] : '';
    $tokens['sub_administrative_area'] = $object['components']['administrative_area_level_2'] ? $tokens['administrative_area_level_2'] : '';
    $tokens['dependent_locality'] = $object['components']['administrative_area_level_3'] ? $tokens['administrative_area_level_3'] : '';
  }

  return $tokens;
}