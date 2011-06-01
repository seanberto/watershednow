<?php

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
function openlayers_ui_styles_get_properties() {
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
      'type' => 'integer',
    ),
    'fillColor' => array(
      'default' => '#EE9900',
      'desc' => t('This is the color used for 
        filling in Polygons. It is also used in the center of marks for 
        points: the interior color of circles or other shapes. It is 
        not used if an externalGraphic is applied to a point.  This should be
        a hexadecimal value like #FFFFFF.'),
    ),
    'strokeColor' => array(
      'default' => '#EE9900',
      'desc' => t('This is color of the line on features. On 
        polygons and point marks, it is used as an outline to the feature. 
        On lines, this is the representation of the feature.  This should be
        a hexadecimal value like #FFFFFF.'),
    ),
    'strokeWidth' => array(
      'default' => 1,
      'desc' => t('This is width of the line on features. On 
        polygons and point marks, it is used as an outline to 
        the feature. On lines, this is the representation of the 
        feature.  This is a value in pixels.'),
      'type' => 'integer',
    ),
    'fillOpacity' => array(
      'default' => 1,
      'desc' => t('This is the opacity used for filling in Polygons. 
        It is also used in the center of marks for points: the interior 
        color of circles or other shapes. It is not used if an 
        externalGraphic is applied to a point.  This should be a value
        between 0 and 1.'),
      'type' => 'float',
    ),
    'strokeOpacity' => array(
      'default' => 1,
      'desc' => t('This is opacity of the line on features. 
        On polygons and point marks, it is used as an outline to the 
        feature. On lines, this is the representation of the feature.
        This should be a value between 0 and 1.'),
      'type' => 'float',
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
      'type' => 'integer',
    ),
    'graphicHeight' => array(
      'default' => '',
      'desc' => t('This properties define the height of an externalGraphic. 
        This is an alternative to the pointRadius symbolizer property 
        to be used when your graphic has different sizes in the X and 
        Y direction.  This should be in pixels.'),
      'type' => 'integer',
    ),
    'graphicOpacity' => array(
      'default' => '1',
      'desc' => t('Opacity of an external graphic.  This should be a 
        value between 0 and 1. Grahics that are already semitransparent,
        like alpha PNGs, should have this set to 1, or rendering problems in 
        Internet Explorer will ensue.'),
      'type' => 'float',
    ),
    'graphicXOffset' => array(
      'default' => '',
      'desc' => t('Where the X value of the "center" of an 
      externalGraphic should be.  This should be in pixels.'),
      'type' => 'integer',
    ),
    'graphicYOffset' => array(
      'default' => '',
      'desc' => t('Where the Y value of the "center" of an 
      externalGraphic should be.  This should be in pixels.'),
      'type' => 'integer',
    ),
    'graphicName' => array(
      'default' => '',
      'desc' => t('Name of a type of symbol to be used  for a point mark. 
      You can use the names "star", "cross", "x", "square", "triangle", 
      and "circle". See <a href="!url">Graphic Name Examples</a>.', array(
          '!url' => 'http://openlayers.org/dev/examples/graphic-name.html',
      )),
    ),
    'graphicZIndex' => array(
      'default' => '',
      'desc' => t('Z index of the feature. This determines drawing order: 
         elements with higher Z indexes are drawn on top of those with
         lower Z indexes.'),
      'type' => 'integer',
    ),
    'backgroundGraphic' => array(
      'default' => '',
      'desc' => t('The URL of an icon to display in addition to the main 
        feature. Use backgroundGraphicZIndex and graphicZIndex to set its 
        drawing order relative to main feature.'),
    ),
    'backgroundWidth' => array(
      'default' => '',
      'desc' => t('Width of a backgroundGraphic, in pixels.'),
      'type' => 'integer',
    ),
    'backgroundHeight' => array(
      'default' => '',
      'desc' => t('Height of a backgroundGraphic, in pixels.'),
      'type' => 'integer',
    ),
    'backgroundXOffset' => array(
      'default' => '',
      'desc' => t('Where the X value of the "center" of a 
        backgroundGraphic should be.  This should be in pixels.'),
      'type' => 'integer',
    ),
    'backgroundYOffset' => array(
      'default' => '',
      'desc' => t('Where the Y value of the "center" of a 
        backgroundGraphic should be.  This should be in pixels.'),
      'type' => 'integer',
    ),
    'backgroundGraphicZIndex' => array(
      'default' => '',
      'desc' => t('Z index of backgroundGraphic. This determines drawing 
         order: elements with higher Z indexes are drawn on top of those 
         with lower Z indexes.'),
      'type' => 'integer',
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
    'label' => array(
      'default' => '',
      'desc' => t('A label to show for features. '
        . 'Typically used with ${attribute} syntax.'),
    ),
    'labelAlign' => array(
      'default' => 'cm',
      'desc' => t('Label alignment.'),
      'options' => array(
        'cm' => t('Center, middle'),
        'ct' => t('Center, top'),
        'cb' => t('Center, bottom'),
        'lm' => t('Left, middle'),
        'lt' => t('Left, top'),
        'lb' => t('Left, bottom'),
        'rm' => t('Right, middle'),
        'rt' => t('Right, top'),
        'rb' => t('Right, bottom'),
      ),
    ),
    'labelXOffset' => array(
      'default' => '',
      'desc' => t('Label X offset. Positive numbers move label right.'),
    ),
    'labelYOffset' => array(
      'default' => '',
      'desc' => t('Label Y offset. Positive numbers move label up.'),
    ),
    'fontColor' => array(
      'default' => '',
      'desc' => t('Label font color.'),
    ),
    'fontSize' => array(
      'default' => '',
      'desc' => t('Label font size.'),
    ),
    'fontFamily' => array(
      'default' => '',
      'desc' => t('Label font family.'),
    ),
    'fontWeight' => array(
      'default' => '',
      'desc' => t('Label font weight.'),
    ),
  );

  return $properties;
}

/**
 * Styles add/edit form.
 */
function openlayers_ui_styles_form(&$form_state, $style = NULL, $edit = FALSE) {
  $form = array();
  
  $properties = openlayers_ui_styles_get_properties();
  
  $style_data = !empty($style->data) ? $style->data : array();

  // Pass style data along
  $form['style_data'] = array(
    '#type' => 'value',
    '#value' => array(
      'definitions' => $properties,
      'defaults' => $style_data
    )
  );

  // Style object basics
  $form['info'] = array(
    '#type' => 'fieldset',
    '#tree' => FALSE,
    '#title' => t('Basic Information'),
    '#description' => t('The basic information for the style, used to refer to and describe the style.'),
    '#collapsible' => TRUE,
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
  $form['data'] = array(
    '#type' => 'fieldset', 
    '#tree' => TRUE,
    '#title' => t('Style Properties and Plugins'),
    '#description' => t('Style properties are properties as 
      defined by the OpenLayers library.  Plugins are dynamically
      process the layer at render time; plugins may override the
      values that you have set for style properies.'),
    '#collapsible' => TRUE,
  );
  foreach ($properties as $key => $prop) {

    $form['data'][$key] = array(
      '#type' => 'fieldset',
      '#tree' => TRUE,
      '#title' => $key,
      '#description' => $prop['desc'],
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );

    $def_value = $prop['default'];
    $def_use_plugin = '';

    if ( isset($style_data[$key]) ) {
      if ( is_array($style_data[$key]) ) {
        $def_use_plugin = $style_data[$key]['plugin'];
      }
      else {
        $def_value = $style_data[$key];
      }
    }

    // Add plugin options, if any
    $handling_plugins = openlayers_ui_get_style_plugins_for_property($key);

    if ( ! empty($handling_plugins) ) {
      $plugin_options = array('' => t('none'));
      foreach ($handling_plugins as $plugname => $plug) {
        $plugin_options[$plugname] = $plug['title'];
      }

      $form['data'][$key]['uses_plugin'] = array(
        '#title' => t('Use plugin'),
        '#type' => 'select',
        '#options' => $plugin_options,
        '#default_value' => $def_use_plugin,
        '#ahah' => array(
          'path' => 'openlayers/ahah/style_plugin/' . $key,
          'wrapper' => $key . '-style-plugin',
          'method' => 'replace',
          'effect' => 'fade', // 'fade', 'none', 'slide'
        )
      );
    }

    // Hackish... but is that new for HTML ? ...
    $form['data'][$key]['plugin_conf_start'] = array(
      '#value' => '<div id="' . $key . '-style-plugin">'
    );

    if ( isset($style_data[$key]) && is_array($style_data[$key]) ) {
      $defaults = $style_data[$key]['conf'];
      $plugname = $style_data[$key]['plugin'];
      $form['data'][$key]['plugin'] =
        openlayers_ui_get_style_plugin_form($def_use_plugin, $defaults);
    }
    else {
      $form['data'][$key]['value'] = array(
        '#type' => !isset($prop['options']) ? 'textfield' : 'select',
        '#default_value' => $def_value
      );
      
      // Add options if needed
      if (isset($prop['options']) && is_array($prop['options'])) {
        $form['data'][$key]['value']['#options'] = $prop['options'];
      }
    }

    // Hackish... but is that new for HTML ? ...
    $form['data'][$key]['plugin_conf_end'] = array(
      '#value' => '</div>'
    );

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

  $prop_defn = $form_state['values']['style_data']['definitions'];
  
  //$data = $form_state['values']['data'];
  $data = $form_state['clicked_button']['#post']['data'];

  // Cast and unset values so JS can handle them better,
  // Unless value is in form ${attribute}
  foreach ($data as $key => $value) {
    if ( empty($data[$key]['uses_plugin']) ) {
      $data[$key] = $data[$key]['value'];
      if ($data[$key] === '') {
        unset($data[$key]);
      }
      elseif (isset($prop_defn[$key]['type']) &&
          strpos($data[$key], '${') !== 0) {
        if ($prop_defn[$key]['type'] == 'integer') {
          $data[$key] = (int) $data[$key];
        }
        elseif ($prop_defn[$key]['type'] == 'float') {
          $data[$key] = (float) $data[$key];
        }
      }
    }
    else {
      $spec = array(
        'plugin' => $data[$key]['uses_plugin'],
      );
      if ( isset($data[$key]['plugin']['conf']) ) {
        $spec['conf'] = $data[$key]['plugin']['conf'];
      }
      $data[$key] = $spec;
    }
  }

  $form_state['values']['data'] = $data;
  $form_state['clicked_button']['#post']['data'] = $data;

  $style = new stdClass();
  $style->name = $form_state['values']['name'];
  $style->title = $form_state['values']['title'];
  $style->description = $form_state['values']['description'];
  $style->data = $data;

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

/**
 * Get a list of style plugins providing handling of a given property
 *
 * @param $property_name Name of the property we're interested in
 */
function openlayers_ui_get_style_plugins_for_property($property_name) {
  $handling = array();
  $available = openlayers_style_plugins();
  foreach ($available as $plugname => $plugin) {
    $plugin_class = ctools_plugin_get_class($plugin, 'style_plugin');
    if (empty($plugin_class)) continue; // should we watchdog here ?
    $plugin_instance = new $plugin_class;
    if ($plugin_instance->can_handle_property($property_name)) {
      $handling[$plugname] = $plugin;
    }
  }
  return $handling;
}

/**
 * Get options of a style plugin by plugin name
 */
function openlayers_ui_get_style_plugin_form($plugname, $defaults) {

  $form = array();

  $available = openlayers_style_plugins();
  if (!$available[$plugname]) {
    watchdog('openlayers_ui', 'Style plugin !name unknown',
      array('!name' => $plugname), WATCHDOG_ERROR);
    return $form;
  }

  $plugin = $available[$plugname];
  $plugin_class = ctools_plugin_get_class($plugin, 'style_plugin');
  if (empty($plugin_class)) {
    watchdog('openlayers_ui', 'Style plugin !name does not have a class?!',
      array('!name' => $plugname), WATCHDOG_ERROR);
    return $form; 
  }

  // Create object and ask it for options
  $style_plugin = new $plugin_class;

  $form = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#description' => $plugin['description'],
    'conf' => $style_plugin->options_form($defaults)
  );

  return $form;
}

function openlayers_ui_style_plugin_ahah($propname) {
  $posted = $_POST["data"][$propname];
  $plugname = $posted['uses_plugin'];

  // Get cached form 
  $form_state = array('storage' => NULL, 'submitted' => FALSE);
  $form_build_id = $_POST['form_build_id'];
  $form_cached = form_get_cache($form_build_id, $form_state);

  // Get style_data from cached form, and defaults
  $style_data = $form_cached['style_data']['#value'];
  $defaults = $style_data['defaults'];

  if (!$plugname) {
    // Find default value
    $property = $style_data['definitions'][$propname];
    $def_value = $property['default'];
    if (isset($defaults[$propname]) && !is_array($defaults[$propname])) {
      $def_value = $defaults[$propname];
    }

    $form = array(
      '#type' => !isset($property['options']) ? 'textfield' : 'select',
      '#default_value' => $def_value
    );
      
    // Add options if needed
    if (isset($property['options']) && is_array($property['options'])) {
      $form['value']['#options'] = $property['options'];
    }

    $form['#parents'] = array('data', $propname, 'value');
  }
  else {
    // Find default value
    $defs = array();
    if ( isset($defaults[$propname]) && is_array($defaults[$propname])
      && $defaults[$propname]['plugin'] == $plugname ) {
      $defs = $defaults[$propname]['conf'];
    }

    $form = openlayers_ui_get_style_plugin_form($plugname, $defs);
    $form['#parents'] = array('data', $propname, 'plugin');
  }


  $form_state = array(); // TODO: what to do with this ?
  $plugin_form_built = form_builder('style_plugin_form', $form, $form_state);

  $output = drupal_render($plugin_form_built);

  // Final rendering callback.
  drupal_json(array('status' => TRUE, 'data' => $output));
}
