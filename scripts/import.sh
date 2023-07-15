#!/bin/sh

# Set the working directory to the script's parent directory
cd "$(dirname "$0")/.."

# Install and enable required modules
composer require drupal/migrate_tools drupal/migrate_plus drupal/migrate_source_csv --update-no-dev

drush en markaspot_default_content -y
drush en migrate_tools migrate_plus migrate_source_csv -y

# Enable custom module


printf "\e[36mCustom module and migration created successfully...\e[0m\n"

# Define migration IDs
MIGRATIONS="
  markaspot_migrate_default_content_page
  markaspot_migrate_default_content_boilerplate
  markaspot_migrate_default_content_service_status
  markaspot_migrate_default_content_service_category
  markaspot_migrate_default_content_service_provider
  markaspot_migrate_default_content_block
"

# Loop over migration IDs
for MIGRATION_ID in $MIGRATIONS; do
  # Import data
  drush migrate-import "$MIGRATION_ID"
done

printf "\e[36mImporting the config for blocks...\e[0m\n"

# Set the source path based on the document root variable
source_path="$PWD/web/profiles/contrib/markaspot/modules/markaspot_default_content/config/_optional/"

# Run the drush command with the dynamic source path
drush cim --source "$source_path" --partial -y


# Disable and uninstall the modules
drush pmu migrate_source_csv migrate_plus migrate_tools markaspot_default_content -y
composer remove drupal/migrate_tools drupal/migrate_plus drupal/migrate_source_csv
