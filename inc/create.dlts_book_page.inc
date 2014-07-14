<?php

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