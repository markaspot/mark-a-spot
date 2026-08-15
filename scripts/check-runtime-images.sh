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

if rg -n 'markaspot/markaspot-cloud|FASTMAP_|MARKASPOT_(VISION|BLUR|NLP)|ABFALLPLUS_' Dockerfile.runtime Dockerfile.runtime-nginx docker/runtime .github/workflows/runtime-image.yml docs/runtime-images.md; then
  echo "Mark-a-Spot runtime references a private or product-specific feature." >&2
  exit 1
fi

rg -q '^private/$' .dockerignore
rg -q '^vendor/$' .dockerignore
rg -q '^web/sites/default/files/$' .dockerignore
rg -q '^config/sync/$' .dockerignore
rg -q '^web/sites/\*/settings.prod.php$' .dockerignore
rg -q 'COPY docker/runtime/settings.php /app/data/web/sites/default/settings.php' Dockerfile.runtime
rg -q 'USER 101:101' Dockerfile.runtime-nginx
rg -q 'fastcgi_pass drupal:9000' docker/runtime/nginx.conf
rg -q 'log_format safe_access' docker/runtime/nginx.conf
rg -q 'fastcgi_param REMOTE_ADDR 127.0.0.1' docker/runtime/nginx.conf
rg -q 'include /tmp/markaspot-trusted-real-ip.conf' docker/runtime/nginx.conf
rg -q 'map "\$trusted_proxy:\$safe_forwarded_proto" \$safe_forwarded_port' docker/runtime/nginx.conf
rg -q '"1:https" 443' docker/runtime/nginx.conf
rg -q 'MARKASPOT_TRUSTED_PROXY_CIDRS' docker/runtime/nginx-entrypoint.sh
rg -q '^ARG DRUPAL_IMAGE$' Dockerfile.runtime-nginx
rg -q '^  DRUPAL_IMAGE: markaspot/markaspot$' .github/workflows/runtime-image.yml
rg -q '^  NGINX_IMAGE: markaspot/markaspot-nginx$' .github/workflows/runtime-image.yml

if rg -q 'COPY --from=builder --chown=' Dockerfile.runtime; then
  echo "Application code must remain root-owned in the runtime image." >&2
  exit 1
fi

echo "Mark-a-Spot runtime contract checks passed."
