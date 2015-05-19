<?php

/**
 * Class \LessEngineLess_php
 */
class LessEngineLess_php extends LessEngine {

  /**
   * @var \Less_Parser
   */
  private $less_php_parser;

  /**
   * Instantiates new instances of \Less_Parser.
   *
   * @param string $input_file_path
   *
   * @see \Less_Parser
   */
  public function __construct($input_file_path) {

    parent::__construct($input_file_path);

    $this->less_php_parser = new Less_Parser();
  }

  /**
   * {@inheritdoc}
   * This compiles using engine specific function calls.
   */
  public function compile() {

    $compiled_styles = NULL;

    try {

      if ($this->source_maps_enabled) {

        $this->less_php_parser->SetOption('sourceMap', $this->source_maps_enabled);

        $this->less_php_parser->SetOption('sourceMapBasepath', $this->source_maps_base_path);
        $this->less_php_parser->SetOption('sourceMapRootpath', $this->source_maps_root_path);
      }

      // Less.js does not allow path aliasing. Set aliases to blank for consistency.
      $this->less_php_parser->SetImportDirs(array_fill_keys($this->import_directories, ''));

      $this->less_php_parser->parseFile($this->input_file_path);

      $this->less_php_parser->ModifyVars($this->variables);

      $compiled_styles = $this->less_php_parser->getCss();

      $this->dependencies = $this->less_php_parser->AllParsedFiles();
    }
    catch (Exception $e) {

      throw $e;
    }

    return $compiled_styles;
  }
}
