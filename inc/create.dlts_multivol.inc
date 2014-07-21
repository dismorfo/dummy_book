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

function create_dlts_multivol_from_xml($files) {
    
  foreach ($files as $file) {
      
    if ( 
      is_object($file) &&
      isset($file->uri)
    )
    {
          
      $xml = simplexml_load_file( $file->uri );

      if ($xml) {

        foreach($xml->children() as $child) {
  
          $node = array(
            'title' => (string)$child->title[0],
            'identifier' => (string)$child->identifier[0],
            'representative_image' => (string)$child->representative_image[0],
            'handle' => (string)$child->handle[0],
          );

          $entity = create_dlts_multivol( $node );

          if (
            $entity && 
            isset($entity->nid)
          )
          {

            $action = (!$entity->exists) ? 'created' : 'updated';

            drush_log( t( 'Multi volume "@title" was @action.', array( '@title' => $entity->title, '@action' => $action ) ), 'ok' );

          }

        }

      }
      
    }
    
  }

}