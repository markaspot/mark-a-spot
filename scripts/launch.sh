#!/bin/sh
vendor/bin/drush entity:delete node --bundle=service_request
vendor/bin/drush entity:delete paragraph
vendor/bin/drush entity:delete media
vendor/bin/drush pathauto:aliases-delete canonical_entities:node
vendor/bin/drush sql-query "TRUNCATE markaspot_request_id"
vendor/bin/drush sql-query < groups.sql
vendor/bin/drush sql-query < groups_field_data.sql
vendor/bin/drush sql-query "ALTER TABLE node AUTO_INCREMENT=100;"
