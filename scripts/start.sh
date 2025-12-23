#!/bin/sh

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"

# =============================================================================
# Output Formatting
# =============================================================================
GREEN='\033[32m'
YELLOW='\033[33m'
CYAN='\033[36m'
RED='\033[31m'
RESET='\033[0m'
BOLD='\033[1m'

step()    { printf "${CYAN}→${RESET} %s\n" "$1"; }
success() { printf "${GREEN}✓${RESET} %s\n" "$1"; }
warn()    { printf "${YELLOW}⚠${RESET} %s\n" "$1"; }
error()   { printf "${RED}✗${RESET} %s\n" "$1"; }
info()    { printf "  %s\n" "$1"; }

# =============================================================================
# Environment Setup
# =============================================================================

# Determine Drupal web root for different container setups (DDEV and legacy Docker).
if [ -d "$PROJECT_ROOT/web/sites/default" ]; then
  WEB_ROOT="$PROJECT_ROOT/web"
elif [ -d "/app/data/web/sites/default" ]; then
  WEB_ROOT="/app/data/web"
else
  echo "ERROR: Unable to locate Drupal web directory. Checked '$PROJECT_ROOT/web' and '/app/data/web'."
  exit 1
fi

# Multisite support: default to "default" for single-site installs
SITE_NAME="default"
SITE_URI=""
DRUSH_URI=""

usage() {
  echo "Usage: start.sh [--site=SITENAME] [-y] [-t] [-a]"
  echo
  echo "Options:"
  echo "    --site=NAME  Site name for multisite (e.g., aachen, bonn). Uses sites/NAME/"
  echo "    -y           Install automatically (default: Köln, Germany, de_DE locale)"
  echo "    -t           Import translation file from the /translations directory and enable translations for terms"
  echo "    -a           Use AI translation (OpenAI) for content artifacts instead of standard translation files"
  echo
  echo "Environment variables for -y mode:"
  echo "    CITY=...             City name (default: Köln)"
  echo "    COUNTRY=...          Country name (default: Germany)"
  echo "    LOCALE=...           Locale code (default: de_DE)"
  echo "    OPENAI_API_KEY=...   Required for -a flag"
  echo
  echo "Examples:"
  echo "    ddev exec scripts/start.sh -y -a                            # Single site: Köln with AI translation"
  echo "    ddev exec scripts/start.sh --site=aachen -y                 # Multisite: Aachen"
  echo "    CITY=Bonn ddev exec scripts/start.sh --site=bonn -y         # Multisite: Bonn"
  exit 1
}

if [ "$1" = "-h" ] || [ "$1" = "--help" ]; then
  usage
fi

# Parse --site parameter first (before other args)
for arg in "$@"; do
  case $arg in
    --site=*)
      SITE_NAME="${arg#*=}"
      shift
      ;;
  esac
done

# Set up multisite paths and URIs
if [ "$SITE_NAME" != "default" ]; then
  SITE_DIR="$WEB_ROOT/sites/$SITE_NAME"
  SITE_URI="$SITE_NAME.ddev.site"
  DRUSH_URI="--uri=$SITE_URI"
  CONFIG_SYNC_DIR="../config/$SITE_NAME"
  CONFIG_SYNC_ABS="$PROJECT_ROOT/config/$SITE_NAME"

  # For multisite, check if site directory exists
  if [ ! -d "$SITE_DIR" ]; then
    error "Site directory not found: $SITE_DIR"
    info "Run install-multisite.sh first to create the site structure."
    exit 1
  fi

  # For multisite, we need to handle config specially:
  # - Fresh install: install without --existing-config, then import config
  # - The config/SITE_NAME directory will be populated after install
  CONFIG_NEEDS_COPY="false"
  if [ ! -d "$CONFIG_SYNC_ABS" ] || [ -z "$(ls -A "$CONFIG_SYNC_ABS" 2>/dev/null)" ]; then
    CONFIG_NEEDS_COPY="true"
    mkdir -p "$CONFIG_SYNC_ABS"
    info "Config directory empty, will export after install"
  fi

  step "Multisite mode: $SITE_NAME ($SITE_URI)"
else
  SITE_DIR="$WEB_ROOT/sites/default"
  CONFIG_SYNC_DIR="../config/sync"
  CONFIG_NEEDS_COPY="false"
  step "Single-site mode"
fi

step "Installing composer dependencies..."
composer install --no-dev


if [ "$ENVIRONMENT" != "prod" ]; then
  step "Installing Mark-a-Spot Distribution..."

  # Define the path to the Drupal settings file
  SETTINGS_FILE="$SITE_DIR/settings.php"
  DEFAULT_SETTINGS_FILE="$WEB_ROOT/sites/default/default.settings.php"

  # For multisite, settings.php may already exist from install-multisite.sh
  if [ "$SITE_NAME" != "default" ] && [ -f "$SETTINGS_FILE" ]; then
    info "Using existing settings.php for multisite $SITE_NAME"
  else
    if [ ! -f "$DEFAULT_SETTINGS_FILE" ]; then
      error "Cannot find default settings file at $DEFAULT_SETTINGS_FILE"
      exit 1
    fi
    cp "$DEFAULT_SETTINGS_FILE" "$SETTINGS_FILE"
  fi

  # Database name: use site name for multisite, 'db' for single site
  if [ "$SITE_NAME" != "default" ]; then
    DB_NAME="$SITE_NAME"
  else
    DB_NAME=${DRUPAL_DATABASE_NAME:-${DB_NAME:-db}}
  fi
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

  # Update the config_sync_directory setting (uses CONFIG_SYNC_DIR for multisite support)
  sed -i "s|# \$settings\['config_sync_directory'\] = '/directory/outside/webroot';|\$settings['config_sync_directory'] = '$CONFIG_SYNC_DIR';|" "$SETTINGS_FILE"

  cat <<'EOF' >> "$SETTINGS_FILE"

// Override the GeoReport API key with environment configuration when available.
if ((isset($app_root) || PHP_SAPI === 'cli') && ($geoKey = getenv('GEOREPORT_API_KEY'))) {
  if ($geoKey !== '*' && $geoKey !== '') {
    $config['services_api_key_auth.api_key.nuxt']['key'] = $geoKey;
  }
}

EOF

  success "Settings configured"

  step "Preparing database..."
  drush $DRUSH_URI sql-drop -y >/dev/null 2>&1


  # Function to query the Nominatim API for city information
  get_city_info() {
      if ! command -v curl >/dev/null 2>&1; then
          error "curl is required but not installed"
          return 1
      fi

      if ! command -v php >/dev/null 2>&1; then
          error "PHP CLI is required but not available"
          return 1
      fi

      city_name=$(php -r 'echo rawurlencode($argv[1]);' "$1")
      country_name=$(php -r 'echo rawurlencode($argv[1]);' "$2")

      if [ -z "$city_name" ] || [ -z "$country_name" ]; then
          error "Empty city or country name"
          return 1
      fi

      response=$(curl -s "https://nominatim.openstreetmap.org/search?city=$city_name&country=$country_name&format=json&limit=10")

      if [ $? -ne 0 ] || [ -z "$response" ]; then
          error "Failed to query Nominatim API"
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
          error "Failed to parse location data"
          return 1
      fi

      count=$(printf "%s" "$locations" | grep -c '^')
      if [ "$count" -eq 0 ]; then
          error "No results found for $1 in $2"
          return 1
      elif [ "$count" -eq 1 ]; then
          selected="$locations"
      else
          if [ "$automatic" = "true" ]; then
              info "Multiple locations found, auto-selecting first result"
              selected=$(printf "%s" "$locations" | head -1)
          else
              printf "\nMultiple locations found. Please select:\n"
              printf "%s" "$locations" | nl -ba
              read -p "Choice: " choice
              if ! printf "%s" "$choice" | grep -Eq '^[0-9]+$' || [ "$choice" -lt 1 ] || [ "$choice" -gt "$count" ]; then
                  error "Invalid selection"
                  return 1
              fi
              selected=$(printf "%s" "$locations" | sed -n "${choice}p")
          fi
      fi

      latitude=$(printf "%s" "$selected" | cut -f1)
      longitude=$(printf "%s" "$selected" | cut -f2)
      city=$(printf "%s" "$selected" | cut -f3-)

      success "Location: $city ($latitude, $longitude)"
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
      auto_city="${CITY:-Köln}"
      auto_country="${COUNTRY:-Germany}"
      locale="${LOCALE:-de_DE}"

      step "Looking up coordinates for $auto_city, $auto_country..."
      get_city_info "$auto_city" "$auto_country"
      if [ $? -ne 0 ]; then
        warn "Nominatim lookup failed, using default Köln coordinates"
        latitude="50.9375"
        longitude="6.9603"
        city="Köln, Nordrhein-Westfalen, Deutschland"
      fi
  else
      printf "\nCity name (blank for manual coordinates): "
      read city_name
      printf "Country name: "
      read country_name
      printf "Locale (e.g. 'en_US', 'de_DE'): "
      read locale

      latitude=""
      longitude=""
      city=""

      if [ -n "$city_name" ] && [ -n "$country_name" ]; then
          step "Looking up coordinates..."
          get_city_info "$city_name" "$country_name"
          if [ $? -ne 0 ]; then
              warn "Lookup failed, please enter manually"
              printf "Latitude: "
              read latitude
              printf "Longitude: "
              read longitude
              printf "City: "
              read city
          fi
      else
          printf "Latitude: "
          read latitude
          printf "Longitude: "
          read longitude
          printf "City: "
          read city
      fi
  fi


  # Progress indicator function
  show_progress() {
    local pid=$1
    local delay=0.3
    local spinstr='|/-\'
    local i=0

    printf "${CYAN}-${RESET} Installing Drupal "

    while ps a | awk '{print $1}' | grep -q "$pid"; do
      local char=$(echo "$spinstr" | cut -c$((i % 4 + 1)))
      printf "\b%s" "$char"
      sleep $delay
      i=$((i + 1))
    done

    printf "\b \n"
  }

  # For multisite with empty config, use site:install directly (no --existing-config)
  # For single-site or sites with config, use markaspot:install
  if [ "$SITE_NAME" != "default" ] && [ "$CONFIG_NEEDS_COPY" = "true" ]; then
    info "Fresh multisite install (no --existing-config)"
    php -d memory_limit=-1 $(which drush) $DRUSH_URI site:install markaspot \
      --account-name=admin --account-pass=admin --account-mail=admin@example.com \
      --site-name="$city" --locale=en -y > markaspot_install.log 2>&1 &
  else
    php -d memory_limit=-1 $(which drush) $DRUSH_URI markaspot:install --lat="$latitude" --lng="$longitude" --city="$city" --locale="$locale" --skip-confirmation > markaspot_install.log 2>&1 &
  fi
  install_pid=$!

  show_progress $install_pid

  wait $install_pid
  install_exit_code=$?

  if drush $DRUSH_URI status --field=bootstrap 2>/dev/null | grep -q "Successful"; then
    success "Drupal installed"
  elif [ $install_exit_code -ne 0 ]; then
    error "Installation failed! Check markaspot_install.log"
    exit 1
  fi

  # For fresh multisite installs, export config to establish baseline
  # For existing sites, import config as usual
  if [ "$SITE_NAME" != "default" ] && [ "$CONFIG_NEEDS_COPY" = "true" ]; then
    step "Exporting config (fresh multisite baseline)..."
    drush $DRUSH_URI config:export -y >/dev/null 2>&1
    success "Config exported"
  else
    step "Importing configuration..."
    drush $DRUSH_URI config:import -y
    success "Config imported"
  fi

  step "Configuring admin user..."
  drush $DRUSH_URI user:role:add "administrator" --uid=1 >/dev/null 2>&1
  success "Admin role assigned"

  step "Configuring themes..."
  drush $DRUSH_URI config:set system.theme admin gin -y >/dev/null 2>&1
  drush $DRUSH_URI config:set system.theme default gin -y >/dev/null 2>&1
  drush $DRUSH_URI cr >/dev/null 2>&1
  success "Themes configured (gin)"

  step "Configuring map coordinates..."

  # markaspot_nuxt.settings - main frontend map center
  drush $DRUSH_URI config:set markaspot_nuxt.settings center_lat -y -- "$latitude" >/dev/null
  drush $DRUSH_URI config:set markaspot_nuxt.settings center_lng -y -- "$longitude" >/dev/null

  # Field default value for geolocation field
  drush $DRUSH_URI config:set field.field.node.service_request.field_geolocation default_value.0.lat -y -- "$latitude" >/dev/null
  drush $DRUSH_URI config:set field.field.node.service_request.field_geolocation default_value.0.lng -y -- "$longitude" >/dev/null

  # Widget settings for form displays (map center in edit forms)
  drush $DRUSH_URI config:set core.entity_form_display.node.service_request.default third_party_settings.geolocation.centre.lat -y -- "$latitude" >/dev/null 2>&1 || true
  drush $DRUSH_URI config:set core.entity_form_display.node.service_request.default third_party_settings.geolocation.centre.lng -y -- "$longitude" >/dev/null 2>&1 || true

  # Update widget center_lat/center_lng settings and geocoding bbox
  # Calculate bounding box for geocoding (Nominatim viewbox format: minLng,minLat,maxLng,maxLat)
  BBOX_RADIUS="0.20"  # ~22km radius for geocoding search area
  BBOX_MIN_LAT=$(awk "BEGIN {printf \"%.6f\", $latitude - $BBOX_RADIUS}")
  BBOX_MAX_LAT=$(awk "BEGIN {printf \"%.6f\", $latitude + $BBOX_RADIUS}")
  BBOX_MIN_LNG=$(awk "BEGIN {printf \"%.6f\", $longitude - $BBOX_RADIUS}")
  BBOX_MAX_LNG=$(awk "BEGIN {printf \"%.6f\", $longitude + $BBOX_RADIUS}")
  LIMIT_VIEWBOX="$BBOX_MIN_LNG,$BBOX_MIN_LAT,$BBOX_MAX_LNG,$BBOX_MAX_LAT"

  # Extract simple city name (first part before comma)
  SIMPLE_CITY_NAME=$(echo "$city" | cut -d',' -f1)

  for form_mode in default management nuxt; do
    drush $DRUSH_URI config:set "core.entity_form_display.node.service_request.$form_mode" content.field_geolocation.settings.center_lat -y -- "$latitude" >/dev/null 2>&1 || true
    drush $DRUSH_URI config:set "core.entity_form_display.node.service_request.$form_mode" content.field_geolocation.settings.center_lng -y -- "$longitude" >/dev/null 2>&1 || true
    drush $DRUSH_URI config:set "core.entity_form_display.node.service_request.$form_mode" content.field_geolocation.settings.limit_viewbox -y -- "$LIMIT_VIEWBOX" >/dev/null 2>&1 || true
    drush $DRUSH_URI config:set "core.entity_form_display.node.service_request.$form_mode" content.field_geolocation.settings.city -y -- "$SIMPLE_CITY_NAME" >/dev/null 2>&1 || true
  done

  success "Map center: $latitude, $longitude"
  info "Geocoding bbox: $LIMIT_VIEWBOX"

  language=$(echo "$locale" | cut -d '_' -f1)

  # Always add the language from locale (en is default, others are additional)
  if [ "$language" != "en" ]; then
    step "Adding language: $language..."
    drush $DRUSH_URI language-add "$language" >/dev/null 2>&1 || true
    success "Language $language added"
  fi

  step "Importing base content..."
  DRUSH_URI="$DRUSH_URI" $SCRIPT_DIR/import.sh >/dev/null 2>&1
  success "Groups, categories, and terms created"

  step "Fetching city boundary..."
  boundary_output=$(drush $DRUSH_URI markaspot:fetch-boundary --city="$city" --group=1 -y 2>&1)
  boundary_exit=$?
  if [ $boundary_exit -eq 0 ]; then
    success "City boundary stored"
  else
    warn "Could not fetch boundary for '$city'"
    echo -e "${RED}$boundary_output${NC}"
    echo ""
    info "This may cause frontend errors. Try manually:"
    info "  ddev drush markaspot:fetch-boundary --city=\"$city, Germany\" --group=1 -y"
  fi

  step "Configuring validation settings..."

  # Create a bounding box WKT polygon (~15km radius around center)
  RADIUS_DEG="0.15"
  MIN_LAT=$(awk "BEGIN {printf \"%.6f\", $latitude - $RADIUS_DEG}")
  MAX_LAT=$(awk "BEGIN {printf \"%.6f\", $latitude + $RADIUS_DEG}")
  MIN_LNG=$(awk "BEGIN {printf \"%.6f\", $longitude - $RADIUS_DEG}")
  MAX_LNG=$(awk "BEGIN {printf \"%.6f\", $longitude + $RADIUS_DEG}")

  WKT="POLYGON(($MIN_LNG $MIN_LAT,$MAX_LNG $MIN_LAT,$MAX_LNG $MAX_LAT,$MIN_LNG $MAX_LAT,$MIN_LNG $MIN_LAT))"

  # Extract simple city name (first part before comma)
  SIMPLE_CITY=$(echo "$city" | cut -d',' -f1)

  drush $DRUSH_URI config:set markaspot_validation.settings wkt "$WKT" -y >/dev/null 2>&1
  drush $DRUSH_URI config:set markaspot_validation.settings location.0 "$SIMPLE_CITY" -y >/dev/null 2>&1
  drush $DRUSH_URI config:set markaspot_validation.settings locality.0 "$SIMPLE_CITY" -y >/dev/null 2>&1
  success "Validation configured for $SIMPLE_CITY"

  step "Configuring GeoReport API..."
  drush $DRUSH_URI config:delete markaspot_open311.settings status_closed.3 -y >/dev/null 2>&1 || true
  drush $DRUSH_URI config:delete markaspot_open311.settings status_closed.4 -y >/dev/null 2>&1 || true
  drush $DRUSH_URI config:set markaspot_open311.settings status_closed.5 5 -y >/dev/null 2>&1
  drush $DRUSH_URI config:set markaspot_open311.settings status_closed.6 6 -y >/dev/null 2>&1

  drush $DRUSH_URI php:eval '
    $config = \Drupal::service("config.factory")->getEditable("group.role.org-anonymous");
    $perms = $config->get("permissions") ?: [];
    if (!in_array("view group_node:service_request entity", $perms)) {
      $perms[] = "view group_node:service_request entity";
      $config->set("permissions", $perms)->save();
    }
  ' 2>/dev/null || true
  success "GeoReport API configured"

  # Handle translations - English is always base, other languages are translations
  if [ "$translation" = true ] || [ "$ai_translate" = true ]; then
    step "Adding language: $language..."
    drush $DRUSH_URI language-add "$language" >/dev/null 2>&1 || true

    step "Enabling multilingual support..."
    drush $DRUSH_URI en markaspot_language -y >/dev/null 2>&1

    step "Importing Drupal translations..."
    DRUSH_URI="$DRUSH_URI" $SCRIPT_DIR/translate.sh "$locale" >/dev/null 2>&1
    success "Translations imported"

    if [ "$ai_translate" = true ]; then
      if [ -z "$OPENAI_API_KEY" ]; then
        warn "OPENAI_API_KEY not set"
        printf "  Enter OpenAI API key: "
        read api_key
        export OPENAI_API_KEY=$api_key
      fi

      step "Running AI translation..."
      chmod +x "$SCRIPT_DIR/ai-translate.sh"
      if sh "$SCRIPT_DIR/ai-translate.sh" $language >/dev/null 2>&1; then
        success "AI translation complete"
      else
        warn "AI translation failed"
      fi

      if [ -f "$SCRIPT_DIR/create-translations.php" ]; then
        step "Creating entity translations..."
        drush $DRUSH_URI php:script "$SCRIPT_DIR/create-translations.php" -- "$language" >/dev/null 2>&1 || warn "Could not create translations"
      fi

      ARTIFACTS_DIR="$PWD/web/profiles/contrib/markaspot/modules/markaspot_default_content/artifacts"
      LANG_DIR="$ARTIFACTS_DIR/$language"
      if [ -d "$LANG_DIR" ]; then
        rm -rf "$LANG_DIR"
      fi
    fi
  else
    info "Hint: Use -t (translations) or -a (AI translation) for multilingual"
  fi

  step "Configuring API key..."
  ENV_GEOREPORT_API_KEY=${GEOREPORT_API_KEY:-}

  if [ -n "$ENV_GEOREPORT_API_KEY" ] && [ "$ENV_GEOREPORT_API_KEY" != "*" ]; then
    GEOREPORT_API_KEY="$ENV_GEOREPORT_API_KEY"
  else
    GEOREPORT_API_KEY=$(php -r 'echo bin2hex(random_bytes(16));')
  fi

  drush $DRUSH_URI config-set services_api_key_auth.api_key.nuxt key "$GEOREPORT_API_KEY" -y >/dev/null 2>&1

  DDEV_ENV_FILE="$PROJECT_ROOT/.ddev/.env"
  if [ -d "$PROJECT_ROOT/.ddev" ]; then
    if [ -f "$DDEV_ENV_FILE" ]; then
      grep -v "^GEOREPORT_API_KEY=" "$DDEV_ENV_FILE" > "${DDEV_ENV_FILE}.tmp" 2>/dev/null || true
      mv "${DDEV_ENV_FILE}.tmp" "$DDEV_ENV_FILE"
    fi
    echo "GEOREPORT_API_KEY=$GEOREPORT_API_KEY" >> "$DDEV_ENV_FILE"
  fi

  export GEOREPORT_API_KEY
  success "API key configured"

  step "Generating test session cookie..."
  if [ -f "$SCRIPT_DIR/get-drupal-session.sh" ]; then
    chmod +x "$SCRIPT_DIR/get-drupal-session.sh"
    SESSION_COOKIE=$("$SCRIPT_DIR/get-drupal-session.sh" 1 cookie 2>/dev/null) || true
    if [ -n "$SESSION_COOKIE" ] && [ -d "$PROJECT_ROOT/.ddev" ]; then
      # Remove old session cookie line if exists
      if [ -f "$DDEV_ENV_FILE" ]; then
        grep -v "^DRUPAL_TEST_SESSION_COOKIE=" "$DDEV_ENV_FILE" > "${DDEV_ENV_FILE}.tmp" 2>/dev/null || true
        mv "${DDEV_ENV_FILE}.tmp" "$DDEV_ENV_FILE"
      fi
      echo "DRUPAL_TEST_SESSION_COOKIE=$SESSION_COOKIE" >> "$DDEV_ENV_FILE"
      success "Session cookie configured"
    else
      warn "Could not generate session cookie"
    fi
  fi

  step "Creating test data..."
  DRUSH_URI="$DRUSH_URI" SITE_URI="$SITE_URI" $SCRIPT_DIR/georeport-client.sh >/dev/null 2>&1
  success "Test users and service requests created"

  step "Configuring groups..."

  # Get actually enabled languages from Drupal
  ENABLED_LANGS=$(drush $DRUSH_URI php:eval "
    \$languages = \Drupal::languageManager()->getLanguages();
    echo implode(',', array_keys(\$languages));
  " 2>/dev/null)

  if [ -n "$ENABLED_LANGS" ]; then
    # Convert comma-separated list to JSON array
    NUXT_AVAILABLE_LANGS=$(echo "$ENABLED_LANGS" | awk -F',' '{
      printf "[";
      for(i=1; i<=NF; i++) {
        printf "\"%s\"", $i;
        if(i<NF) printf ", ";
      }
      printf "]"
    }')
    # Use site default language
    NUXT_DEFAULT_LANG=$(drush $DRUSH_URI php:eval "echo \Drupal::languageManager()->getDefaultLanguage()->getId();" 2>/dev/null)
    [ -z "$NUXT_DEFAULT_LANG" ] && NUXT_DEFAULT_LANG="en"
  else
    # Fallback to original logic
    if [ "$translation" = true ] || [ "$ai_translate" = true ]; then
      NUXT_DEFAULT_LANG="$language"
      # Avoid duplicating "en" if language is already English
      if [ "$language" = "en" ]; then
        NUXT_AVAILABLE_LANGS="[\"en\"]"
      else
        NUXT_AVAILABLE_LANGS="[\"en\", \"$language\"]"
      fi
    else
      NUXT_DEFAULT_LANG="en"
      NUXT_AVAILABLE_LANGS="[\"en\"]"
    fi
  fi

  drush $DRUSH_URI php:eval "
    \$group = \Drupal::entityTypeManager()->getStorage('group')->load(1);
    if (\$group && \$group->getGroupType()->id() === 'jur') {
      \$group->set('label', '$city');
      \$existing_config = [];
      if (\$group->hasField('field_nuxt_config') && !\$group->get('field_nuxt_config')->isEmpty()) {
        \$existing_json = \$group->get('field_nuxt_config')->value;
        \$existing_config = json_decode(\$existing_json, true) ?: [];
      }
      \$existing_config['languages'] = [
        'default' => '$NUXT_DEFAULT_LANG',
        'available' => json_decode('$NUXT_AVAILABLE_LANGS', true)
      ];
      \$existing_config['client'] = \$existing_config['client'] ?? [];
      \$existing_config['client']['name'] = '$city';
      \$group->set('field_nuxt_config', json_encode(\$existing_config, JSON_PRETTY_PRINT | JSON_UNESCAPED_UNICODE));
      \$group->save();
    }
  " 2>/dev/null || true

  drush $DRUSH_URI php:eval "
    \$user = \Drupal\user\Entity\User::load(1);
    \$groups = \Drupal::entityTypeManager()->getStorage('group')->loadMultiple();
    foreach (\$groups as \$group) {
      if (!\$group->getMember(\$user)) {
        \$group->addMember(\$user);
      }
    }
  " 2>/dev/null || true

  drush $DRUSH_URI php:eval "
    \$group_storage = \Drupal::entityTypeManager()->getStorage('group');
    \$user_storage = \Drupal::entityTypeManager()->getStorage('user');
    \$jur = \$group_storage->load(1);
    \$dept1 = \$group_storage->load(2);
    \$dept2 = \$group_storage->load(3);
    foreach (['api_user', 'moderation_1', 'moderation_2'] as \$name) {
      \$users = \$user_storage->loadByProperties(['name' => \$name]);
      \$user = reset(\$users);
      if (\$user && \$jur && !\$jur->getMember(\$user)) {
        \$jur->addMember(\$user);
      }
    }
    \$mod1 = \$user_storage->loadByProperties(['name' => 'moderation_1']);
    \$mod1 = reset(\$mod1);
    if (\$mod1 && \$dept1 && !\$dept1->getMember(\$mod1)) {
      \$dept1->addMember(\$mod1);
    }
    \$mod2 = \$user_storage->loadByProperties(['name' => 'moderation_2']);
    \$mod2 = reset(\$mod2);
    if (\$mod2 && \$dept2 && !\$dept2->getMember(\$mod2)) {
      \$dept2->addMember(\$mod2);
    }
  " 2>/dev/null || true

  success "Groups and memberships configured"

  # =============================================================================
  # Installation Summary
  # =============================================================================
  printf "\n"
  success "Mark-a-Spot Installation Complete!"
  printf "\n"
  printf "  City:      %s\n" "$city"
  printf "  Locale:    %s\n" "$locale"
  printf "  API Key:   %s\n" "$GEOREPORT_API_KEY"
  printf "  Users:     admin, api_user, moderation_1, moderation_2\n"
  printf "  Data:      50 test service requests\n"

  printf "\n"
  step "One-Time Login:"
  if [ -n "$SITE_URI" ]; then
    printf "  %s\n" "$(drush $DRUSH_URI uli --uri="https://$SITE_URI" 2>/dev/null)"
  elif [ -n "$DDEV_HOSTNAME" ]; then
    printf "  %s\n" "$(drush uli --uri="https://$DDEV_HOSTNAME" 2>/dev/null)"
  else
    printf "  %s\n" "$(drush uli --uri=http://localhost 2>/dev/null)"
  fi

  printf "\n"
  step "Next Steps:"
  printf "  1. Run 'ddev restart' to apply API key to frontend\n"
  if [ -n "$SITE_URI" ]; then
    printf "  2. Access site: https://%s\n\n" "$SITE_URI"
  elif [ -n "$DDEV_HOSTNAME" ]; then
    printf "  2. Access frontend: https://%s:8040\n\n" "$DDEV_HOSTNAME"
  else
    printf "  2. Access frontend: http://localhost:3000\n\n"
  fi
fi
