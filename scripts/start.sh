#!/bin/sh

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"

usage() {
  echo "Usage: start.sh [-y] [-t] [-a]"
  echo
  echo "Options:"
  echo "    -y    Install automatically with predefined values (latitude: 40.73, longitude: -73.93, city: New York, locale: en_US)"
  echo "    -t    Import translation file from the /translations directory and enable translations for terms"
  echo "    -a    Use AI translation (OpenAI) for content artifacts instead of standard translation files"
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
          echo "ERROR: Empty city or country name"
          return 1
      fi

      # Check if jq is installed
      if ! command -v jq &> /dev/null; then
          echo "ERROR: jq is required but not installed. Please install jq first."
          return 1
      fi

      response=$(curl -s "https://nominatim.openstreetmap.org/search?city=$city_name&country=$country_name&format=json")

      # Check if curl request was successful and response is valid JSON
      if [ $? -ne 0 ] || ! echo "$response" | jq empty 2>/dev/null; then
          echo "ERROR: Failed to query the Nominatim API or invalid response."
          return 1
      fi

      count=$(echo "$response" | jq length)
      if [ "$count" -eq 0 ]; then
          echo "ERROR: No results found for $1 in $2."
          return 1
      elif [ "$count" -eq 1 ]; then
          selected=$(echo "$response" | jq -r ".[0]")
      else
          echo "Multiple locations found. Please select one by entering the corresponding number:"
          for i in $(seq 0 $(($count - 1))); do
              echo "$(($i + 1)): $(echo "$response" | jq -r ".[$i].display_name")"
          done
          read -p "Choice: " choice
          if ! [[ "$choice" =~ ^[0-9]+$ ]] || [ "$choice" -lt 1 ] || [ "$choice" -gt "$count" ]; then
              echo "ERROR: Invalid selection."
              return 1
          fi
          selected=$(echo "$response" | jq -r ".[$(($choice - 1))]")
      fi

      # Set global variables directly
      latitude=$(echo "$selected" | jq -r '.lat')
      longitude=$(echo "$selected" | jq -r '.lon')
      city=$(echo "$selected" | jq -r '.display_name')

      echo "Selected Location: $city"
      echo "Latitude: $latitude"
      echo "Longitude: $longitude"
      
      # Return success
      return 0
  }

  translation="false"
  automatic="false"
  ai_translate="false"

  # Process command line options
  for arg in "$@"; do
    case $arg in
      -y)
        automatic="true"
        shift
        ;;
      -t)
        translation="true"
        shift
        ;;
      -a)
        ai_translate="true"
        shift
        ;;
      *)
        # unknown option
        echo "Invalid option: $arg" >&2
        usage
        ;;
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
      echo "Please enter the locale (format as 'language_country', e.g. 'en_US'):"
      read locale

      # Initialize variables
      latitude=""
      longitude=""
      city=""

      if [ -n "$city_name" ] && [ -n "$country_name" ]; then
          # Call the function - it will set latitude, longitude, and city if successful
          get_city_info "$city_name" "$country_name"
          if [ $? -ne 0 ]; then
              # Function failed, ask for manual input
              echo "Failed to fetch location data. Please enter manually."
              echo "Please enter the latitude:"
              read latitude
              echo "Please enter the longitude:"
              read longitude
              echo "Please enter the city:"
              read city
          fi
      else
          echo "Please enter the latitude:"
          read latitude
          echo "Please enter the longitude:"
          read longitude
          echo "Please enter the city:"
          read city
      fi
  fi


  # Progress indicator function
  show_progress() {
    local pid=$1
    local delay=0.5
    local spinstr='|/-\'
    local temp
    echo "Starting Markaspot installation..."
    echo -n "Progress: ["
    # Create a 50-character progress bar
    for i in $(seq 1 50); do
      echo -n " "
    done
    echo -n "]"
    echo -n $'\r'
    echo -n "Progress: ["
    
    while ps a | awk '{print $1}' | grep -q "$pid"; do
      local temp=${spinstr#?}
      printf " %c " "$spinstr"
      local spinstr=$temp${spinstr%"$temp"}
      sleep $delay
      printf "\b\b\b"
      
      # Update progress bar randomly to simulate progress
      if [ $((RANDOM % 10)) -eq 0 ]; then
        local pos=$((RANDOM % 50))
        echo -n $'\r'
        echo -n "Progress: ["
        for i in $(seq 1 50); do
          if [ $i -le $pos ]; then
            echo -n "#"
          else
            echo -n " "
          fi
        done
        echo -n "]"
      fi
    done
    
    # Complete the progress bar
    echo -n $'\r'
    echo -n "Progress: ["
    for i in $(seq 1 50); do
      echo -n "#"
    done
    echo -n "]"
    echo
    echo "Installation complete!"
  }

  # Run the markaspot:install command in the background and capture its PID
  php -d memory_limit=-1 $(which drush) markaspot:install --lat="$latitude" --lng="$longitude" --city="$city" --locale="$locale" --skip-confirmation > markaspot_install.log 2>&1 &
  install_pid=$!
  
  # Show progress while the installation is running
  show_progress $install_pid
  
  # Wait for the installation to complete
  wait $install_pid
  
  # Check if the installation was successful
  if [ $? -ne 0 ]; then
    echo -e "\e[31mInstallation failed! Check markaspot_install.log for details.\e[0m"
    exit 1
  fi
  
  echo -e "\e[32mMarkaspot installation completed successfully!\e[0m"
  
  # Display the log file if needed
  echo "Installation log saved to markaspot_install.log"
  printf "\e[36mAdd Admin Role...\e[0m\n"
  drush user:role:add "administrator" --uid=1



  # Process language settings
  language=$(echo "$locale" | cut -d '_' -f1)
  
  # Handle translations
  if [ "$translation" = true ] && [ "$ai_translate" = true ]; then
    printf "\e[36mImporting language and using AI translation...\e[0m\n"
    # Pass the full locale to translate.sh which will handle extraction if needed
    $SCRIPT_DIR/translate.sh "$locale"
    
    # Check if OPENAI_API_KEY is set
    if [ -z "$OPENAI_API_KEY" ]; then
      printf "\e[33mWarning: OPENAI_API_KEY environment variable not set.\e[0m\n"
      printf "\e[33mPlease enter your OpenAI API key: \e[0m"
      read api_key
      export OPENAI_API_KEY=$api_key
    fi
    
    # Run AI translation
    printf "\e[36mRunning AI translation for content artifacts...\e[0m\n"
    
    # Ensure the script is executable
    chmod +x "$SCRIPT_DIR/ai-translate.sh"
    
    
    # Execute with explicit shell to ensure it runs in any environment
    sh "$SCRIPT_DIR/ai-translate.sh" $language 2>&1 || printf "\e[31mAI translation failed. Check if the script exists and is properly set up.\e[0m\n"
    
    # Set the site default language to match the chosen language
    printf "\e[36mSetting up site default language to $language\e[0m\n"
    drush config:set system.site default_langcode $language -y
    
    printf "\e[33mNote: Translation is being performed in the container using the available shell.\e[0m\n"
    printf "\e[33mFor more extensive translation capabilities, you can also run the script on your host system:\e[0m\n"
    printf "\e[33m  OPENAI_API_KEY=your_key ./scripts/ai-translate.sh $language\e[0m\n"
  elif [ "$translation" = true ]; then
    printf "\e[36mImporting language...\e[0m\n"
    # Pass the full locale to translate.sh which will handle extraction if needed
    $SCRIPT_DIR/translate.sh "$locale"
  elif [ "$ai_translate" = true ]; then
    printf "\e[36mSetting up language and using AI translation...\e[0m\n"
    
    # Pass the full locale for language setup
    language=$(echo "$locale" | cut -d '_' -f1)
    drush language-add "$language"
    
    # Check if OPENAI_API_KEY is set
    if [ -z "$OPENAI_API_KEY" ]; then
      printf "\e[33mWarning: OPENAI_API_KEY environment variable not set.\e[0m\n"
      printf "\e[33mPlease enter your OpenAI API key: \e[0m"
      read api_key
      export OPENAI_API_KEY=$api_key
    fi
    
    # Run AI translation
    printf "\e[36mRunning AI translation for content artifacts...\e[0m\n"
    
    # Ensure the script is executable
    chmod +x "$SCRIPT_DIR/ai-translate.sh"
    
    
    # Execute with explicit shell to ensure it runs in any environment
    sh "$SCRIPT_DIR/ai-translate.sh" $language 2>&1 || printf "\e[31mAI translation failed. Check if the script exists and is properly set up.\e[0m\n"
    
    # Set the site default language to match the chosen language
    printf "\e[36mSetting up site default language to $language\e[0m\n"
    drush config:set system.site default_langcode $language -y
    
    printf "\e[33mNote: Translation is being performed in the container using the available shell.\e[0m\n"
    printf "\e[33mFor more extensive translation capabilities, you can also run the script on your host system:\e[0m\n"
    printf "\e[33m  OPENAI_API_KEY=your_key ./scripts/ai-translate.sh $language\e[0m\n"
  else
    printf "\e[33mHint: For a multilingual site, use the -t option to import a Drupal translation file\e[0m\n"
    printf "\e[33mor use the -a option to use AI translation for content artifacts.\e[0m\n"
  fi

  printf "\e[36mImporting ..\e[0m\n"
  $SCRIPT_DIR/import.sh
  
  # If we used AI translation, restore original files after import and clean up
  if [ "$ai_translate" = true ]; then
    printf "\e[36mRestoring original artifact files...\e[0m\n"
    ARTIFACTS_DIR="$PWD/web/profiles/contrib/markaspot/modules/markaspot_default_content/artifacts"
    
    # Restore original files from backups
    for backup_file in "$ARTIFACTS_DIR"/*.bak; do
      if [ -f "$backup_file" ]; then
        original_file=$(echo "$backup_file" | sed 's/\.bak$//')
        mv "$backup_file" "$original_file"
        printf "  Restored %s\n" "$(basename "$original_file")"
      fi
    done
    
    # Clean up language directories
    printf "\e[36mCleaning up language-specific directories...\e[0m\n"
    LANG_DIR="$ARTIFACTS_DIR/$language"
    if [ -d "$LANG_DIR" ]; then
      # Remove temporary files but keep translated CSVs
      rm -f "$LANG_DIR"/prompt_*.txt
      rm -f "$LANG_DIR"/content_*.txt
      rm -f "$LANG_DIR"/request_*.json
      rm -f "$LANG_DIR"/response_*.json
      rm -f "$LANG_DIR"/translated_*.csv
      printf "  Cleaned up temporary files in %s\n" "$LANG_DIR"
    fi
  fi

  printf "\e[36mExecuting georeport client to import initial service requests...\e[0m\n"
  $SCRIPT_DIR/georeport-client.sh

  printf "\e[36mOne-Time Login for User 1 ...\e[0m\n"
  printf "\e[36m...\e[0m\n"
  drush uli --uri=http://localhost
  printf "\e[36m \e[0m\n"
  printf "\e[36mInstallation completed...\e[0m\n"
fi