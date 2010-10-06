DESCRIPTION
********************************************************************************
Follow adds sitewide and per user links that link to various social networking 
sites.  The links reside in two blocks.  The Follow Site block lists all the 
links for the site itself, and by default is visible on all pages.  The Follow
User block lists all the follow links for the user and is visible only on user
profile pages.  There is a setting in the Follow Site block to have it not
display on user profile pages.

INSTALLATION
********************************************************************************
Enable the module at admin/build/modules, then go to user/admin/permissions to 
set which roles are allowed to edit the sitewide follow links, edit own follow
links, edit all user follow links, or administer follow.  Go to
admin/build/blocks to enable the blocks. User follow links can be edited at
user/%/follow, whereas sitewide follow links are at admin/build/follow

RECOMMENDED ADD ON
********************************************************************************
It is recommended to enable a module such as external [1] that will pop open
the follow links in a new tab or window; target="_blank" is not used since it
does not validate.

[1] http://drupal.org/project/external

SOCIAL NETWORKS
********************************************************************************
Here's a list of the currently supported social networks:
 - Facebook
 - Virb
 - MySpace
 - Twitter
 - Picasa
 - Flickr
 - YouTube
 - Vimeo
 - blip.tv
 - last.fm
 - LinkedIn
 - Delicious
 - Tumblr

ICONS
********************************************************************************
Although a few of the icons were hand made, most of them were created by either
Eli Burford [1] or Yichi [2].  Thanks, you guys!

[1] http://www.blogperfume.com/social-media-icons-pack-in-3-sizes-for-download/
[2] http://vikiworks.com/2007/06/15/social-bookmark-iconset/

