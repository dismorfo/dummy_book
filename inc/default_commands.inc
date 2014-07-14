<?php

/** list of commands */

function default_commands () {
  return array(

    array(
      'label' => t('Create dummy content'),
      'callback' => array(
        'create_dummy_book',
      ),
    ),

  );
}

/** commands callback */

function fix() {

  $collections = file_scan_directory( __DIR__ . '/dummy_book/source/collections', '/(.*)\.xml$/', array('recurse' => FALSE ));

  $partners = file_scan_directory( __DIR__ . '/dummy_book/source/partners', '/(.*)\.xml$/', array('recurse' => FALSE ));

  $series = file_scan_directory( __DIR__ . '/dummy_book/source/series', '/(.*)\.xml$/', array('recurse' => FALSE ));

  foreach ($collections as $key => $value) {

    $xml = simplexml_load_file($value->uri);

    foreach($xml->children() as $child) {
  
      $type = (string)$child->type[0];

      $node = array(
        'title' => (string)$child->title[0],
        'status' => (string)$child->status[0],
        'type' => 'dlts_collection',
        'language' => (string)$child->language[0],
        'identifier' => (string)$child->identifier[0],
        'code' => (string)$child->code[0],
        'partner' => (string)$child->partner[0],
        'name' => (string)$child->name[0],        
      );
  
      create_dlts_collection( $node );

    }

  }

  foreach ($partners as $key => $value) {

    $xml = simplexml_load_file($value->uri);

    foreach($xml->children() as $child) {
  
    $type = (string)$child->type[0];

    $node = array(
        'title' => (string)$child->title[0],
        'status' => (string)$child->status[0],
        'type' => 'dlts_partner',
        'language' => (string)$child->language[0],
        'identifier' => (string)$child->identifier[0],
        'code' => (string)$child->code[0],
        'name' => (string)$child->name[0],
    );

      create_dlts_partner( $node  );

    }

  }

  foreach ($series as $key => $value) {

    $xml = simplexml_load_file($value->uri);

    foreach($xml->children() as $child) {
   
      $type = (string)$child->type[0];

      $node = array(
        'title' => (string)$child->title[0],
        'status' => (string)$child->status[0],
        'type' => 'dlts_series',
        'language' => (string)$child->language[0],
        'identifier' => (string)$child->identifier[0],
        'representative_image' => (string)$child->representative_image[0],
        'handle' => (string)$child->handle[0],
      );

      create_dlts_series( $node );

    }

  }

}

function create_dummy_multi_volume() {
	
  $multi_volume = file_scan_directory( __DIR__ . '/dummy_book/source/multivol', '/(.*)\.xml$/', array('recurse' => FALSE ));
  
  foreach ($multi_volume as $key => $value) {

    $xml = simplexml_load_file($value->uri);

    foreach($xml->children() as $child) {
  
      $node = array(
        'title' => (string)$child->title[0],
        'identifier' => (string)$child->identifier[0],
        'representative_image' => (string)$child->representative_image[0],
        'handle' => (string)$child->handle[0],
      );

	  $entity = create_dlts_multivol( $node );

	  if ($entity && isset($entity->nid)) {

		$action = (!$entity->exists) ? 'created' : 'updated';

        drush_log(
		  t('Multi volume "@title" was @action.', array('@title' => $entity->title, '@action' => $action)), 'ok'
		);

	  }

    }

  }
  	
}

function create_dummy_book() {
  
  
  create_dummy_multi_volume();
  
  fix();

  create_dlts_book(
    file_scan_directory( __DIR__ . '/dummy_book/source/books', '/(.*)\.xml$/', array('recurse' => FALSE ))
  );

}