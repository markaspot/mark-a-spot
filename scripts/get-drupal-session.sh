#!/bin/bash
#
# Generate a Drupal session cookie for testing authenticated API requests.
#
# Usage:
#   ./scripts/get-drupal-session.sh [USER_ID] [OUTPUT_FORMAT]
#
# Arguments:
#   USER_ID       - Drupal user ID (default: 1 = admin)
#   OUTPUT_FORMAT - "cookie" for just the cookie, "env" for export statement (default: cookie)
#
# Examples:
#   ./scripts/get-drupal-session.sh 1 cookie      # Just the session cookie
#   ./scripts/get-drupal-session.sh 1 env         # Export statement for .env
#   eval $(./scripts/get-drupal-session.sh 1 env) # Set in current shell
#

set -e

USER_ID="${1:-1}"
OUTPUT_FORMAT="${2:-cookie}"
BASE_URL="${DDEV_PRIMARY_URL:-https://${DDEV_HOSTNAME:-localhost}}"

# Get one-time login URL for the user
OTL=$(drush uli --uid="$USER_ID" --uri="$BASE_URL" 2>/dev/null)

if [ -z "$OTL" ]; then
    echo "Error: Could not generate login URL for user $USER_ID" >&2
    exit 1
fi

# Follow the one-time login and capture the session cookie
# The OTL redirects and sets a session cookie
COOKIE_JAR=$(mktemp)
trap "rm -f $COOKIE_JAR" EXIT

# Follow redirects and capture cookies
curl -s -L -c "$COOKIE_JAR" -o /dev/null "$OTL" 2>/dev/null

# Extract the session cookie (SESSxxxxxxxx=value format)
SESSION_COOKIE=$(grep -E "SESS[a-f0-9]+" "$COOKIE_JAR" | awk '{print $6"="$7}' | head -1)

if [ -z "$SESSION_COOKIE" ]; then
    echo "Error: Could not extract session cookie" >&2
    exit 1
fi

case "$OUTPUT_FORMAT" in
    env)
        echo "export DRUPAL_TEST_SESSION_COOKIE=\"$SESSION_COOKIE\""
        ;;
    cookie|*)
        echo "$SESSION_COOKIE"
        ;;
esac
