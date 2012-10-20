; Drush Make (http://drupal.org/project/drush_make)
api = 2
core = 7.x

libraries[spyc][download][type] = "get"
libraries[spyc][download][url] = "http://spyc.googlecode.com/svn/trunk/spyc.php"
libraries[spyc][filename] = "../spyc.php"
libraries[spyc][directory_name] = "lib"
libraries[spyc][destination] = "modules/contrib/services/servers/rest_server"
