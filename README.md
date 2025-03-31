# Mark-a-Spot

<div align="center">
  <img src="https://www.markaspot.de/assets/images/logo.svg" width="200px" alt="Mark-a-Spot Logo"/>
  <h3>Open-Source Civic Issue Tracking and Open311 Platform for Drupal 11</h3>
</div>

[![Docker Image CI](https://github.com/markaspot/mark-a-spot/actions/workflows/docker-image.yml/badge.svg)](https://github.com/markaspot/mark-a-spot/actions/workflows/docker-image.yml)
[![License: GPL v2+](https://img.shields.io/badge/License-GPL%20v2%2B-blue.svg)](https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html)
[![Drupal: 11](https://img.shields.io/badge/Drupal-11-brightgreen.svg)](https://www.drupal.org/project/markaspot)

### Key Features

- **Citizen Engagement:**  
  An intuitive interface lets citizens report issues easily—complete with photos, detailed descriptions, and geolocation data—to ensure that community problems are visible and actionable.
  
- **Nuxt.js Decoupled Frontend:**  
  A dynamic frontend, built with Nuxt.js, offers a sleek, responsive, and engaging user experience. It seamlessly complements the Drupal administration backend, creating an integrated platform designed for modern civic engagement.

- **Advanced Mapping & Geolocation:**  
  Interactive maps allow users to pinpoint exact problem locations, making it easier for public authorities to identify and respond to issues efficiently.
  
- **Open311 Compliant API:**  
  The platform offers a standardized [GeoReport v2 API](https://wiki.open311.org/GeoReport_v2) for seamless integration with mobile applications, municipal services, and third-party systems.
  
- **Efficient Workflow Management:**  
  Built-in tools help municipal staff manage issue submissions, track progress, and assign tasks, ensuring that problems are addressed from reporting to resolution.
  
- **Multi-language & Localization Support:**  
  With built-in support for multiple languages and region-specific configurations, the platform can be tailored to local needs and global standards.
  
- **Data-driven Insights:**  
  Analytics and reporting tools provide actionable insights, helping decision-makers prioritize resources and plan improvements effectively.
  
- **Customizable & Extensible:**  
  Drupal's flexible architecture allows municipalities to customize workflows, integrate additional features, and expand functionality to suit diverse requirements.

### Who Uses Mark-a-Spot?

Mark-a-Spot is designed for:
- **Municipal Governments & City Administrations:**  
  To streamline citizen reporting and improve the management of civic issues.
  
- **Public Service Departments:**  
  To enhance coordination and responsiveness in public service delivery.
  
- **Community Organizations:**  
  To foster a participatory approach in addressing local issues.
  
- **Civic Tech Innovators:**  
  To leverage an adaptable, open-source solution for developing and deploying civic technologies.

> **Note**: This is the 11.7.x-dev branch, which is compatible with Drupal 11. For older versions, see the [10.6.x-dev](https://github.com/markaspot/mark-a-spot/tree/10.6.x-dev) or [8.5.x-dev](https://github.com/markaspot/mark-a-spot/tree/8.5.x-dev) branches.

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

3. Run the `start.sh` script

   If it gives you an error of permission denied make it executable with: `chmod a+x ./scripts/start.sh`)
    ```bash
   docker exec -it markaspot ./scripts/start.sh -y
    ```
   The `start.sh` script has the following options:
   - `-y` For automatic installation with predefined values (latitude: 40.73, longitude: -73.93, city: New York, locale: en_US)
   - `-t` To import translation file from the `/translations` directory and enable translations for terms
   - `-a` To use OpenAI translation for content artifacts, see below

#### AI Translation Feature

The (AI) translation feature allows you to automatically translate the Drupal UI and content artifacts (taxonomy terms, pages, blocks, etc.) using OpenAI's language models. This provides translations for your Mark-a-Spot installation's default content. 

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

Once the script has executed, the application should be accessible at http://localhost. Please exercise caution when executing the script, as it will drop the database and initialize Mark-a-Spot from scratch. Additionally, familiarize yourself with the Drupal development process, including configuring changes, backing up databases, and other relevant procedures.

### Services

The Docker Compose setup includes the following services:

- `web`: The Nginx web server
- `markaspot`: The Mark-a-Spot Drupal application
- `ui`: The frontend (Nuxt), remember to delete application cache when reinstall
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

## License

Mark-a-Spot is freely available under the [GNU General Public License, version 2 or any later version](https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html) license.