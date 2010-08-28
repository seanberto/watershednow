; $Id: README.txt,v 1.1.2.4.2.3 2009/08/10 03:53:24 markuspetrux Exp $

CONTENTS OF THIS FILE
=====================
- USING MULTIGROUPS
- FIELDS AND WIDGETS THAT WORK IN MULTIGROUPS
- VIEWS INTEGRATION
- TROUBLESHOOTING


USING MULTIGROUPS
=================

The Multigroup group treats all included fields like a single field, keeping
the related delta values of all included fields synchronized.

To use a Multigroup, create a new group, make it the 'Multigroup' type, set
the number of multiple values for all the fields in the Multigroup, and drag
into it the fields that should be included.

All fields in the Multigroup will automatically get the group setting for
multiple values. On the node form, the group is rearranged to keep the delta
values for each field in a single drag 'n drop group, by transposing the
normal array(group_name => field_name => delta => value) into
array(group_name => delta => field_name => value).

During validation and submission, the field values are restored to their
normal positions.


FIELDS AND WIDGETS THAT WORK IN MULTIGROUPS
===========================================

All fields that allow the Content module to handle their multiple values should
work here. Fields that handle their own multiple values will not be allowed
into Multigroups unless they implement hook_content_multigroup_allowed_widgets()
to add their widgets to the allowed widget list. Example:

  @code
  function MODULE_content_multigroup_allowed_widgets() {
    return array('WIDGET_NAME_1', 'WIDGET_NAME_2', ...);
  }
  @endcode

All fields that allow the Content module to handle their multiple values
should work correctly when a field placed on a Multigroup is moved off, to a
normal field group, or to the top level of the form. Fields that handle their
own multiple values which may store different results in Multigroup and
standard groups should implement hook_content_multigroup_no_remove_widgets()
to add their widgets to the list of widgets that cannot be removed from
Multigroups. Example:

  @code
  function MODULE_content_multigroup_no_remove_widgets() {
    return array('WIDGET_NAME_1', 'WIDGET_NAME_2', ...);
  }
  @endcode

The Content Taxonomy module [1] is an example where it implements the previous
hooks for a few widgets.

[1] http://drupal.org/project/content_taxonomy

If a simple array of widgets is not sufficient to test whether this action
will work, modules can implement hook_content_multigroup_allowed_in()
and hook_content_multigroup_allowed_out() to intervene. Both hooks should
return an array as in the following example:

  @code
  function MODULE_content_multigroup_allowed_in() {
    return array(
      'allowed' => FALSE,
      'message' => t('This change is not allowed. Reason here...'),
    );
  }
  @endcode

Custom code and modules that add fields to groups outside of the UI should
use content_multigroup_allowed_in() and content_multigroup_allowed_out() to
test whether fields are allowed in or out of a Multigroup. These functions
can be located in content_multigroup.admin.inc.


VIEWS INTEGRATION
=================

For each multigroup, there is a new filter under "Content multigroup" category
in Views that provides a method to synchronize fields by delta.


TROUBLESHOOTING
===============

The most likely cause of problems with field modules not working in multigroup
is if they wipe out #element_validate with their own validation functions, or
they hard-code assumptions into submit or validation processes that the form
is structured in the usual field => delta => value order instead of allowing
for the possibility of a different structure. See Nodereference for an example
of a field that handles validation without making assumptions about the form
structure.
