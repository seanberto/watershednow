<?php
// $Id: flag.inc,v 1.1.2.4.2.1 2009/03/03 18:22:06 quicksketch Exp $

/**
 * Create a new Flag type.
 *
 * @param $data
 *   An array containing the flag information to store.
 *     [content_type]  The content type that is to be associated. The term
 *     "content_type" is from the flag module and ONLY refers to "node" or "comment".
 *     []
 *
 * @return integer
 *   The $fid of the stored flag if successfull, boolean FALSE otherwise.
 */
function install_flag_add_flag($data = array()) {
  // Make sure the module is installed.
  $enabled = (module_exists('flag')) ? TRUE : FALSE;
  // Initialize return.
  $fid = FALSE;

  // Initialize the required vars.
  $name         = $data['name'];
  $title        = isset($data['title']) ? $data['title'] : $data['name'];
  $content_type = isset($data['content_type']) ? $data['content_type'] : 'node';
  $flag_short   = isset($data['flag_short']) ? $data['flag_short'] : '';
  $unflag_short = isset($data['unflag_short']) ? $data['unflag_short'] : '';
  $roles        = isset($data['roles']) ? $data['roles'] : array();
  $types        = isset($data['types']) ? $data['types'] : FALSE;

  // If we've filled our requirements then we can move on.
  if ($enabled && $name && $flag_short && $types) {
    // Test that roles are valid.
    $r_types = (is_array($roles)) ? $roles : array($roles);
    $role_arr = array();
    foreach ($r_types as $idx => $role) {
      $r = install_get_rid($role);
      // If role exists add it.
      if ($r) {
        $role_arr[] = $r;
      }
    }

    // Test that content types are valid.
    $n_types = (is_array($types)) ? $types : array($types);
    $node_types = node_get_types('names');
    $type_arr = array();
    foreach ($node_types as $type => $val) {
      $n = array_search($type, $n_types);
      // If the content type exists then add it to the params.
      if ($n !== FALSE) {
        $type_arr[] = $type;
      }
    }

    // If we have valid content types.
    if (!empty($type_arr)) {
      // Update roles to a string we can store.
      $roles = $role_arr;
      $types = $type_arr;

      // Initialize flag array.
      $flag = array();
      // Populate the flag vars.
      $flag['fid']            = NULL;
      $flag['content_type']   = $content_type;
      $flag['name']           = $name;
      $flag['title']          = $title;
      $flag['flag_short']     = $flag_short;
      $flag['flag_long']      = isset($data['flag_long']) ? $data['flag_long'] : '';
      $flag['flag_message']   = isset($data['flag_message']) ? $data['flag_message'] : '';
      $flag['unflag_short']   = $unflag_short;
      $flag['unflag_long']    = isset($data['unflag_long']) ? $data['unflag_long'] : '';
      $flag['unflag_message'] = isset($data['unflag_message']) ? $data['unflag_message'] : '';
      $flag['roles']          = $roles;
      $flag['global']         = isset($data['global']) ? $data['global'] : 0;
      $flag['types']          = $types;
      $flag['display']        = array();

      // If content type is node.
      if ($content_type === 'node') {
        $flag['display']['show_on_teaser'] = ($data['show_on_teaser']) ? 1 : 0;
        $flag['display']['show_on_page']   = ($data['show_on_page']) ? 1 : 0;
        $flag['display']['show_on_form']   = ($data['show_on_form']) ? 1 : 0;
      }
      else {
        $flag['display']['show_on_comment'] = ($data['show_on_comment']) ? 1 : 0;
      }

      // Send flag to insert function and return fid.
      $fid = _install_flag_insert($flag);
      
    }
  }

  return $fid;
}

/**
 * Insert a full and populated flag array.
 *
 * @param $flag
 *   The flag object.
 * @return
 *   The fid of the stored flag if successful, boolean FALSE otherwise.
 */
function _install_flag_insert($flag_properties) {
  $fid = FALSE;

  // Only store if the flag is actually populated.
  if (is_array($flag_properties) && !empty($flag_properties)) {
    $flag = flag_create_handler('node');
    foreach ($flag_properties as $property => $value) {
      $flag->$property = $value;
    }
    $flag->save();
    $fid = $flag->fid;
  }

  return $fid;
}

/**
 * Return a flag by its machine readable name
 *
 * @param $flag
 *   The machine readable name of the flag.
 *
 * @return
 *   The full flag object.
 */
function install_flag_get_flag($flag) {
  return (array)flag_get_flag($flag);
}

/**
 * Update/Overwrite a flags assigned roles
 *
 * @param $flag
 *   The name of the flag to update.
 * @param $roles
 *   The roles to assign to the flag.
 *   An array of roles (eg: array('role1', 'role2') or array(1,2))
 * @param $clear
 *   Boolean to determine if the new roles should replace the current assigned
 *   roles or just append to the current assigned roles.
 *
 * @return  $fid
 *   The fid of the row that was updated or FALSE if the function failed
 */
function install_flag_roles_update($flag, $roles, $clear = FALSE) {
  $fid = FALSE;

  $numeric = (is_numeric($flag)) ? TRUE : FALSE;

  $flag = install_flag_get_flag($flag);

  $new_roles = '';
  $temp_roles = array();
  
  if ($flag) {
    // if role is an array
    if (is_array($roles)) {
      foreach ($roles as $idx => $role) {
        if (is_numeric($role)) {
          $temp_roles[] = $role;
        } else {
          $r = install_get_rid($role);
          $r = ($r) ? $r : install_add_role($role);
          $temp_roles[] = $r;
        }
      }
    }
    
    // If $flag is an array then we are adding to the roles.
    if (is_array($flag)) {
      $fid = $flag['fid'];
      $current_roles = explode(',', $flag['roles']);
      $new_roles = implode(',', array_unique(array_merge($current_roles, $temp_roles)));
      db_query("UPDATE {flags} SET roles = '%s' WHERE fid = %d", $new_roles, $flag['fid']);
    }
    else {
      $fid = $flag;
      $new_roles = implode(',', $temp_roles);
      db_query("UPDATE {flags} SET roles = '%s' WHERE fid = %d", $new_roles, $flag);
    }
  }

  return $fid;
}


/**
 * Update/Overwrite a flags assigned nodes.
 *
 * @param $flag
 *   The flag name of the flag to update.
 * @param $node_types
 *   An array of machine readable node_types to assign to the flag.
 * @param $clear
 *   Boolean to determine to overwrite data or to append data.
 *
 * @return $fid
 *   The fid of the row that was updated or FALSE if the function fails.
 */
function install_flag_nodes_update($flag, $node_types, $clear = FALSE) {
  $fid = FALSE;
  
  $flag = install_flag_get_flag($flag);
  $new_types = array();
  $current_types = array();
  
  if ($flag) {
    // if clear then we delete all current stored types
    if ($clear) {
      install_flag_delete_content_types($flag);
    } else {
      $current_types = install_flag_get_content_types($flag);
    }
    
    $temp_types = array();
    $all_nodes = node_get_types('names');
    foreach ($node_types as $idx => $type) {
      // be sure that node_types are valid
      if (isset($all_nodes[$type])) {
        $temp_types[] = $type;
      }
    }
    
    $temp_types = (empty($current_types)) ? $temp_types : array_unique(array_merge($current_types, $temp_types));
    $new_types  = ($clear) ? $temp_types : array_diff($temp_types, $current_types);
    
    $fid = $flag;
    install_flag_insert_content_types($fid, $new_types);
  }
  
  return $fid;
}


/**
 * Return an array of node_types associated with a fid.
 *
 * @param $fid
 *   The fid of the flag to search.
 * @return
 *   An array containing the node_types associated with the fid.
 */
function install_flag_get_content_types($fid) {
  $return = array();
  $result = db_query("SELECT type FROM {flag_types} WHERE fid = %d", $fid);
  while ($row = db_fetch_array($result)) {
    $return[] = $row['type'];
  }
  return $return;
}


/**
 * Delete node_types associated with a specific fid.
 *
 * If no node_types are passed to this function then this function will clear
 * all node_types associated with this fid.
 *
 * @param $fid
 *   The fid of the flag to search.
 * @param $types
 *   An array of node types to remove.
 */
function install_flag_delete_content_types($fid, $types = array()) {
  if (empty($types)) {
    db_query("DELETE FROM {flag_types} WHERE fid = %d", $fid);
  } else {
    foreach ($types as $idx => $type) {
      db_query("DELETE FROM {flag_types} WHERE fid = %d AND type = '%s'", $fid, $type);
    }
  }
}


/**
 * Insert node_types with a specific fid.
 *
 * @param $fid
 *   The fid of the flag to search.
 * @param $types
 *   An array of node_types that should be inserted.
 */
function install_flag_insert_content_types($fid, $types = array()) {
  foreach ($types as $idx => $type) {
    db_query("INSERT INTO {flag_types} (fid, type) VALUES (%d, '%s')",$fid, $type);
  }
}

