#!/bin/sh
set -eu

required_files='
Dockerfile.runtime
Dockerfile.runtime-nginx
docker/runtime/settings.php
docker/runtime/docker-entrypoint.sh
docker/runtime/nginx.conf
docker/runtime/nginx-entrypoint.sh
.github/workflows/runtime-image.yml
docs/runtime-images.md
'

for file in $required_files; do
  if [ ! -f "$file" ]; then
    echo "Missing Mark-a-Spot runtime file: $file" >&2
    exit 1
  fi
done

if [ "${SKIP_PHP_LINT:-0}" != "1" ]; then
  php -l docker/runtime/settings.php
fi
sh -n docker/runtime/docker-entrypoint.sh
sh -n docker/runtime/nginx-entrypoint.sh

if grep -rnE 'markaspot/markaspot-cloud|FASTMAP_|MARKASPOT_(VISION|BLUR|NLP)|ABFALLPLUS_' Dockerfile.runtime Dockerfile.runtime-nginx docker/runtime .github/workflows/runtime-image.yml docs/runtime-images.md; then
  echo "Mark-a-Spot runtime references a private or product-specific feature." >&2
  exit 1
fi

grep -qE '^private/$' .dockerignore
grep -qE '^vendor/$' .dockerignore
grep -qE '^web/sites/default/files/$' .dockerignore
grep -qE '^config/sync/$' .dockerignore
grep -qE '^web/sites/\*/settings.prod.php$' .dockerignore
grep -qE 'COPY docker/runtime/settings.php /app/data/web/sites/default/settings.php' Dockerfile.runtime
grep -qE 'USER 101:101' Dockerfile.runtime-nginx
grep -qE 'fastcgi_pass drupal:9000' docker/runtime/nginx.conf
grep -qE 'log_format safe_access' docker/runtime/nginx.conf
grep -qE 'fastcgi_param REMOTE_ADDR 127.0.0.1' docker/runtime/nginx.conf
grep -qE 'include /tmp/markaspot-trusted-real-ip.conf' docker/runtime/nginx.conf
grep -qE 'map "\$trusted_proxy:\$safe_forwarded_proto" \$safe_forwarded_port' docker/runtime/nginx.conf
grep -qE '"1:https" 443' docker/runtime/nginx.conf
grep -qE 'MARKASPOT_TRUSTED_PROXY_CIDRS' docker/runtime/nginx-entrypoint.sh
grep -qE '^ARG DRUPAL_IMAGE$' Dockerfile.runtime-nginx
grep -qE '^  DRUPAL_IMAGE: markaspot/markaspot$' .github/workflows/runtime-image.yml
grep -qE '^  NGINX_IMAGE: markaspot/markaspot-nginx$' .github/workflows/runtime-image.yml

if grep -qE 'COPY --from=builder --chown=' Dockerfile.runtime; then
  echo "Application code must remain root-owned in the runtime image." >&2
  exit 1
fi

echo "Mark-a-Spot runtime contract checks passed."
