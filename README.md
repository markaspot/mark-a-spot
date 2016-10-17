# Drupal project template for Platform.sh

This project provides a starter kit for Drupal 8 projects hosted on [Platform.sh](http://platform.sh). It
is very closely based on the [Drupal Composer project](https://github.com/drupal-composer/drupal-project).

## Starting a new project

To start a new Drupal 8 project on Platform.sh, you have 2 options:

1. Create a new project through the Platform.sh user interface and select "start
   new project from a template".  Then select Drupal 8 as the template. That will
   create a new project using this repository as a starting point.

2. Take an existing project, add the necessary Platform.sh files, and push it
   to a Platform.sh Git repository. This template includes examples of how to
   set up a Drupal 8 site.  (See the "differences" section below.)

## Using as a reference

You can also use this repository as a reference for your own Drupal projects, and
borrow whatever code is needed.  The most important parts are the `.platform.app.yaml` file,
the `.platform` directory, and the changes made to `settings.php`.

## Managing a Drupal site built with Composer

Once the site is installed, there is no difference between a site hosted on Platform.sh
and a site hosted anywhere else.  It's just Composer.  See the [Drupal documentation](https://www.drupal.org/node/2404989)
for tips on how best to leverage Composer with Drupal 8.

## How does this starter kit differ from vanilla Drupal from Drupal.org?

1. The `vendor` directory (where non-Drupal code lives) and the `config` directory
   (used for syncing configuration from development to production) are outside
   the web root. This is a bit more secure as those files are now not web-accessible.

2. The `settings.php` and `settings.platformsh.php` files are provided by
   default. The `settings.platformsh.php` file automatically sets up the database connection on Platform.sh, and allows controlling Drupal configuration from environment variables.

3. We include recommended `.platform.app.yaml` and `.platform` files that should suffice
   for most use cases. You are free to tweak them as needed for your particular site.
