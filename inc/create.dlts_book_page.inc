<?php

function create_dlts_book_page( $node ) {

  global $user;

  $book_exist = book_nid( $node['is_part_of'], 'en' );

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
  
  $entity->language = LANGUAGE_NONE;  
  
  $ewrapper->title->set( $node['title'] );

  $ewrapper->field_book->set( $book_exist );
  
  if (!empty($node['is_part_of'])) {
    $ewrapper->field_is_part_of->set( $node['is_part_of'] );
  }

  if (!empty($node['hand_side'])) {
    $ewrapper->field_hand_side->set( $node['hand_side'] );
  }

  if (!empty($node['page_type'])) {
    $ewrapper->field_page_type->set( $node['page_type'] );
  }

  if (!empty($node['real_page_number'])) {
    $ewrapper->field_real_page_number->set( $node['real_page_number'] );
  }

  if (!empty($node['sequence_number'])) {
    $ewrapper->field_sequence_number->set( $node['sequence_number'] );
  }

  if (!empty($node['visible'])) {
    $ewrapper->field_visible->set( $node['visible'] );
  }
  
  if (!empty($node['service_copy'])) {
      
    /**
     * Files folder real path
     */
    $dupal_file_path = drupal_realpath('public://');

    $localFile = $dupal_file_path . '/' . basename( $node['service_copy'] );

    $pathInfo = pathinfo($localFile);
    
    if (
        $pathInfo['extension'] != 'jpg' 
    ) 
    {

        if (!file_exists($localFile)) {
          $put_success = file_put_contents($localFile, import_get_content($node['service_copy']));   
        }
        else {
          $put_success = TRUE;
        }

        if ( $put_success ) {

          $localFile = image_to_jpg( $localFile, FALSE );

          $filemime = file_get_mimetype($localFile);

          $file = (object) array(
            'uid' =>  $user->uid,
            'uri' => $localFile,
            'filemime' => $filemime,
            'status' => 1,
          );

          $service_copy = file_copy($file, 'public://');
        }

    }
    
    else {
      $service_copy = import_add_file($node['service_copy'], FALSE);
    }
    
    if (
      $service_copy
    ) 
    {
      $ewrapper->field_service_copy->set(array('fid' => $service_copy->fid )); 
    }
  }

  if (!empty($node['cropped_master'])) {
    
    // send FALSE overwrite while developing
    $cropped_master = import_add_file($node['cropped_master'], FALSE);

    if (
      $cropped_master
    ) 
    {
      /** the old way */
      $entity->field_cropped_master['und'][0] = (array)$cropped_master;
    }

  }

  $ewrapper->save();

  return $entity;

}