<?php

/**
 * @file
 * Contains Drush related functions.
 */

/**
 * Implements hook_drush_cache_clear().
 * 
 * This adds an option on drush 'cache-clear'.
 */
function less_drush_cache_clear(&$types) {
  $types['less'] = 'less_flush_caches';
}
