<?php

/** list of commands */

function default_commands () {

  return array(

    array(
      'label' => t('Create dummy content'),
      'callback' => array(
        'create_dummy_collections',  
        'create_dummy_partners',
        'create_dummy_series',
        'create_dummy_multivol',
        'create_awdl_books',
        'create_liberator_books',        
      ),
    ),

    array(
      'label' => t('Import books from AWDL Drupal 6 site'),
      'callback' => array(
        'create_awdl_books',
      ),
    ),

    array(
      'label' => t('Delete all Books'),
      'callback' => array(
        'delete_dlts_books',
      ),
    ),    

    array(
      'label' => t('Delete all Book Pages'),
      'callback' => array(
        'delete_dlts_book_page',
      ),
    ),

    array(
      'label' => t('Delete all Book Stitched Pages'),
      'callback' => array(
        'delete_dlts_book_stitched_page',
      ),
    ),

    array(
      'label' => t('Delete all Multi-Volume Books'),
      'callback' => array(
        'delete_dlts_book_multivol_book',
      ),
    ),

    array(
      'label' => t('Delete all files from registry'),
      'callback' => array(
        'delete_files',
      ),
    ),  

    array(
      'label' => t('Run test'),
      'callback' => array(
        'test',
      ),
    ),    

  );
}


/** commands callback */

function test() {
    
}

function create_dummy_multivol() {
 create_dlts_multivol_from_xml(
    file_scan_directory( __DIR__ . '/dummy_book/source/multivol', '/(.*)\.xml$/', array('recurse' => FALSE ))
  );
}

function create_dummy_series() {
 create_dlts_collection_from_xml(
    file_scan_directory( __DIR__ . '/dummy_book/source/series', '/(.*)\.xml$/', array('recurse' => FALSE ))
  );
}

function create_dummy_partners() {
 create_dlts_collection_from_xml(
    file_scan_directory( __DIR__ . '/dummy_book/source/partners', '/(.*)\.xml$/', array('recurse' => FALSE ))
  );
}

function create_dummy_collections() {
 create_dlts_collection_from_xml(
    file_scan_directory( __DIR__ . '/dummy_book/source/collections', '/(.*)\.xml$/', array('recurse' => FALSE ))
  );
}

function create_awdl_books() {
  create_dlts_book_from_xml(
    file_scan_directory( __DIR__ . '/dummy_book/source/books/awdl', '/(.*)\.xml$/', array('recurse' => FALSE ))
  );
}

function create_liberator_books() {
  create_dlts_book_from_xml(
    file_scan_directory( __DIR__ . '/dummy_book/source/books/liberator', '/(.*)\.xml$/', array('recurse' => FALSE ))
  );
}
