# Export and Group regression

Run from the repository root with development dependencies and Drupal modules installed:

```sh
ddev exec env SIMPLETEST_DB=mysql://db:db@db/db composer test:export-group
```

Use a local test database. Drupal KernelTestBase creates and removes isolated
prefixed tables. Never point this command at a production database. The test
requires Group and Views Data Export, including the Composer patches shipped
by this repository. This database-backed suite is separate from the cloud
unit-only default runner; run it explicitly when changing these patches.

The regression verifies the actual batch count and initiating UID, then compares
all exported query IDs across multiple chunks for two accounts with disjoint
Group permissions. Unpublished groups and an account without Group access stay
excluded. Switching back to the first account also checks cache separation.
Without the export metadata patch, batch initialization throws the original
empty-entity-type exception.
