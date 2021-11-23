#!/bin/sh

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"

usage() {
  echo "Usage: start.sh [-y] [-t]"
  echo
  echo "Options:"
  echo "    -y    Install automatically with predefined values (latitude: 40.73, longitude: -73.93, city: New York, locale: en_US)"
  echo "    -t    Import translation file from the /translations directory and enable translations for terms"
  exit 1
}

if [ "$1" = "-h" ] || [ "$1" = "--help" ]; then
  usage
fi

if [ "$ENVIRONMENT" != "prod" ]; then
  printf "\e[32mNo Prod deployment. Installing Drupal with the Mark-a-Spot Distribution...\e[0m\n"
  printf "\e[36mDropping all tables in the database...\e[0m\n"
  drush sql-drop -y
  printf "\e[36mExecuting the Markaspot:install command...\e[0m\n"

  translation=false
  automatic=false

  while getopts "yt" opt
  do
    case $opt in
      y) automatic=true ;;
      t) translation=true ;;
      *) echo "Invalid option: -$opt" >&2
         usage ;;
    esac
  done

  if [ "$automatic" = true ]; then
    latitude="40.73"
    longitude="-73.93"
    city="New York"
    locale="en_US"
  else
    echo "Please enter the latitude:"
    read latitude
    echo "Please enter the longitude:"
    read longitude
    echo "Please enter the city:"
    read city
    echo "Please enter the locale (format as 'language_country', e.g. 'en_US'):"
    read locale
  fi

  php -d memory_limit=-1 $(which drush) markaspot:install --lat="$latitude" --lng="$longitude" --city="$city" --locale="$locale" --skip-confirmation

  printf "\e[36mAdd Admin Role...\e[0m\n"
  drush user-add-role "administrator" --uid=1



  if [ "$translation" = true ]; then
    printf "\e[36mImporting language...\e[0m\n"
    language=$(echo "$locale" | cut -d '_' -f1)
    $SCRIPT_DIR/translate.sh $language
  else
    printf "\e[33mHint: For a multilingual site, use the -t option to import a Drupal translation file from the /translations directory and enable translations for terms.\e[0m\n"
  fi

  printf "\e[36mImporting ..\e[0m\n"
  $SCRIPT_DIR/import.sh

  printf "\e[36mExecuting georeport client to import initial service requests...\e[0m\n"
  $SCRIPT_DIR/georeport-client.sh

  printf "\e[36mOne-Time Login for User 1 ...\e[0m\n"
  printf "\e[36m...\e[0m\n"
  drush uli
  printf "\e[36m \e[0m\n"
  printf "\e[36mInstallation completed...\e[0m\n"
fi
