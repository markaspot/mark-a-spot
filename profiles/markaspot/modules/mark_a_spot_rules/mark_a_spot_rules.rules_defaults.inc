<?php
/**
 * @file
 * mark_a_spot_rules.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function mark_a_spot_rules_default_rules_configuration() {
  $items = array();
  $items['rules_new_report_notification'] = entity_import('rules_config', '{ "rules_new_report_notification" : {
      "LABEL" : "New Report - Notification",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "REQUIRES" : [ "rules" ],
      "ON" : { "node_insert--report" : { "bundle" : "report" } },
      "DO" : [
        { "mail" : {
            "to" : "[site:mail]\\t",
            "subject" : "New Report for category [node:field-category]",
            "message" : "Hello,\\r\\n\\r\\na new report with the ID #[node:uuid] has been added for [site:name].\\r\\n\\r\\nCategory:\\r\\n[node:field-category]\\r\\nDescription: \\r\\n[node:body]\\r\\n\\r\\nYou may view [1] or edit [2] this report.\\r\\nMark-a-Spot. Your friendly Open311 reporting platform. \\r\\n\\r\\n[1]  [node:url]\\r\\n[2]  [node:edit-url]",
            "from" : "[site:mail]\\t",
            "language" : [ "" ]
          }
        }
      ]
    }
  }');
  $items['rules_report_added'] = entity_import('rules_config', '{ "rules_report_added" : {
      "LABEL" : "User confirm: Report added",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "REQUIRES" : [ "rules" ],
      "ON" : { "node_insert" : [] },
      "IF" : [
        { "node_is_of_type" : { "node" : [ "node" ], "type" : { "value" : { "report" : "report" } } } }
      ],
      "DO" : [
        { "mail" : {
            "to" : "[node:field-e-mail]",
            "subject" : "Thanks for reporting!",
            "message" : "Hello!\\r\\n\\r\\nThanks for reporting an issue at [site:name]. The report #ID [node:uuid] has been filed.\\r\\n\\r\\nPlease check for new status any time:\\r\\n[node:url]\\t\\r\\n\\r\\n",
            "from" : "[site:e-mail]",
            "language" : [ "" ]
          }
        }
      ]
    }
  }');
  return $items;
}
