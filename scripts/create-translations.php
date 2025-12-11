#!/usr/bin/env drush
<?php

/**
 * @file
 * Create translations from CSV files using Drupal Entity API.
 *
 * This script creates translations for default content.
 * The base language is determined by the site's default language.
 * Translations are created from the CSV files in artifacts/<lang_code>/.
 *
 * Usage: drush php:script scripts/create-translations.php -- <language-code>
 * Example: drush php:script scripts/create-translations.php -- en
 *
 * Workflow for German site with English translations:
 * 1. Run ai-translate.sh de (translates EN CSVs to DE)
 * 2. Copy DE CSVs to main artifacts/ (replaces EN as base content)
 * 3. Run migration (creates DE base content)
 * 4. Run this script with "en" to add English translations from original CSVs
 *
 * In DDEV: ddev drush php:script scripts/create-translations.php -- en
 */

use Drupal\Core\Language\LanguageInterface;

// Drush 12/13 compatibility: drush_print() no longer exists
if (!function_exists('drush_print')) {
  function drush_print($message = '') {
    echo $message . PHP_EOL;
  }
}

// Get language code from arguments
$lang_code = $extra[0] ?? NULL;

if (empty($lang_code)) {
  drush_print("Usage: drush php:script scripts/create-translations.php -- <language-code>");
  drush_print("Example: drush php:script scripts/create-translations.php -- en");
  drush_print("");
  drush_print("This adds translations in the specified language to existing base content.");
  exit(1);
}

// Get site default language
$default_langcode = \Drupal::languageManager()->getDefaultLanguage()->getId();
drush_print("Site default language: $default_langcode");
drush_print("Creating translations for: $lang_code");

if ($lang_code === $default_langcode) {
  drush_print("Error: Cannot create translations in the site's default language.");
  drush_print("The target language must be different from '$default_langcode'.");
  exit(1);
}

drush_print("\n=== Creating $lang_code translations ===\n");

// Find artifacts directory
$artifacts_dir = DRUPAL_ROOT . '/profiles/contrib/markaspot/modules/markaspot_default_content/artifacts';

// For translations, we look in the language subdirectory OR the base directory
// If translating TO English, we use the original English CSVs (from .bak files if available)
// If translating TO another language, we use the translated CSVs in artifacts/<lang>/
$use_bak_files = FALSE;
if ($lang_code === 'en') {
  // English translations come from the original (backup) English files
  $source_dir = $artifacts_dir;
  // Check if .bak files exist (original EN content before AI translation)
  if (glob($artifacts_dir . '/*.bak')) {
    $use_bak_files = TRUE;
    drush_print("Using original English CSVs from .bak files in: $source_dir");
  } else {
    drush_print("Using English CSVs from: $source_dir");
  }
} else {
  $source_dir = $artifacts_dir . '/' . $lang_code;
  if (!is_dir($source_dir)) {
    drush_print("Error: Translated directory not found at $source_dir");
    drush_print("Please run 'ai-translate.sh $lang_code' first to generate translations.");
    exit(1);
  }
  drush_print("Using translated CSVs from: $source_dir");
}

drush_print("");

/**
 * Parse a CSV file and return rows as arrays.
 *
 * @param string $file_path
 *   Path to the CSV file.
 * @param bool $try_bak
 *   If TRUE and the file doesn't exist, try .bak extension.
 */
function parse_csv_file($file_path, $try_bak = FALSE) {
  // If file doesn't exist and we should try .bak, check for backup
  if (!file_exists($file_path) && $try_bak) {
    $bak_path = $file_path . '.bak';
    if (file_exists($bak_path)) {
      $file_path = $bak_path;
      drush_print("  Using backup file: " . basename($bak_path));
    }
  }

  if (!file_exists($file_path)) {
    return [];
  }

  $rows = [];
  if (($handle = fopen($file_path, 'r')) !== FALSE) {
    $header = fgetcsv($handle);
    while (($data = fgetcsv($handle)) !== FALSE) {
      if (!empty($data[0])) {
        $row = [];
        foreach ($header as $index => $column) {
          $row[$column] = $data[$index] ?? '';
        }
        $rows[] = $row;
      }
    }
    fclose($handle);
  }

  return $rows;
}

/**
 * Create page node translations.
 */
function create_page_translations($source_dir, $lang_code) {
  $csv_file = $source_dir . '/page.csv';

  // Try backup file if main doesn't exist (for EN from originals)
  if (!file_exists($csv_file) && file_exists($source_dir . '/page.csv.bak')) {
    $csv_file = $source_dir . '/page.csv.bak';
  }

  $rows = parse_csv_file($csv_file);

  if (empty($rows)) {
    drush_print("Skipping page translations: No data found");
    return;
  }

  drush_print("Processing page translations...");
  $storage = \Drupal::entityTypeManager()->getStorage('node');

  // Load all page nodes
  $all_pages = $storage->loadByProperties(['type' => 'page']);

  foreach ($rows as $row) {
    $title = trim($row['title'] ?? '');
    $body = $row['body'] ?? '';

    if (empty($title)) {
      continue;
    }

    // Find matching node - try by position/order since titles might differ
    $node = NULL;
    foreach ($all_pages as $page) {
      // Match by checking if this is the base language version
      if ($page->get('default_langcode')->value) {
        // For now, match by title pattern (first word or similar structure)
        $page_title = $page->label();
        // Simple matching - could be improved
        if (similar_text(strtolower($title), strtolower($page_title)) > 3) {
          $node = $page;
          break;
        }
      }
    }

    // If no fuzzy match, just process in order
    if (!$node) {
      static $page_index = 0;
      $pages_array = array_values($all_pages);
      if (isset($pages_array[$page_index])) {
        $node = $pages_array[$page_index];
        $page_index++;
      }
    }

    if ($node) {
      try {
        if (!$node->hasTranslation($lang_code)) {
          $translation = $node->addTranslation($lang_code, [
            'title' => $title,
            'body' => [
              'value' => $body,
              'format' => 'full_html',
            ],
          ]);
          $translation->save();
          drush_print("  Created $lang_code translation: $title");
        }
        else {
          $translation = $node->getTranslation($lang_code);
          $translation->set('title', $title);
          $translation->set('body', [
            'value' => $body,
            'format' => 'full_html',
          ]);
          $translation->save();
          drush_print("  Updated $lang_code translation: $title");
        }
      }
      catch (\Exception $e) {
        drush_print("  Error processing '$title': " . $e->getMessage());
      }
    }
    else {
      drush_print("  No matching node found for: $title");
    }
  }
}

/**
 * Create block content translations.
 */
function create_block_translations($source_dir, $lang_code) {
  $csv_file = $source_dir . '/block.csv';

  if (!file_exists($csv_file) && file_exists($source_dir . '/block.csv.bak')) {
    $csv_file = $source_dir . '/block.csv.bak';
  }

  $rows = parse_csv_file($csv_file);

  if (empty($rows)) {
    drush_print("Skipping block translations: No data found");
    return;
  }

  drush_print("\nProcessing block translations...");
  $storage = \Drupal::entityTypeManager()->getStorage('block_content');

  foreach ($rows as $row) {
    $title = trim($row['title'] ?? '');
    $uuid = trim($row['uuid'] ?? '');
    $body = $row['body'] ?? '';

    if (empty($uuid)) {
      continue;
    }

    $blocks = $storage->loadByProperties(['uuid' => $uuid]);

    if ($block = reset($blocks)) {
      try {
        if (!$block->hasTranslation($lang_code)) {
          $translation = $block->addTranslation($lang_code, [
            'info' => $title,
            'body' => [
              'value' => $body,
              'format' => 'full_html',
            ],
          ]);
          $translation->save();
          drush_print("  Created $lang_code translation: $title");
        }
        else {
          $translation = $block->getTranslation($lang_code);
          $translation->set('info', $title);
          $translation->set('body', [
            'value' => $body,
            'format' => 'full_html',
          ]);
          $translation->save();
          drush_print("  Updated $lang_code translation: $title");
        }
      }
      catch (\Exception $e) {
        drush_print("  Error processing block '$title': " . $e->getMessage());
      }
    }
    else {
      drush_print("  Block not found with UUID: $uuid");
    }
  }
}

/**
 * Create taxonomy term translations.
 */
function create_taxonomy_translations($source_dir, $filename, $vocab_name, $lang_code) {
  $csv_file = $source_dir . '/' . $filename;

  if (!file_exists($csv_file) && file_exists($source_dir . '/' . $filename . '.bak')) {
    $csv_file = $source_dir . '/' . $filename . '.bak';
  }

  $rows = parse_csv_file($csv_file);

  if (empty($rows)) {
    drush_print("Skipping $vocab_name translations: No data found");
    return;
  }

  drush_print("\nProcessing $vocab_name translations...");
  $storage = \Drupal::entityTypeManager()->getStorage('taxonomy_term');

  foreach ($rows as $row) {
    $uuid = trim($row['uuid'] ?? '');
    $name = trim($row['name'] ?? '');
    $description = $row['description__value'] ?? '';

    if (empty($uuid) || empty($name)) {
      continue;
    }

    $terms = $storage->loadByProperties(['uuid' => $uuid]);

    if ($term = reset($terms)) {
      try {
        if (!$term->hasTranslation($lang_code)) {
          $translation_data = ['name' => $name];
          if (!empty($description) && $description !== 'null') {
            $translation_data['description'] = [
              'value' => $description,
              'format' => 'plain_text',
            ];
          }
          $translation = $term->addTranslation($lang_code, $translation_data);
          $translation->save();
          drush_print("  Created $lang_code translation: $name");
        }
        else {
          $translation = $term->getTranslation($lang_code);
          $translation->set('name', $name);
          if (!empty($description) && $description !== 'null') {
            $translation->set('description', [
              'value' => $description,
              'format' => 'plain_text',
            ]);
          }
          $translation->save();
          drush_print("  Updated $lang_code translation: $name");
        }
      }
      catch (\Exception $e) {
        drush_print("  Error processing term '$name': " . $e->getMessage());
      }
    }
    else {
      drush_print("  Term not found with UUID: $uuid");
    }
  }
}

/**
 * Create boilerplate node translations.
 */
function create_boilerplate_translations($source_dir, $lang_code) {
  $csv_file = $source_dir . '/boilerplate.csv';

  // Try backup file if main doesn't exist (for EN from originals)
  if (!file_exists($csv_file) && file_exists($source_dir . '/boilerplate.csv.bak')) {
    $csv_file = $source_dir . '/boilerplate.csv.bak';
  }

  $rows = parse_csv_file($csv_file);

  if (empty($rows)) {
    drush_print("Skipping boilerplate translations: No data found");
    return;
  }

  drush_print("\nProcessing boilerplate translations...");
  $storage = \Drupal::entityTypeManager()->getStorage('node');

  // Load all boilerplate nodes
  $all_boilerplates = $storage->loadByProperties(['type' => 'boilerplate']);
  $boilerplates_array = array_values($all_boilerplates);

  foreach ($rows as $index => $row) {
    $title = trim($row['title'] ?? '');
    $body = $row['body'] ?? '';

    if (empty($title)) {
      continue;
    }

    // Match by index position since titles differ between languages
    $node = $boilerplates_array[$index] ?? NULL;

    if ($node) {
      try {
        if (!$node->hasTranslation($lang_code)) {
          $translation = $node->addTranslation($lang_code, [
            'title' => $title,
            'body' => [
              'value' => $body,
              'format' => 'full_html',
            ],
          ]);
          $translation->save();
          drush_print("  Created $lang_code translation: $title");
        }
        else {
          $translation = $node->getTranslation($lang_code);
          $translation->set('title', $title);
          $translation->set('body', [
            'value' => $body,
            'format' => 'full_html',
          ]);
          $translation->save();
          drush_print("  Updated $lang_code translation: $title");
        }
      }
      catch (\Exception $e) {
        drush_print("  Error processing boilerplate '$title': " . $e->getMessage());
      }
    }
    else {
      drush_print("  No matching boilerplate found for index $index: $title");
    }
  }
}

// Execute translations
create_page_translations($source_dir, $lang_code);
create_boilerplate_translations($source_dir, $lang_code);
create_block_translations($source_dir, $lang_code);
create_taxonomy_translations($source_dir, 'taxonomy_service_categories.csv', 'service_category', $lang_code);
create_taxonomy_translations($source_dir, 'taxonomy_service_status.csv', 'service_status', $lang_code);
create_taxonomy_translations($source_dir, 'taxonomy_service_provider.csv', 'service_provider', $lang_code);

// Clear caches
drush_print("\nClearing caches...");
drupal_flush_all_caches();

drush_print("\n=== Translation creation complete ===\n");
drush_print("Verify at: /admin/content, /admin/structure/block/block-content, /admin/structure/taxonomy");
