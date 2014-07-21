<?php

function create_dlts_collection( $node ) {

  global $user;
  
  $exists = collection_nid( $node['identifier'] );
  
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
      'type' => 'dlts_collection',
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
    $ewrapper->title->set( 'collection ' . $uniqid );
  }
  
  if ( isset( $node['identifier'] ) ) {
    $ewrapper->field_identifier->set( $node['identifier'] );
  }
  else {
    $ewrapper->title->set( 'collection ' . $uniqid );
  }
  
  if ( isset( $node['code'] ) ) {
    $ewrapper->field_code->set( $node['code'] );
  }

  if ( isset( $node['name'] ) ) {
    $ewrapper->field_name->set( $node['name'] );
  }  

  if ( isset( $node['partner'] ) ) {
    $partner_nid = partner_nid($node['partner']);
    if ($partner_nid) {
      $ewrapper->field_partner->set( array( $partner_nid ));
    }
  }  

  $ewrapper->save();

  return $entity;

}

function create_dlts_collection_from_xml($files) {
    
  foreach ($files as $file) {
      
    if ( 
      is_object($file) &&
      isset($file->uri)
    )
    {
          
      $xml = simplexml_load_file($file->uri);

      if ($xml) {
            
        foreach($xml->children() as $child) {
  
          $type = (string)$child->type[0];

          $node = array(
            'title' => (string)$child->title[0],
            'identifier' => (string)$child->identifier[0],
            'code' => (string)$child->code[0],
            'partner' => (string)$child->partner[0],
            'name' => (string)$child->name[0],
          );

          $entity = create_dlts_collection( $node );

          if (
            $entity && 
            isset($entity->nid)
          )
          {

            $action = (!$entity->exists) ? 'created' : 'updated';

            drush_log( t( 'Collection "@title" was @action.', array( '@title' => $entity->title, '@action' => $action ) ), 'ok' );

          }

        }

      }
        
      else {
       drush_log( t( 'There was a problem reading file @uri as XML.', array( '@uri' => $value->uri ) ), 'error' );   
      }
    
    }
      
    else {
      drush_log( t('There was a problem reading file.'), 'error' );
    }
          
  }
    
}