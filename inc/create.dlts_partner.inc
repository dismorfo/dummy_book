<?php

function create_dlts_partner( $node ) {
	
  global $user;
  
  $exists = partner_nid($node['identifier']);  

  if ($exists) {

    // Load the node by NID
    $entity = node_load($exists);

    // Wrap it with Entity API
    $ewrapper = entity_metadata_wrapper('node', $entity);

  }
  
  else {
  
    // entity_create replaces the procedural steps in the first example of
    // creating a new object $node and setting its 'type' and uid property
    $values = array(
      'type' => 'dlts_partner',
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
  
  $entity->exists = $exists;

  $uniqid = (string) uniqid();

  if ( isset( $node['title'] ) ) {
    $ewrapper->title->set( $node['title'] );
  }
  else {
    $ewrapper->title->set( 'partner ' . $uniqid );
  }
  
  if ( isset( $node['identifier'] ) ) {
    $ewrapper->field_identifier->set( $node['identifier'] );
  }
  else {
    $ewrapper->field_identifier->set( $uniqid );
  }
  
  if ( isset( $node['code'] ) ) {
    $ewrapper->field_code->set( $node['code'] );
  }

  if ( isset( $node['name'] ) ) {
    $ewrapper->field_name->set( $node['name'] );
  } 
  
  $ewrapper->save();

  return $entity;

}

function create_dlts_partner_from_xml( $files ) {
    
  foreach ($files as $file) {
      
    if ( 
      is_object($file) &&
      isset($file->uri)
    )
    {
          
      $xml = simplexml_load_file( $file->uri );

      if ($xml) {

        foreach( $xml->children() as $child ) {
  
          $node = array(
            'title' => (string) $child->title[0],
            'identifier' => (string) $child->identifier[0],
            'code' => (string) $child->code[0],
            'name' => (string) $child->name[0],
          );

          $entity = create_dlts_partner( $node  );
          
          if (
            $entity && 
            isset($entity->nid)
          )
          {

            $action = (!$entity->exists) ? 'created' : 'updated';

            drush_log( t( 'Partner "@title" was @action.', array( '@title' => $entity->title, '@action' => $action ) ), 'ok' );

          }          

        }

      }
      
    }
    
  }

}