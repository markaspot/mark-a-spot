<?php

/**
 * Interface \LessEngineInterface
 */
interface LessEngineInterface {

  /**
   * Set list of lookup directories for @import statements.
   *
   * @param string[] $directories
   *   Flat array of paths relative to DRUPAL_ROOT.
   */
  public function setImportDirectories(array $directories);

  /**
   * Enable
   *
   * @param bool $enabled
   *   Set the source maps flag.
   * @param string $base_path
   *   Leading value to be stripped from each source map URL.
   *   @link http://lesscss.org/usage/#command-line-usage-source-map-basepath
   * @param string $root_path
   *   Value to be prepended to each source map URL.
   *   @link http://lesscss.org/usage/#command-line-usage-source-map-rootpath
   */
  public function setSourceMaps($enabled, $base_path, $root_path);

  /**
   * Set/override variables.
   *
   * Variables defined here work in the "modify" method. They are applied after
   * parsing but before compilation.
   *
   * @param string[] $variables
   *
   * @link http://lesscss.org/usage/#command-line-usage-modify-variable
   */
  public function modifyVariables(array $variables);

  /**
   * Returns list of dependencies.
   *
   * Returns a list of files that included through @import statements. This list
   * is used to determine if parent file needs to be recompiled based on changes
   * in dependencies.
   *
   * @return string[]
   *   List of paths relative to DRUPAL_ROOT
   */
  public function getDependencies();

  /**
   * This returns the compiled output from the LESS engine.
   *
   * All output, including source maps, should be contained within the returned
   * string.
   *
   * @return string
   *   Plain CSS.
   *
   * @throws Exception
   *   Rethrows exception from implementation library.
   */
  public function compile();
}
