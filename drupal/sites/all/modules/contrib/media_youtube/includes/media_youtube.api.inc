<?php
// $Id: media_youtube.api.inc,v 1.1.2.7 2011/02/04 17:59:08 aaron Exp $

/**
 * @file
 * Miscellaneous API function calls for Media: YouTube.
 */

/**
 * Check the upload status of a video.
 *
 * @param string $video_id
 *  The video to check.
 * @param string $youtube_username
 *  The youtube username owning the video to check.
 * @param string $youtube_password
 *  The password of the youtube user.
 * @return array
 *  An associative array, keyed as follows:
 *    'status'  => The status returned from YouTube.
 *    'message' => A message describing the video's status.
 */
function _media_youtube_check_upload($video_id, $youtube_username = NULL, $youtube_password = NULL) {
  _media_youtube_set_include_path();

  $path = media_youtube_zend_path();
  Zend_Loader::loadClass('Zend_Gdata_YouTube', $path);
  Zend_Loader::loadClass('Zend_Gdata_App_Exception', $path);
  Zend_Loader::loadClass('Zend_Gdata_App_HttpException', $path);
  Zend_Loader::loadClass('Zend_Gdata_App_Extension_Control', $path);
  Zend_Loader::loadClass('Zend_Gdata_YouTube_Extension_State', $path);
  Zend_Loader::loadClass('Zend_Gdata_AuthSub', $path);
  Zend_Loader::loadClass('Zend_Gdata_ClientLogin', $path);
  Zend_Loader::loadClass('Zend_Uri_Http', $path);

  $httpClient = _media_youtube_get_auth_sub_http_client($youtube_username, $youtube_password);
  $youTubeService = new Zend_Gdata_YouTube($httpClient);

  try {
    $feed = $youTubeService->getuserUploads('default');
  }
  catch (Exception $e) {
    $message = 'Unable to retrieve upload feed: @error.';
    $variables = array('@error' => $e->getMessage());
    watchdog('media_youtube', $message, $variables, WATCHDOG_ERROR);
    return array('status' => t('error'), 'message' => t($message, $variables));
  }

  $status = 'unknown';
  $message = 'No further status information available yet.';

  if (is_array($feed)) {
    foreach ($feed as $videoEntry) {
      if ($videoEntry->getVideoId() == $video_id) {
        // Check if video is in draft status.
        try {
          $control = $videoEntry->getControl();
        } catch (Zend_Gdata_App_Exception $e) {
          $message = 'Unable to retrieve control element: @error.';
          $variables = array('@error' => $e->getMessage());
          watchdog('media_youtube', $message, $variables, WATCHDOG_ERROR);
          return array('status' => t('error'), 'message' => t($message, $variables));
        } catch (Zend_Gdata_App_HttpException $e) {
          $message = 'Unable to retrieve control element: @error.';
          $variables = array('@error' => $e->getMessage());
          watchdog('media_youtube', $message, $variables, WATCHDOG_ERROR);
          return array('status' => t('error'), 'message' => t($message, $variables));
        }

        if ($control instanceof Zend_Gdata_App_Extension_Control) {
          if (($control->getDraft() != null) &&
            ($control->getDraft()->getText() == 'yes')) {
            $state = $videoEntry->getVideoState();
            if ($state instanceof Zend_Gdata_YouTube_Extension_State) {
              $status = $state->getName();
              $message = $state->getText();
            }
          }
        }
      }
    }
  }
  return array('status' => t($status), 'message' => t($message));
}

/**
 * Convenience method to obtain an authenticted Zend_Http_Client object.
 *
 * @return Zend_Http_Client An authenticated client.
 */
function _media_youtube_get_auth_sub_http_client($youtube_username = NULL, $youtube_password = NULL) {
  $youtube_username = isset($youtube_username) ? $youtube_username : media_youtube_variable_get('youtube_username');
  $youtube_password = isset($youtube_password) ? $youtube_password : media_youtube_variable_get('youtube_password');
  _media_youtube_set_include_path();

  $path = media_youtube_zend_path();
  Zend_Loader::loadClass('Zend_Gdata_ClientLogin', $path);
  Zend_Loader::loadClass('Zend_Gdata_App_Exception', $path);
  Zend_Loader::loadClass('Zend_Gdata_App_HttpException', $path);
  try {
    $httpClient = Zend_Gdata_ClientLogin::getHttpClient($youtube_username, $youtube_password, 'youtube', NULL, MEDIA_YOUTUBE_APP_ID, NULL, NULL, MEDIA_YOUTUBE_AUTH_URL);
  } catch (Zend_Gdata_App_Exception $e) {
    $message = 'Could not obtain authenticated Http client object: @error.';
    $variables = array('@error' => $e->getMessage());
    watchdog('media_youtube', $message, $variables, WATCHDOG_ERROR);
    return;
  } catch (Zend_Gdata_App_HttpException $e) {
    $message = 'Could not obtain authenticated Http client object: @error.';
    $variables = array('@error' => $e->getMessage());
    watchdog('media_youtube', $message, $variables, WATCHDOG_ERROR);
    return;
  }
  $httpClient->setHeaders('X-GData-Key', 'key='. media_youtube_variable_get('api_key'));
  return $httpClient;
}

function _media_youtube_save_status_data(&$node, $code, $item, $field_name, $delta) {
  // Check the real status of the video.
  $status = media_youtube_check_status($code);

  // If the status has changed, then save it in the node.
  if (($status != $item['status']) && $node->nid) {
    foreach ($node->{$field_name} as $delta => $field) {
      if (($field['provider'] == 'youtube') && ($field['value'] == $code) && ($field['status'] == $item['status'])) {
        $node->{$field_name}[$delta]['status'] = $status;
      }
    }
    node_save($node);
  }

  return $status;
}
