#!/bin/bash
#
# Fetch city boundaries from OpenStreetMap Nominatim
# Usage: ./scripts/fetch-boundaries.sh [city_name]
#
# If no city name provided, reads from Drupal's markaspot_validation.settings
#

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

# Output directory
OUTPUT_DIR="frontend/public/data/boundaries"
OUTPUT_FILE="$OUTPUT_DIR/default.json"

# Get city name from argument or Drupal config
if [ -n "$1" ]; then
    CITY="$1"
    echo -e "${CYAN}Using city from argument: ${CITY}${NC}"
else
    echo -e "${CYAN}Fetching city from Drupal config...${NC}"
    # Try to get city from Drupal validation settings
    if command -v ddev &> /dev/null; then
        CITY=$(ddev drush cget markaspot_validation.settings location.0 --format=string 2>/dev/null || echo "")
    elif command -v drush &> /dev/null; then
        CITY=$(drush cget markaspot_validation.settings location.0 --format=string 2>/dev/null || echo "")
    fi

    if [ -z "$CITY" ]; then
        echo -e "${RED}Error: Could not determine city name.${NC}"
        echo "Usage: $0 [city_name]"
        echo "Example: $0 'New York City'"
        exit 1
    fi
    echo -e "${CYAN}Found city in config: ${CITY}${NC}"
fi

# URL encode the city name
CITY_ENCODED=$(printf '%s' "$CITY" | jq -sRr @uri)

echo -e "${CYAN}Fetching boundary from Nominatim for: ${CITY}${NC}"

# Fetch from Nominatim with polygon_geojson
NOMINATIM_URL="https://nominatim.openstreetmap.org/search?q=${CITY_ENCODED}&format=geojson&polygon_geojson=1&limit=1"

# Make the request (with User-Agent as required by Nominatim)
RESPONSE=$(curl -s -A "Mark-a-Spot/1.0 (https://markaspot.de)" "$NOMINATIM_URL")

# Check if we got a valid response
FEATURE_COUNT=$(echo "$RESPONSE" | jq '.features | length')

if [ "$FEATURE_COUNT" -eq 0 ]; then
    echo -e "${RED}Error: No results found for '${CITY}'${NC}"
    echo "Try a more specific search term, e.g., 'New York City, USA'"
    exit 1
fi

# Extract the first feature
GEOMETRY_TYPE=$(echo "$RESPONSE" | jq -r '.features[0].geometry.type')
DISPLAY_NAME=$(echo "$RESPONSE" | jq -r '.features[0].properties.display_name')

echo -e "${GREEN}Found: ${DISPLAY_NAME}${NC}"
echo -e "${CYAN}Geometry type: ${GEOMETRY_TYPE}${NC}"

# Check if geometry is a polygon (not just a point)
if [ "$GEOMETRY_TYPE" = "Point" ]; then
    echo -e "${YELLOW}Warning: Only point geometry returned. Try a more specific admin boundary search.${NC}"
    echo "Tip: Search for 'City of X' or add country, e.g., '${CITY}, Germany'"

    # For points, we can create a simple bounding box from the bbox
    BBOX=$(echo "$RESPONSE" | jq -r '.features[0].bbox // empty')
    if [ -n "$BBOX" ]; then
        echo -e "${CYAN}Creating bounding box polygon from bbox...${NC}"

        # Extract bbox coordinates [minLon, minLat, maxLon, maxLat]
        MIN_LON=$(echo "$RESPONSE" | jq -r '.features[0].bbox[0]')
        MIN_LAT=$(echo "$RESPONSE" | jq -r '.features[0].bbox[1]')
        MAX_LON=$(echo "$RESPONSE" | jq -r '.features[0].bbox[2]')
        MAX_LAT=$(echo "$RESPONSE" | jq -r '.features[0].bbox[3]')

        # Create a simple polygon from bbox
        GEOJSON=$(cat <<EOF
{
  "type": "FeatureCollection",
  "features": [
    {
      "type": "Feature",
      "id": 0,
      "properties": {
        "NAME_LOCAL": "${CITY}",
        "NAME_ENGLI": "${CITY}",
        "display_name": "${DISPLAY_NAME}"
      },
      "geometry": {
        "type": "Polygon",
        "coordinates": [
          [
            [${MIN_LON}, ${MIN_LAT}],
            [${MAX_LON}, ${MIN_LAT}],
            [${MAX_LON}, ${MAX_LAT}],
            [${MIN_LON}, ${MAX_LAT}],
            [${MIN_LON}, ${MIN_LAT}]
          ]
        ]
      }
    }
  ]
}
EOF
)
    else
        echo -e "${RED}Error: No bbox available for point geometry${NC}"
        exit 1
    fi
else
    # Transform Nominatim response to our expected format
    GEOJSON=$(echo "$RESPONSE" | jq '{
        type: "FeatureCollection",
        features: [
            {
                type: "Feature",
                id: 0,
                properties: {
                    NAME_LOCAL: .features[0].properties.name,
                    NAME_ENGLI: .features[0].properties.name,
                    display_name: .features[0].properties.display_name,
                    osm_id: .features[0].properties.osm_id,
                    osm_type: .features[0].properties.osm_type
                },
                geometry: .features[0].geometry
            }
        ]
    }')
fi

# Ensure output directory exists
mkdir -p "$OUTPUT_DIR"

# Write the file
echo "$GEOJSON" | jq '.' > "$OUTPUT_FILE"

echo -e "${GREEN}Boundary saved to: ${OUTPUT_FILE}${NC}"

# Show summary
COORD_COUNT=$(echo "$GEOJSON" | jq '.features[0].geometry.coordinates | if type == "array" then (if .[0][0] | type == "array" then .[0] | length else length end) else 0 end')
echo -e "${CYAN}Polygon has ${COORD_COUNT} coordinate points${NC}"

# Optionally show the bounding box
if [ "$GEOMETRY_TYPE" != "Point" ]; then
    BBOX=$(echo "$RESPONSE" | jq -r '.features[0].bbox | "\(.[0]),\(.[1]) to \(.[2]),\(.[3])"')
    echo -e "${CYAN}Bounding box: ${BBOX}${NC}"
fi

echo -e "${GREEN}Done! Restart your frontend to use the new boundaries.${NC}"
