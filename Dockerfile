ARG BASE_IMAGE=php:8.2-rc-fpm-alpine

# PHP Dependency install via Composer.
# Build the Docker image for Drupal.
FROM $BASE_IMAGE as build
WORKDIR '/app/data'

# Install dependencies
RUN apk add --no-cache \
    curl \
    jq \
    freetype \
    freetype-dev \
    git \
    libjpeg-turbo \
    libjpeg-turbo-dev \
    libpng \
    libpng-dev \
    mysql-client \
    patch \
    libzip-dev \
 && apk add --no-cache --virtual .build-deps \
    $PHPIZE_DEPS \
 && docker-php-ext-install zip opcache mysqli pdo pdo_mysql \
 && docker-php-ext-configure gd --with-freetype --with-jpeg \
 && docker-php-ext-install -j$(nproc) gd \
 && docker-php-ext-install exif \
 && apk del --no-cache .build-deps

# Copy custom php.ini
COPY conf/php.ini "$PHP_INI_DIR/php.ini"

# Copy composer files to image
COPY composer.json composer.lock ./

# Only copy custom modules in prod / mounted volume in dev

COPY config ./config
COPY web ./web
COPY scripts ./scripts
COPY drush ./drush

RUN curl -OL https://github.com/drush-ops/drush-launcher/releases/latest/download/drush.phar \
 && chmod +x drush.phar \
 && mv drush.phar /usr/local/bin/drush \
 && curl -sS https://getcomposer.org/installer | php -- --install-dir=/usr/bin/ --filename=composer \
 && composer install \
    --ignore-platform-reqs \
    --no-interaction \
    --no-dev \
    --prefer-dist;

RUN mkdir -p /app/data/web/sites/default/files

# Set the permissions
RUN chown -R www-data:www-data /app/data && \
    find /app/data/web/sites/default/files -type d -exec chmod 755 {} \; && \
    find /app/data/web/sites/default/files -type f -exec chmod 644 {} \;

# Switch to www-data user
USER www-data
