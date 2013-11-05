
-- SUMMARY --

Adds a formatter for text/list/taxonomy fields to render as CSS classes on
nodes/pages/fields.

The *Field formatter CSS class* module allows you to set *any
text/list/option/taxonomy field* to render as *CSS class on the node*. This
enables the node author to select predefined CSS styling *per node*.

For a full description of the module, visit the project page:
  http://drupal.org/project/field_formatter_css_class

To submit bug reports and feature suggestions, or to track changes:
  http://drupal.org/project/issues/field_formatter_css_class


-- REQUIREMENTS --

None.


-- INSTALLATION --

* Install as usual, see Installing contributed modules (Drupal 7) for further
  information:
    http://drupal.org/documentation/install/modules-themes/modules-7


-- CONFIGURATION --

* Add a text / list / option / taxonomy field to any content type (on manage
  fields admin page)

* Set the display to "CSS class" (on manage display admin page)

* Create CSS rules in your theme to match the list options or text.


-- OPTIONS --

Choose if you want the original field content to show. Default is to hide it.

Select the tag to target with the CSS class. Options include

* Field
* Fieldgroup
* Entity
* Field collection item
* Fieldable panels pane
* Node
* Block
* Region
* Page

If you have nested collections or references you can decide to skip some
levels. I.e. pull the CSS class to outer wrapping tags.


-- EXAMPLES --

* Create a boolean option for left/right/none content alignment.

* Create a taxonomy for background color (think featured node).

* Create a list option for content width / column count.

* ...


-- SUPPORTED FIELD TYPES --

This formatter can be applied to the following field types:

* Text fields
* List fields
* Boolean fields
* Taxonomy fields


-- SUPPORTED ENTITY TYPES --

This formatter works with the following entity types:

* nodes. Fields in a node can set a CSS class on the node entity.

* field_collection module. Fields in a collection can set a CSS class on
  every wrapping tag, e.g. the collection entity.

* fieldable_panels_panes. Fields in a pane can set a CSS class on e.g. the
  Fieldable panel pane.

* entityreference module. Fields in referenced entities can set CSS classes
  to every wrapping tag, e.g. the reference field.

Feel free to file an issue if you need support for other (general) kinds of
entities.

-- SIMILAR / RELATED MODULES --

See the field_formatter_class module if you need the *site administrators*
to add classes *for each display*.


-- CONTACT --

Current maintainers:
* Christian W. Zuckschwerdt (zany) - http://drupal.org/user/284183

This project has been sponsored by:
* Laboratorio
  Agency for Advertisment / Strategy / Design.
  Visit http://www.laboratorio-hh.de/ for more information.
