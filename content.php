<?php

/**
 * A command-line Drupal script to create a dummy book
 * drush -r /Users/albertoortizflores/tools/sites/book_test --user=1 --uri=http://localhost/book_test scr import_dummy_book.php
 */
function create_dummy_books($books) {

  foreach ($books as $value) {

    $xml = simplexml_load_file($value->uri);

    foreach($xml->node as $child) {

      $pages_array = $multivols = $other_versions = $isbns = $authors = $collections = $contributors = $creators = $editors = $publishers = $subjects = array();

      /** book pages */
      foreach($child->pages->children() as $page) {

        $page = (array) $page;

        $page['service_copy'] = (array) $page['service_copy'];

        $page['cropped_master'] = (array) $page['cropped_master'];

        $pages_array[] = (array) $page;

      }
    
      /** contributors */ 
      foreach($child->contributors->children() as $contributor) {
        $contributors[] = (string) $contributor[0];
      }

      /** authors */    
      foreach($child->authors->children() as $author) {
        $authors[] = (string) $author[0];
      }

      /** collections the book belong */
      foreach($child->isPartOf->collection as $collection) {
        $collections[] = (string) $collection[0];
      }
	  
	  foreach($child->isPartOf->multivol as $multivol) {
        $multivols[] = array(
          'identifier' => (string) $multivol[0],
          'volume' => (int) $multivol->attributes()->volume[0], 
		);
	  }

      /** creators */
      foreach($child->creators->children() as $creator) {
        $creators[] = (string) $creator[0];
      }      
      
      /** editor */
      foreach($child->editors->children() as $editor) {
        $editors[] = (string) $editor[0];
      }     

      /** publishers */
      foreach($child->publishers->children() as $publisher) {
        $publishers[] = (string) $publisher[0];
      }     
      
      /** subjects */
      foreach($child->subjects->children() as $subject) {
        $subjects[] = (string) $subject[0];
      }     
      
      /** isbns */
      foreach($child->isbns->children() as $isbn) {
        $isbns[] = (string) $isbn[0];
      }     
      
      /** other_versions */
      foreach($child->other_versions->children() as $other_version) {
        $other_versions[] = array(
          'url' => (string) $other_version[0]
        );
      }           

      $book_identifier = (string)$child->identifier[0];
      
      $book_title = (string)$child->title[0];
      
      $representative_image_source = (string) $child->representative_image->uri;
      
      $representative_image = add_file( __DIR__ . '/source/images/' . $representative_image_source);

      $node = array(
        'title' => $book_title,
        'title_long' => (string)$child->title_long[0],        
        'status' => (string) $child->status[0],
        'language' => (string) $child->language[0],
        'identifier' => (string) $child->identifier[0],
        'subtitle' => (string) $child->subtitle[0],
        'page_count' => (int) $child->page_count[0],
        'binding_orientation' => (int)$child->binding_orientation[0],
        'call_number' => (string)$child->call_number[0],
        'description' => (string)$child->description[0],
        'dimensions' => (string)$child->dimensions[0],
        'handle' => (string) $child->handle[0],
        'language' => (string)$child->language[0],
        'language_code' => (string)$child->language_code[0],
        'number' => (string)$child->number[0],
        'other_version' => $other_versions,
        'pdf_file' => (string)$child->pdf_file[0],
        'read_order' => (int)$child->read_order[0],
        'representative_image' => $representative_image,
        'scan_order' => (int)$child->scan_order[0],
        'scanning_notes' => (string)$child->scanning_notes[0],
        'volume' => (string)$child->volume[0],
        'publication_date' => (string)$child->publication_date[0],
        'scan_date' => (string)$child->scan_date[0],
        'isbn' => $isbns,
        'identifier' => $book_identifier,
        'collections' => $collections,
        'author' => $authors,
        'creator' => $creators,
        'contributor' => $contributors,
        'sequence_count' => count($pages_array),
        'editor' => $editors,
        'publisher' => $publishers,
        'subjects' => $subjects,
        'pages' => $pages_array,
        'multivol' => $multivols,        
      );
	  
      $created = create_dlts_book($node);

      if ($created->nid) {

        drush_log(t('Book "@title" was successfully processed. (nid: @nid)', array('@nid' => $created->nid, '@title' => $created->title )), 'ok');

        foreach($pages_array as $key => $page) {

          $cropped_master_filename = $page['cropped_master']['uri'];

          $service_copy_filename = $page['service_copy']['uri'];

          $cropped_master = add_file( __DIR__ . '/source/images/' . $cropped_master_filename);

          $service_copy = add_file( __DIR__ . '/source/images/' . $service_copy_filename);

          $title = (isset($page['title'])) ? $page['title'] : $book_title . ' ' . t('page @number', array('@number' => $page['sequence_number']));

          $page_node = array(
            'book' => $created->nid,
            'status' => 1,            
            'language' => LANGUAGE_NONE, // only the book object can have multiple language
            'is_part_of' => $book_identifier,
            'title' => $title,
            'hand_side' => $page['hand_side'],
            'page_type' => $page['page_type'],
            'real_page_number' => $page['real_page_number'],
            'sequence_number' => $page['sequence_number'],
            'visible' => $page['visible'],
            'cropped_master' => $cropped_master,  
            'service_copy' => $service_copy,          
          );

          $page = create_dlts_book_page( $page_node );

          if ($page->nid) {
             drush_log( t('Book page "@title" part of "@book" was successfully processed. (nid: @nid)', array('@nid' => $page->nid, '@title' => $page->title, '@book' => $created->title )), 'ok');
          }
           
        }
      }
    }
  }
}

create_dummy_books(
  file_scan_directory( __DIR__ . '/source/books', '/(.*)\.xml$/', array('recurse' => FALSE ))
);

function init() {

  $collections = file_scan_directory( __DIR__ . '/source/collections', '/(.*)\.xml$/', array('recurse' => FALSE ));

  $partners = file_scan_directory( __DIR__ . '/source/partners', '/(.*)\.xml$/', array('recurse' => FALSE ));

  $series = file_scan_directory( __DIR__ . '/source/series', '/(.*)\.xml$/', array('recurse' => FALSE ));

  $multi_volume = file_scan_directory( __DIR__ . '/source/multi_volume', '/(.*)\.xml$/', array('recurse' => FALSE ));

  foreach ($collections as $key => $value) {

    $xml = simplexml_load_file($value->uri);

    foreach($xml->children() as $child) {
  
      $type = (string)$child->type[0];
    
      $node = array(
        'title' => (string)$child->title[0],
        'status' => (string)$child->status[0],
        'type' => $type,
        'language' => (string)$child->language[0],
        'identifier' => (string)$child->identifier[0],
        'code' => (string)$child->code[0],
        'partner' => (string)$child->partner[0],
        'name' => (string)$child->name[0],        
      );
  
      create_base_node( $node, 'create_' . $type );

    }

  }

  foreach ($partners as $key => $value) {

    $xml = simplexml_load_file($value->uri);

    foreach($xml->children() as $child) {
  
    $type = (string)$child->type[0];

    $node = array(
        'title' => (string)$child->title[0],
        'status' => (string)$child->status[0],
        'type' => $type,
        'language' => (string)$child->language[0],
        'identifier' => (string)$child->identifier[0],
        'code' => (string)$child->code[0],
        'name' => (string)$child->name[0],
    );

      create_base_node( $node, 'create_' . $type );

    }

  }

  foreach ($series as $key => $value) {

    $xml = simplexml_load_file($value->uri);

    foreach($xml->children() as $child) {
   
      $type = (string)$child->type[0];

      $node = array(
        'title' => (string)$child->title[0],
        'status' => (string)$child->status[0],
        'type' => $type,
        'language' => (string)$child->language[0],
        'identifier' => (string)$child->identifier[0],
        'representative_image' => (string)$child->representative_image[0],
        'handle' => (string)$child->handle[0],
      );
  
      create_base_node( $node, 'create_' . $type );

    }

  }

  foreach ($multi_volume as $key => $value) {

    $xml = simplexml_load_file($value->uri);

    foreach($xml->children() as $child) {
  
      $type = (string)$child->type[0];
    
      $node = array(
        'title' => (string)$child->title[0],
        'status' => (string)$child->status[0],
        'type' => $type,
        'language' => (string)$child->language[0],
        'identifier' => (string)$child->identifier[0],
        'representative_image' => (string)$child->representative_image[0],
        'handle' => (string)$child->handle[0],
      );
  
      create_base_node( $node, 'create_' . $type );

    }

  }  

}

function collection_nid($identifier) {

  $query = new EntityFieldQuery;
  
  $result = $query
    ->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'dlts_collection')
    ->fieldCondition('field_identifier', 'value', $identifier, '=')
    ->execute();
    
  if (empty($result['node'])) return FALSE;

  $keys = array_keys($result['node']);

  return (int)$keys[0];

}

function multivol_nid($identifier) {

  $query = new EntityFieldQuery;
  
  $result = $query
    ->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'dlts_multivol')
    ->fieldCondition('field_identifier', 'value', $identifier, '=')
    ->execute();
    
  if (empty($result['node'])) return FALSE;

  $keys = array_keys($result['node']);

  return (int)$keys[0];

}

function multivol_book_nid($identifier, $volume_page) {

  $query = new EntityFieldQuery;
  
  $result = $query
    ->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'dlts_multivol')
    ->fieldCondition('field_identifier', 'value', $identifier, '=')
	->fieldCondition('field_volume_number', 'value', $volume_page, '=')
    ->execute();
    
  if (empty($result['node'])) return FALSE;

  $keys = array_keys($result['node']);

  return (int)$keys[0];

}

function book_page_nid($book_nid, $sequence_number) {

  $query = new EntityFieldQuery;
  
  $result = $query
    ->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'dlts_book_page')
    ->fieldCondition('field_sequence_number', 'value', $sequence_number, '=')
    ->fieldCondition('field_book', 'nid', $book_nid, '=')    
    ->execute();
    
  if (empty($result['node'])) return FALSE;

  $keys = array_keys($result['node']);

  return (int)$keys[0];

}

function book_nid($identifier) {

  $query = new EntityFieldQuery;
  
  $result = $query
    ->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'dlts_book')
    ->fieldCondition('field_identifier', 'value', $identifier, '=')
    ->execute();
    
  if (empty($result['node'])) return FALSE;

  $keys = array_keys($result['node']);

  return (int)$keys[0];

}

function create_dlts_book_page( $node ) {

  global $user;

  $book_exist = book_nid($node['is_part_of']);
  
  if (!$book_exist) return;
  
  $page_exist = book_page_nid($book_exist, $node['sequence_number']);  
  
  if ($page_exist) {
  
    // Load the node by NID
    $entity = node_load($page_exist);
    
    // Wrap it with Entity API
    $ewrapper = entity_metadata_wrapper('node', $entity);
    
  }
  
  else {
  
    // entity_create replaces the procedural steps in the first example of
    // creating a new object $node and setting its 'type' and uid property
    $values = array(
      'type' => 'dlts_book_page',
      'uid' => $user->uid,
      'status' => 1,
      'comment' => 0,
      'promote' => 0,
    );
  
    $entity = entity_create('node', $values);
  
    // The entity is now created, but we have not yet simplified use of it.
    // Now create an entity_metadata_wrapper around the new node entity
    // to make getting and setting values easier
    $ewrapper = entity_metadata_wrapper('node', $entity);
  
  }
  
  $ewrapper->title->set( $node['title'] );
  
  $entity->language = $node['language'];

  $ewrapper->field_book->set( $node['book'] );

  $ewrapper->field_is_part_of->set( $node['is_part_of'] );

  $ewrapper->field_hand_side->set( $node['hand_side'] );

  $ewrapper->field_page_type->set( $node['page_type'] );

  $ewrapper->field_real_page_number->set( $node['real_page_number'] );

  $ewrapper->field_sequence_number->set( $node['sequence_number'] );

  $ewrapper->field_visible->set( $node['visible'] );
  
  $ewrapper->field_service_copy->set( array('fid' => $node['service_copy']->fid ) ); 
  
  /** the old way */
  $entity->field_cropped_master['und'][0] = (array)$node['cropped_master'];

  $ewrapper->save();

  return $entity;

}

function create_dlts_multivol_book($node) {
	
  global $user;
  
  $multivol_book_exist = multivol_book_nid($node['identifier'], $node['volume_number']);
  
  if ($multivol_book_exist) {
  
    // Load the node by NID
    $entity = node_load($book_exist);
    
    // Wrap it with Entity API
    $ewrapper = entity_metadata_wrapper('node', $entity);

  }
  
  else {
  
    // entity_create replaces the procedural steps in the first example of
    // creating a new object $node and setting its 'type' and uid property
    $values = array(
      'type' => 'dlts_multivol_book',
      'uid' => $user->uid,
      'status' => 1,
      'comment' => 0,
      'promote' => 0,
    );
  
    $entity = entity_create('node', $values);
  
    // The entity is now created, but we have not yet simplified use of it.
    // Now create an entity_metadata_wrapper around the new node entity
    // to make getting and setting values easier
    $ewrapper = entity_metadata_wrapper('node', $entity);
  
  }
  
  $ewrapper->title->set( $node['title'] );
  
  if ($node['collections']) {

  	$collections_nids = array();

    foreach($node['collections'] as $collection) {
  	  $nid = collection_nid($collection);
  	  if ($nid) {
        $collections_nids[] = $nid;
	  }
	  else {
	    drush_log( t('Multi Volume Book "@title" is part of collection "@collection", but collection does not exist. Skip.', array( '@title' => $node['title'], '@collection' => $collection, ) ), 'warning' );
	  }
    }

	$ewrapper->field_collection->set( $collections_nids );

  }
  
  if ($node['identifier']) {
    $ewrapper->field_identifier->set( $node['identifier'] );
  }

  if ($node['volume_number']) {
    $ewrapper->field_volume_number->set( $node['volume_number'] );
  }
  
  if ($node['book']) {
    $book_exist = book_nid($node['book']);
    if ($book_exist) {
      $ewrapper->field_book->set( $book_exist ) ;
    }
  }
  
  if ($node['multivol']) {
  	
    $multivol_exist = multivol_nid($node['multivol']);
	  
	$multivol_node = node_load($multivol_exist);
	
    if ($multivol_exist) {
      $ewrapper->field_multivol->set( $multivol_exist ) ;
    }
  }  
  
  
  $ewrapper->save();
  
  return $entity;
  
}

function create_dlts_book( $node ) {

  global $user;
  
  $book_exist = book_nid($node['identifier']);
  
  if ($book_exist) {
  
    // Load the node by NID
    $entity = node_load($book_exist);
    
    // Wrap it with Entity API
    $ewrapper = entity_metadata_wrapper('node', $entity);

  }
  
  else {
  
    // entity_create replaces the procedural steps in the first example of
    // creating a new object $node and setting its 'type' and uid property
    $values = array(
      'type' => 'dlts_book',
      'uid' => $user->uid,
      'status' => 1,
      'comment' => 0,
      'promote' => 0,
    );
  
    $entity = entity_create('node', $values);
  
    // The entity is now created, but we have not yet simplified use of it.
    // Now create an entity_metadata_wrapper around the new node entity
    // to make getting and setting values easier
    $ewrapper = entity_metadata_wrapper('node', $entity);
  
  }
  
  $ewrapper->title->set( $node['title'] );
  
  $entity->language = $node['language']; // we only set language to the book object

  $pages = (array) $node['pages'];

  $publication_date = $node['publication_date'];

  $scan_date = $node['scan_date'];

  $handle = $node['handle'];
  
  $ewrapper->field_identifier->set( $node['identifier'] );
  
  $collections_nids = array();
  
  foreach($node['collections'] as $collection) {
  	$nid = collection_nid($collection);
  	if ($nid) {
      $collections_nids[] = $nid;
	}
	else {
	  drush_log( t('Book "@title" is part of collection "@collection", but collection does not exist. Skip.', array( '@title' => $node['title'], '@collection' => $collection, ) ), 'warning' );
	}
  }
  
  $multivol_nids[] = array();
  
  $ewrapper->field_collection->set( $collections_nids );
  
  if (!empty($node['isbn'])) {
    $ewrapper->field_isbn->set( $node['isbn'] );
  }

  if (!empty($node['handle'])) {
    $ewrapper->field_handle->set( array( 'url' => $node['handle'] ) );
  }
  
  if (!empty($node['title_long'])) {
    $ewrapper->field_title->set( $node['title_long'] );
  }  
  
  if (!empty($node['subtitle'])) {
    $ewrapper->field_subtitle->set( $node['subtitle'] );
  }  
  
  if (!empty($node['description'])) {
    $ewrapper->field_description->set( $node['description'] );
  }  
  
  if (!empty($node['page_count'])) {
    $ewrapper->field_page_count->set( $node['page_count'] );
  }  
  
  if (!empty($node['editor'])) {
    $ewrapper->field_editor->set( $node['editor'] );
  }

  if (!empty($node['creator'])) {
    $ewrapper->field_creator->set( $node['creator'] );
  }
  
  if (!empty($node['author'])) {
    $ewrapper->field_author->set( $node['author'] );
  }  
  
  if (!empty($node['publisher'])) {
    $ewrapper->field_publisher->set( $node['publisher'] );
  }  
  
  if (!empty($node['contributor'])) {
    $ewrapper->field_contributor->set( $node['contributor'] );
  }  
  
  if (!empty($node['dimensions'])) {
    $ewrapper->field_dimensions->set( $node['dimensions'] );
  }

  if (!empty($node['volume'])) {
    $ewrapper->field_volume->set( $node['volume'] );
  }
  
  if (!empty($node['number'])) {
    $ewrapper->field_number->set( $node['number'] );
  }    
  
  if (!empty($node['call_number'])) {
    $ewrapper->field_call_number->set( $node['call_number'] );
  }
  
  if (!empty($node['other_version'])) {
    $ewrapper->field_other_version->set( $node['other_version'] );
  }

  if (!empty($node['binding_orientation'])) {
    $ewrapper->field_binding_orientation->set( $node['binding_orientation'] );
  }

  if (!empty($node['scan_order'])) {    
    $ewrapper->field_scan_order->set( $node['scan_order'] );
  }

  if (!empty($node['read_order'])) {  
    $ewrapper->field_read_order->set( $node['read_order'] );
  }
  
  if (!empty($node['publisher'])) {
    $ewrapper->field_publisher->set( $node['publisher'] );  
  }
  
  if (!empty($node['representative_image'])) {
    $ewrapper->field_representative_image ->set( array('fid' => $node['representative_image']->fid ) ); 
  }
  
  $ewrapper->field_sequence_count->set( count($pages) );

  $ewrapper->save();
  
  /** make sure book exist before creating relationship */

  foreach($node['multivol'] as $multivol) {

  	$multivol_nid = multivol_nid($multivol['identifier']);
  	
  	if ($multivol_nid) {
  	  	
  	  // Volume exist; find book
  	  
  	  $multivol_book_nid = multivol_book_nid($multivol_nid, $multivol['volume']);

	  if (!$multivol_book_nid) {

	  	$multivol_node = node_load($multivol_nid);

        $multivol_book = array(
	      'title' => $multivol_node->title . ' ' . t('vol.') . ' ' . $multivol['volume'],	      
	      'identifier' => $multivol['identifier'] . '-volNumber-' . $multivol['volume'],
	      'book' => $node['identifier'],
	      'collections' => $node['collections'],
	      'multivol' => $multivol['identifier'],
	      'volume_number' => $multivol['volume'],
	    );

	    $multivol_book = create_dlts_multivol_book($multivol_book);

		if ($multivol_book->nid) {
		  drush_log('Multi Volume book relationship was created for book "@title"', array ('@title' => $node['title']), 'ok');
		}

	  }

	}
	
	else {
	  drush_log( t('Book "@title" is part of multi volume set "@multivol", but multi volume does not exist. Skip.', array( '@title' => $node['title'], '@multivol' => $multivol['identifier'], ) ), 'warning' );
	}
  }  
  
  //if (!empty($node['subjects'])) {
  //  $ewrapper->field_subject->set( $node['subjects'] );  
  //}
  
  return $entity;
  
}

function create_dlts_collection( $node, &$ewrapper, &$entity ) {

  // Using the wrapper, we do not have to worry about telling Drupal
  // what language we are using. The Entity API handles that for us.
  $ewrapper->title->set( $node['title'] );

  $ewrapper->field_code->set( $node['code'] );

  $ewrapper->field_name->set( $node['name'] );  
  
  $partner_nid = (int)$node['partner'];
  
  // Note that the entity id (e.g., nid) must be passed as an integer not a string
  $ewrapper->field_partner->set( array( $partner_nid ));

}

function create_dlts_partner( $node, &$ewrapper, &$entity ) {

  $ewrapper->field_identifier->set( $node['identifier'] );

  $ewrapper->field_code->set( $node['code'] );

  $ewrapper->field_name->set( $node['name'] );

}

function create_dlts_series( $node, &$ewrapper, &$entity ) {

  $ewrapper->field_identifier->set( $node['identifier'] );
  
  $ewrapper->field_handle->set( array('url' => $node['handle'] ) );

  // $ewrapper->representative_image->set( array('url' => $node['representative_image'] ) );

}

function create_dlts_multivol( $node, &$ewrapper, &$entity ) {

  $ewrapper->field_identifier->set( $node['identifier'] );
  
  $ewrapper->field_handle->set( array('url' => $node['handle'] ) );

  // $ewrapper->representative_image->set( array('url' => $node['representative_image'] ) );

}

function create_base_node( $node, $callback ) {

  global $user;

  // entity_create replaces the procedural steps in the first example of
  // creating a new object $node and setting its 'type' and uid property
  $values = array(
    'type' => $node['type'],
    'uid' => $user->uid,
    'status' => 1,
    'comment' => 0,
    'promote' => 0,
  );
  
  $entity = entity_create('node', $values);
  
  // The entity is now created, but we have not yet simplified use of it.
  // Now create an entity_metadata_wrapper around the new node entity
  // to make getting and setting values easier
  $ewrapper = entity_metadata_wrapper('node', $entity);
  
  // Using the wrapper, we do not have to worry about telling Drupal
  // what language we are using. The Entity API handles that for us.
  $ewrapper->title->set( $node['title'] );

  $callback( $node, $ewrapper, $entity );
  
  $ewrapper->save();
  
  return $entity;

}

function add_file($path) {

  global $user;

  $file = (object)array(
    'uid' => $user->uid,
    'uri' => $path,
    'filemime' => file_get_mimetype($path),
    'status' => 1,
  );

  $file = file_copy($file, 'public://', FILE_EXISTS_REPLACE);

  return $file;

}