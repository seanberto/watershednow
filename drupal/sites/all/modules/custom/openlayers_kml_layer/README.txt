A Drupal module that will create an openlayer layer and associate it with one or more map presets when creating or updating configured nodes.

Installation:
* Place the module files into sites/all/modules or any other valid modules directory within your Drupal site. Ref, http://drupal.org/getting-started/install-contrib/modules
* Enable the module.

Configuration:
* Configure at least one content type with at least one filefield.
* Visit /admin/build/openlayers/layers/openlayers_kml_layer and select which filefields in each content type are KML files.

That's it. Then, when you create, edit, or delete configured nodes, OpenLayer's KML layers will be created, edited, or deleted, and added to the selected presets.