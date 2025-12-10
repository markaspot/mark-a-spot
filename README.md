# Mark-a-Spot

<div align="center">
  <img src="https://www.markaspot.de/assets/images/logo.svg" width="100px" alt="Mark-a-Spot Logo"/>
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

## Architecture

Mark-a-Spot uses a **decoupled/headless architecture** with Drupal as the backend API server and a modern JavaScript frontend.

```
┌─────────────────────────────────────────────────────────────┐
│                     Frontend (PWA)                          │
│         Vue 3 · TypeScript · Tailwind · MapLibre            │
│                                                             │
│     Components ─── Stores (Pinia) ─── API Composables       │
└─────────────────────────────┬───────────────────────────────┘
                              │
                    HTTPS/JSON (REST)
                              │
┌─────────────────────────────▼───────────────────────────────┐
│                      Drupal 11 Backend                      │
│              PHP 8.3 · MySQL · Search API                   │
│                                                             │
│  ┌──────────────────────┐  ┌──────────────────────┐         │
│  │     Open311 API      │  │       JSON:API       │         │
│  │     (GeoReport)      │  │        (CRUD)        │         │
│  └──────────────────────┘  └──────────────────────┘         │
└─────────────────────────────────────────────────────────────┘
```

The backend exposes two APIs:
- **Open311 GeoReport v2** – Standardized civic issue API for reading/writing service requests
- **JSON:API** – Drupal's RESTful API for content management

## Requirements

- **PHP** 8.3+
- **Node.js** 22+ (LTS)
- **MySQL** 8.0+ or MariaDB 10.6+
- **Composer** 2.x

## Getting Started

These instructions will guide you through getting a copy of the project up and running on your local machine for development and testing purposes.

### Prerequisites

Install one (or both) of the following toolchains before you begin:

- [DDEV](https://ddev.readthedocs.io/en/stable/users/install/ddev-installation/) – recommended local development environment built on Docker
- Docker and Docker Compose (v2 `docker compose` CLI works too)

## Installation

**Quick start (DDEV)** *(requires DDEV to be installed first)*

```bash
git clone https://github.com/markaspot/mark-a-spot.git
cd mark-a-spot
ddev start
ddev ssh
./scripts/start.sh -y
exit
```

After installation, access:
- **Backend (Drupal):** https://mark-a-spot.ddev.site
- **Frontend (UI):** https://mark-a-spot.ddev.site:8040
- **Admin login:** Run `ddev drush uli` to get a one-time login link

### Which environment should I use?
- **DDEV** – best for day-to-day development, automatic HTTPS, and a config you can share with teammates.
- **Docker Compose** – mirrors the legacy stack; handy if you need to run the shipped `docker-compose.yml` as-is.

> The installer always drops and recreates the Drupal database. Back up any local work before rerunning it.

### Installer flags (all environments)

- `-y` Autopilot: uses default New York coordinates/locale and skips prompts.
- `-t` Drupal translation import: installs language packs from `translations/`.
- `-a` AI translation: runs `ai-translate.sh` to translate default content (needs `OPENAI_API_KEY`).
- Combine as needed, e.g. `./scripts/start.sh -t -a` for a multilingual build.
- On first run the installer generates a fresh GeoReport API key and prints it. To reuse a specific key, set `GEOREPORT_API_KEY` in your environment or `.env` before launching the installer (Docker/DDEV front-end services read the same variable).

### AI Translation Feature

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

Once the script has executed, the application should be accessible at http://localhost or the ddev URLs you will be retrieve via `ddev describe`.

 Please exercise caution when executing the script, as it will drop the database and initialize Mark-a-Spot from scratch. Additionally, familiarize yourself with the Drupal development process, including configuring changes, backing up databases, and other relevant procedures.


#### Docker Compose workflow

1. Clone this repository (if you haven't already):
    ```bash
    git clone https://github.com/markaspot/mark-a-spot.git
    cd mark-a-spot
    ```

2. Start the stack:
    ```bash
    docker-compose up -d
    ```

3. Run the installer:

   If the script is not executable, make it so with `chmod a+x ./scripts/start.sh`.
    ```bash
    docker exec -it markaspot ./scripts/start.sh -y
    ```


## Environment Variables

Key environment variables for deployment:

| Variable | Description | Example |
|----------|-------------|---------|
| `GEOREPORT_API_KEY` | API key for GeoReport v2 authentication | `abc123...` |
| `DRUPAL_DATABASE_*` | Database connection settings | See `env.example` |

The installer generates a fresh `GEOREPORT_API_KEY` on first run. To use a specific key, set it before installation.

## API Documentation

Mark-a-Spot implements the [Open311 GeoReport v2](https://wiki.open311.org/GeoReport_v2) standard.

**Endpoints:**
- `GET /georeport/v2/services.json` – List available service categories
- `GET /georeport/v2/requests.json` – List service requests
- `GET /georeport/v2/requests/{id}.json` – Get single request
- `POST /georeport/v2/requests.json` – Create new request

**Authentication:**
- **Read**: API key via `api_key` parameter or header
- **Write**: Anonymous (with CSRF token) or authenticated session

## Contributing

Contributions are welcome! Please feel free to submit issues and pull requests.

- **Issues**: [GitHub Issues](https://github.com/markaspot/mark-a-spot/issues)
- **Profile**: [markaspot/markaspot](https://github.com/markaspot/markaspot)

## License

Mark-a-Spot is freely available under the [GNU General Public License, version 2 or any later version](https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html) license.
