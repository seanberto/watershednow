<?php
// $Id: media_vimeo.variables.inc,v 1.1.2.1 2010/09/23 21:04:55 aaron Exp $

/**
 * @file media_vimeo/includes/media_vimeo.variables.inc
 * Contains the variables and defaults used by Media: Vimeo.
 */

/**
 * The variable namespace for Media: Vimeo.
 */
define('MEDIA_VIMEO_NAMESPACE', 'media_vimeo__');

/**
 * This is the main URL for Vimeo.
 */
define('MEDIA_VIMEO_MAIN_URL', 'http://vimeo.com/');

/**
 * This is the URL to the API of Vimeo.
 */
define('MEDIA_VIMEO_API_INFO', 'http://vimeo.com/api');

/**
 * This defines the version of the content data array that we serialize
 * in data(). If we change the expected keys of that array,
 * we must increment this value, which will allow older content to be updated
 * to the new version automatically.
 */
define('MEDIA_VIMEO_DATA_VERSION', 2);

/**
 * These are player defaults. @TODO: Use variable namespace instead.
 */
define('MEDIA_VIMEO_COLOR_DEFAULT', '#01AAEA');

/**
 * Wrapper for variable_get() that uses the Media: Vimeo variable registry.
 *
 * @param string $name
 *  The variable name to retrieve. Note that it will be namespaced by
 *  pre-pending MEDIA_VIMEO_NAMESPACE, as to avoid variable collisions
 *  with other modules.
 * @param unknown $default
 *  An optional default variable to return if the variable hasn't been set
 *  yet. Note that within this module, all variables should already be set
 *  in the media_vimeo_variable_default() function.
 * @return unknown
 *  Returns the stored variable or its default.
 *
 * @see media_vimeo_variable_set()
 * @see media_vimeo_variable_del()
 * @see media_vimeo_variable_default()
 */
function media_vimeo_variable_get($name, $default = NULL) {
  // Allow for an override of the default.
  // Useful when a variable is required (like $path), but namespacing still desired.
  if (!isset($default)) {
    $default = media_vimeo_variable_default($name);
  }
  // Namespace all variables
  $variable_name = MEDIA_VIMEO_NAMESPACE . $name;
  return variable_get($variable_name, $default);
}

/**
 * Wrapper for variable_set() that uses the Media: Vimeo variable registry.
 *
 * @param string $name
 *  The variable name to set. Note that it will be namespaced by
 *  pre-pending MEDIA_VIMEO_NAMESPACE, as to avoid variable collisions with
 *  other modules.
 * @param unknown $value
 *  The value for which to set the variable.
 * @return unknown
 *  Returns the stored variable after setting.
 *
 * @see media_vimeo_variable_get()
 * @see media_vimeo_variable_del()
 * @see media_vimeo_variable_default()
 */
function media_vimeo_variable_set($name, $value) {
  $variable_name = MEDIA_VIMEO_NAMESPACE . $name;
  return variable_set($variable_name, $value);
}

/**
 * Wrapper for variable_del() that uses the Media: Vimeo variable registry.
 *
 * @param string $name
 *  The variable name to delete. Note that it will be namespaced by
 *  pre-pending MEDIA_VIMEO_NAMESPACE, as to avoid variable collisions with
 *  other modules.
 *
 * @see media_vimeo_variable_get()
 * @see media_vimeo_variable_set()
 * @see media_vimeo_variable_default()
 */
function media_vimeo_variable_del($name) {
  $variable_name = MEDIA_VIMEO_NAMESPACE . $name;
  variable_del($variable_name);
}

/**
 * The default variables within the Media: Vimeo namespace.
 *
 * @param string $name
 *  Optional variable name to retrieve the default. Note that it has not yet
 *  been pre-pended with the MEDIA_VIMEO_NAMESPACE namespace at this time.
 * @return unknown
 *  The default value of this variable, if it's been set, or NULL, unless
 *  $name is NULL, in which case we return an array of all default values.
 *
 * @see media_vimeo_variable_get()
 * @see media_vimeo_variable_set()
 * @see media_vimeo_variable_del()
 */
function media_vimeo_variable_default($name = NULL) {
  static $defaults;

  if (!isset($defaults)) {
    $defaults = array(
      'color_override' => FALSE,
      'color' => MEDIA_VIMEO_COLOR_DEFAULT,
      'on_screen_info' => array('portrait', 'title', 'byline'),
      'full_screen' => 1,
      'api_key' => '',
      'api_secret' => '',
      'thumb_size' => '160',
      'default_width' => 400,
      'default_height' => 227,
      'universal' => TRUE,
    );
  }

  if (!isset($name)) {
    return $defaults;
  }

  if (isset($defaults[$name])) {
    return $defaults[$name];
  }
}

/**
 * Return the fully namespace variable name.
 *
 * @param string $name
 *  The variable name to retrieve the namespaced name.
 * @return string
 *  The fully namespace variable name, prepended with
 *  MEDIA_VIMEO_NAMESPACE.
 */
function media_vimeo_variable_name($name) {
  return MEDIA_VIMEO_NAMESPACE . $name;
}
