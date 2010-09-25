$Id: README.txt,v 1.2.2.1 2010/09/23 14:23:22 antoniodemarco Exp $

## Description

This module extends OpenLayers CCK  input widget allowing to mark a location on
the map by simply providing its address. A future version will also include
support for reverse geocoding.

The integration with Token module allows to auto-fill other text CCK fields on
the node submission page with values coming from the geocoding response
(like city, country, postal code, administrative areas, etc...). Text and
select widgets are supported.

OpenLayers Geocoder 2.x only works with Openlayers 2.x.

## Usage
After enabling the module select "OpenLayers Geocoder" widget for your
OpenLayers CCK fields.