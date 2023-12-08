# Mark-a-Spot Drupal Docker Setup

Mark-a-Spot is an open-source Civic Issue Tracking and Open311 Server built on Drupal CMS. This repository provides a Docker setup to facilitate running Mark-a-Spot.

## Getting Started

These instructions will guide you through getting a copy of the project up and running on your local machine for development and testing purposes.

### Prerequisites

- Docker
- Docker Compose

### Installation

1. Clone this repository:
    ```bash
    git clone https://github.com/markaspot/mark-a-spot.git
    cd mark-a-spot
    ```

2. Run the Docker containers:
    ```bash
    docker-compose up -d
    ```

3. Run the `start.sh`script
   If it gives you an error of permission denied make it executable with: `chmod a+x ./scripts/start.sh`)
    ```bash
   docker exec -it markaspot ./scripts/start.sh -y
    ```
   The `start.sh` script has two options:
   - `-y` For automatic installation with predefined values (latitude: 40.73, longitude: -73.93, city: New York, locale: en_US)
   - `-t` To import translation file from the `/translations` directory and enable translations for terms

Once the script has executed, the application should be accessible at http://localhost. Please exercise caution when executing the script, as it will drop the database and initialize Mark-a-Spot from scratch. Additionally, familiarize yourself with the Drupal development process, including configuring changes, backing up databases, and other relevant procedures.

### Services

The Docker Compose setup includes the following services:

- `web`: The Nginx web server
- `markaspot`: The Mark-a-Spot Drupal application
- `db`: The MariaDB database
- `phpmyadmin`: PHPMyAdmin for database management

### Configuration

You can adjust the configuration of the Docker services by editing the `docker-compose.yml` file. For example, you can modify the database username and password, the PHP memory limit, and other settings.

## Usage

Access the Drupal application at `http://localhost`.

PHPMyAdmin is available at `http://localhost:8080` for database management.

## Development

For local development, we recommend using [Docksal](https://docksal.io/), a Docker-based development environment.

### Prerequisites

- Docksal
- Docker
- Docker Compose

### Installation

1. Install Docksal on your machine, following the instructions on the [Docksal website](https://docksal.io/installation).

2. Clone this repository:

    ```bash
    git clone https://github.com/markaspot/mark-a-spot.git
    cd mark-a-spot
    ```

3. Initialize the Docksal project:

    ```bash
    fin up
    ```
4. Copy the local settings file to the web sites default directory and change your settings.php

    ```bash
    cp web/sites/example.settings.local.php web/sites/default/settings.local.php
    ```
5. Swap drush version as long https://github.com/docksal/docksal/issues/1783 is not solved:

    ```bash
    fin bash
    echo -e "\n"'export PATH="${PROJECT_ROOT:-/var/www}/vendor/bin:$PATH"' >> $HOME/.profile;
    source $HOME/.profile
    ```
6. Run `scripts/start.sh` in docksal CLI (see above)


### Configuration

You can adjust the configuration of the Docker services by editing the `docksal.yml` file. For example, you can modify the database username and password, the PHP memory limit, and other settings.

### Usage

You can access the Drupal application at `http://mark-a-spot.docksal.site`.

## License

Mark-a-Spot is freely available under the [GNU General Public License, version 2 or any later version](https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html) license.


[![Docker Image CI](https://github.com/markaspot/mark-a-spot/actions/workflows/docker-image.yml/badge.svg)](https://github.com/markaspot/mark-a-spot/actions/workflows/docker-image.yml)
