<?php
// $Id: openlayers_ui.styles.inc,v 1.1.2.7 2010/06/25 15:47:04 tmcw Exp $

/**
 * @file
 * This file holds the functions handling styles in the
 * Openlayers UI.
 *
 * @ingroup openlayers
 */

/**
 * Styles add/edit form.
 */
function openlayers_ui_styles_form(&$form_state, $style = NULL, $edit = FALSE) {
  $form = array();
  
  // Available styling properies.  Defaults and descriptions are taken
  // from OpenLayers.
  // @see http://docs.openlayers.org/library/feature_styling.html
  $properties = array(
    'externalGraphic' => array(
      'default' => '',
      'desc' => t('The URL of an icon for points in a layer.'),
    ),
    'pointRadius' => array(
      'default' => 6,
      'desc' => t('The radius of a vector point or the size of
        an icon. Note that, when using icons, this value should be half the
        width of the icon image.'),
    ),
    'fillColor' => array(
      'default' => '#EE9900',
      'desc' => t('This is the color used for 
        filling in Polygons. It is also used in the center of marks for 
        points: the interior color of circles or other shapes. It is 
        not used if an externalGraphic is applied to a point.  This should be
        a hexideicmal value like #FFFFFF.'),
    ),
    'strokeColor' => array(
      'default' => '#EE9900',
      'desc' => t('This is color of the line on features. On 
        polygons and point marks, it is used as an outline to the feature. 
        On lines, this is the representation of the feature.  This should be
        a hexideicmal value like #FFFFFF.'),
    ),
    'strokeWidth' => array(
      'default' => 1,
      'desc' => t('This is width of the line on features. On 
        polygons and point marks, it is used as an outline to 
        the feature. On lines, this is the representation of the 
        feature.  This is a value in pixels.'),
    ),
    'fillOpacity' => array(
      'default' => 1,
      'desc' => t('This is the opacity used for filling in Polygons. 
        It is also used in the center of marks for points: the interior 
        color of circles or other shapes. It is not used if an 
        externalGraphic is applied to a point.  This should be a value
        between 0 and 1.'),
    ),
    'strokeOpacity' => array(
      'default' => 1,
      'desc' => t('This is opacity of the line on features. 
        On polygons and point marks, it is used as an outline to the 
        feature. On lines, this is the representation of the feature.
        This should be a value between 0 and 1.'),
    ),
    'strokeLinecap' => array(
      'default' => 'round',
      'desc' => t('Options are butt, round, square. This property 
        is similar to the SVG stroke-linecap property. It determines 
        what the end of lines should look like. See the SVG link for 
        image examples.'),
      'options' => array(
        'butt' => t('Butt'),
        'round' => t('Round'),
        'square' => t('Square'),
      ),
    ),
    'strokeDashstyle' => array(
      'default' => 'solid',
      'desc' => t('Options are dot, dash, dashdot, longdash, longdashdot, solid.'),
      'options' => array(
        'dot' => t('Dot'),
        'dash' => t('Dash'),
        'dashdot' => t('Dash-dot'),
        'longdash' => t('Long-dash'),
        'longdashdot' => t('Long-dash-dot'),
        'solid' => t('Solid'),
      ),
    ),
    'cursor' => array(
      'default' => '',
      'desc' => t('Cursor used when mouse is over the feature. Default 
        is an empty string, which inherits from parent elements.  See
        <a href="!url">CSS cursor styles</a>.', array(
          '!url' => 'http://www.w3schools.com/css/pr_class_cursor.asp',
        )),
    ),
    'graphicWidth' => array(
      'default' => '',
      'desc' => t('This properties define the width of an externalGraphic. 
        This is an alternative to the pointRadius symbolizer property 
        to be used when your graphic has different sizes in the X and 
        Y direction.  This should be in pixels.'),
    ),
    'graphicHeight' => array(
      'default' => '',
      'desc' => t('This properties define the height of an externalGraphic. 
        This is an alternative to the pointRadius symbolizer property 
        to be used when your graphic has different sizes in the X and 
        Y direction.  This should be in pixels.'),
    ),
    'graphicOpacity' => array(
      'default' => '1',
      'desc' => t('Opacity of an external graphic.  This should be a 
      value between 0 and 1. Grahics that are already semitransparent,
      like alpha PNGs, should have this set to 1, or rendering problems in 
      Internet Explorer will ensue.'),
    ),
    'graphicXOffset' => array(
      'default' => '',
      'desc' => t('Where the X value of the "center" of an 
      externalGraphic should be.  This should be in pixels.'),
    ),
    'graphicYOffset' => array(
      'default' => '',
      'desc' => t('Where the Y value of the "center" of an 
      externalGraphic should be.  This should be in pixels.'),
    ),
    'graphicName' => array(
      'default' => '',
      'desc' => t('Name of a type of symbol to be used 
        for a point mark.'),
    ),
    'rotation' => array(
      'default' => '',
      'desc' => t('The rotation angle in degrees clockwise for 
        a point symbolizer.'),
    ),
    'display' => array(
      'default' => '',
      'desc' => t('Can be set to "none" to hide features 
        from rendering.'),
      'options' => array(
        '' => t('On'),
        'none' => t('None (off)'),
      ),
    ),
  );

  // Style object basics
  $form['info'] = array(
    '#type' => 'fieldset',
    '#tree' => FALSE,
  );
  $form['info']['name'] = array(
    '#title' => t('Name'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => !empty($style->name) ? $style->name : '',
  );
  $form['info']['title'] = array(
    '#title' => t('Title'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => !empty($style->title) ? $style->title : '',
  );
  $form['info']['description'] = array(
    '#title' => t('Description'),
    '#type' => 'textfield',
    '#default_value' => !empty($style->description) ? $style->description : '',
  );

  // OpenLayers style properties
  $form['data'] = array('#type' => 'fieldset', '#tree' => TRUE);

  foreach ($properties as $key => $prop) {
    $form['data'][$key] = array(
      '#type' => !isset($prop['options']) ? 'textfield' : 'select',
      '#title' => $key,
      '#description' => $prop['desc'],
      '#default_value' => isset($style->data[$key]) ?
        $style->data[$key] : $prop['default'],
    );
    
    // Add options if needed
    if (isset($prop['options']) && is_array($prop['options'])) {
      $form['data'][$key]['#options'] = $prop['options'];
    }
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  return $form;
}

/**
 * Submit handler for layers.
 */
function openlayers_ui_styles_form_submit(&$form, &$form_state) {
  $style = new stdClass();
  $style->name = $form_state['values']['name'];
  $style->title = $form_state['values']['title'];
  $style->description = $form_state['values']['description'];
  $style->data = $form_state['values']['data'];

  $success = openlayers_style_save($style);

  // Redirect to edit page
  if ($success) {
    drupal_set_message(t('Style saved.'));
    $form_state['redirect'] = 'admin/build/openlayers/styles/' . $style->name . '/edit';
  }
  else {
    form_set_error('openlayers', t('Error trying to save style.'));
  }
}

// /**
//  * Import a preset from cut & paste
//  */
// function openlayers_ui_styles_import_form(&$form_state) {
//   $form['name'] = array(
//     '#type' => 'textfield',
//     '#title' => t('Style name'),
//     '#description' => t('Enter the name to use for this style if it is different from the source style. Leave blank to use the name of the style.'),
//   );
//
//   $form['style'] = array(
//     '#type' => 'textarea',
//     '#title' => t('Paste style code here'),
//     '#required' => TRUE,
//   );
//
//   $form['submit'] = array(
//     '#type' => 'submit',
//     '#value' => t('Import'),
//     '#submit' => array('openlayers_ui_style_import_submit'),
//     '#validate' => array('openlayers_ui_style_import_validate'),
//   );
//   return $form;
// }
//
// /**
//  * Validate handler to import a preset
//  */
// function openlayers_ui_styles_import_validate($form, &$form_state) {
//   $view = '';
//   ob_start();
//   eval($form_state['values']['style']);
//   ob_end_clean();
//
//   if (!is_object($style)) {
//     return form_error($form['style'], t('Unable to interpret preset code.'));
//   }
//
//   // View name must be alphanumeric or underscores, no other punctuation.
//   if (!empty($form_state['values']['name']) && preg_match('/[^a-zA-Z0-9_]/', $form_state['values']['name'])) {
//     form_error($form['name'], t('Preset name must be alphanumeric or underscores only.'));
//   }
//
//   if ($form_state['values']['name']) {
//     $preset->name = $form_state['values']['name'];
//   }
//
//   /*
//    * TODO: replace with style code
//   if ($test && $test->type != t('Default')) {
//     form_set_error('', t('A preset by that name already exists; please choose a different name'));
//   }
//   */
//
//   // TODO: The import process should test layer existence
//   $form_state['style'] = &$style;
//   $a = array();
//   $form = openlayers_ui_styles_form($a);
//   openlayers_ui_styles_form_submit(&$form, &$form_state);
// }
