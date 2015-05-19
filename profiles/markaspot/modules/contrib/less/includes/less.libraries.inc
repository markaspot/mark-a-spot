<?php

/**
 * @file
 *   Contains Libraries API integration.
 */

/**
 * Implements hook_libraries_info().
 */
function less_libraries_info() {
  $libraries = array();
  
  /**
   * Legacy leafo/lessphp library.
   */
  $libraries['lessphp'] = array(
    'name' => 'lessphp (Not recommended)',
    'vendor url' => 'http://leafo.net/lessphp/', 
    'download url' => 'http://leafo.net/lessphp/',
    'version arguments' => array(
      'file' => 'lessc.inc.php', 
      'pattern' => '/VERSION\s*=\s*["\']v?([\d\.]+)/', 
      'lines' => 50, 
    ),
    'files' => array(
      'php' => array(
        'lessc.inc.php',
      ),
    ),
  );
  
  _less_lessphp_locate($libraries['lessphp']);
  
  /**
   * Newer oyejorge/less.php library. Closer to canonical spec from lesscss.org.
   */
  $libraries['less.php'] = array(
    'name' => 'less.php',
    'vendor url' => 'http://lessphp.gpeasy.com/', 
    'download url' => 'http://lessphp.gpeasy.com/#integration-with-other-projects',
    'version arguments' => array(
      'file' => 'Version.php', 
      'pattern' => '/version\s*=\s*["\']([\d\.]+)/', 
      'lines' => 20, 
    ),
    'files' => array(
      'php' => array(
        'Less.php',
      ),
    ),
    'versions' => array(
      '1.7.0' => array(),
    ),
  );
  
  _less_less_php_locate($libraries['less.php']);
  
  /**
   * Canonical version of LESS language.
   */
  $libraries['less.js'] = array(
    'name' => 'less.js',
    'vendor url' => 'http://lesscss.org/', 
    'download url' => 'http://lesscss.org/usage/#using-less-environments',
    'library path' => drupal_get_path('module', 'less') . '/classes',
    'version callback' => array('Lessjs', 'version'),
    /**
     * Integer indexed 'version arguments' array causes 'version callback' to be
     * run through call_user_func_array().
     *
     * @see call_user_func_array()
     */
    'version arguments' => array(
      0 => 'not used',
    ),
    'files' => array(
      'php' => array(
        'class.lessjs.inc',
      ),
    ),
    'versions' => array(
      '1.5.0' => array(),
    ),
  );
  
  /**
   * Autoprefixer
   */
  $libraries['lessautoprefixer'] = array(
    'name' => 'Autoprefixer',
    'vendor url' => 'https://github.com/ai/autoprefixer', 
    'download url' => 'https://github.com/ai/autoprefixer/releases',
    'library path' => drupal_get_path('module', 'less') . '/classes',
    'version callback' => array('LessAutoprefixer', 'version'),
    /**
     * Integer indexed 'version arguments' array causes 'version callback' to be
     * run through call_user_func_array().
     *
     * @see call_user_func_array()
     */
    'version arguments' => array(
      0 => 'not used',
    ),
    'files' => array(
      'php' => array(
        'class.lessautoprefixer.inc',
      ),
    ),
    'versions' => array(
      '1.1' => array(),
    ),
  );
  
  return $libraries;
}

/**
 * Locates oyejorge/less.php in the many possible places it could be.
 * 
 * @param array $library
 *   Libraries definition array.
 */
function _less_less_php_locate(&$library) {
  
  $locations = array();
  
  // Primary libraries location
  $locations[] = libraries_get_path('less.php');
  
  // lessphp drop-in replacement location
  $locations[] = libraries_get_path('lessphp');
  
  // Composer location
  $locations[] = drupal_get_path('module', 'less') . '/vendor/oyejorge/less.php';
  
  $version_files = array(
    'lib/Less/Version.php' => 'lessc.inc.php', // Source code
    'Version.php' => 'Less.php', // Compiled
  );
  
  _less_libraries_determine_location($library, $locations, $version_files);
}


/**
 * Locates leafo/lessphp in the many possible places it could be.
 * 
 * @param array $library
 *   Libraries definition array.
 */
function _less_lessphp_locate(&$library) {
  
  $locations = array();
  
  // Primary libraries location
  $locations[] = libraries_get_path('lessphp');
  
  // Legacy bundled location
  $locations[] = drupal_get_path('module', 'less') . '/lessphp';
  
  // Composer location
  $locations[] = drupal_get_path('module', 'less') . '/vendor/leafo/lessphp';
  
  /*
   * oyejorge/less.php does not have the actual version number in lessc.inc.php,
   * so we don't have to worry about mistaken identity.
   */
  $version_files = array(
    'lessc.inc.php' => 'lessc.inc.php',
  );
  
  _less_libraries_determine_location($library, $locations, $version_files);
}

/**
 * Helper function that checks locations for LESS libraries.
 * 
 * @param array &$library
 *   Library in question. Paths to found libraries will be added here.
 * @param array $locations
 *   Array of paths of potential library installs relative to DRUPAL_ROOT.
 * @param array $version_files
 *   Array of key => value pairs, where key is location library version number,
 *   and value is the location of that file that to be included when this
 *   library is loaded with libraries_load().
 */
function _less_libraries_determine_location(array &$library, array $locations, array $version_files) {
  
  foreach (array_filter($locations) as $location) {

    foreach ($version_files as $version_file => $class_file) {

      if (file_exists($location . DIRECTORY_SEPARATOR . $version_file)) {

        $library['library path'] = $location;
        $library['files'] = array(
          'php' => array(
            $class_file,
          ),
        );

        $library['version arguments']['file'] = $version_file;

        return; // File has been found, skip remaining $locations and $version_files
      }
    }
  }
}
