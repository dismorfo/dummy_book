<?php

function create_dlts_multivol_book($node) {
	
  global $user;
  
  $multivol_exist = multivol_nid($node['identifier']);  
  
  if (!$multivol_exist) return FALSE;

  $multivol_book_exist = multivol_book_nid($node['identifier'], $node['volume'], $node['collection']);
  
  if ($multivol_book_exist) {
  
    // Load the node by NID
    $entity = node_load($multivol_book_exist);

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
      'language' => LANGUAGE_NONE, // make sure we only translate the book object
    );
  
    $entity = entity_create('node', $values);
  
    // The entity is now created, but we have not yet simplified use of it.
    // Now create an entity_metadata_wrapper around the new node entity
    // to make getting and setting values easier
    $ewrapper = entity_metadata_wrapper('node', $entity);
  
  }
  
  $ewrapper->title->set( $node['title'] );
  
  if ($node['collection']) {
  
    $nid = collection_nid($node['collection']);
    
  	if ($nid) {
      $ewrapper->field_collection->set( array( $nid ) );
  	} 
	else {
	    drush_log( t('Multi Volume Book "@title" is part of collection "@collection", but collection does not exist. Skip.', array( '@title' => $node['title'], '@collection' => $collection, ) ), 'warning' );
	}
  }
  
  if ($node['identifier']) {
    $ewrapper->field_identifier->set( uniqid() );
  }
  
  if ($multivol_exist) {
    $ewrapper->field_multivol->set( $multivol_exist );
  }  

  if ($node['volume']) {
    $ewrapper->field_volume_number->set( (int)$node['volume'] );
  }
  
  if ($node['book']) {
  
    $book_exist = book_nid($node['book'], 'en');
    
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