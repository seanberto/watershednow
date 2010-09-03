<?php
// $Id: search-theme-form.tpl.php,v 1.1 2009/10/01 03:34:49 ishmaelsanchez Exp $ 

/**
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
?>
<input type="text" name="search_theme_form" id="edit-search-theme-form-l" size="12" value="" title="Enter the terms you wish to search for." class="form-text" />
<input class="form-submit" type="submit" name="op" value="Go"  />
<input type="hidden" name="form_id" id="edit-search-theme-form" value="search_theme_form" />
<input type="hidden" name="form_token" id="a-unique-id" value="<?php print drupal_get_token('search_theme_form'); ?>" />
