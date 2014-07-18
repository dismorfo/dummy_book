<?php

/**
 * Create a taxonomy term and return the tid.
 */
function create_taxonomy_term($name, $vid, $parent_id = 0) {

  drush_log( t('Adding new term @term', array('@term' => $name) ), 'ok') ;

  $term = new stdClass();
  $term->name = $name;
  $term->vid = $vid;
  $term->parent = array($parent_id);
  
  taxonomy_term_save($term);
  
  return $term->tid;
  
}

foreach ( file_scan_directory( __DIR__, '/create.(.*)\.inc$/', array('recurse' => FALSE )) as $file) {	
  include_once($file->uri);
}