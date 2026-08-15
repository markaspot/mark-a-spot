#!/bin/sh
set -eu

APP_USER=app
APP_GROUP=app
PUBLIC_FILES_DIR=/app/data/web/sites/default/files
PRIVATE_FILES_DIR=/app/data/private
DRUPAL_TMP_DIR=/tmp/drupal
SERVICES_FILE=/app/data/web/sites/default/runtime.services.yml

run_as_app() {
  if [ "$(id -u)" = "0" ]; then
    su-exec "$APP_USER" "$@"
  else
    "$@"
  fi
}

ensure_writable_dir() {
  directory="$1"
  mkdir -p "$directory"
  if [ "$(id -u)" = "0" ]; then
    chown "$APP_USER:$APP_GROUP" "$directory"
    chmod ug+rwx "$directory"
  fi
}

reject_multiline() {
  name="$1"
  value="$2"
  carriage_return="$(printf '\r')"
  case "$value" in
    *'
'*|*"$carriage_return"*)
      echo "ERROR: $name must not contain line breaks" >&2
      exit 1
      ;;
  esac
}

ensure_writable_dir "$PUBLIC_FILES_DIR"
ensure_writable_dir "$PUBLIC_FILES_DIR/styles"
ensure_writable_dir "$PRIVATE_FILES_DIR"
ensure_writable_dir "$DRUPAL_TMP_DIR"

cookie_domain="${COOKIE_DOMAIN:-}"
smtp_host="${SMTP_HOST:-}"
smtp_port="${SMTP_PORT:-}"
smtp_from="${SMTP_FROM:-}"
smtp_user="${SMTP_USER:-}"
smtp_password="${SMTP_PASSWORD:-}"
hash_salt="${DRUPAL_HASH_SALT:-}"

reject_multiline COOKIE_DOMAIN "$cookie_domain"
reject_multiline SMTP_HOST "$smtp_host"
reject_multiline SMTP_PORT "$smtp_port"
reject_multiline SMTP_FROM "$smtp_from"
reject_multiline SMTP_USER "$smtp_user"
reject_multiline SMTP_PASSWORD "$smtp_password"
reject_multiline DRUPAL_HASH_SALT "$hash_salt"

if [ "${1:-}" = "php-fpm" ] && [ "${#hash_salt}" -lt 32 ]; then
  echo "ERROR: DRUPAL_HASH_SALT must contain at least 32 characters" >&2
  exit 1
fi

if [ -n "$cookie_domain" ]; then
  if ! printf '%s' "$cookie_domain" | grep -qE '^\.[a-zA-Z0-9]([a-zA-Z0-9-]*[a-zA-Z0-9])?(\.[a-zA-Z0-9]([a-zA-Z0-9-]*[a-zA-Z0-9])?)*$'; then
    echo "ERROR: COOKIE_DOMAIN must use the form .example.org" >&2
    exit 1
  fi
  {
    printf '%s\n' 'parameters:'
    printf '%s\n' '  session.storage.options:'
    printf '%s\n' '    gc_probability: 1'
    printf '%s\n' '    gc_divisor: 100'
    printf '%s\n' '    gc_maxlifetime: 200000'
    printf '%s\n' '    cookie_lifetime: 2000000'
    printf "    cookie_domain: '%s'\n" "$cookie_domain"
  } > "$SERVICES_FILE"
else
  rm -f "$SERVICES_FILE"
fi

msmtp_host="${smtp_host:-mailpit}"
msmtp_port="${smtp_port:-1025}"
msmtp_from="${smtp_from:-noreply@example.invalid}"
if [ "${MARKASPOT_MAIL_MODE:-}" != "production" ]; then
  msmtp_host=mailpit
  msmtp_port=1025
fi

rm -f /tmp/msmtprc
{
  printf '%s\n' 'defaults'
  printf '%s\n' 'logfile /tmp/msmtp.log'
  printf '%s\n' 'account default'
  printf 'host %s\n' "$msmtp_host"
  printf 'port %s\n' "$msmtp_port"
  printf 'from %s\n' "$msmtp_from"
  if [ "${MARKASPOT_MAIL_MODE:-}" = "production" ] && [ -n "$smtp_user" ] && [ -n "$smtp_password" ]; then
    printf '%s\n' 'tls on' 'tls_starttls on' 'auth on'
    printf 'user %s\n' "$smtp_user"
    printf 'password %s\n' "$smtp_password"
  else
    printf '%s\n' 'tls off' 'auth off'
  fi
} > /tmp/msmtprc
chmod 0600 /tmp/msmtprc
if [ "$(id -u)" = "0" ]; then
  chown "$APP_USER:$APP_GROUP" /tmp/msmtprc
fi

cron_enabled="${MARKASPOT_CRON_ENABLED:-false}"
case "$cron_enabled" in
  true|false) ;;
  *)
    echo "ERROR: MARKASPOT_CRON_ENABLED must be true or false" >&2
    exit 1
    ;;
esac

if [ "$cron_enabled" = "true" ]; then
  cron_interval="${MARKASPOT_CRON_INTERVAL:-900}"
  case "$cron_interval" in
    ''|*[!0-9]*)
      echo "ERROR: MARKASPOT_CRON_INTERVAL must be an integer from 60 to 86400" >&2
      exit 1
      ;;
  esac
  if [ "${#cron_interval}" -gt 5 ] || [ "$cron_interval" -lt 60 ] || [ "$cron_interval" -gt 86400 ]; then
    echo "ERROR: MARKASPOT_CRON_INTERVAL must be an integer from 60 to 86400" >&2
    exit 1
  fi
  run_as_app env MARKASPOT_CRON_INTERVAL="$cron_interval" sh -lc '
    while true; do
      sleep "$MARKASPOT_CRON_INTERVAL"
      cd /app/data
      ./vendor/bin/drush cron || true
    done
  ' &
fi

if [ "$(id -u)" = "0" ] && [ "${1:-}" != "php-fpm" ]; then
  exec su-exec "$APP_USER" "$@"
fi

exec "$@"
