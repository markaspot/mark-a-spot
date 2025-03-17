ARG BASE_IMAGE=php:8.3-fpm-alpine

# =========================
#  Builder stage
# =========================
FROM ${BASE_IMAGE} AS builder
WORKDIR /app/data

RUN apk update && apk upgrade --no-cache

# Install minimum required extensions and dependencies for Drupal (build-time)
RUN apk add --no-cache \
        git \
        curl \
        mariadb-client \
        libzip-dev \
        freetype-dev \
        libjpeg-turbo-dev \
        libpng-dev \
        oniguruma-dev \
        libxml2-dev \
        jq \
        ${PHPIZE_DEPS} \
    && docker-php-ext-configure gd --with-freetype --with-jpeg \
    && docker-php-ext-install -j"$(nproc)" \
        gd \
        zip \
        opcache \
        pdo \
        pdo_mysql \
        mbstring \
        xml \
    && curl -sS https://getcomposer.org/installer | php -- --install-dir=/usr/bin/ --filename=composer \
    && apk del --no-cache ${PHPIZE_DEPS}

# Copy application files and install dependencies
COPY . .
RUN composer install --no-dev --optimize-autoloader \
    && composer clear-cache

# Generate SBOM in builder
RUN apk add --no-cache syft \
    && syft . -o spdx-json=/sbom.json


# =========================
#  Production stage
# =========================
FROM ${BASE_IMAGE}
WORKDIR /app/data

RUN apk update && apk upgrade --no-cache \
    && apk add --no-cache \
         libzip-dev \
         freetype-dev \
         libjpeg-turbo-dev \
         libpng-dev \
         oniguruma-dev \
         libxml2-dev \
         git \
         curl \
         mariadb-client \
         jq \
         ${PHPIZE_DEPS} \
    && docker-php-ext-configure gd --with-freetype --with-jpeg \
    && docker-php-ext-install -j"$(nproc)" \
         gd \
         zip \
         opcache \
         pdo \
         pdo_mysql \
         mbstring \
         xml \
    && curl -sS https://getcomposer.org/installer | php -- --install-dir=/usr/bin/ --filename=composer \
    && apk del --no-cache ${PHPIZE_DEPS}

# Copy built application and settings from builder
COPY --from=builder /app/data ./

# Add non-root user
RUN adduser -D -u 1000 app \
    && chown -R app:app .

# Create Drush symlink for convenience
RUN ln -sf /app/data/vendor/drush/drush/drush /usr/local/bin/drush

USER app
EXPOSE 9000

HEALTHCHECK --interval=30s --timeout=3s \
    CMD curl -f http://localhost/ || exit 1

# Copy SBOM from builder
LABEL org.opencontainers.image.source="https://github.com/markaspot/markaspot"
LABEL org.opencontainers.image.authors="Mark-a-Spot"
LABEL org.opencontainers.image.licenses="GPL-2.0-or-later"
LABEL org.opencontainers.image.url="https://mark-a-spot.com"
LABEL org.opencontainers.image.description="Mark-a-Spot application"
LABEL org.opencontainers.image.sbom="sbom.json"
