; Running this make file will reapply patches to contrib modules that have been updated after
; installation
;
; See https://www.acquia.com/blog/patching-drush-make
; This has been tested with drush 7.0
; Run $ drush make --no-core mas-update-post-contrib.make

api = 2
core = 7.x

projects[uuid][contrib_destination] = "modules/contrib"
projects[uuid][patch][] = https://drupal.org/files/issues/custom_method_of_UUID_creation_2161375_1.patch

