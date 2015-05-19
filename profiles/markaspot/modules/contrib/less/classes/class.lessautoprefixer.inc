<?php

/**
 * @file
 * Contains 'LessAutoprefixer' class; an abstraction layer for command line Autoprefixer.
 */

/**
 * 'Autoprefixer' class.
 */
class LessAutoprefixer {

  // Base command is hardcoded here to reduce security vulnerability.
  const BASE_COMMAND = 'autoprefixer';
  
  protected $input_file = NULL;
  
  protected $source_maps_enabled = FALSE;
  
  /**
   * Constructor function for 'LessAutoprefixer'.
   * 
   * @param string $input_file
   *   Path for .less file relative to getcwd().
   */
  protected function __construct($input_file) {
    
    $this->input_file = $input_file;
  }

  /**
   * @param string $input_file
   *
   * @return LessAutoprefixer
   */
  public static function create($input_file) {

    return new self($input_file);
  }
  
  /**
   * Returns the version string from command line Autoprefixer.
   * 
   * @return string|null
   *   Version string from Autoprefixer, or null if no version found.
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
   * Enable source maps for current file, and configure source map paths.
   * 
   * @param bool $enabled
   *   Set the source maps flag.
   */
  public function source_maps($enabled) {
    $this->source_maps_enabled = $enabled;
  }
  
  /**
   * Provides list to command line arguments for execution.
   * 
   * @return array
   *   Array of command line arguments.
   */
  protected function command_arguments() {
    
    $arguments = array();
    
    // Set service map flags.
    if ($this->source_maps_enabled) {
      
      $arguments[] = '--map';
      $arguments[] = '--inline-map';
    }
    
    // Input file should be last argument.
    $arguments[] = $this->input_file;
    
    return $arguments;
  }
  
  /**
   * Executes auto-prefixing of LESS output file.
   * 
   * @return string
   *   Compiled CSS.
   */
  public function compile() {
    
    return $this->proc_open($this->command_arguments());
  }
  
  protected function proc_open($command_arguments = array()) {
    
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
