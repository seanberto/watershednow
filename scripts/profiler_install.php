<?php

/**
 *
 * Just change different settings and run
 * php profiler-install.php
 *
 * Script will produce html for all urls visited. To collect the result use:
 * php profiler-install.php > drul.html
 *  
 * Based upon the work of dennis iversen: http://www.os-cms.net/content/article/view/1
 */

// your site
$site_url = 'http://watershednow.com';

// profile
$profile = 'watershednow';

// locale
$locale = 'en';

// end of settings

// we need to do the same address again because cookie
// is only set second time, then we get cookie and move on.

$ch = curl_init();
curl_setopt($ch, CURLOPT_RETURNTRANSFER,1);
curl_setopt($ch, CURLOPT_COOKIEJAR, "./cookieFileName");
curl_setopt($ch, CURLOPT_URL,"$site_url/install.php?locale=$locale&profile=$profile");
curl_setopt($ch, CURLOPT_POST, 1);
$html = curl_exec ($ch); // execute the curl command
curl_close ($ch);
unset($ch);
echo $html;

// install modules part one 80 %
$ch = curl_init();
curl_setopt($ch, CURLOPT_RETURNTRANSFER,1);
curl_setopt($ch, CURLOPT_COOKIEFILE, "./cookieFileName");
curl_setopt($ch, CURLOPT_URL,"$site_url/install.php?profile=$profile&locale=$locale&op=do_nojs&id=1");
$html = curl_exec ($ch); // execute the curl command
curl_close ($ch);
unset($ch);
echo $html;

// install last 20 %
$ch = curl_init();
curl_setopt($ch, CURLOPT_RETURNTRANSFER,1);
curl_setopt($ch, CURLOPT_COOKIEFILE, "./cookieFileName");
curl_setopt($ch, CURLOPT_URL,"$site_url/install.php?profile=$profile&locale=$locale&op=do_nojs&id=2");
$html = curl_exec ($ch); // execute the curl command
curl_close ($ch);
unset($ch);
echo $html;
   
// enter these settings in a last curl session
$ch = curl_init();
curl_setopt($ch, CURLOPT_RETURNTRANSFER,1);
curl_setopt($ch, CURLOPT_COOKIEFILE, "./cookieFileName");
curl_setopt($ch, CURLOPT_URL,"$site_url/install.php?profile=$profile&locale=$locale");
$html = curl_exec ($ch);
curl_close ($ch);
echo $html;

?>