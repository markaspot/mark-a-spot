TO CUSTOMIZE EMBER
--------------

to make css customizations to ember, all the changes should be made in the SCSS files, which will compile into the correct CSS file. 

Furthermore, Ember uses a style framework called ux_styleguides, which you can find under extensions > ux_styleguide. This is where much of the administration core CSS from Drupal is overridden. If you need to customize this, updating those files will compile and work correctly. 

Ember utilizes Bundler to manage the gems needed to compile the css correctly. It also implements compass to take advantage of mixins and browser prefix correcting. 

SETTING UP EMBER SCSS FRAMEWORK IN VIA CLI
---------------

make sure you have bundler installed on your machine http://bundler.io/
(gem install bundler)

In the root directory of Ember:

:bundle install
:compass watch

Compass should now be polling for changes in that directory and will compile on save.



