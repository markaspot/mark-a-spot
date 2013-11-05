## Bootstrap fieldgroup

This module adds a new set of formatters to the Field Group options when
configuring a view mode display, to render field groups with Bootstrap markup.

The formatters can be applied to the node create/edit form, as well as on
display.

### Usage

The most interesting formatter is 'Bootstrap nav', which handles most of the
heavy lifting of rendering the Bootstrap markup.

#### Single field per tab

Fields can be nested directly under the 'Bootstrap nav' group, and each field
will receive its own tab/pill.

e.g.

Main Container (Bootstrap nav)
 - Field 1
 - Field 2
 - Field 3
 - Field 4

#### Multiple fields per tab

If you need to organize more than one field under each tab/pill, you can nest
a 'Bootstrap nav item' group, and then organize your fields under these.

e.g.

Main Container (Bootstrap nav)
 - Group Container 1 (Bootstrap nav item)
   - Field 1
   - Field 2
 - Group Container 2 (Bootstrap nav item)
   - Field 3
   - Field 4

### Configuration

#### Nav type

Specify either 'Tabs' or 'Pills'

#### Stacked

Whether the tabs will be stacked vertically. The default is No; they will be
horizontal.

#### Orientation

The orientation of the nav in relation to the content. Specify either 'Top',
'Right', 'Down', 'Left'.

**NOTE:** For pills, you may only specify 'Top', or 'Bottom'. Bootstrap only
seems to support left/right orientation for tabs. 

