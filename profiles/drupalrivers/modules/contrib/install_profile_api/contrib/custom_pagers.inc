<?php
// $Id: custom_pagers.inc,v 1.1.2.1 2008/08/15 20:40:41 quicksketch Exp $

/**
 * Create a custom pager.
 *
 * @param $data
 *   An array of pager properties
 *   - title
 *   - list_php
 *   - view
 *   - args
 *   - position
 *   - visibility_php
 *   - node_type
 *   - reverse_list
 *   - cache_list
 *
 * @return
 *   The pid of the new pager if successful, boolean FALSE otherwise.
 */
function install_custom_pagers_add_pager($data = array()) {
  $pid = FALSE;

  // Check if both custom pager and views modules are enabled.
  if (module_exists('custom_pagers') && module_exists('views')) {
    $all_views = views_get_all_views();
    $view_to_use = NULL;
    foreach ($all_views as $view) {
      // Only views that have fields are valid for the custom pager.
      if (!empty($view->display['default']->display_options['fields']) && $view->base_table == 'node') {
        $view_to_use = ($data['view'] === $view->name) ? $view->name: $view_to_use;
      }
    }
    
    $data['view'] = $view_to_use;

    // Check that we have a title and a view to associate this pager with.
    $valid = (!$data['title'] || !$data['view']) ? FALSE : TRUE;
    if ($valid) {
      // Insure the node type exists.
      $node_types = node_get_types('names');
      $node_type = NULL;
      foreach ($node_types as $type => $val) {
        $node_type = ($data['node_type'] === $type || $data['node_type'] === $val) ? $type : $node_type;
      }
      
      // Check that we have a node type.
      if ($node_type) {
        $data['list_php'] = ($data['list_php']) ? $data['list_php'] : '';
        $data['args'] = ($data['args']) ? implode("\n", $data['args']) : '';
        $data['position'] = ($data['position']) ? $data['position'] : 'top';
        $data['visibility_php'] = ($data['visibility_php']) ? $data['visibility_php'] : '';
        $data['node_type'] = ($data['node_type']) ? $data['node_type'] : '';
        $data['reverse_list'] = ($data['reverse_list']) ? $data['reverse_list'] : 0;
        $data['cache_list'] = ($data['cache_list']) ? $data['cache_list'] : 0;

        // Key the fields.
        $fields = array_keys($data);

        // Prepare the query:
        foreach ($data as $key => $value) {
          if (in_array((string) $key, $fields)) {
            $k[] = db_escape_string($key);
            $v[] = $value;
            $s[] = is_numeric($value) ? "%d" : "'%s'";
          }
        }
        
        db_query("INSERT INTO {custom_pager} (". implode(", ", $k) .") VALUES (". implode(", ", $s) .")", $v);
        $pid = db_last_insert_id('custom_pager', 'pid');
      }
      else {
        drupal_set_message('Custom pagers requires a valid node type.', 'error');
      }
    }
    else {
      drupal_set_message('Custom pagers requires both a valid title and a valid view for association.', 'error');
    }
  }
  else {
    drupal_set_message('Both modules "custom_pagers" and "views" need to be dependents in order to create custom pagers.', 'error');
  }

  return $pid;
}