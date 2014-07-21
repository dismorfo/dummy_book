<?php

function create_dlts_book_stitched_page( $node ) {

  global $user;

  $book_exist = book_nid( $node['is_part_of'], 'en' );

  if (!$book_exist) return;

  $stitched_page_exist = stitched_page_nid($book_exist, $node['sequence_number_left'], $node['sequence_number_right']);

  if ($stitched_page_exist) {

    // Load the node by NID
    $entity = node_load($stitched_page_exist);

    // Wrap it with Entity API
    $ewrapper = entity_metadata_wrapper('node', $entity);

  }

  else {
  
    // entity_create replaces the procedural steps in the first example of
    // creating a new object $node and setting its 'type' and uid property
    $values = array(
      'type' => 'dlts_book_stitched_page',
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

  $ewrapper->field_book->set( $book_exist );

  if (!empty($node['is_part_of'])) {
    $ewrapper->field_is_part_of->set( $node['is_part_of'] );
  }
  
  if (!empty($node['sequence_number_left'])) {
    $ewrapper->field_sequence_number_left->set( (int) $node['sequence_number_left'] );
  }

  if (!empty($node['page_number_left'])) {
    $ewrapper->field_page_number_left->set( (int) $node['page_number_left'] );
  }  

  if (!empty($node['page_number_right'])) {
    $ewrapper->field_page_number_right->set( (int) $node['page_number_right'] );
  }  

  if (!empty($node['sequence_number_right'])) {
    $ewrapper->field_sequence_number_right->set( (int) $node['sequence_number_right'] );
  }  

  if (!empty($node['stitch_image'])) {

    // send FALSE overwrite while developing
    $stitch_image = import_add_file($node['stitch_image'], FALSE);

    if (
      $stitch_image
    ) 
    {
      /** the old way */
      $entity->field_stitch_image['und'][0] = (array)$stitch_image;
    }

  }

  $ewrapper->save();

  return $entity;

}