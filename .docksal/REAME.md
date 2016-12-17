# Docksal Integration

This folder provides [Docksa](http://docksal.io/) integration, an easy to use set of docker containers for local development.

To use:

1. Follow [https://github.com/docksal/docksal](https://github.com/docksal/docksal) installation instructions.
2. Run ``fin up``
3. Go to  ``http://markaspot.docksal``

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

