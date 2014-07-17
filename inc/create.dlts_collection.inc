<?php

function create_dlts_collection( $node ) {

  global $user;
  
  $exist = collection_nid($node['identifier']);
  
  if ($exist) {
  
    // Load the node by NID
    $entity = node_load($exist);
    
    // Wrap it with Entity API
    $ewrapper = entity_metadata_wrapper('node', $entity);

  }
  
  else {
  
    // entity_create replaces the procedural steps in the first example of
    // creating a new object $node and setting its 'type' and uid property
    $values = array(
      'type' => 'dlts_collection',
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

  // Using the wrapper, we do not have to worry about telling Drupal
  // what language we are using. The Entity API handles that for us.
  $ewrapper->title->set( $node['title'] );

  $ewrapper->field_code->set( $node['code'] );

  $ewrapper->field_name->set( $node['name'] );  
  
  $ewrapper->field_identifier->set( $node['identifier'] );    

  $partner_nid = partner_nid($node['partner']);

  if ($partner_nid) {
    $ewrapper->field_partner->set( array( $partner_nid ));
  }

  $ewrapper->save();

  return $entity;

}