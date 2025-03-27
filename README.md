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
   The `start.sh` script has the following options:
   - `-y` For automatic installation with predefined values (latitude: 40.73, longitude: -73.93, city: New York, locale: en_US)
   - `-t` To import translation file from the `/translations` directory and enable translations for terms
   - `-a` To use OpenAI translation for content artifacts instead of standard translation files

#### AI Translation Feature

The AI translation feature allows you to automatically translate content artifacts (taxonomy terms, pages, blocks, etc.) using OpenAI's language models. This provides high-quality translations for your Mark-a-Spot installation's default content.

To use this feature:
- Run `./scripts/start.sh -t -a` to use both standard Drupal translation files and AI translation for content
- You'll need an OpenAI API key, which you can either:
  - Set as an environment variable: `export OPENAI_API_KEY=your_api_key`
  - Enter when prompted during the installation process

The `-t -a` option will:
1. Install the selected language in Drupal
2. Import standard translation files for the Drupal interface
3. Use AI to translate all default content to the selected language
4. Set up the site with the translated content as the default content

The translation covers:
- Taxonomy terms (categories, statuses, providers)
- Static pages (About, Contact, etc.)
- Boilerplate content
- Block content

All technical fields like service codes, colors, and icons are preserved during translation.

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
Access the Frontend at `http://localhost:3000`.
PHPMyAdmin is available at `http://localhost:8080` for database management.

## Development

For local development, we recommend using [DDEV](https://ddev.com), a Docker-based development environment.


### Configuration

You can adjust the configuration of the Docker services by editing the `docksal.yml` file. For example, you can modify the database username and password, the PHP memory limit, and other settings.

## License

Mark-a-Spot is freely available under the [GNU General Public License, version 2 or any later version](https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html) license.


[![Docker Image CI](https://github.com/markaspot/mark-a-spot/actions/workflows/docker-image.yml/badge.svg)](https://github.com/markaspot/mark-a-spot/actions/workflows/docker-image.yml)
