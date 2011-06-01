
/***********/
 Embedded Video Field
/***********/

Author: Aaron Winborn
Development Began 2007-02-23

Requires: Drupal 6, Content (CCK), emfield
Optional: Views

This extensible module will create a field for node content types that can be used to display video and thumbnails
from various third party video providers. When entering the content, the user will simply paste the URL or embed code
of the video, and the module will automatically determine which content provider is being used. When displaying
the video, the proper embedding format will be used.

The module already supports YouTube, Google, Revver, MySpace, MetaCafe, JumpCut, BrightCove, SevenLoad, iFilm and Blip.TV
video formats. More are planned to be supported soon. An api allows other third party video providers to be supported using
simple include files and provided hooks. (Developers: examine the documentation of /providers/youtube.inc for help in adding
support for new providers).

The administer of a site may decide whether to allow all content providers, or only a certain number of them. They may
further be limited when configuring the field.

On the Display Fields settings page, the administrator may further choose how to display the video, for teasers and body.
Videos may be displayed in a preview or full size, each of configurable sizes. When available by a provider, thumbnails may
also be displayed, and sized appropriately. Any necessary API calls to third party providers are cached.

Other features available are allowing a video to autoplay, or changing the size of the video. Those features will be set
when creating or editing the specific field. Note that not all options are supported by all providers. You can see a list
of what features are currently supported by a provider at admin/content/emfield.

Some providers may provide other features that are supported by Video CCK, such as affiliate programs with Revver, or related
video thumbnails with YouTube, embedded within the video. You can find those settings at admin/content/emfield, in the
fieldset for the specific provider.

Questions can be directed to winborn at advomatic dot com
