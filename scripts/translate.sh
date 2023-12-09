#!/bin/sh

# Default to German if no language is supplied
if [ -z "$1" ]; then
    echo "No language supplied, defaulting to German (de)"
    language="de"
else
    language="$1"
fi

# Determine the Drupal version
drupal_version=$(drush status drupal-version --format=string)
if [ -z "$drupal_version" ]; then
    echo "Error: Unable to determine Drupal version."
    exit 1
fi

printf "\e[36mImporting translation for $language...\e[0m\n"
drush language-add $language


# Check if the .po file was downloaded successfully
if [ -f "$translations_dir/drupal-$drupal_version.$language.po" ]; then

    # Set the working directory to the script's parent directory
    cd "$(dirname "$0")/.."

    # Set configuration for the language
    drush config:set language.types negotiation.language_interface.enabled.$language 1 -y
    drush config:set language.types negotiation.language_interface.method_id language-browser -y
    drush config:set language.administration negotiation.language_administration_language $language -y
    drush user:modify 1 --langcode=$language

    drush config:set system.site default_langcode $language -y

    # Enable multilingual support
    printf "\e[36mEnabling translatable Taxonomy Terms for multilingual Georeport Services...\e[0m\n"
    drush en markaspot_language
    drush cr

else
    echo "Error: .po file not found. Download failed or incorrect language code."
fi
