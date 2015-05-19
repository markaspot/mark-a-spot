<?php

/**
 * Class \LessEngineLessphp
 */
class LessEngineLessphp extends LessEngine {

  /**
   * @var \lessc
   */
  private $less_php_parser;

  /**
   * Instantiates new instances of \lessc.
   *
   * @param string $input_file_path
   *
   * @see \lessc
   */
  public function __construct($input_file_path) {

    parent::__construct($input_file_path);

    $this->less_php_parser = new lessc();
  }

  /**
   * {@inheritdoc}
   * This compiles using engine specific function calls.
   */
  public function compile() {

    $compiled_styles = NULL;

    try {

      foreach ($this->import_directories as $directory) {
        $this->less_php_parser->addImportDir($directory);
      }

      $cache = $this->less_php_parser->cachedCompile($this->input_file_path);

      $this->dependencies = array_keys($cache['files']);

      $compiled_styles = $cache['compiled'];
    }
    catch (Exception $e) {

      throw $e;
    }

    return $compiled_styles;
  }
}
