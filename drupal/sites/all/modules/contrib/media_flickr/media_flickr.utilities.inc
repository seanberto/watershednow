<?php
// $Id: media_flickr.utilities.inc,v 1.1.2.4 2009/12/03 19:07:28 aaron Exp $

/**
 *  @file
 *  Utility functions for Media: Flickr.
 */

/**
 *  Based on the Photo ID of a Flickr image, this will return the URL to the
 *  image itself.
 *  @param $photo_code
 *    The Flickr photo code.
 *  @param $width
 *  @param $height
 *    We use this to guess the actual size provided by Flickr.
 *  @param $cached
 *    If TRUE, then we'll cache the remote URL if the attempt to save the file
 *    locally fails.
 */
function _media_flickr_photo_url($photo_code, $width = 0, $height = 0, $cached = TRUE) {
  if ($photo_code) {
    $size = media_flickr_guess_size($width, $height);
    return media_flickr_photo_url_from_size($photo_code, $size);
  }
}

function _media_flickr_photo_url_from_size($photo_code, $size = 5) {
  if ($photo_code) {
    // If we're storing our files locally, then grab the filepath now.
    if ($store = variable_get('media_flickr_store_local', FALSE)) {
      $filepath = db_result(db_query("SELECT f.filepath FROM {files} f INNER JOIN {media_flickr_sizes} m ON m.fid = f.fid AND m.code = '%s' AND m.size = %d", $photo_code, $size));
    }
    // If we don't have a filepath, it's either because we're not storing
    // them locally, or it hasn't been saved locally yet.
    if (!$filepath && $store) {
      // Let's attempt to store our file locally.
      $filepath = media_flickr_store_local($photo_code, $size);
    }
    // We're not storing locally, or have otherwise failed,
    // so now we'll just grab the remote URL.
    if (!$filepath) {
      $filepath = media_flickr_photo_remote_url($photo_code, $size);
    }
    // URLize that path...
    $filepath = url($filepath, array('absolute' => TRUE));
    return $filepath;
  }
}

function _media_flickr_store_local($photo_code, $size) {
  static $count;
  // Obviously, only go forward if we have a photo_code.
  if ($photo_code) {
    // Get the URL to the original thumbnail.
    $thumbnail = media_flickr_photo_remote_url($photo_code, $size);

    // Go forward only if we have a URL to go by, and we haven't already
    // fetched our alloted remote images for this page load.
    if ($thumbnail && ((($max = variable_get('media_flickr_max_saves', 10)) && ($count < $max)) || !$max)) {
      // If we've attempted to save a thumbnail and failed,
      // let's not try again for awhile.
      if ($cached && $cache = cache_get('media_flickr:'. $photo_code .':'. $size, 'cache')) {
        return $cache->data;
      }

      // We should only attempt to save a few photos at a time,
      // to keep from initial slow page loads.
      $count++;

      // Attempt to fetch the thumbnail from the provided URL.
      $image = file_get_contents($thumbnail);

      // Only go forward if we actually have an image stream.
      if ($image) {

        // Add in our check of the the file name length.
        $validators['file_validate_name_length'] = array();

        // Allow for transliteration, which will take unicode data and convert
        // it to US-ASCII for better file storage.
        if (module_exists('transliteration')) {
          // Transliterate our original URL.
          $thumbnail = transliteration_get($thumbnail);
        }

        // Our new filepath will be in the form of media-flickr-7351895427047-5.jpg.
        $basename = 'media-flickr-'. $photo_code .'-'. $size .'.'. pathinfo($thumbnail, PATHINFO_EXTENSION);

        // Get the base Drupal files path.
        $directory = file_directory_path();
        $dir = variable_get('media_flickr_image_path', '');
        if ($dir) {
          // Add the field's image path here.
          $directory .= '/'. $dir;
        }
        // Create a new filepath from our desired filename.
        $filepath = file_create_filename($basename, $directory);

        // Begin building file object.
        $file = new stdClass();

        // The new file will be associated with the global user.
        global $user;

        $file->uid = $user->uid;
        // Strip out the query if provided.
        $file->filename = parse_url($basename, PHP_URL_PATH);
        $file->filepath = parse_url($filepath, PHP_URL_PATH);

        // If we have mimedetect, then do so. Otherwise we make a best guess
        // based on the filename.
        $file->filemime = module_exists('mimedetect') ? mimedetect_mime($file) : file_get_mimetype($file->filename);

        // Rename potentially executable files, to help prevent exploits.
        if (preg_match('/\.(php|pl|py|cgi|asp|js)$/i', $file->filename) && (substr($file->filename, -4) != '.txt')) {
          $file->filemime = 'text/plain';
          $file->filepath .= '.txt';
          $file->filename .= '.txt';
        }

        $file->source = 'media_flickr_fetch_remote_thumbnail';
        $file->destination = file_destination($file->filepath, $replace);
        $file->filesize = strlen($image);

        // Call the validation functions.
        $errors = array();
        foreach ($validators as $function => $args) {
          array_unshift($args, $file);
          $errors = array_merge($errors, call_user_func_array($function, $args));
        }

        // Check for validation errors.
        if (!empty($errors)) {
          $message = 'The remote Flickr image %name could not be saved.';
          $arguments = array('%name' => $file->filename);
          if (count($errors) > 1) {
            $message .= '<ul><li>'. implode('</li><li>', $errors) .'</li></ul>';
          }
          else {
            $message .= ' '. array_pop($errors);
          }
          watchdog('media_flickr', $message, $arguments, WATCHDOG_ERROR);

          // Let's also cache it for awhile, as this can take a long time.
          cache_set('media_flickr:'. $photo_code .':'. $size, url($thumbnail), 'cache', time() + variable_get('emfield_cache_duration', 3600));

          return $thumbnail;
        }

        if (!file_save_data($image, $file->filepath, FILE_EXISTS_RENAME)) {
          watchdog('file', 'Upload error. Could not move file %file to destination %destination.', array('%file' => $file->filename, '%destination' => $file->destination));
          // Let's also cache it for awhile, as this can take a long time.
          cache_set('media_flickr:'. $photo_code .':'. $size, url($thumbnail), 'cache', time() + variable_get('emfield_cache_duration', 3600));

          return $thumbnail;
        }
        // If we made it this far it's safe to record this file in the database.
        $file->status = FILE_STATUS_PERMANENT;
        $file->timestamp = time();
        drupal_write_record('files', $file);

        $flickr = new stdClass();
        $flickr->fid = $file->fid;
        $flickr->size = $size;
        $flickr->code = $photo_code;
        drupal_write_record('media_flickr_sizes', $flickr);

        // Let modules add additional properties to the yet barebone file object.
        // This uses the future hook_file from d7's API. Not sure if anything
        // actually uses this right now, but they might in the future.
        foreach (module_implements('file_insert') as $module) {
          $function =  $module .'_file_insert';
          $function($file);
        }

        return $file->filepath;
      }
      // Let's also cache it for awhile, as this can take a long time.
      cache_set('media_flickr:'. $photo_code .':'. $size, url($thumbnail), 'cache', time() + variable_get('emfield_cache_duration', 3600));
    }

    // As a failsafe, let's return the remote URL.
    return $thumbnail;
  }
}

/**
 *  Returns an array of all URLs for photos associated with a photoset,
 *  associated by photo code. These will be of the Flickr specified size (1-5),
 *  and may be local or remote, based on settings and availability.
 *
 *  @param $photoset
 *    The Flickr photoset id.
 *  @param $size
 *    Optional. A number from 1-5 (small to large).
 */
function _media_flickr_photoset_load_photos($photoset, $size = 5) {
  static $photosets;

  if (is_null($photosets)) {
    $photosets = array();
  }

  $id = $photoset['photoset']['id'];

  // We cache our results in a static variable.
  if (is_null($photosets[$id])) {
    // Reset the array of photos.
    $photosets[$id] = array();

    // If we're allowed to store images locally, then grab the latest list
    // of local URL's.
    if ($store_local = variable_get('media_flickr_store_local', FALSE)) {
      $results = db_query("SELECT z.code, f.filepath FROM {media_flickr_sets} s INNER JOIN {media_flickr_sizes} z ON z.code = s.code AND z.size = %d INNER JOIN {files} f ON f.fid = z.fid WHERE s.photoset = '%s'", $size, $photoset['photoset']['id']);
      while ($result = db_fetch_object($results)) {
        $photosets[$id][$result->code] = $result->filepath;
      }
    }

    // @TODO: remove 'dead' photos from photoset...

    // Ensure we have a URL for each photo in the photoset.
    // This can be local or remote.
    foreach ($photoset['photoset']['photo'] as $photo_code => $photo) {
      // If we don't have a URL yet, grab one.
      if (!isset($photosets[$id][$photo_code])) {
        // If we're allowed to store local photos, then attempt to do so now.
        if ($store_local) {
          // First ensure we have the photo associated with this photoset.
          if (!db_result(db_query("SELECT COUNT(*) FROM {media_flickr_sets} WHERE photoset = '%s' AND code = '%s'", $id, $photo_code))) {
            // The photo is newly associated to this photoset. Put it in place.
            $record = array(
              'photoset' => $id,
              'code' => $photo_code,
            );
            drupal_write_record('media_flickr_sets', $record);

            // Now record the metadata associated with a photo.
            media_flickr_record_photo($photo_code);

            // Now we'll try to grab the URL manually, as it may have already
            // been stored locally through another photoset. If not, this will
            // attempt to fetch a photo and store it, or return the remote URL.
            $photosets[$id][$photo_code] = media_flickr_photo_url_from_size($photo_code, $size);
          }
          else {
            // We have the proper association; we just need this size photo.
            // Hopefully we'll grab a local photo. If not, we'll get the remote.
            $photosets[$id][$photo_code] = media_flickr_store_local($photo_code, $size);
          }
        }
        else {
          // Let's be happy with the remote URL.
          $photosets[$id][$photo_code] = media_flickr_photo_remote_url($photo_code, $size);
        }
      }
    }
  }

  return $photosets[$id];
}
