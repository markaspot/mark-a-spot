#!/bin/sh

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"

# Determine Drupal web root for different container setups (DDEV and legacy Docker).
if [ -d "$PROJECT_ROOT/web/sites/default" ]; then
  WEB_ROOT="$PROJECT_ROOT/web"
elif [ -d "/app/data/web/sites/default" ]; then
  WEB_ROOT="/app/data/web"
else
  echo "ERROR: Unable to locate Drupal web directory. Checked '$PROJECT_ROOT/web' and '/app/data/web'."
  exit 1
fi

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
  SETTINGS_FILE="$WEB_ROOT/sites/default/settings.php"
  DEFAULT_SETTINGS_FILE="$WEB_ROOT/sites/default/default.settings.php"

  if [ ! -f "$DEFAULT_SETTINGS_FILE" ]; then
    echo "ERROR: Cannot find default settings file at $DEFAULT_SETTINGS_FILE"
    exit 1
  fi

  cp "$DEFAULT_SETTINGS_FILE" "$SETTINGS_FILE"

  DB_NAME=${DRUPAL_DATABASE_NAME:-${DB_NAME:-db}}
  DB_USER=${DRUPAL_DATABASE_USERNAME:-${DB_USER:-db}}
  DB_PASS=${DRUPAL_DATABASE_PASSWORD:-${DB_PASSWORD:-db}}
  DB_HOST=${MARKASPOT_MARIADB_SERVICE_HOST:-${DB_HOST:-db}}
  DB_PORT=${DRUPAL_DATABASE_PORT:-${DB_PORT:-3306}}
  HASH_SALT=${DRUPAL_HASH_SALT:-$(tr -dc 'a-z0-9' </dev/urandom | head -c 32)}

  # Custom database configuration
  CUSTOM_DB_CONFIG="\\
  \$databases['default']['default'] = [\\
      'database' => '$DB_NAME',\\
      'username' => '$DB_USER',\\
      'password' => '$DB_PASS',\\
      'prefix' => '',\\
      'host' => '$DB_HOST',\\
      'port' => $DB_PORT,\\
      'namespace' => 'Drupal\\\\\\\\Core\\\\\\\\Database\\\\\\\\Driver\\\\\\\\mysql',\\
      'driver' => 'mysql',\\
  ];"

  # Add the custom database configuration after the $databases declaration
  sed -i "/\$databases = \[\];/a $CUSTOM_DB_CONFIG" "$SETTINGS_FILE"

  # Custom hash salt configuration
  CUSTOM_HASH_SALT="\$settings['hash_salt'] = '$HASH_SALT';"

  # Replace the existing hash salt configuration with the custom one
  sed -i "s/\$settings\['hash_salt'\] = '';$/$CUSTOM_HASH_SALT/" "$SETTINGS_FILE"

  # Update the config_sync_directory setting
  sed -i "s|# \$settings\['config_sync_directory'\] = '/directory/outside/webroot';|\$settings['config_sync_directory'] = '../config/sync';|" "$SETTINGS_FILE"

  cat <<'EOF' >> "$SETTINGS_FILE"

// Override the GeoReport API key with environment configuration when available.
if ((isset($app_root) || PHP_SAPI === 'cli') && ($geoKey = getenv('GEOREPORT_API_KEY'))) {
  if ($geoKey !== '*' && $geoKey !== '') {
    $config['services_api_key_auth.api_key.nuxt']['key'] = $geoKey;
  }
}

EOF

  printf "\e[32mCustom configuration added to $SETTINGS_FILE\e[0m\n"

  printf "\e[36mDropping all tables in the database...\e[0m\n"
  drush sql-drop -y
  printf "\e[36mExecuting the Markaspot:install command...\e[0m\n"


  # Function to query the Nominatim API for city information
  get_city_info() {
      # Ensure curl and php are available
      if ! command -v curl >/dev/null 2>&1; then
          echo "ERROR: curl is required but not installed."
          return 1
      fi

      if ! command -v php >/dev/null 2>&1; then
          echo "ERROR: PHP CLI is required but not available."
          return 1
      fi

      city_name=$(php -r 'echo rawurlencode($argv[1]);' "$1")
      country_name=$(php -r 'echo rawurlencode($argv[1]);' "$2")

      # Exit the function if either city name or country name is empty
      if [ -z "$city_name" ] || [ -z "$country_name" ]; then
          echo "ERROR: Empty city or country name"
          return 1
      fi

      response=$(curl -s "https://nominatim.openstreetmap.org/search?city=$city_name&country=$country_name&format=json&limit=10")

      # Check if curl request was successful and response is valid JSON
      if [ $? -ne 0 ] || [ -z "$response" ]; then
          echo "ERROR: Failed to query the Nominatim API."
          return 1
      fi

      locations=$(printf "%s" "$response" | php -r '
          $data = json_decode(stream_get_contents(STDIN), true);
          if (!is_array($data) || empty($data)) {
              exit(1);
          }
          foreach ($data as $row) {
              if (!isset($row["lat"], $row["lon"], $row["display_name"])) {
                  continue;
              }
              $display = str_replace(["\n", "\r"], " ", $row["display_name"]);
              echo $row["lat"], "\t", $row["lon"], "\t", $display, "\n";
          }
      ')

      if [ $? -ne 0 ] || [ -z "$locations" ]; then
          echo "ERROR: Failed to parse location data."
          return 1
      fi

      count=$(printf "%s" "$locations" | grep -c '^')
      if [ "$count" -eq 0 ]; then
          echo "ERROR: No results found for $1 in $2."
          return 1
      elif [ "$count" -eq 1 ]; then
          selected="$locations"
      else
          echo "Multiple locations found. Please select one by entering the corresponding number:"
          printf "%s" "$locations" | nl -ba
          read -p "Choice: " choice
          if ! printf "%s" "$choice" | grep -Eq '^[0-9]+$' || [ "$choice" -lt 1 ] || [ "$choice" -gt "$count" ]; then
              echo "ERROR: Invalid selection."
              return 1
          fi
          selected=$(printf "%s" "$locations" | sed -n "${choice}p")
      fi

      # Set global variables directly using tab-delimited values
      latitude=$(printf "%s" "$selected" | cut -f1)
      longitude=$(printf "%s" "$selected" | cut -f2)
      city=$(printf "%s" "$selected" | cut -f3-)

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
  install_exit_code=$?

  # Check if Drupal was actually installed (more reliable than exit code)
  if drush status --field=bootstrap 2>/dev/null | grep -q "Successful"; then
    echo -e "\e[32mDrupal bootstrap verified successfully.\e[0m"
  elif [ $install_exit_code -ne 0 ]; then
    echo -e "\e[31mInstallation failed! Check markaspot_install.log for details.\e[0m"
    exit 1
  fi
  
  echo -e "\e[32mMarkaspot installation completed successfully!\e[0m"
  
  # Display the log file if needed
  echo "Installation log saved to markaspot_install.log"
  printf "\e[36mAdd Admin Role...\e[0m\n"
  drush user:role:add "administrator" --uid=1

  # Set coordinates in all config locations
  printf "\e[36mConfiguring map coordinates...\e[0m\n"

  # markaspot_nuxt.settings - main frontend map center
  drush config:set markaspot_nuxt.settings center_lat -y -- "$latitude" >/dev/null
  drush config:set markaspot_nuxt.settings center_lng -y -- "$longitude" >/dev/null

  # Field default value for geolocation field
  drush config:set field.field.node.service_request.field_geolocation default_value.0.lat -y -- "$latitude" >/dev/null
  drush config:set field.field.node.service_request.field_geolocation default_value.0.lng -y -- "$longitude" >/dev/null

  # Widget settings for form displays (map center in edit forms)
  drush config:set core.entity_form_display.node.service_request.default third_party_settings.geolocation.centre.lat -y -- "$latitude" >/dev/null 2>&1 || true
  drush config:set core.entity_form_display.node.service_request.default third_party_settings.geolocation.centre.lng -y -- "$longitude" >/dev/null 2>&1 || true

  # Update widget center_lat/center_lng settings
  for form_mode in default management nuxt; do
    drush config:set "core.entity_form_display.node.service_request.$form_mode" content.field_geolocation.settings.center_lat -y -- "$latitude" >/dev/null 2>&1 || true
    drush config:set "core.entity_form_display.node.service_request.$form_mode" content.field_geolocation.settings.center_lng -y -- "$longitude" >/dev/null 2>&1 || true
  done

  printf "\e[32mMap coordinates set to: %s, %s\e[0m\n" "$latitude" "$longitude"

  # Fix GeoReport status configuration
  # status_closed should be 5,6 (Closed, Archived) not 3,4
  printf "\e[36mConfiguring GeoReport status mappings...\e[0m\n"
  drush config:delete markaspot_open311.settings status_closed.3 -y >/dev/null 2>&1 || true
  drush config:delete markaspot_open311.settings status_closed.4 -y >/dev/null 2>&1 || true
  drush config:set markaspot_open311.settings status_closed.5 5 -y >/dev/null 2>&1
  drush config:set markaspot_open311.settings status_closed.6 6 -y >/dev/null 2>&1

  # Add view permission to org-anonymous group role for API access
  printf "\e[36mConfiguring Group permissions for anonymous API access...\e[0m\n"
  drush php:eval '
    $config = \Drupal::service("config.factory")->getEditable("group.role.org-anonymous");
    $perms = $config->get("permissions") ?: [];
    if (!in_array("view group_node:service_request entity", $perms)) {
      $perms[] = "view group_node:service_request entity";
      $config->set("permissions", $perms)->save();
    }
  ' 2>/dev/null || true

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

  # If we used AI translation, create English translations from original CSVs
  if [ "$ai_translate" = true ]; then
    printf "\e[36mCreating English translations from original CSV files...\e[0m\n"
    ARTIFACTS_DIR="$PWD/web/profiles/contrib/markaspot/modules/markaspot_default_content/artifacts"

    # The .bak files contain the original English content
    # The create-translations.php script can read .bak files directly
    if [ -f "$SCRIPT_DIR/create-translations.php" ]; then
      printf "\e[36mAdding English translations via Entity API...\e[0m\n"
      drush php:script "$SCRIPT_DIR/create-translations.php" -- en 2>&1 || printf "\e[33mWarning: Could not create English translations\e[0m\n"
    else
      printf "\e[33mWarning: create-translations.php not found, skipping English translations\e[0m\n"
    fi

    printf "\e[36mRestoring original artifact files...\e[0m\n"

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
  # Ensure GeoReport API key exists in environment and Drupal config.
  # Config file has key: '*' (safe for git), real key injected via settings.php.
  ENV_GEOREPORT_API_KEY=${GEOREPORT_API_KEY:-}

  if [ -n "$ENV_GEOREPORT_API_KEY" ] && [ "$ENV_GEOREPORT_API_KEY" != "*" ]; then
    GEOREPORT_API_KEY="$ENV_GEOREPORT_API_KEY"
  else
    # Generate a new API key
    GEOREPORT_API_KEY=$(php -r 'echo bin2hex(random_bytes(16));')
  fi

  # Set the key in Drupal config for immediate use during installation
  drush config-set services_api_key_auth.api_key.nuxt key "$GEOREPORT_API_KEY" -y >/dev/null
  printf "\e[32mAPI key set in Drupal config\e[0m\n"

  # Write to .ddev/.env for DDEV environments (persists across restarts)
  DDEV_ENV_FILE="$PROJECT_ROOT/.ddev/.env"
  if [ -d "$PROJECT_ROOT/.ddev" ]; then
    # Create or update .ddev/.env with the API key
    if [ -f "$DDEV_ENV_FILE" ]; then
      # Remove existing GEOREPORT_API_KEY line if present
      grep -v "^GEOREPORT_API_KEY=" "$DDEV_ENV_FILE" > "${DDEV_ENV_FILE}.tmp" 2>/dev/null || true
      mv "${DDEV_ENV_FILE}.tmp" "$DDEV_ENV_FILE"
    fi
    echo "GEOREPORT_API_KEY=$GEOREPORT_API_KEY" >> "$DDEV_ENV_FILE"
    printf "\e[32mAPI key written to .ddev/.env (persists after restart)\e[0m\n"
  fi

  export GEOREPORT_API_KEY
  printf "GeoReport API key: %s\n" "$GEOREPORT_API_KEY"

  # Restart DDEV to apply API key to UI container
  if [ -n "$DDEV_HOSTNAME" ]; then
    printf "\e[36mRestarting DDEV to apply API key to UI container...\e[0m\n"
    ddev restart
  fi

  $SCRIPT_DIR/georeport-client.sh

  # Configure groups and memberships
  printf "\e[36mConfiguring groups and user memberships...\e[0m\n"

  # Update jurisdiction group label with city name
  drush php:eval "
    \$group = \Drupal::entityTypeManager()->getStorage('group')->load(1);
    if (\$group && \$group->getGroupType()->id() === 'jur') {
      \$group->set('label', '$city');
      \$group->save();
      echo 'Jurisdiction renamed to: $city' . PHP_EOL;
    }
  " 2>/dev/null || true

  # Add admin to all groups (admin roles are auto-assigned via global_role config)
  drush php:eval "
    \$user = \Drupal\user\Entity\User::load(1);
    \$groups = \Drupal::entityTypeManager()->getStorage('group')->loadMultiple();
    foreach (\$groups as \$group) {
      if (!\$group->getMember(\$user)) {
        \$group->addMember(\$user);
        echo 'Admin added to ' . \$group->label() . PHP_EOL;
      }
    }
  " 2>/dev/null || true

  # Add users to jurisdiction group and departments
  drush php:eval "
    \$group_storage = \Drupal::entityTypeManager()->getStorage('group');
    \$user_storage = \Drupal::entityTypeManager()->getStorage('user');

    \$jur = \$group_storage->load(1);
    \$dept1 = \$group_storage->load(2);
    \$dept2 = \$group_storage->load(3);

    // Add api_user and moderators to jurisdiction
    foreach (['api_user', 'moderation_1', 'moderation_2'] as \$name) {
      \$users = \$user_storage->loadByProperties(['name' => \$name]);
      \$user = reset(\$users);
      if (\$user && \$jur && !\$jur->getMember(\$user)) {
        \$jur->addMember(\$user);
      }
    }

    // Add moderation_1 to Department 1
    \$mod1 = \$user_storage->loadByProperties(['name' => 'moderation_1']);
    \$mod1 = reset(\$mod1);
    if (\$mod1 && \$dept1 && !\$dept1->getMember(\$mod1)) {
      \$dept1->addMember(\$mod1);
    }

    // Add moderation_2 to Department 2
    \$mod2 = \$user_storage->loadByProperties(['name' => 'moderation_2']);
    \$mod2 = reset(\$mod2);
    if (\$mod2 && \$dept2 && !\$dept2->getMember(\$mod2)) {
      \$dept2->addMember(\$mod2);
    }

    echo 'Users assigned to groups' . PHP_EOL;
  " 2>/dev/null || true

  printf "\e[32mGroup configuration complete.\e[0m\n"

  # Truncate city name if too long (max 60 chars to fit in box)
  display_city=$(printf "%.60s" "$city")
  if [ ${#city} -gt 60 ]; then
    display_city="${display_city}..."
  fi

  printf "\n\e[32m╔════════════════════════════════════════════════════════════════════════╗\e[0m\n"
  printf "\e[32m║\e[0m %-72s \e[32m║\e[0m\n" "Mark-a-Spot Installation Complete!"
  printf "\e[32m╠════════════════════════════════════════════════════════════════════════╣\e[0m\n"
  printf "\e[32m║\e[0m %-72s \e[32m║\e[0m\n" "City: $display_city"
  printf "\e[32m║\e[0m %-72s \e[32m║\e[0m\n" "Locale: $locale"
  printf "\e[32m║\e[0m %-72s \e[32m║\e[0m\n" "API Key: $GEOREPORT_API_KEY"
  printf "\e[32m╠════════════════════════════════════════════════════════════════════════╣\e[0m\n"
  printf "\e[32m║\e[0m %-72s \e[32m║\e[0m\n" "Users: admin, api_user, moderation_1, moderation_2"
  printf "\e[32m║\e[0m %-72s \e[32m║\e[0m\n" "Service Requests: 50 test entries"
  printf "\e[32m╚════════════════════════════════════════════════════════════════════════╝\e[0m\n"

  printf "\n\e[36mOne-Time Login:\e[0m\n"
  if [ -n "$DDEV_HOSTNAME" ]; then
    drush uli --uri="https://$DDEV_HOSTNAME"
  else
    drush uli --uri=http://localhost
  fi

  printf "\n\e[33mNext steps:\e[0m\n"
  printf "  1. Run 'ddev restart' to apply API key to frontend\n"
  if [ -n "$DDEV_HOSTNAME" ]; then
    printf "  2. Access frontend at: https://%s:8040\n\n" "$DDEV_HOSTNAME"
  else
    printf "  2. Access frontend at: http://localhost:3000\n\n"
  fi
fi
