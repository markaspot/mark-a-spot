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

printf "\e[32mInstall all libraries with composer..\e[0m\n"
composer install --no-dev


if [ "$ENVIRONMENT" != "prod" ]; then
  printf "\e[32mNo Prod deployment. Installing Drupal with the Mark-a-Spot Distribution...\e[0m\n"

  # Define the path to the Drupal settings file
  SETTINGS_FILE="/app/data/web/sites/default/settings.php"

  cp /app/data/web/sites/default/default.settings.php $SETTINGS_FILE

  # Custom database configuration
  CUSTOM_DB_CONFIG="\\
  \$databases['default']['default'] = [\\
      'database' => getenv('DRUPAL_DATABASE_NAME'),\\
      'username' => getenv('DRUPAL_DATABASE_USERNAME'),\\
      'password' => getenv('DRUPAL_DATABASE_PASSWORD'),\\
      'prefix' => '',\\
      'host' => getenv('MARKASPOT_MARIADB_SERVICE_HOST'),\\
      'port' => 3306,\\
      'namespace' => 'Drupal\\\\\\\\Core\\\\\\\\Database\\\\\\\\Driver\\\\\\\\mysql',\\
      'driver' => 'mysql',\\
  ];"

  # Add the custom database configuration after the $databases declaration
  sed -i "/\$databases = \[\];/a $CUSTOM_DB_CONFIG" "$SETTINGS_FILE"

  # Custom hash salt configuration
  CUSTOM_HASH_SALT="\$settings['hash_salt'] = getenv('DRUPAL_HASH_SALT');"

  # Replace the existing hash salt configuration with the custom one
  sed -i "s/\$settings\['hash_salt'\] = '';$/$CUSTOM_HASH_SALT/" "$SETTINGS_FILE"

  # Update the config_sync_directory setting
  sed -i "s|# \$settings\['config_sync_directory'\] = '/directory/outside/webroot';|\$settings['config_sync_directory'] = '../config/sync';|" "$SETTINGS_FILE"


  printf "\e[32mCustom configuration added to $SETTINGS_FILE"


  printf "\e[36mDropping all tables in the database...\e[0m\n"
  drush sql-drop -y
  printf "\e[36mExecuting the Markaspot:install command...\e[0m\n"


  # Function to query the Nominatim API for city information
  get_city_info() {
      city_name=$(printf "%s" "$1" | jq -sRr @uri)
      country_name=$(printf "%s" "$2" | jq -sRr @uri)

      # Exit the function if either city name or country name is empty
      if [ -z "$city_name" ] || [ -z "$country_name" ]; then
          return 1
      fi

      response=$(curl -s "https://nominatim.openstreetmap.org/search?city=$city_name&country=$country_name&format=json")

      count=$(echo $response | jq length)
      if [ "$count" -eq 0 ]; then
          echo "No results found for $1 in $2."
          return 1
      elif [ "$count" -eq 1 ]; then
          selected=$(echo $response | jq -r ".[0]")
      else
          echo "Multiple locations found. Please select one by entering the corresponding number:"
          for i in $(seq 0 $(($count - 1))); do
              echo "$(($i + 1)): $(echo $response | jq -r ".[$i].display_name")"
          done
          read -p "Choice: " choice
          selected=$(echo $response | jq -r ".[$(($choice - 1))]")
      fi

      latitude=$(echo $selected | jq -r '.lat')
      longitude=$(echo $selected | jq -r '.lon')
      city=$(echo $selected | jq -r '.display_name')

      echo "Selected Location: $city"
      echo "Latitude: $latitude"
      echo "Longitude: $longitude"
  }

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
      echo "Please enter the city name (or leave blank to enter latitude and longitude manually):"
      read city_name
      echo "Please enter the country name:"
      read country_name

      if ! get_city_info "$city_name" "$country_name"; then
          echo "City not found or not entered. Please enter the latitude:"
          read latitude
          echo "Please enter the longitude:"
          read longitude
          echo "Please enter the city:"
          read city
          echo "Please enter the locale (format as 'language_country', e.g. 'en_US'):"
          read locale
      fi
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
  drush uli --uri=http://localhost
  printf "\e[36m \e[0m\n"
  printf "\e[36mInstallation completed...\e[0m\n"
fi
