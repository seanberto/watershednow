<?php
// $Id: context_reaction_block.inc,v 1.1.2.21 2010/05/25 18:56:21 yhahn Exp $

/**
 * Expose blocks as context reactions.
 */
class context_reaction_block extends context_reaction {
  /**
   * Options form.
   */
  function options_form($context) {
    $theme_key = variable_get('theme_default', 'garland');
    $weight_delta = $this->max_block_weight();

    $form = array(
      '#tree' => TRUE,
      '#theme' => 'context_block_form',
      'max_block_weight' => array(
        '#value' => $weight_delta,
        '#type' => 'value',
      ),
      'state' => array(
        '#type' => 'hidden',
        '#attributes' => array('class' => 'context-blockform-state'),
      ),
    );

    /**
     * Selector.
     */
    $modules = module_list();
    $form['selector'] = array(
      '#type' => 'item',
      '#tree' => TRUE,
      '#prefix' => '<div class="context-blockform-selector">',
      '#suffix' => '</div>',
    );
    foreach ($this->get_blocks() as $block) {
      if (!isset($form['selector'][$block->module])) {
        $form['selector'][$block->module] = array(
          '#type' => 'checkboxes',
          '#title' => $modules[$block->module],
          '#options' => array(),
        );
      }
      $form['selector'][$block->module]['#options'][$block->bid] = check_plain($block->info);
    }
    ksort($form['selector']);

    /**
     * Regions.
     */
    $form['blocks'] = array(
      '#tree' => TRUE,
      '#theme' => 'context_block_regions_form',
    );
    foreach (system_region_list($theme_key) as $region => $label) {
      $form['blocks'][$region] = array(
        '#type' => 'item',
        '#title' => $label,
        '#tree' => TRUE,
      );
      foreach ($this->get_blocks($region, $context) as $block) {
        if (!empty($block->context)) {
          $form['blocks'][$region][$block->bid] = array(
            '#value' => check_plain($block->info),
            '#weight' => $block->weight,
            '#type' => 'markup',
            '#tree' => TRUE,
            'weight' => array('#type' => 'weight', '#delta' => $weight_delta, '#default_value' => 0),
          );
        }
      }
    }
    return $form;
  }

  /**
   * Options form submit handler.
   */
  function options_form_submit($values) {
    $blocks = array();
    $block_info = $this->get_blocks();

    // Retrieve blocks from submitted JSON string.
    if (!empty($values['state'])) {
      $edited = $this->json_decode($values['state']);
    }
    else {
      $edited = array();
    }

    foreach ($edited as $region => $bids) {
      foreach ($bids as $position => $bid) {
        if (isset($block_info[$bid])) {
          $blocks[$bid] = array(
            'module' => $block_info[$bid]->module,
            'delta' => $block_info[$bid]->delta,
            'region' => $region,
            'weight' => $position,
          );
        }
      }
    }
    return array('blocks' => $blocks);
  }

  /**
   * Context editor form for blocks.
   */
  function editor_form($context) {
    $form = array();
    if (module_exists('jquery_ui')) {
      jquery_ui_add(array('ui.draggable', 'ui.droppable', 'ui.sortable'));
      drupal_add_js(drupal_get_path('module', 'context_ui') .'/json2.js');
      drupal_add_js(drupal_get_path('module', 'context') .'/plugins/context_reaction_block.js');
      drupal_add_css(drupal_get_path('module', 'context') .'/plugins/context_reaction_block.css');

      // We might be called multiple times so use a static to ensure this is set just once.
      static $once;
      if (!isset($once)) {
        $settings = array(
          'path' => $_GET['q'],
          'ajax' => url('context/ajax-block-view'),
          'scriptPlaceholder' => theme('context_block_script_placeholder', ''),
        );
        drupal_add_js(array('contextBlockEditor' => $settings), 'setting');
        $once = TRUE;
      }

      $form['state'] = array(
        '#type' => 'hidden',
        '#attributes' => array('class' => 'context-block-editor-state'),
      );
      $form['browser'] = array(
        '#type' => 'markup',
        '#value' => theme('context_block_browser', $this->get_blocks(), $context),
      );
    }
    return $form;
  }

  /**
  * Submit handler context editor form.
  */
  function editor_form_submit(&$context, $values) {
    $edited = !empty($values['state']) ? (array) $this->json_decode($values['state']) : array();

    $options = array();

    // Take the existing context values and remove blocks that belong affected regions.
    $affected_regions = array_keys($edited);
    if (!empty($context->reactions['block']['blocks'])) {
      $options = $context->reactions['block'];
      foreach ($options['blocks'] as $key => $block) {
        if (in_array($block['region'], $affected_regions)) {
          unset($options['blocks'][$key]);
        }
      }
    }

    // Iterate through blocks and add in the ones that belong to the context.
    foreach ($edited as $region => $blocks) {
      foreach ($blocks as $weight => $block) {
        if ($block->context === $context->name) {
          $split = explode('-', $block->bid);
          $options['blocks'][$block->bid] = array(
            'module' => array_shift($split),
            'delta' => implode('-', $split),
            'region' => $region,
            'weight' => $weight,
          );
        }
      }
    }

    return $options;
  }

  /**
   * Execute.
   */
  function execute($region) {
    return $this->render_editable($region);
  }

  /**
   * Render a region with editable markup.
   */
  protected function render_editable($region) {
    // Generally speaking, it does not make sense to allow anonymous users to
    // edit a context inline. Though it may be possible to save (and restore)
    // an edited context to an anonymous user's cookie or session, it's an
    // edge case and probably not something we want to encourage anyway.
    global $user;
    if (module_exists('jquery_ui') && $user->uid) {
      jquery_ui_add(array('ui.draggable', 'ui.droppable', 'ui.sortable'));
      drupal_add_js(drupal_get_path('module', 'context') .'/plugins/context_reaction_block.js');
      drupal_add_css(drupal_get_path('module', 'context') .'/plugins/context_reaction_block.css');
      $output = theme('context_block_editable_region', $region, $this->block_list($region));
      return $output . drupal_get_content($region);
    }
    return $this->render_region($region);
  }

  /**
   * Render a region.
   */
  protected function render_region($region) {
    $output = '';
    if ($list = $this->block_list($region)) {
      foreach ($list as $key => $block) {
        $output .= theme("block", $block);
      }
    }
    return $output . drupal_get_content($region);
  }

  /**
  * An alternative version of block_list() that provides any context enabled blocks.
  */
  protected function block_list($region) {
    // Static cache active context blocks when first called.
    static $context_blocks;
    if (!isset($context_blocks)) {
      $context_blocks = array();
      $info = $this->get_blocks();
      foreach ($this->get_contexts() as $context) {
        $options = $this->fetch_from_context($context);
        if (!empty($options['blocks'])) {
          foreach ($options['blocks'] as $block) {
            $block = (object) $block;
            $block->context = $context->name;
            $block->bid = "{$block->module}-{$block->delta}";
            $block->cache = isset($info[$block->bid]->cache) ? $info[$block->bid]->cache : BLOCK_NO_CACHE;
            $block = $this->build_block($block);
            if (!empty($block->content)) {
              $context_blocks[$block->region][] = $block;
            }
          }
        }
      }
    }

    // Get blocks provided by Drupal core block visibility & merge context blocks in.
    $blocks = block_list($region);
    if (isset($context_blocks[$region])) {
      $blocks = array_merge($blocks, $context_blocks[$region]);
      uasort($blocks, 'context_reaction_block::block_sort');
    }
    return $blocks;
  }

  /**
   * Build a block's content. Largely taken from block_list().
   */
  protected function build_block($block, $reset = FALSE) {
    // Block caching is not compatible with node_access modules. We also
    // preserve the submission of forms in blocks, by fetching from cache
    // only if the request method is 'GET'.
    static $cacheable;
    if (!isset($cacheable) || $reset) {
      $cacheable = !count(module_implements('node_grants')) && $_SERVER['REQUEST_METHOD'] == 'GET';
    }

    if (!isset($block->content)) {
      $block->content = '';
      // Try fetching the block from cache.
      if ($cacheable && ($cid = _block_get_cache_id($block))) {
        if ($cache = cache_get($cid, 'cache_block')) {
          $array = $cache->data;
        }
        else {
          $array = module_invoke($block->module, 'block', 'view', $block->delta);
          cache_set($cid, $array, 'cache_block', CACHE_TEMPORARY);
        }
      }
      // Otherwise build block.
      else {
        $array = module_invoke($block->module, 'block', 'view', $block->delta);
      }
      if (isset($array) && is_array($array)) {
        foreach ($array as $k => $v) {
          $block->$k = $v;
        }
      }
    }
    if (!empty($block->content)) {
      global $user, $theme_key;
      $block->title = db_result(db_query("SELECT title FROM {blocks} WHERE module = '%s' AND delta = '%s' AND theme = '%s'", $block->module, $block->delta, $theme_key));
      // Override default block title if a custom display title is present.
      if (!empty($block->title)) {
        // Check plain here to allow module generated titles to keep any markup.
        $block->subject = $block->title == '<none>' ? '' : check_plain($block->title);
      }
      if (!isset($block->subject)) {
        $block->subject = '';
      }
    }
    return $block;
  }

  /**
   * Generate the safe weight range for a block being added to a region such that
   * there are enough potential unique weights to support all blocks.
   */
  protected function max_block_weight() {
    $blocks = $this->get_blocks();
    $block_count = 0;
    foreach ($blocks as $region => $block_list) {
      $block_count += count($block_list);
    }
    // Add 2 to make sure there's space at either end of the block list
    return round(($block_count + 2) / 2);
  }

  /**
   * Helper function to generate a list of blocks from a specified region. If provided a context object,
   * will generate a full list of blocks for that region distinguishing between system blocks and
   * context-provided blocks.
   *
   * @param $region
   *   The string identifier for a theme region. e.g. "left"
   * @param $context
   *   A context object.
   *
   * @return
   *   A keyed (by "module_delta" convention) array of blocks.
   */
  function get_blocks($region = NULL, $context = NULL, $reset = FALSE) {
    static $block_info;

    if (!isset($block_info) || $reset) {
      $block_info = array();

      // Force a reset of the block info cache when on admin pages as a
      // variety of administrative UI tasks can create or alter block info.
      if (arg(0) === 'admin') {
        context_cache_set('block_info', NULL);
      }
      else if (!$reset) {
        $block_info = context_cache_get('block_info');
      }
      if (empty($block_info)) {
        $block_info = array();
        foreach (module_implements('block') as $module) {
          $module_blocks = module_invoke($module, 'block', 'list');
          if (!empty($module_blocks)) {
            foreach ($module_blocks as $delta => $block) {
              $block = (object) $block;
              $block->module = $module;
              $block->delta = $delta;
              $block->bid = "{$block->module}-{$block->delta}";
              $block_info[$block->bid] = $block;
            }
          }
        }
        context_cache_set('block_info', $block_info);
      }
      // Allow other modules that may declare blocks dynamically to alter
      // this list.
      drupal_alter('context_block_info', $block_info);

      // Gather only region info from the database.
      $theme_key = variable_get('theme_default', 'garland');
      $result = db_query("SELECT module,weight,delta,region,status FROM {blocks} WHERE theme = '%s' ORDER BY weight ASC", $theme_key);
      while ($row = db_fetch_object($result)) {
        if ($row->status && isset($block_info["{$row->module}-{$row->delta}"])) {
          $block_info["{$row->module}-{$row->delta}"]->weight = $row->weight;
          $block_info["{$row->module}-{$row->delta}"]->region = $row->region;
        }
      }
    }

    $blocks = array();

    // No region specified, provide all blocks.
    if (!isset($region)) {
      $blocks = $block_info;
    }
    // Region specified.
    else {
      foreach ($block_info as $bid => $block) {
        if (isset($block->region) && $block->region == $region) {
          $blocks[$bid] = $block;
        }
      }
    }

    // Add context blocks if provided.
    if (is_object($context) && $options = $this->fetch_from_context($context)) {
      if (!empty($options['blocks'])) {
        foreach ($options['blocks'] as $block) {
          if (
            isset($block_info["{$block['module']}-{$block['delta']}"]) && // Block is valid.
            (!isset($region) || (!empty($region) && $block['region'] == $region)) // No region specified or regions match.
          ) {
            $context_block = $block_info["{$block['module']}-{$block['delta']}"];
            $context_block->weight = $block['weight'];
            $context_block->region = $block['region'];
            $context_block->context = $context->name;
            $blocks[$context_block->bid] = $context_block;
          }
        }
      }
      uasort($blocks, "context_reaction_block::block_sort");
    }
    return $blocks;
  }

  /**
   * Sort callback.
   */
  static function block_sort($a, $b) {
    return ($a->weight - $b->weight);
  }

  /**
   * Compatibility wrapper around json_decode().
   */
  static function json_decode($json, $assoc = FALSE) {
    // Requires PHP 5.2.
    if (function_exists('json_decode')) {
      return json_decode($json, $assoc);
    }
    return context_reaction_block::_json_decode($json);
  }

  /**
   * From http://www.php.net/manual/en/function.json-decode.php#91216
   * with modifications for consistency with output of json_decode().
   *
   * Original author: walidator.info 2009.
   */
  static function _json_decode($json) {
    $comment = false;
    $out = '$x = ';
    for ($i=0; $i < strlen($json); $i++) {
      if (!$comment) {
        switch ($json[$i]) {
          case '{':
            $out .= ' (object) array(';
            break;
          case '}':
            $out .= ')';
            break;
          case '[':
            $out .= ' array(';
            break;
          case ']':
            $out .= ')';
            break;
          case ':';
            $out .= '=>';
            break;
          default:
            $out .= $json[$i];
            break;
        }
      }
      else {
        $out .= $json[$i];
      }
      if ($json[$i] == '"') {
        $comment = !$comment;
      }
    }
    eval($out . ';');
    return $x;
  }

  /**
   * Menu callback for AJAX requests. See context_ui_menu() to see how this is called.
   */
  function ajax($args) {
    if (isset($_GET['bid'], $_GET['context'])) {
      // Set the header right away. This will inform any players in the stack
      // that we are in the middle of responding to an AJAX request.
      drupal_set_header('Content-Type: text/javascript; charset=utf-8');

      $bid = $_GET['bid'];
      $context = $_GET['context'];
      list($module, $delta) = explode('-', $bid, 2);

      // Spoof the $_GET request and go so far as executing the page handler in
      // case the block relies on some contextual information from the page build.
      if (isset($_GET['path'])) {
        $_GET['q'] = $_GET['path'];
        menu_execute_active_handler($_GET['q']);
      }

      // Ensure $bid is valid.
      $info = $this->get_blocks();
      if (isset($info[$bid])) {
        $block = $info[$bid];
        $block->context = $context;
        $block->region = '';

        $block_view = module_invoke($module, 'block', 'view', $delta);
        if (!empty($block_view)) {
          foreach ($block_view as $k => $v) {
            $block->{$k} = $v;
          }
        }
        if (empty($block->content)) {
          $block->content = "<div class='context-block-empty'>". t('This block appears empty when displayed on this page.') ."</div>";
        }
        $block = $this->build_block($block);

        $css = array();

        // On behalf of panels we try to load some css, but we don't support
        // panels inside panels currently.
        if ($block->module == 'panels_mini') {
          $panel = panels_mini_load($block->delta);
          $layout = panels_get_layout($panel->display->layout);
          if (!empty($layout['css'])) {
            if (file_exists(path_to_theme() . '/' . $layout['css'])) {
              $css[] = path_to_theme() . '/' . $layout['css'];
            }
            else {
              $css[] = $layout['path'] . '/' . $layout['css'];
            }
          }
        }

        echo drupal_to_js(array(
          'status' => 1,
          'block' => theme('context_block_editable_block', $block),
          'css' => $css,
        ));
        exit;
      }
    }
    echo drupal_to_js(array('status' => 0));
    exit;
  }
}
