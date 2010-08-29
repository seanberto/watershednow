<?php print '<?xml version="1.0" encoding="utf-8"?>' ?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml"
  xmlns:widget="http://www.netvibes.com/ns/">

  <head>
    <meta name="author" content="Netvibes" />
    <meta name="description" content="Drupal" />
    <meta name="apiVersion" content="1.0" />
    <meta name="debugMode" content="true" />
 
    <link rel="stylesheet" type="text/css"
      href="http://www.netvibes.com/themes/uwa/style.css" />

    <script type="text/javascript"
      src="http://www.netvibes.com/js/UWA/load.js.php?env=Standalone"></script>
    <title><?php print $title ?></title>
    <link rel="icon" type="image/png" href="http://www.netvibes.com/favicon.ico" />

    <script type="text/javascript">

    var WebWidgets = {};

    WebWidgets.dataProcessor = function (content) {
      widget.setBody(content);
    }

    widget.onLoad = function() {

      UWA.Data.request(
        '<?php print $url ?>',
        {
          method: 'get',
          proxy: 'ajax',
          type: 'text',
          cache: 3600,
          onComplete: WebWidgets.dataProcessor
        }
      );
    }
    </script>

  </head>
  <body>
    <p>Loading...</p>
  </body>
</html>
