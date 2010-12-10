<?php
// $Id: token.api.php,v 1.1.2.2 2010/09/23 01:04:12 davereid Exp $

/**
 * @file
 * Hooks provided by the token module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Provide metadata about available placeholder tokens and token types.
 *
 * @param $type
 *   The type of tokens to list (e.g. 'global', 'node', or 'user'). To list all
 *   tokens, use 'all'.
 *
 * @return
 *   An associative array of available tokens. The base array is keys of token
 *   types and an array of its tokens. The token arrays are keys containing the
 *   raw name of the token and values containing its user-friendly name.
 */
function hook_token_list($type = 'all') {
  $tokens = array();

  if ($type == 'global' || $type == 'all') {
    $tokens['global']['random-number'] = t('A randomly generated number.');

  }
  if ($type == 'node' || $type == 'all') {
    $tokens['node']['node-random-nid'] = t("A randomly generated number between one and the node's unique ID.");
  }

  return $tokens;
}

/**
 * Provide replacement values for placeholder tokens.
 *
 * @param $type
 *   The type of token being replaced (e.g. 'global', 'node', or 'user').
 * @param $data
 *   (optional) An object to be used when generating replacement values.
 * @param $options
 *   (optional) A associative array of options to control the token replacement
 *   process.
 *
 * @return
 *   An associative array of replacement values, keyed by the original 'raw'
 *   tokens that were found in the source text. For example:
 *   $values['title-raw'] = 'My new node';
 */
function hook_token_values($type, $object = NULL, $options = array()) {
  $values = array();

  if ($type == 'global') {
    $values['random-number'] = mt_rand();
  }

  if ($type == 'node' && !empty($object)) {
    $values['node-random-nid'] = mt_rand(1, $object->nid);
  }

  return $values;
}

/**
 * @} End of "addtogroup hooks".
 */
