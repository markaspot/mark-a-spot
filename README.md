# Mark-a-Spot project template for Development

This repo provides a starter kit for Mark-a-Spot 3.x. It is very closely based on the [Drupal Composer project](https://github.com/drupal-composer/drupal-project) and the [version of platform.sh](https://github.com/platformsh/platformsh-example-drupal8/tree/drupal-8-2).

## Install Mark-a-Spot


```
$ git clone -b master-8.x --single-branch https://github.com/markaspot/mark-a-spot.git
$ composer install

```

## Managing a Mark-a-Spot 3.x site built with Composer

Once the site is installed, there is no difference between a site hosted on Platform.sh
and a site hosted anywhere else.  It's just Composer.  See the [Drupal documentation](https://www.drupal.org/node/2404989) for tips on how best to leverage Composer with Drupal 8.

## How does this starter kit differ from vanilla Drupal from Drupal.org?

1. The `vendor` directory (where non-Drupal code lives) and the `config` directory
   (used for syncing configuration from development to production) are outside
   the web root. This is a bit more secure as those files are now not web-accessible.
