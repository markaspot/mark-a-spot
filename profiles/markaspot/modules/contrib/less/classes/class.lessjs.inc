<?php

/**
 * @file
 * Contains 'lessjs' class; an abstraction layer for command line less.js.
 */

/**
 * 'lessjs' class.
 */
class Lessjs {
  
  /**
   * Base command is hardcoded here to reduce security vulnerability.
   *
   * @var string
   */
  const BASE_COMMAND = 'lessc';

  /**
   * Path to .less input file.
   *
   * @var null|string
   */
  protected $input_file;

  /**
   * @var string[] $include_paths
   *
   * @link http://lesscss.org/usage/#command-line-usage-include-paths
   */
  protected $include_paths = array();

  /**
   * @var string[] $modify_variables
   *
   * @link http://lesscss.org/usage/#command-line-usage-modify-variable
   */
  protected $modify_variables = array();
  
  protected $source_maps_enabled = FALSE;

  /**
   * @var null|string $source_map_rootpath
   *
   * @link http://lesscss.org/usage/#command-line-usage-source-map-rootpath
   */
  protected $source_map_rootpath = NULL;

  /**
   * @var null|string $source_map_basepath
   *
   * @link http://lesscss.org/usage/#command-line-usage-source-map-basepath
   */
  protected $source_map_basepath = NULL;
  
  /**
   * Constructor function for 'lessjs'.
   * 
   * @param string $input_file
   *   Path for .less file relative to getcwd().
   */
  private function __construct($input_file) {
    
    $this->input_file = $input_file;
  }

  public static function create($input_file = NULL) {

    return new self($input_file);
  }
  
  /**
   * Returns the version string from command line less.js.
   * 
   * @return string|null
   *   Version string from less.js, or null if no version found.
   */
  public static function version() {

    $version = NULL;

    try {

      $version_response = self::create(NULL)->proc_open(array('--version'));

      $version = preg_replace('/.*?([\d\.]+).*/', '$1', $version_response);
    }
    catch (Exception $e) {

    }

    return $version;
  }
  
  /**
   * Add include path that will be set with '--include-path' argument.
   * 
   * @link http://lesscss.org/usage/#command-line-usage-include-paths
   * 
   * @param string $include_path
   *   Path relative to getcwd().
   */
  public function include_path($include_path) {
    
    $this->include_paths[] = $include_path;
    
  }
  
  /**
   * Add LESS variable that will be set with the '--modify-var' argument.
   * 
   * @param string $variable_name
   *   The variable name.
   * @param string $variable_value
   *   The variable value.
   */
  public function modify_var($variable_name, $variable_value) {
    
    $this->modify_variables[$variable_name] = $variable_value;
  }

  /**
   * Enable source maps for current file, and configure source map paths.
   *
   * @param bool   $enabled
   *   Set the source maps flag.
   * @param string $base_path
   *   Leading value to be stripped from each source map URL.
   * @param string $root_path
   *   Value to be prepended to each source map URL.
   *
   * @link http://lesscss.org/usage/#command-line-usage-source-map-rootpath
   * @link http://lesscss.org/usage/#command-line-usage-source-map-basepath
   */
  public function source_maps($enabled, $base_path = NULL, $root_path = NULL) {
    $this->source_maps_enabled = $enabled;
    
    $this->source_map_basepath = $base_path;
    $this->source_map_rootpath = $root_path;
  }
  
  /**
   * Provides list to command line arguments for execution.
   * 
   * @return string[]
   *   Array of command line arguments.
   */
  private function command_arguments() {
    
    $arguments = array();
    
    // Add include paths.
    if (count($this->include_paths) > 0) {
      
      $arguments[] = '--include-path=' . implode(PATH_SEPARATOR, array_map('escapeshellarg', $this->include_paths));
      
      // @link http://lesscss.org/usage/#command-line-usage-relative-urls
      $arguments[] = '--relative-urls';
    }
    
    // Add any defined variables.
    foreach ($this->modify_variables as $modify_variable_name => $modify_variable_value) {

      /**
       * @link http://lesscss.org/usage/#command-line-usage-modify-variable
       */
      $arguments[] = '--modify-var=' . escapeshellarg($modify_variable_name . '=' . $modify_variable_value);
    }
    
    // Set source map flags.
    if ($this->source_maps_enabled) {
      
      if (isset($this->source_map_rootpath)) {
        
        $arguments[] = '--source-map-rootpath=' . escapeshellarg($this->source_map_rootpath);
      }
      
      if (isset($this->source_map_basepath)) {
        
        $arguments[] = '--source-map-basepath=' . escapeshellarg($this->source_map_basepath);
      }

      /**
       * @link http://lesscss.org/usage/#command-line-usage-source-map-map-inline
       */
      $arguments[] = '--source-map-map-inline';
    }
    
    // Input file should be last argument.
    // @link http://lesscss.org/usage/#command-line-usage-command-line-usage
    $arguments[] = $this->input_file;
    
    return $arguments;
  }
  
  /**
   * Returns list of files that input file depends on.
   * 
   * @return string[]
   *   List of @import'ed files.
   */
  public function depends() {
    
    $output_key = 'depends';
    
    $depends_arguments = array();
    
    $depends_arguments[] = '--depends';
    
    $depends_arguments[] = drupal_realpath(LESS_DIRECTORY) . DIRECTORY_SEPARATOR . $output_key;

    $depends_files_spaced = $this->proc_open(array_merge($this->command_arguments(), $depends_arguments));
    
    // {$output_key}: /path/to/file/1 /path/to/file/2
    $depends_files_spaced = str_replace($output_key . ':', '', $depends_files_spaced);
    
    return explode(' ', trim($depends_files_spaced));
  }
  
  /**
   * Executes compilation of LESS input.
   * 
   * @return string
   *   Compiled CSS.
   */
  public function compile() {
    
    return $this->proc_open($this->command_arguments());
  }

  /**
   * Execute compilation command through proc_open().
   *
   * @param string[] $command_arguments
   *
   * @return null|string
   * @throws Exception
   *
   * @see proc_open()
   */
  private function proc_open(array $command_arguments = array()) {
    
    $output_data = NULL;
    
    $command = implode(' ', array_merge(array(self::BASE_COMMAND), $command_arguments));
    
    // Handles for data exchange.
    $pipes = array(
      0 => NULL, // STDIN
      1 => NULL, // STDOUT
      2 => NULL, // STDERR
    );
    
    // Sets permissions on $pipes.
    $descriptors = array(
      0 => array('pipe', 'r'), // STDIN
      1 => array('pipe', 'w'), // STDOUT
      2 => array('pipe', 'w'), // STDERR
    );

    try {

      $process = proc_open($command, $descriptors, $pipes);

      if (is_resource($process)) {

        fclose($pipes[0]); // fclose() on STDIN executes $command, if program is expecting input from STDIN.

        $output_data = stream_get_contents($pipes[1]);
        fclose($pipes[1]);

        $error = stream_get_contents($pipes[2]);
        fclose($pipes[2]);

        if (!empty($error)) {
          throw new Exception($error);
        }

        proc_close($process);
      }
    }
    catch (Exception $e) {

      throw $e;
    }
    
    return $output_data;
  }
}
