<?php

/**
 * Class \LessEngineLess_js
 */
class LessEngineLess_js extends LessEngine {

  /**
   * @var \Lessjs
   */
  private $less_js_parser;

  /**
   * Instantiates new instances of \Lessjs.
   *
   * @param string $input_file_path
   *
   * @see \Lessjs
   */
  public function __construct($input_file_path) {

    parent::__construct($input_file_path);

    $this->less_js_parser = Lessjs::create($this->input_file_path);
  }

  /**
   * We override here because getting dependencies from less.js requires another
   * full parse. This way we only do that if dependencies are requested.
   *
   * @return string[]
   *
   * @see \Lessjs::depends()
   */
  public function getDependencies() {

    $this->dependencies = $this->less_js_parser->depends();

    return parent::getDependencies();
  }

  /**
   * {@inheritdoc}
   * This compiles using engine specific function calls.
   */
  public function compile() {

    $compiled_styles = NULL;

    try {

      $this->less_js_parser->source_maps($this->source_maps_enabled, $this->source_maps_base_path, $this->source_maps_root_path);

      foreach ($this->import_directories as $directory) {
        $this->less_js_parser->include_path($directory);
      }

      foreach ($this->variables as $var_name => $var_value) {
        $this->less_js_parser->modify_var(trim($var_name, '@'), trim($var_value, ';'));
      }

      $compiled_styles = $this->less_js_parser->compile();
    }
    catch (Exception $e) {

      throw $e;
    }

    return $compiled_styles;
  }
}
