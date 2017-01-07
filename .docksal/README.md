# Docksal Integration

This folder provides [Docksal](http://docksal.io/) integration, an easy to use set of docker containers for local development.

To use:

1. Follow [https://github.com/docksal/docksal](https://github.com/docksal/docksal) installation instructions.
2. Run ``fin up``
3. Go to  ``http://markaspot.docksal``
4. In order to use the static_json cron of mark-a-spot (needed for the map) make sure to allow containers IPs
as host-name. Just add it to the trusted_host_patterns settings.

```
/**
 * Trust localhost.
 *
 * This will configure several common hostnames used for local development to
 * be trusted hosts.
 */
$settings['trusted_host_patterns'] = array(
  '^localhost$',
  '^localhost\.*',
  '172.*',
  '^maarkaspot.docksal$',
);
```

To install for the first time you need the MySQL connection info:

```
user: root
pass: root
database name: default
port: see "finding the port"
host: see note 2
```

### Finding the Database Port

Run `fin st` and look at the database port. Should be something like ``0.0.0.0:32768->3306/tcp``. In this case **32768** is the port.

### Finding the Database Host

Type ``env | grep DOCKER_HOST``. The IP address is the host
