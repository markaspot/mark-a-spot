#!/bin/sh

if [ -z "$1" ]
  then
    echo "No language supplied, defaulting to German (de)"
    language="de"
  else
    language="$1"
fi

printf "\e[36mImporting translation for $language...\e[0m\n"
drush language-add $language

# Download .po file
printf "\e[36mDownloading .po file...\e[0m\n"
wget "https://ftp.drupal.org/files/translations/10.x/drupal/drupal-$language.po" -P ../translations

# Import .po file
drush locale:import $language ../translations/drupal-$language.po

# Set the working directory to the script's parent directory
cd "$(dirname "$0")/.."


drush config:set language.types negotiation.language_interface.enabled.$language 1 -y
drush config:set language.types negotiation.language_interface.method_id language-browser -y
drush config:set system.site default_langcode $language -y


# Enable multilingual support
printf "\e[36mEnabling translatable Taxonmy Terms for multilingual Georeport Services...\e[0m\n"
drush en markaspot_language
drush cr
