<?php

function create_dlts_multivol( $node ) {
	
  global $user;
  
  $exists = multivol_nid($node['identifier']);
  
  if (!$exists) {
  
    // entity_create replaces the procedural steps in the first example of
    // creating a new object $node and setting its 'type' and uid property
    $values = array(
      'type' => 'dlts_multivol',
      'uid' => $user->uid,
      'status' => 1,
      'comment' => 0,
      'promote' => 0,
    );
  
    $entity = entity_create('node', $values);
	
  }
  
  else {

    $entity = node_load($exists);
	  
  }
  
  $entity->language = LANGUAGE_NONE;
  
  // The entity is now created, but we have not yet simplified use of it.
  // Now create an entity_metadata_wrapper around the new node entity
  // to make getting and setting values easier
  $ewrapper = entity_metadata_wrapper('node', $entity);
  
  // Using the wrapper, we do not have to worry about telling Drupal
  // what language we are using. The Entity API handles that for us.
  $ewrapper->title->set( $node['title'] );

  $ewrapper->field_identifier->set( $node['identifier'] );
  
  $ewrapper->field_handle->set( array('url' => $node['handle'] ) );

  // $ewrapper->representative_image->set( array('url' => $node['representative_image'] ) );
  
  $ewrapper->save();

  return $entity;

}