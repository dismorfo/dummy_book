<?php

/** I'm going Megaupload on you */
function delete_files() {
  db_delete('file_managed')
    ->condition('uri', 'kill em all', '<>')
    ->execute();
}

/** I just want to poke your left eye */
function delete_dlts_book_stitched_page() {
  foreach (find_nodes('dlts_book_stitched_page') as $node) {
    node_delete($node->nid);
  }
}

/** nat' today */
function delete_dlts_book_multivol_book() {
  foreach (find_nodes('dlts_multivol_book') as $node) {
    node_delete($node->nid);
  }   
}

/** to hell with all of you! */
function delete_dlts_books() {

  delete_dlts_book_page();

  delete_dlts_book_stitched_page();
  
  delete_dlts_book_multivol_book();

  foreach (find_nodes('dlts_book') as $node) {
    node_delete($node->nid);
  }
  
  delete_files();

}

function delete_dlts_book_page() {
  foreach (find_nodes('dlts_book_page') as $node) {
    node_delete($node->nid);
  }
}

foreach ( file_scan_directory( __DIR__, '/delete.(.*)\.inc$/', array('recurse' => FALSE )) as $file) {  
  include_once($file->uri);
}