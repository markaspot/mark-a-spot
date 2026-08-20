#!/bin/sh
set -eu

REAL_IP_INCLUDE=/tmp/markaspot-trusted-real-ip.conf
GEO_INCLUDE=/tmp/markaspot-trusted-proxy-geo.conf
trusted_proxy_cidrs="${MARKASPOT_TRUSTED_PROXY_CIDRS:-}"

: > "$REAL_IP_INCLUDE"
: > "$GEO_INCLUDE"

if [ -n "$trusted_proxy_cidrs" ]; then
  compact="$(printf '%s' "$trusted_proxy_cidrs" | tr -d ' ')"
  case "$compact" in
    *[!0-9.,/]*)
      echo "ERROR: MARKASPOT_TRUSTED_PROXY_CIDRS must be a comma-separated IPv4 CIDR list" >&2
      exit 1
      ;;
  esac

  printf '%s\n' "$compact" | tr ',' '\n' | while IFS= read -r cidr; do
    if ! printf '%s\n' "$cidr" | grep -Eq '^([0-9]{1,3}\.){3}[0-9]{1,3}(/([0-9]|[12][0-9]|3[0-2]))?$'; then
      echo "ERROR: Invalid trusted proxy IPv4 CIDR" >&2
      exit 1
    fi
    printf 'set_real_ip_from %s;\n' "$cidr" >> "$REAL_IP_INCLUDE"
    printf '%s 1;\n' "$cidr" >> "$GEO_INCLUDE"
  done
fi

exec "$@"
