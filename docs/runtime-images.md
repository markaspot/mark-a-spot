# Mark-a-Spot runtime images

The public Mark-a-Spot runtime packages the canonical, self-hostable application
from this repository. It provides the common operating-system and Drupal layer
without customer-specific code or private product services. The private
Mark-a-Spot Cloud runtime remains a separate downstream deployment layer.

## Images

- `markaspot/markaspot`: Drupal, PHP-FPM, Drush and Composer runtime.
- `markaspot/markaspot-nginx`: matching nginx image with the exact
  static web root from the Drupal image.

Both images are built for `linux/amd64` and `linux/arm64`.

Each build job publishes a unique
`sha-GIT_SHA-run-RUN_ID-RUN_ATTEMPT` tag. Pushes to `11.9.x-dev` and
deliberate `X.Y.Z` profile release tags trigger a paired build. The successful
workflow summary records both manifest digests. Deployments should pin those
two digests. A partial workflow rerun can produce different unique tags for
the Drupal and nginx jobs, so the digest pair is the release contract.
Mutable aliases are intentionally not published because two
Docker Hub repositories cannot be retagged atomically.

This workflow is additive during migration. The existing `docker-image.yml`
workflow remains responsible for the current `markaspot/markaspot:latest`
contract. Moving `latest` or publishing semantic image aliases requires a
separate, deliberate cutover after the paired runtime has passed CI and tenant
acceptance.

## Runtime contract

The application root is `/app/data`. PHP-FPM listens on port 9000 and runs its
workers as UID and GID 1000. nginx listens as an unprivileged user on port 8080
and connects to `drupal:9000`.

Forwarded headers are accepted only from the proxies listed in
`MARKASPOT_TRUSTED_PROXY_CIDRS`. With no list, nginx ignores incoming forwarded
headers. Drupal trusts only nginx at the FastCGI boundary. Deployments behind a
TLS terminator must configure its exact IPv4 address or network so Drupal sees
the external scheme and client address.

The following paths must be persistent when their data is needed:

- `/app/data/web/sites/default/files` for public files. Mount the same volume
  at this path in both the Drupal and nginx containers.
- `/app/data/private` for local private files

Core runtime configuration is supplied through environment variables:

- Database: `DB_HOST`, `DB_PORT`, `DB_NAME`, `DB_USER`, `DB_PASSWORD`
- Drupal: required `DRUPAL_HASH_SALT` with at least 32 characters,
  `DRUPAL_TRUSTED_HOST`, `FRONTEND_HOSTS`,
  `COOKIE_DOMAIN`, `SITE_IDENTIFIER`
- Proxy: `MARKASPOT_TRUSTED_PROXY_CIDRS` as a comma-separated list of the
  IPv4 addresses or CIDRs of the actual TLS/ingress proxies
- API: `GEOREPORT_API_KEYS` or the single-key `GEOREPORT_API_KEY` fallback
- Geocoding: `GEOCODER_PROVIDER`, `GEOCODER_LANGUAGE`,
  `GEOCODER_API_KEY`, `MAPBOX_TOKEN`
- Mail: `MARKASPOT_MAIL_MODE`, `SMTP_HOST`, `SMTP_PORT`, `SMTP_FROM`,
  `SMTP_USER`, `SMTP_PASSWORD`

Mail is fail-safe. Only the exact value `MARKASPOT_MAIL_MODE=production` can
use the supplied production SMTP endpoint. Missing or unknown values use the
in-stack `mailpit:1025` endpoint.

Set `MARKASPOT_CRON_ENABLED=true` on exactly one Drupal container to run Drush
cron. The optional `MARKASPOT_CRON_INTERVAL` is expressed in seconds and
defaults to 900. Accepted values range from 60 to 86400.

## Tenant images

A customer or municipality keeps its own modules and configuration in a small
overlay image:

```dockerfile
ARG BASE_IMAGE=markaspot/markaspot@sha256:REPLACE_WITH_DRUPAL_DIGEST
FROM ${BASE_IMAGE}

COPY web/modules/custom/city_module \
  /app/data/web/modules/custom/city_module
```

The matching tenant nginx image should derive from the built tenant Drupal
image, not from a mutable tag. This keeps PHP code and public assets on the
same revision. `DRUPAL_IMAGE` is therefore a required build argument for
`Dockerfile.runtime-nginx` and should contain an immutable digest.

`SITE_IDENTIFIER` is limited to 1 to 64 letters, digits, underscores or
hyphens so it cannot escape the private-files directory.

The public runtime does not include private Cloud services, customer
configuration, logos, uploaded files or database contents. An image release
also never deploys or updates an existing tenant automatically.
