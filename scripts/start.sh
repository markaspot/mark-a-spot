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
  echo "    -y    Install automatically (default: Köln, Germany, de_DE locale)"
  echo "    -t    Import translation file from the /translations directory and enable translations for terms"
  echo "    -a    Use AI translation (OpenAI) for content artifacts instead of standard translation files"
  echo
  echo "Environment variables for -y mode:"
  echo "    CITY=...             City name (default: Köln)"
  echo "    COUNTRY=...          Country name (default: Germany)"
  echo "    LOCALE=...           Locale code (default: de_DE)"
  echo "    OPENAI_API_KEY=...   Required for -a flag"
  echo
  echo "Examples:"
  echo "    ddev exec scripts/start.sh -y -a                      # Köln with AI translation"
  echo "    CITY=Bonn LOCALE=de_DE ddev exec scripts/start.sh -y  # Bonn without translation"
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
      # Use environment variables or default to Köln (dev environment)
      auto_city="${CITY:-Köln}"
      auto_country="${COUNTRY:-Germany}"
      locale="${LOCALE:-de_DE}"

      # Query Nominatim for coordinates
      printf "\e[36mLooking up coordinates for %s, %s...\e[0m\n" "$auto_city" "$auto_country"
      get_city_info "$auto_city" "$auto_country"
      if [ $? -ne 0 ]; then
        # Fallback to hardcoded Köln coordinates
        printf "\e[33mNominatim lookup failed, using default Köln coordinates\e[0m\n"
        latitude="50.9375"
        longitude="6.9603"
        city="Köln, Nordrhein-Westfalen, Deutschland"
      fi
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

  # Import config from config/sync (group types, roles, etc.)
  printf "\e[36mImporting configuration from config/sync...\e[0m\n"
  drush config:import -y

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

  # Process language settings
  language=$(echo "$locale" | cut -d '_' -f1)

  # Import English content FIRST (base language) - creates groups, categories, etc.
  printf "\e[36mImporting English base content (creates groups, categories, etc.)...\e[0m\n"
  $SCRIPT_DIR/import.sh

  # Fetch city boundary from Nominatim (requires group to exist from import.sh)
  printf "\e[36mFetching city boundary from Nominatim...\e[0m\n"
  if drush markaspot:fetch-boundary --city="$city" --group=1 -y 2>&1; then
    printf "\e[32mCity boundary stored successfully!\e[0m\n"
  else
    printf "\e[33mWarning: Could not fetch boundary. Run manually:\e[0m\n"
    printf "\e[33m  ddev drush markaspot:fetch-boundary --city=\"%s\" --group=1 -y\e[0m\n" "$city"
  fi

  # Update markaspot_validation.settings with WKT bounding box
  printf "\e[36mUpdating validation settings for location...\e[0m\n"

  # Create a bounding box WKT polygon (~15km radius around center)
  RADIUS_DEG="0.15"
  MIN_LAT=$(awk "BEGIN {printf \"%.6f\", $latitude - $RADIUS_DEG}")
  MAX_LAT=$(awk "BEGIN {printf \"%.6f\", $latitude + $RADIUS_DEG}")
  MIN_LNG=$(awk "BEGIN {printf \"%.6f\", $longitude - $RADIUS_DEG}")
  MAX_LNG=$(awk "BEGIN {printf \"%.6f\", $longitude + $RADIUS_DEG}")

  WKT="POLYGON(($MIN_LNG $MIN_LAT,$MAX_LNG $MIN_LAT,$MAX_LNG $MAX_LAT,$MIN_LNG $MAX_LAT,$MIN_LNG $MIN_LAT))"

  # Extract simple city name (first part before comma)
  SIMPLE_CITY=$(echo "$city" | cut -d',' -f1)

  drush config:set markaspot_validation.settings wkt "$WKT" -y >/dev/null 2>&1
  drush config:set markaspot_validation.settings location.0 "$SIMPLE_CITY" -y >/dev/null 2>&1
  drush config:set markaspot_validation.settings locality.0 "$SIMPLE_CITY" -y >/dev/null 2>&1

  printf "\e[32mValidation WKT set for: %s\e[0m\n" "$SIMPLE_CITY"

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

  # Handle translations - English is always base, other languages are translations
  if [ "$translation" = true ] || [ "$ai_translate" = true ]; then
    # Add target language
    printf "\e[36mAdding language: %s...\e[0m\n" "$language"
    drush language-add "$language" 2>/dev/null || true

    # Enable multilingual support for entities
    printf "\e[36mEnabling multilingual support for entities...\e[0m\n"
    drush en markaspot_language -y

    # Always import Drupal UI translations and set language negotiation
    # This ensures the target language becomes the "selected" interface language
    printf "\e[36mImporting Drupal translations and configuring language settings...\e[0m\n"
    $SCRIPT_DIR/translate.sh "$locale"

    # AI translate content artifacts if -a flag
    if [ "$ai_translate" = true ]; then
      # Check if OPENAI_API_KEY is set
      if [ -z "$OPENAI_API_KEY" ]; then
        printf "\e[33mWarning: OPENAI_API_KEY environment variable not set.\e[0m\n"
        printf "\e[33mPlease enter your OpenAI API key: \e[0m"
        read api_key
        export OPENAI_API_KEY=$api_key
      fi

      # Run AI translation (creates CSVs in artifacts/<lang>/, does NOT modify originals)
      printf "\e[36mRunning AI translation for content artifacts...\e[0m\n"
      chmod +x "$SCRIPT_DIR/ai-translate.sh"
      sh "$SCRIPT_DIR/ai-translate.sh" $language 2>&1 || printf "\e[31mAI translation failed.\e[0m\n"

      # Add translations from the translated CSVs
      if [ -f "$SCRIPT_DIR/create-translations.php" ]; then
        printf "\e[36mAdding %s translations via Entity API...\e[0m\n" "$language"
        drush php:script "$SCRIPT_DIR/create-translations.php" -- "$language" 2>&1 || printf "\e[33mWarning: Could not create translations\e[0m\n"
      fi

      # Clean up language directory
      ARTIFACTS_DIR="$PWD/web/profiles/contrib/markaspot/modules/markaspot_default_content/artifacts"
      LANG_DIR="$ARTIFACTS_DIR/$language"
      if [ -d "$LANG_DIR" ]; then
        printf "\e[36mCleaning up temporary files...\e[0m\n"
        rm -rf "$LANG_DIR"
      fi
    fi
  else
    printf "\e[33mHint: For a multilingual site, use -t (Drupal translations) and/or -a (AI content translation)\e[0m\n"
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

  # Run georeport client to create users and test data
  $SCRIPT_DIR/georeport-client.sh

  # Configure groups and memberships
  printf "\e[36mConfiguring groups and user memberships...\e[0m\n"

  # Determine language configuration for frontend
  if [ "$translation" = true ] || [ "$ai_translate" = true ]; then
    # Multilingual: target language is default, English as fallback
    NUXT_DEFAULT_LANG="$language"
    NUXT_AVAILABLE_LANGS="[\"$language\", \"en\"]"
  else
    # English only
    NUXT_DEFAULT_LANG="en"
    NUXT_AVAILABLE_LANGS="[\"en\"]"
  fi

  # Update jurisdiction group: label and nuxt config with language
  printf "\e[36mConfiguring jurisdiction with language: %s...\e[0m\n" "$NUXT_DEFAULT_LANG"
  drush php:eval "
    \$group = \Drupal::entityTypeManager()->getStorage('group')->load(1);
    if (\$group && \$group->getGroupType()->id() === 'jur') {
      // Set city name as label
      \$group->set('label', '$city');

      // Get existing nuxt config or create new
      \$existing_config = [];
      if (\$group->hasField('field_nuxt_config') && !\$group->get('field_nuxt_config')->isEmpty()) {
        \$existing_json = \$group->get('field_nuxt_config')->value;
        \$existing_config = json_decode(\$existing_json, true) ?: [];
      }

      // Merge language configuration
      \$existing_config['languages'] = [
        'default' => '$NUXT_DEFAULT_LANG',
        'available' => json_decode('$NUXT_AVAILABLE_LANGS', true)
      ];

      // Also set client name from city
      \$existing_config['client'] = \$existing_config['client'] ?? [];
      \$existing_config['client']['name'] = '$city';

      // Save updated config
      \$group->set('field_nuxt_config', json_encode(\$existing_config, JSON_PRETTY_PRINT | JSON_UNESCAPED_UNICODE));
      \$group->save();

      echo 'Jurisdiction configured: $city (language: $NUXT_DEFAULT_LANG)' . PHP_EOL;
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
