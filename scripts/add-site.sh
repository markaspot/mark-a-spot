#!/bin/bash
#
# Add a new site to Mark-a-Spot Multisite
# Fetches coordinates from Nominatim and updates all configuration files
#
# Usage: add-site.sh <sitename> <city> <country> [locale]
# Example: add-site.sh muenster "Münster" Germany de_DE
#

set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
NC='\033[0m'

usage() {
  echo "Usage: add-site.sh <sitename> <city> <country> [locale]"
  echo ""
  echo "Arguments:"
  echo "    sitename    Site identifier (lowercase, no spaces, ASCII only)"
  echo "    city        City name (can include umlauts)"
  echo "    country     Country name"
  echo "    locale      Locale code (default: de_DE)"
  echo ""
  echo "Examples:"
  echo "    add-site.sh muenster \"Münster\" Germany de_DE"
  echo "    add-site.sh maastricht Maastricht Netherlands nl_NL"
  echo "    add-site.sh liege \"Liège\" Belgium fr_BE"
  exit 1
}

# Check arguments
if [ -z "$1" ] || [ -z "$2" ] || [ -z "$3" ]; then
  usage
fi

SITE_NAME="$1"
CITY="$2"
COUNTRY="$3"
LOCALE="${4:-de_DE}"

# Validate sitename (lowercase, no spaces, ASCII only)
if ! echo "$SITE_NAME" | grep -qE '^[a-z][a-z0-9_-]*$'; then
  echo -e "${RED}ERROR: Site name must be lowercase ASCII, start with a letter${NC}"
  echo "       Got: $SITE_NAME"
  echo "       Example: muenster (not Münster)"
  exit 1
fi

printf "${CYAN}Adding site: %s (%s, %s)${NC}\n" "$SITE_NAME" "$CITY" "$COUNTRY"

# Fetch coordinates from Nominatim
printf "${CYAN}Fetching coordinates from Nominatim...${NC}\n"

CITY_ENCODED=$(php -r 'echo rawurlencode($argv[1]);' "$CITY")
COUNTRY_ENCODED=$(php -r 'echo rawurlencode($argv[1]);' "$COUNTRY")

NOMINATIM_URL="https://nominatim.openstreetmap.org/search?city=$CITY_ENCODED&country=$COUNTRY_ENCODED&format=json&limit=1"

RESPONSE=$(curl -s -A "Mark-a-Spot-Multisite/1.0" "$NOMINATIM_URL")

if [ -z "$RESPONSE" ] || [ "$RESPONSE" = "[]" ]; then
  echo -e "${RED}ERROR: No results from Nominatim for $CITY, $COUNTRY${NC}"
  exit 1
fi

LAT=$(echo "$RESPONSE" | php -r '$d=json_decode(file_get_contents("php://stdin"),true); echo $d[0]["lat"] ?? "";')
LNG=$(echo "$RESPONSE" | php -r '$d=json_decode(file_get_contents("php://stdin"),true); echo $d[0]["lon"] ?? "";')
DISPLAY_NAME=$(echo "$RESPONSE" | php -r '$d=json_decode(file_get_contents("php://stdin"),true); echo $d[0]["display_name"] ?? "";')

if [ -z "$LAT" ] || [ -z "$LNG" ]; then
  echo -e "${RED}ERROR: Could not parse coordinates from Nominatim${NC}"
  exit 1
fi

printf "${GREEN}Found: %s${NC}\n" "$DISPLAY_NAME"
printf "  Latitude:  %s\n" "$LAT"
printf "  Longitude: %s\n" "$LNG"

# Check if site already exists
if [ -d "$PROJECT_ROOT/web/sites/$SITE_NAME" ]; then
  echo -e "${YELLOW}WARNING: Site directory already exists: web/sites/$SITE_NAME${NC}"
fi

if grep -q "^  $SITE_NAME:" "$PROJECT_ROOT/config/sites.yaml" 2>/dev/null; then
  echo -e "${YELLOW}WARNING: Site already in sites.yaml${NC}"
fi

# Add to sites.yaml
printf "${CYAN}Adding to config/sites.yaml...${NC}\n"

cat >> "$PROJECT_ROOT/config/sites.yaml" << EOF

  $SITE_NAME:
    city: $CITY
    country: $COUNTRY
    locale: $LOCALE
    coordinates:
      lat: $LAT
      lng: $LNG
    theme:
      primary: blue
      secondary: sky
      neutral: slate
    features:
      voting: false
      statistics: true
      photoReporting: true
      feedback: true
EOF

printf "${GREEN}✓ Added to sites.yaml${NC}\n"

# Add to DDEV config
DDEV_CONFIG="$PROJECT_ROOT/.ddev/config.yaml"
if [ -f "$DDEV_CONFIG" ]; then
  printf "${CYAN}Adding to .ddev/config.yaml...${NC}\n"

  # Check if already in additional_hostnames
  if ! grep -q "^\s*- $SITE_NAME\$" "$DDEV_CONFIG"; then
    # Add to additional_hostnames
    sed -i.bak "/^additional_hostnames:/a\\
  - $SITE_NAME" "$DDEV_CONFIG"
    rm -f "$DDEV_CONFIG.bak"
    printf "${GREEN}✓ Added to additional_hostnames${NC}\n"
  else
    printf "${YELLOW}Already in additional_hostnames${NC}\n"
  fi

  # Check if already in additional_databases
  if ! grep -qE "^\s*-\s*(name:\s*)?$SITE_NAME\$" "$DDEV_CONFIG"; then
    # Add to additional_databases
    sed -i.bak "/^additional_databases:/a\\
  - $SITE_NAME" "$DDEV_CONFIG"
    rm -f "$DDEV_CONFIG.bak"
    printf "${GREEN}✓ Added to additional_databases${NC}\n"
  else
    printf "${YELLOW}Already in additional_databases${NC}\n"
  fi
fi

# Create site directory
SITE_DIR="$PROJECT_ROOT/web/sites/$SITE_NAME"
if [ ! -d "$SITE_DIR" ]; then
  printf "${CYAN}Creating site directory...${NC}\n"
  mkdir -p "$SITE_DIR/files"
  chmod 775 "$SITE_DIR/files"

  # Create settings.php
  cat > "$SITE_DIR/settings.php" << SETTINGS
<?php
/**
 * @file
 * Drupal settings for the $CITY site.
 */

\$settings['hash_salt'] = '$SITE_NAME-multisite-hash-salt-change-in-production';
\$settings['site_identifier'] = '$SITE_NAME';
\$settings['config_sync_directory'] = '../config/$SITE_NAME';
\$settings['file_public_path'] = 'sites/$SITE_NAME/files';
\$settings['file_private_path'] = '../private/$SITE_NAME';

\$settings['trusted_host_patterns'] = [
  '^$SITE_NAME\.ddev\.site\$',
  '^.+\.platformsh\.site\$',
  '^.+\.upsun\.app\$',
  '^$SITE_NAME\.markaspot\.cloud\$',
];

// DDEV database (default)
\$databases['default']['default'] = [
  'database' => '$SITE_NAME',
  'username' => 'db',
  'password' => 'db',
  'host' => 'db',
  'port' => '3306',
  'driver' => 'mysql',
  'prefix' => '',
];

// Upsun/Platform.sh override
if (getenv('PLATFORM_RELATIONSHIPS')) {
  \$relationships = json_decode(base64_decode(getenv('PLATFORM_RELATIONSHIPS')), TRUE);
  if (isset(\$relationships['${SITE_NAME}_db'][0])) {
    \$db = \$relationships['${SITE_NAME}_db'][0];
    \$databases['default']['default'] = [
      'database' => \$db['path'],
      'username' => \$db['username'],
      'password' => \$db['password'],
      'host' => \$db['host'],
      'port' => \$db['port'],
      'driver' => 'mysql',
      'prefix' => '',
    ];
  }
}

if (isset(\$app_root) && isset(\$site_path) && file_exists(\$app_root . '/' . \$site_path . '/settings.local.php')) {
  include \$app_root . '/' . \$site_path . '/settings.local.php';
}
SETTINGS

  chmod 644 "$SITE_DIR/settings.php"
  printf "${GREEN}✓ Created site directory and settings.php${NC}\n"
else
  printf "${YELLOW}Site directory already exists${NC}\n"
fi

# Create config directory
CONFIG_DIR="$PROJECT_ROOT/config/$SITE_NAME"
if [ ! -d "$CONFIG_DIR" ]; then
  mkdir -p "$CONFIG_DIR"
  printf "${GREEN}✓ Created config directory${NC}\n"
fi

# Create private directory
PRIVATE_DIR="$PROJECT_ROOT/private/$SITE_NAME"
if [ ! -d "$PRIVATE_DIR" ]; then
  mkdir -p "$PRIVATE_DIR"
  printf "${GREEN}✓ Created private directory${NC}\n"
fi

echo ""
printf "${GREEN}╔════════════════════════════════════════════════════════════════╗${NC}\n"
printf "${GREEN}║${NC} Site '%s' added successfully!                              ${GREEN}║${NC}\n" "$SITE_NAME"
printf "${GREEN}╠════════════════════════════════════════════════════════════════╣${NC}\n"
printf "${GREEN}║${NC} City: %-55s ${GREEN}║${NC}\n" "$CITY"
printf "${GREEN}║${NC} Coordinates: %s, %s                          ${GREEN}║${NC}\n" "$LAT" "$LNG"
printf "${GREEN}╚════════════════════════════════════════════════════════════════╝${NC}\n"
echo ""
printf "${YELLOW}Next steps:${NC}\n"
printf "  1. Run: ddev restart\n"
printf "  2. Run: ddev exec \"CITY='%s' COUNTRY='%s' scripts/start.sh --site=%s -y\"\n" "$CITY" "$COUNTRY" "$SITE_NAME"
echo ""
