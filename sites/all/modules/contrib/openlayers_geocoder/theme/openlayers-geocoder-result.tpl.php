<?php
// $Id: openlayers-geocoder-result.tpl.php,v 1.1.2.4 2010/03/17 08:38:42 antoniodemarco Exp $
/**
 * @file: openlayers-geocoder-result.tpl.php
 *
 * Template file theming geocoder's response results.
 */
?>
<span class="openlayers-geocoder-result detail-row-1"><?php print $result['street_address']; ?></span>
<span class="openlayers-geocoder-result detail-row-2"><?php print $result['postal_code'] .' '. $result['locality']; ?><?php print $result['locality'] ? ' - ' : ''; ?><?php print $result['country']; ?></span>