<?php

/**
 *
 * This script is designed for commandline installation of the Watershed Now Drupal installation
 * The script leverages cURL, and sets through the installation process with javacript disabled.
 *
 * Script will produce html for all urls visited. To collect the result use:
 * php profiler-install.php > installation_steps.txt
 *  
 * Based upon the work of dennis iversen: http://www.os-cms.net/content/article/view/1
 */

// URL options
$site_url = 'http://wn.dev/';  // Site URL with trailing slash.
$profile = 'watershednow'; // Name of selected install profile
$locale = 'en'; // locale
$tmpdir = '/tmp/'; // Temp directory on destination server with trailing slash.

// URIs for installation steps
/* @TODO - Uh, get rid of this hacky mess. These URLs were determined based upon manually parsing apache logs. */
/* This is really fragile. Should probably hook into the installation profile itself. */
$uris = array();
$uris[] = 'install.php?locale=' . $locale . '&profile=' . $profile;
$uris[] = 'install.php?locale=' . $locale . '&profile=' . $profile . '&op=start&id=1';
$uris[] = 'install.php?locale=' . $locale . '&profile=' . $profile . '&id=1&op=do_nojs';
$uris[] = 'install.php?locale=' . $locale . '&profile=' . $profile . '&id=1&op=finished';
$uris[] = 'install.php?locale=' . $locale . '&profile=' . $profile;
$uris[] = 'install.php?locale=' . $locale . '&profile=' . $profile . '&op=start&id=2';
$uris[] = 'install.php?locale=' . $locale . '&profile=' . $profile . '&id=2&op=do_nojs';
$uris[] = 'install.php?locale=' . $locale . '&profile=' . $profile . '&id=2&op=do_nojs';
$uris[] = 'install.php?locale=' . $locale . '&profile=' . $profile . '&id=2&op=do_nojs';
$uris[] = 'install.php?locale=' . $locale . '&profile=' . $profile . '&id=2&op=do_nojs';
$uris[] = 'install.php?locale=' . $locale . '&profile=' . $profile . '&id=2&op=do_nojs';
$uris[] = 'install.php?locale=' . $locale . '&profile=' . $profile . '&id=2&op=finished';
$uris[] = 'install.php?locale=' . $locale . '&profile=' . $profile;

unset($_COOKIE['has_js']); // In case installing on a domain that's already set the javascript cookie. See: http://drupal.org/node/608826

// This the initial URI once to set up the Cookie Jar.
$ch = curl_init();
curl_setopt($ch, CURLOPT_RETURNTRANSFER,1);
curl_setopt($ch, CURLOPT_COOKIEJAR, $tmpdir . "cookieFileName");
curl_setopt($ch, CURLOPT_URL, $site_url . 'install.php?locale=' . $locale . '&profile=' . $profile);
$html = curl_exec ($ch);
curl_close ($ch);
echo $html;

// Then pass through all the installation steps listed above.
foreach ($uris as $uri) {
  $ch = curl_init();
  curl_setopt($ch, CURLOPT_RETURNTRANSFER,1);
  curl_setopt($ch, CURLOPT_COOKIEFILE, $tmpdir . "cookieFileName");
  curl_setopt($ch, CURLOPT_URL, $site_url . $uri);
  $html = curl_exec ($ch);
  curl_close ($ch);
  echo $html;
}

?>