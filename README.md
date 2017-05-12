# Mark-a-Spot composer-based installer

This is a composer-based installer for the [Mark-a-Spot](https://www.drupal.org/project/markaspot) Drupal distribution.
It can be used to start a project based on this crowdmapping platform. 

## Installation

Please follow the installation instructions and configuration guide at the official [documentation](http://docs.markaspot.de).


### TL;TR
```
$ curl -s https://getcomposer.org/installer | php
$ sudo mv composer.phar /usr/local/bin/composer
$ composer create-project markaspot/mark-a-spot project-dir --stability dev
```

This command will install the Mark-a-Spot distribution into a project directory.

You may then use the [included](https://github.com/markaspot/mark-a-spot/tree/master/.docksal) Docksal integration for easy development or go on with drush in your local dev environment.
Otherwise just replace the connection string with credentials of your database and run the following drush command.

```
$ drush si markaspot -y  --db-url=mysql://root:root@db:3306/markaspot

```

## Support, Hosting
Holger Kreis | twitter: @markaspot | http://mark-a-spot.org
