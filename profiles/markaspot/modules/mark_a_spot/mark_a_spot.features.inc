<?php
/**
 * @file
 * mark_a_spot.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function mark_a_spot_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "field_group" && $api == "field_group") {
    return array("version" => "1");
  }
  if ($module == "oauth_common" && $api == "oauth") {
    return array("version" => "1");
  }
  if ($module == "page_manager" && $api == "pages_default") {
    return array("version" => "1");
  }
  if ($module == "services" && $api == "services") {
    return array("version" => "3");
  }
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function mark_a_spot_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_image_default_styles().
 */
function mark_a_spot_image_default_styles() {
  $styles = array();

  // Exported image style: mas_medium.
  $styles['mas_medium'] = array(
    'name' => 'mas_medium',
    'effects' => array(
      5 => array(
        'label' => 'Scale and crop',
        'help' => 'Scale and crop will maintain the aspect-ratio of the original image, then crop the larger dimension. This is most useful for creating perfectly square thumbnails without stretching the image.',
        'effect callback' => 'image_scale_and_crop_effect',
        'dimensions callback' => 'image_resize_dimensions',
        'form callback' => 'image_resize_form',
        'summary theme' => 'image_resize_summary',
        'module' => 'image',
        'name' => 'image_scale_and_crop',
        'data' => array(
          'width' => 720,
          'height' => 480,
        ),
        'weight' => 1,
      ),
    ),
    'label' => 'mas_medium',
  );

  // Exported image style: mas_thumb.
  $styles['mas_thumb'] = array(
    'name' => 'mas_thumb',
    'effects' => array(
      4 => array(
        'label' => 'Scale and crop',
        'help' => 'Scale and crop will maintain the aspect-ratio of the original image, then crop the larger dimension. This is most useful for creating perfectly square thumbnails without stretching the image.',
        'effect callback' => 'image_scale_and_crop_effect',
        'dimensions callback' => 'image_resize_dimensions',
        'form callback' => 'image_resize_form',
        'summary theme' => 'image_resize_summary',
        'module' => 'image',
        'name' => 'image_scale_and_crop',
        'data' => array(
          'width' => 100,
          'height' => 100,
        ),
        'weight' => 1,
      ),
    ),
    'label' => 'mas_thumb',
  );

  return $styles;
}

/**
 * Implements hook_node_info().
 */
function mark_a_spot_node_info() {
  $items = array(
    'report' => array(
      'name' => t('Report'),
      'base' => 'node_content',
      'description' => t('Mark-a-Spot reports'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => t('Describe the issue you found'),
    ),
    'statement' => array(
      'name' => t('Statement'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}

/**
 * Implements hook_default_search_api_index().
 */
function mark_a_spot_default_search_api_index() {
  $items = array();
  $items['default_report_index'] = entity_import('search_api_index', '{
    "name" : "Default report index",
    "machine_name" : "default_report_index",
    "description" : "Mark-a-Spot reports",
    "server" : "mysql",
    "item_type" : "node",
    "options" : {
      "index_directly" : 1,
      "cron_limit" : "50",
      "fields" : {
        "author" : { "type" : "integer", "entity_type" : "user" },
        "body:value" : { "type" : "text" },
        "changed" : { "type" : "date" },
        "created" : { "type" : "date" },
        "field_address" : { "type" : "text" },
        "field_category" : { "type" : "integer", "entity_type" : "taxonomy_term" },
        "field_category:field_category_hex:rgb" : { "type" : "string" },
        "field_category:field_category_icon:class" : { "type" : "string" },
        "field_category:name" : { "type" : "text" },
        "field_geo:lat" : { "type" : "decimal" },
        "field_geo:lng" : { "type" : "decimal" },
        "field_notify_user" : { "type" : "boolean" },
        "field_statement" : { "type" : "list\\u003Cinteger\\u003E", "entity_type" : "node" },
        "field_statement_comment" : { "type" : "text" },
        "field_status" : { "type" : "integer", "entity_type" : "taxonomy_term" },
        "field_status:field_status_hex:rgb" : { "type" : "string" },
        "field_status:field_status_icon:class" : { "type" : "string" },
        "field_status:name" : { "type" : "text" },
        "field_status:tid" : { "type" : "integer" },
        "nid" : { "type" : "integer" },
        "search_api_access_node" : { "type" : "list\\u003Cstring\\u003E" },
        "search_api_language" : { "type" : "string" },
        "status" : { "type" : "boolean" },
        "title" : { "type" : "text" },
        "uuid" : { "type" : "text" }
      },
      "data_alter_callbacks" : {
        "search_api_alter_bundle_filter" : {
          "status" : 1,
          "weight" : "-10",
          "settings" : { "default" : "0", "bundles" : { "report" : "report" } }
        },
        "search_api_alter_node_access" : { "status" : 1, "weight" : "0", "settings" : [] },
        "search_api_alter_node_status" : { "status" : 1, "weight" : "0", "settings" : [] },
        "search_api_alter_add_hierarchy" : { "status" : 0, "weight" : "0", "settings" : { "fields" : [] } },
        "search_api_alter_add_viewed_entity" : { "status" : 0, "weight" : "0", "settings" : { "mode" : "full" } },
        "search_api_alter_add_url" : { "status" : 0, "weight" : "0", "settings" : [] },
        "search_api_alter_add_aggregation" : { "status" : 0, "weight" : "0", "settings" : [] }
      },
      "processors" : {
        "search_api_case_ignore" : {
          "status" : 1,
          "weight" : "0",
          "settings" : { "fields" : {
              "title" : true,
              "uuid" : true,
              "field_address" : true,
              "body:value" : true
            }
          }
        },
        "search_api_html_filter" : {
          "status" : 1,
          "weight" : "10",
          "settings" : {
            "fields" : { "body:value" : true },
            "title" : 0,
            "alt" : 1,
            "tags" : "h1 = 5\\r\\nh2 = 3\\r\\nh3 = 2\\r\\nstrong = 2\\r\\nb = 2\\r\\nem = 1.5\\r\\nu = 1.5"
          }
        },
        "search_api_tokenizer" : {
          "status" : 1,
          "weight" : "20",
          "settings" : {
            "fields" : { "body:value" : true },
            "spaces" : "[^[:alnum:]]",
            "ignorable" : "[\\u0027]"
          }
        },
        "search_api_stopwords" : {
          "status" : 0,
          "weight" : "30",
          "settings" : {
            "fields" : { "body:value" : true },
            "file" : "",
            "stopwords" : "but\\r\\ndid\\r\\nthe this that those\\r\\netc"
          }
        },
        "search_api_highlighting" : {
          "status" : 0,
          "weight" : "35",
          "settings" : {
            "prefix" : "\\u003Cstrong\\u003E",
            "suffix" : "\\u003C\\/strong\\u003E",
            "excerpt" : 1,
            "excerpt_length" : "256",
            "exclude_fields" : [],
            "highlight" : "always"
          }
        }
      }
    },
    "enabled" : "1",
    "read_only" : "0"
  }');
  return $items;
}

/**
 * Implements hook_default_search_api_server().
 */
function mark_a_spot_default_search_api_server() {
  $items = array();
  $items['mysql'] = entity_import('search_api_server', '{
    "name" : "mysql",
    "machine_name" : "mysql",
    "description" : "",
    "class" : "search_api_db_service",
    "options" : {
      "database" : "default:default",
      "min_chars" : "1",
      "partial_matches" : 1,
      "indexes" : { "default_report_index" : {
          "nid" : {
            "table" : "search_api_db_default_report_index",
            "column" : "nid",
            "type" : "integer",
            "boost" : "1.0"
          },
          "status" : {
            "table" : "search_api_db_default_report_index",
            "column" : "status",
            "type" : "boolean",
            "boost" : "1.0"
          },
          "created" : {
            "table" : "search_api_db_default_report_index",
            "column" : "created",
            "type" : "date",
            "boost" : "1.0"
          },
          "changed" : {
            "table" : "search_api_db_default_report_index",
            "column" : "changed",
            "type" : "date",
            "boost" : "1.0"
          },
          "author" : {
            "table" : "search_api_db_default_report_index",
            "column" : "author",
            "type" : "integer",
            "boost" : "1.0"
          },
          "uuid" : {
            "table" : "search_api_db_default_report_index_text",
            "type" : "text",
            "boost" : "1.0",
            "column" : "uuid"
          },
          "field_address" : {
            "table" : "search_api_db_default_report_index_text",
            "type" : "text",
            "boost" : "1.0"
          },
          "field_category" : {
            "table" : "search_api_db_default_report_index",
            "column" : "field_category",
            "type" : "integer",
            "boost" : "1.0"
          },
          "field_notify_user" : {
            "table" : "search_api_db_default_report_index",
            "column" : "field_notify_user",
            "type" : "boolean",
            "boost" : "1.0"
          },
          "field_statement" : {
            "table" : "search_api_db_default_report_index_field_statement",
            "column" : "value",
            "type" : "list\\u003Cinteger\\u003E",
            "boost" : "1.0"
          },
          "field_status" : {
            "table" : "search_api_db_default_report_index",
            "column" : "field_status",
            "type" : "integer",
            "boost" : "1.0"
          },
          "search_api_language" : {
            "table" : "search_api_db_default_report_index",
            "column" : "search_api_language",
            "type" : "string",
            "boost" : "1.0"
          },
          "search_api_access_node" : {
            "table" : "search_api_db_default_report_index_search_api_access_node",
            "column" : "value",
            "type" : "list\\u003Cstring\\u003E",
            "boost" : "1.0"
          },
          "body:value" : {
            "table" : "search_api_db_default_report_index_text",
            "type" : "text",
            "boost" : "1.0"
          },
          "field_category:name" : {
            "table" : "search_api_db_default_report_index_text",
            "type" : "text",
            "boost" : "1.0"
          },
          "field_geo:lat" : {
            "table" : "search_api_db_default_report_index",
            "column" : "field_geo_lat",
            "type" : "decimal",
            "boost" : "1.0"
          },
          "field_geo:lng" : {
            "table" : "search_api_db_default_report_index",
            "column" : "field_geo_lng",
            "type" : "decimal",
            "boost" : "1.0"
          },
          "field_status:tid" : {
            "table" : "search_api_db_default_report_index",
            "column" : "field_status_tid",
            "type" : "integer",
            "boost" : "1.0"
          },
          "field_status:name" : {
            "table" : "search_api_db_default_report_index_text",
            "type" : "text",
            "boost" : "1.0"
          },
          "field_category:field_category_hex:rgb" : {
            "table" : "search_api_db_default_report_index",
            "column" : "field_category_field_category_hex_rgb",
            "type" : "string",
            "boost" : "1.0"
          },
          "field_status:field_status_hex:rgb" : {
            "table" : "search_api_db_default_report_index",
            "column" : "field_status_field_status_hex_rgb",
            "type" : "string",
            "boost" : "1.0"
          },
          "title" : {
            "table" : "search_api_db_default_report_index_text",
            "type" : "text",
            "boost" : "1.0",
            "column" : "title"
          },
          "field_statement_comment" : {
            "table" : "search_api_db_default_report_index_text",
            "type" : "text",
            "boost" : "1.0"
          },
          "field_status:field_status_icon:class" : {
            "table" : "search_api_db_default_report_index",
            "column" : "field_status_field_status_icon_class",
            "type" : "string",
            "boost" : "1.0"
          },
          "field_category:field_category_icon:class" : {
            "table" : "search_api_db_default_report_index",
            "column" : "field_category_field_category_icon_class",
            "type" : "string",
            "boost" : "1.0"
          }
        }
      }
    },
    "enabled" : "1"
  }');
  return $items;
}
