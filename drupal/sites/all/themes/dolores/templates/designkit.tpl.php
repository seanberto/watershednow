<style type='text/css'>
/**
 * This template should be overridden by implementing themes to establish
 * the styles they would like to use with DesignKit settings. The following
 * template is provided as a simple example of how you can generate CSS
 * styles from DesignKit settings.
 *
 * .designkit-color { color: <?php print $foreground ?>; }
 * .designkit-bg { background-color: <?php print $background ?>; }
 */

body {
  background-color: <?php print $background ?>;
}

#header, #main, #footer {
  <?php
    $col1 = designkit_colorshift($background, '#000000', .02);
    $col2 = designkit_colorshift($background, '#000000', .04);
    $col3 = designkit_colorshift($background, '#000000', .06);
    $col4 = designkit_colorshift($background, '#000000', .08);
    $col5 = designkit_colorshift($background, '#000000', .1);
    $cols = $col1 . ' ' . $col2 . ' ' . $col3 . ' ' . $col4 . ' ' . $col5;
  ?>;
  border-color: <?php print $col5 ?>;
  border-width: 1px;
  -moz-border-bottom-colors: <?php print $cols ?>;
  -moz-border-top-colors: <?php print $cols ?>;
  -moz-border-left-colors: <?php print $cols ?>;
  -moz-border-right-colors: <?php print $cols ?>;
}
 
</style>