#!/bin/bash
set -e

# Determine project root (script is in /scripts, project is parent)
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"

# Change to project root for composer
cd "$PROJECT_ROOT"

# DRUSH_URI can be passed as environment variable for multisite support
# e.g., DRUSH_URI="--uri=aachen.ddev.site" ./import.sh

# Install and enable required modules (with retry for network issues)
MAX_RETRIES=3
RETRY_COUNT=0
LAST_ERROR=""

while [ $RETRY_COUNT -lt $MAX_RETRIES ]; do
    # Capture stderr to show actual errors
    if LAST_ERROR=$(composer require drupal/migrate_tools drupal/migrate_plus drupal/migrate_source_csv --update-no-dev 2>&1); then
        break
    fi
    RETRY_COUNT=$((RETRY_COUNT + 1))
    if [ $RETRY_COUNT -lt $MAX_RETRIES ]; then
        echo "Composer failed ($RETRY_COUNT/$MAX_RETRIES), error: $LAST_ERROR"
        echo "Retrying in 2s..."
        sleep 2
    else
        echo "ERROR: composer require failed after $MAX_RETRIES attempts" >&2
        echo "Last error: $LAST_ERROR" >&2
        exit 1
    fi
done

drush $DRUSH_URI en markaspot_default_content -y
drush $DRUSH_URI en migrate_tools migrate_plus migrate_source_csv -y

# Enable custom module


printf "\e[36mCustom module and migration created successfully...\e[0m\n"

# Define migration IDs (groups first, then content)
MIGRATIONS="
  markaspot_migrate_default_content_group_jurisdiction
  markaspot_migrate_default_content_group_organisation
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
  drush $DRUSH_URI migrate-import "$MIGRATION_ID"
done

printf "\e[36mChecking for optional block configs...\e[0m\n"

# Set the source path based on the project root
source_path="$PROJECT_ROOT/web/profiles/contrib/markaspot/modules/markaspot_default_content/config/_optional/"

# Check if the directory exists before importing
if [ -d "$source_path" ]; then
  printf "\e[36mImporting the config for blocks...\e[0m\n"
  # Run the drush command with the dynamic source path
  drush $DRUSH_URI cim --source "$source_path" --partial -y
else
  printf "\e[33mOptional config directory not found at: $source_path\e[0m\n"
  printf "\e[33mSkipping optional config import...\e[0m\n"
fi


# Disable and uninstall the modules
drush $DRUSH_URI pmu migrate_source_csv migrate_plus migrate_tools markaspot_default_content -y

# Remove migrate modules (ensure we're in project root)
cd "$PROJECT_ROOT"
composer remove drupal/migrate_tools drupal/migrate_plus drupal/migrate_source_csv 2>&1 || echo "Warning: composer remove failed (may already be removed)"
