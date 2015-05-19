<?php

/**
 * Class \LessEngine
 */
abstract class LessEngine implements LessEngineInterface {

  /**
   * Path to the input .less file.
   *
   * @var string
   */
  protected $input_file_path;

  /**
   * This will get populated with a list of files that $input_file_path depended
   * on through @import statements.
   *
   * @var string[]
   */
  protected $dependencies = array();

  /**
   * This contains any variables that are to be modified into the output.
   *
   * Key => value pairs, where the key is the LESS variable name.
   *
   * @var string[]
   */
  protected $variables = array();

  /**
   * List of directories that are to be used for @import lookups.
   *
   * @var string[]
   */
  protected $import_directories = array();

  /**
   * Flag if source maps are enabled.
   *
   * @var bool
   */
  protected $source_maps_enabled = FALSE;

  /**
   * @var string|NULL
   */
  protected $source_maps_base_path = NULL;

  /**
   * @var string|NULL
   */
  protected $source_maps_root_path = NULL;

  /**
   * Basic constructor.
   *
   * Sets input_file_path property.
   *
   * @param string $input_file_path
   */
  public function __construct($input_file_path) {

    $this->input_file_path = $input_file_path;
  }

  /**
   * {@inheritdoc}
   */
  public function setImportDirectories(array $directories) {

    $this->import_directories = $directories;
  }

  /**
   * {@inheritdoc}
   */
  public function setSourceMaps($enabled = FALSE, $base_path = NULL, $root_path = NULL) {

    $this->source_maps_enabled = $enabled;
    $this->source_maps_base_path = $base_path;
    $this->source_maps_root_path = $root_path;
  }

  /**
   * {@inheritdoc}
   */
  public function modifyVariables(array $variables) {

    $this->variables = $variables;
  }

  /**
   * {@inheritdoc}
   */
  public function getDependencies() {

    return $this->dependencies;
  }

  /**
   * {@inheritdoc}
   */
  abstract public function compile();
}
