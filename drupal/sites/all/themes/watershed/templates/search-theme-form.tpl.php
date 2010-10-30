<?php
// $Id: search-theme-form.tpl.php,v 1.3 2010/09/17 21:36:06 eternalistic Exp $

/**
 * @file search-theme-form.tpl.php
 * Default theme implementation for displaying a search form directly into the
 * theme layout. Not to be confused with the search block or the search page.
 *
 * Available variables:
 * - $search_form: The complete search form ready for print.
 * - $search: Array of keyed search elements. Can be used to print each form
 *   element separately.
 *
 * Default keys within $search:
 * - $search['search_theme_form']: Text input area wrapped in a div.
 * - $search['submit']: Form submit button.
 * - $search['hidden']: Hidden form elements. Used to validate forms when submitted.
 *
 * Since $search is keyed, a direct print of the form element is possible.
 * Modules can add to the search form so it is recommended to check for their
 * existance before printing. The default keys will always exist.
 *
 *   <?php if (isset($search['extra_field'])): ?>
 *     <div class="extra-field">
 *       <?php print $search['extra_field']; ?>
 *     </div>
 *   <?php endif; ?>
 *
 * To check for all available data within $search, use the code below.
 *
 *   <?php print '<pre>'. check_plain(print_r($search, 1)) .'</pre>'; ?>
 *
 * @see template_preprocess_search_theme_form()
 */
/* Set search form label values and functions */
$search_label =  theme_get_setting('watershed_search_help',t('Search...'));
$search_phrase = t('Enter the terms you wish to search for.');
$on_blur = "if(this.value=='') this.value='" . $search_label . "';";
$on_focus = "if(this.value=='" . $search_label . "') this.value='';";
?>

<div id="search">
  <div id="search-input">
    <input type="text" maxlength="128" name="search_theme_form" id="edit-search-theme-form-header" size="15" title="<?php print $search_phrase; ?>" value="<?php print $search_label; ?>" onblur="<?php print $on_blur; ?>" onfocus="<?php print $on_focus; ?>" class="form-text" />
  </div>

  <div id="search-button">
    <input type="image" name="op" value="search" id="edit-submit-2" src="<?php print base_path() . $directory; ?>/images/search_icon.gif" alt="Search" />
  </div>

  <?php print $search['hidden'] ?>
</div>