#!/bin/sh

./vendor/bin/drush user-create "api_user" --password="my_password" && drush user-add-role "api_user" "api_user"
UUID=$(./vendor/bin/drush user-information api_user --fields=uuid --format=list)
UUID=$(./vendor/bin/drush sql:query "SELECT uuid FROM users WHERE uid = 2" --database=default)

./vendor/bin/drush config-set services_api_key_auth.api_key.test_mas user_uuid $UUID -y

# Get the API key from the configuration
API_KEY=$(./vendor/bin/drush config-get services_api_key_auth.api_key.test_mas | awk '/key:/ {print $2}')

# Set the center latitude and longitude
CENTER_LAT=$(./vendor/bin/drush cget markaspot_map.settings center_lat --format=string)
CENTER_LNG=$(./vendor/bin/drush cget markaspot_map.settings center_lng --format=string)

# Set the radius in kilometers
RADIUS=15

# Calculate the radius in degrees using the approximation that 1 degree is equal to 111.32 kilometers
RADIUS_IN_DEGREES=$(awk "BEGIN {print ($RADIUS / 111.32)}")

# Retrieve the services list from the server
services_json=$(curl -s -w '\n%{http_code}\n' http://$VIRTUAL_HOST/georeport/v2/services.json)
# Check for errors in the response
response_code=$(echo "$services_json" | tail -n 1)
if [ "$response_code" != "200" ]; then
  echo "Error: Failed to retrieve service codes (HTTP $response_code)"
  exit 1
fi

# Extract the service codes from the JSON response and store them in an array
SERVICES=$(echo "$services_json" | head -n -1 | grep -o '"service_code":"[^"]*"' | awk -F':' '{print $2}' | tr -d '"')

echo "-----------------------------------------------------------------------------------------------------------------e-"
printf "%-10s %-30s %-15s %-15s %-12s %-15s %-8s\n" "Request #" "Email" "Latitude" "Longitude" "Request Time" "Response Code" "Service Code"
echo "------------------------------------------------------------------------------------------------------------------"


for i in $(seq 1 50); do
  # Generate a random angle in radians between 0 and 2*pi
  RANDOM_ANGLE=$(awk -v seed="$RANDOM$((i * 10))" 'BEGIN {srand(seed); print rand() * 2 * 3.141592653589793;}')

  # Generate a random radius within the circle
  RANDOM_RADIUS=$(awk -v seed="$RANDOM$((i * 10))" -v max="$RADIUS_IN_DEGREES" 'BEGIN {srand(seed); print sqrt(rand()) * max;}')

  # Calculate the latitude and longitude offsets
  LATITUDE_OFFSET=$(awk -v radius="$RANDOM_RADIUS" -v angle="$RANDOM_ANGLE" 'BEGIN {print radius * sin(angle);}')
  LONGITUDE_OFFSET=$(awk -v radius="$RANDOM_RADIUS" -v angle="$RANDOM_ANGLE" 'BEGIN {print radius * cos(angle);}')

  # Calculate the actual latitude and longitude
  LATITUDE=$(awk -v center_lat="$CENTER_LAT" -v offset="$LATITUDE_OFFSET" 'BEGIN {print center_lat + offset;}')
  LONGITUDE=$(awk -v center_lng="$CENTER_LNG" -v offset="$LONGITUDE_OFFSET" 'BEGIN {print center_lng + offset;}')

  RANDOM_SERVICE_CODE=$(printf "%s\n" "$SERVICES" | awk 'BEGIN {srand();}{a[NR]=$0}END{print a[int(rand()*NR)+1]}')
  EMAIL="test_$(head /dev/urandom | tr -dc A-Za-z0-9 | head -c10)@example.com"
  # Set the description as a multi-line string
  DESCRIPTION="Duris sanctius sic erectos cepit vos erat quin. Fuerat arce pontus sine nisi melioris. \
  Haec inposuit pendebat sibi septemque caesa pluvialibus. Feras effigiem aurea animalibus. Vesper ante \
  quod frigore animal! Caecoque lucis terrae his utque. Quarum foret suis praeter videre crescendo obsistitur."

  # Generate a random number from 1 to 6
  RANDOM_NUMBER=$(shuf -i 1-6 -n 1)

  # If RANDOM_NUMBER is empty, set it to a default value
  if [ -z "$RANDOM_NUMBER" ]; then
    RANDOM_NUMBER=1
  fi

  # Set the media URL with the random number
  MEDIA_URL="https://markaspot.de/demo-images/image_${RANDOM_NUMBER}.jpg"

  REQUEST_START=$(date +%s.%N)
  RESPONSE=$(curl -s --location 'http://web/georeport/v2/requests.json?api_key='"$API_KEY"'' \
    --header 'Content-Type: application/x-www-form-urlencoded' \
    --data-urlencode 'service_code='"$RANDOM_SERVICE_CODE"'' \
    --data-urlencode 'description='"$DESCRIPTION"'' \
    --data-urlencode 'email='"$EMAIL"'' \
    --data-urlencode 'lat='"$LATITUDE"'' \
    --data-urlencode 'long='"$LONGITUDE"'' \
    --data-urlencode 'media_url='"$MEDIA_URL"'' \
    --write-out "%{http_code}" \
    --output /dev/null)

  REQUEST_END=$(date +%s.%N)
  REQUEST_TIME=$(awk "BEGIN {print ($REQUEST_END - $REQUEST_START)}")
  printf "%-10s %-30s %-15s %-15s %-12s %-15s %-8s\n" "$i" "$EMAIL" "$LATITUDE" "$LONGITUDE" "$REQUEST_TIME" "$RESPONSE" "$RANDOM_SERVICE_CODE"
done

echo "------------------------------------------------------------------------------------------------------------------"
