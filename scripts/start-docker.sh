#!/bin/sh
cd /app/data;
echo 'memory_limit = -1' >> /usr/local/etc/php/conf.d/docker-php-ram-limit.ini

# Check if Drupal is already installed
msg=$(drush uli 2>&1)
if [[ "$msg" =~ Error* ]]
then
  echo "No drupal installation found!"
  mkdir -p web/sites/default/files/tmp
  mkdir -p private
  # DELETE FROM `default`.`key_value` WHERE (CONVERT(`collection` USING utf8) LIKE '%token-8.x-1.5.de.po%' OR CONVERT(`name` USING utf8) LIKE '%token-8.x-1.5.de.po%' OR CONVERT(`value` USING utf8) LIKE '%token-8.x-1.5.de.po%')
  echo "Importing initial data"
  # gunzip -c scripts/db-start.sql.gz |drush sqlc
  #drush pathauto:aliases-delete canonical_entities:node
  #drush sql-query "TRUNCATE markaspot_request_id"
  drush sqlc < scripts/groups.sql
  drush sqlc < scripts/groups_field_data.sql
  drush sqlc < scripts/groups_field_revision.sql
  drush sql-query "DROP table authmap"
  drush user:password admin "admin"
  rm -rf db-start.sql.gz *.sql
  echo "Installation done";
  drush markaspot:install
  drush ia -y --choice=full
  echo "Initially imported all structured content"

else
  echo "Drupal installation found, let's update config"
  # Update
  # drush si -y --existing-config
  drush -y config-set system.performance css.preprocess 0
  drush -y config-set system.performance js.preprocess 0
  drush sset system.maintenance_mode TRUE
  drush deploy -y
  drush -y config-set system.performance css.preprocess 1
  drush -y config-set system.performance js.preprocess 1
  drush sset system.maintenance_mode FALSE
fi
