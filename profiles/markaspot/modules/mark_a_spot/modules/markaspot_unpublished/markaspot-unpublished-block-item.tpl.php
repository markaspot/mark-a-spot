<?php
// $Id$

/**
 * @file
 * HTML for an item in the single blog's block listing.
 *
 * Available variables:
 * - $classes: String of classes that can be used to style contextually through
 *   CSS. It can be manipulated through the variable $classes_array from
 *   preprocess functions. The default values can be one or more of the
 *   following:
 *   - single-blog-block-item: The current template type, i.e., "theming hook".
 * - $date: Formatted creation date. Preprocess functions can reformat it by
 *   calling format_date() with the desired parameters on the $created variable.
 * - $title: A renderable array that that provides a title and link to the node.
 * - $name: Themed username of node author output from theme_username().
 *
 * - $classes_array: Array of html class attribute values. It is flattened
 *   into a string within the variable $classes.
 *
 * Other variables:
 * The following variables are provided for contextual information.
 * - $node: Partial node object. Contains data that may not be safe.
 * - $created: Time the node was published formatted in Unix timestamp.
 * - $user: The user object of the node author.
 *
 * @see template_preprocess_markaspot_unpublished_block_item()
 */
?>
<div class="<?php print $classes; ?>">
   <span<?php print $title_attributes; ?>><?php print render($title); ?></span>
 <span class="meta-info date"><?php echo t('by !username', array('!username' => $name));echo " | ".t('Ticket ID')." "."#".$node->nid; echo'</span>&nbsp;<span>' . t('@time ago', array('@time' => format_interval(REQUEST_TIME - $created))) . '</span>'
  ?>
 </div>
