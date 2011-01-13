$Id: README.txt,v 1.14.2.2 2010/12/28 17:48:12 ezrag Exp $

The signup module allows users to sign up for nodes of any type.  For
each signup-enabled node, this module provides options for sending a
notification email to a selected email address upon a new user signup
(good for notifying event coordinators, etc.), and a confirmation
email to users who sign up.  When used on event-enabled nodes
(http://drupal.org/project/event) or nodes that have a CCK date field
(http://drupal.org/project/date), it can also send out reminder emails
to all signups X days before the start of the event (per node
setting), and automatically close signups X hours before their start
(general setting).  There are settings for restricting signups to
selected roles and content types.

There is support for both registered and anonymous users to sign up.
Both can receive confirmation and reminder emails, and registered
users are optionally able to cancel or edit their own signups and view
listings of their current signups.

The Views module (http://druapl.org/project/views) is a dependency
which is used to create various listings, and there is extensive views
support for your own customizations.  Installing the View Bulk
Operations (VBO) module (http://druapl.org/project/views_bulk_operations)
is strongly encouraged to facilitate various administrative tasks.
There is also support to embed the signup form as a pane using panels
(http://druapl.org/project/panels).

Some add-on modules that enhance the basic functionality are available
in the "modules" subdirectory, and there is a listing of other add-on
modules on the Signup project page (http://druapl.org/project/signup).

For installation instructions, see INSTALL.txt.

For instructions on upgrading, see UPGRADE.txt.

For information about theming, see theme/README.txt.

Send feature requests and bug reports to the issue tracking system for
the signup module: http://drupal.org/node/add/project-issue/signup.
For a list of future work, also see http://groups.drupal.org/node/5044.

This module was originally co-developed by Chad Phillips and Jeff
Robbins, and sponsored by Jeff Robbins.  It is now being maintained
by: Derek Wright (http://drupal.org/user/46549) a.k.a. "dww".

