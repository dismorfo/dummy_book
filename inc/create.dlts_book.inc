<?php

function save_dlts_book( $node, $language = 'en' ) {

  global $user;
  
  $node_language = ( isset($node['node_language']) ? $node['node_language'] : $language );
  
  $book_exist = book_nid( $node['identifier'], $node_language );
  
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
    
    if ( $node_language == LANGUAGE_NONE)
    {
      $node_language = 'en';
    }
    
    if ( $node_language != 'en' ) 
    {
        
      // find if the English version of this book exist
      $tnid = book_nid( $node['identifier'], 'en' );
      
      if ( $tnid ) 
      {
          
        // make sure the language exists
        $enabled_language = add_language($node_language);     
        
        // if language does not exists, do not add the book
        if (!$enabled_language) return FALSE;
             
        // load the source
        $source_node = node_load( $tnid );
        
        // wrap it with Entity API
        $source_node_ewrapper = entity_metadata_wrapper( 'node', $source_node );

        // set nid as tnid
        $source_node->tnid = $tnid;

        // save source
        $source_node_ewrapper->save(); 

        // not sure why this is needed
        $values['translate'] = 0;

        // set source nid as the tnid for the translation
        $values['tnid'] = $tnid;      

        // English is the default language for the books object
        $values['source'] = 'en';

      }
      
      // return as soon as possible
      else {

        drush_log( t( 'English version does not exist', 'error' ) );

        return FALSE;

      }

    }
    
    $values['language'] = $node_language;

    $entity = entity_create('node', $values);
  
    // The entity is now created, but we have not yet simplified use of it.
    // Now create an entity_metadata_wrapper around the new node entity
    // to make getting and setting values easier
    $ewrapper = entity_metadata_wrapper('node', $entity);

  }

  $collections_nids = 
    $subjects = 
    $pdf_files = array();

  // find the max for this field and trunk it
  $ewrapper->title->set( $node['title'] );

  if ( !empty( $node['identifier'] ) ) {
    $ewrapper->field_identifier->set( $node['identifier'] );
  }

  foreach ( $node['collections'] as $collection ) {

    $nid = collection_nid($collection);

    if ( $nid ) {
      $collections_nids[] = $nid;
    }
    
    else {
      drush_log( t( 'Book "@title" is part of collection "@collection", but collection does not exist. Skip.', array( '@title' => $node['title'], '@collection' => $collection, ) ), 'warning' );
    }
    
  }

  if ( !empty($collections_nids) ) {
    $ewrapper->field_collection->set( $collections_nids );
  }
  
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
    $ewrapper->field_binding_orientation->set( (int)$node['binding_orientation'] );
  }

  if (!empty($node['scan_order'])) {    
    $ewrapper->field_scan_order->set( (int)$node['scan_order'] );
  }
  
  if (!empty($node['page_count'])) {    
    $ewrapper->field_page_count->set( (int)$node['page_count'] );
  }

  if (!empty($node['sequence_count'])) {    
    $ewrapper->field_sequence_count->set( (int)$node['sequence_count'] );
  }

  if (!empty($node['read_order'])) {  
    $ewrapper->field_read_order->set( (int)$node['read_order'] );
  }
  
  if (!empty($node['publication_location'])) {  
    $ewrapper->field_publication_location->set( $node['publication_location'] );
  }  
  
  if (!empty($node['publisher'])) {
    $ewrapper->field_publisher->set( $node['publisher'] );
  }
  
  if (!empty($node['publication_date_text'])) {
    $ewrapper->field_publication_date_text->set( $node['publication_date_text'] );
  }  
  
  if (!empty($node['representative_image'])) {
    // send FALSE overwrite while developing
    $representative_image = import_add_file($node['representative_image'], FALSE);
    if ($representative_image) {
      $ewrapper->field_representative_image ->set(array('fid' => $representative_image->fid )); 
    }
  }

  if (!empty($node['language'])) {
    $ewrapper->field_language ->set( array( $node['language'] ) ); 
  }  

  if (!empty($node['language_code'])) {
    $ewrapper->field_language_code ->set( array( $node['language_code'] ) ); 
  }  

  if (!empty($node['pdf_files'])) {
    foreach ($node['pdf_files'] as $file) {
      // send FALSE overwrite while developing
      $content = import_add_file($file, FALSE);
	  if (isset($content) && isset($content->fid )) {
	    $pdf_files[] = array('fid' => $content->fid, 'display' => 0 );
	  }
    }
    $ewrapper->field_pdf_file->set( $pdf_files );
  }

  if (!empty($node['subjects'])) {
    $vocabulary = taxonomy_vocabulary_machine_name_load('subjects');
    foreach($node['subjects'] as $subject) {
      $term = taxonomy_get_term_by_name($subject);
      if ($term) {
        $subjects = array_merge($subjects, array_keys($term));
      }
      else {
        $term = create_taxonomy_term($subject, $vocabulary->vid);
        $subjects = array_merge($subjects, array_keys($term));        
      }
      $ewrapper->field_subject->set($subjects);
    }
  }
  
  $ewrapper->save(); 
  
  /**
   * Multi volume book
   */

  if ( $node_language == 'en' ) {
  
    foreach( $node['multivol'] as $multivol ) {
  
      $multivol_nid = multivol_nid($multivol['identifier']);
    
      if ($multivol_nid) {
    
        // $multivol['title'] = $node['title_long'] . ' ' . t('Volume') . ' ' . $multivol['volume'];
        
        $multivol['title'] = $node['title_long'];
       
        $multivol['book'] = $node['identifier'];
    
        $dlts_multivol_book = create_dlts_multivol_book($multivol);
    
      }
    
      else {
        drush_log( t('Book "@title" is part of multi volume set "@multivol", but multi volume does not exist. Skip.', array( '@title' => $node['title'], '@multivol' => $multivol['identifier'], ) ), 'warning' );
      }

    }
  }
  
  return $entity;
  
}

function create_dlts_book_from_xml($books) {
	
  foreach ($books as $value) {
      
    $xml = simplexml_load_file($value->uri);
    
    if (!$xml) { 
      
      drush_log( 
        t('File @file not found', array('@file' => $value->uri)), 'error'
      );
      
      die();
       
    }

    foreach ($xml->node as $child) {
      
      $metadata = $child->metadata;
      
      $pages_array =
        $stitch_page_array =
        $multivols =
        $other_versions =
        $isbns =
        $authors =
        $collections =
        $contributors =
        $creators =
        $editors =
        $publishers =
        $pdf_files =
        $subjects = array();

      $node_language = (string) $child->language[0];

      // $book_title = $book_identifier = (string) uniqid();

      $book_title = (string) $child->title[0];
      
      $book_identifier = (string) $child->identifier[0];

      if (
        $node_language == 'en' || 
        $node_language == LANGUAGE_NONE
      ) 
      {
        /** book pages */
        foreach ($child->pages->children() as $page) {
          $page = (array) $page;
          $page['service'] = (array) $page['service'];
          $page['image'] = (array) $page['image'];
          $pages_array[] = (array) $page;
        }
        
        /** book pages */
        foreach ($child->stitch_pages->children() as $stitch_page) {
          $stitch_page_array[] = (array) $stitch_page;
        }
        
      }
      
      /** contributors */ 
      if ($metadata->contributors) {
        foreach ($metadata->contributors->children() as $contributor) {
          $contributors[] = (string) $contributor[0];
        }
      }

      /** authors */    
      if ($metadata->authors) {      
        foreach ($metadata->authors->children() as $author) {
          $authors[] = (string) $author[0];
        }
      }

      /** collections the book belong */
      if ($child->isPartOf->collection) {
        foreach ($child->isPartOf->collection as $collection) {
          $collections[] = (string) $collection[0];
        }
      }
      
      if ($child->isPartOf->multivol) {
        foreach ($child->isPartOf->multivol as $multivol) {
          $multivols[] = array(
            'identifier' => (string) $multivol[0],
            'volume' => (int) $multivol->attributes()->volume[0], 
            'collection' => (string)$multivol->attributes()->collection[0], 
          );
	    }
	  }

      /** creators */
      if ($metadata->creators) {
        foreach ($metadata->creators->children() as $creator) {
          $creators[] = (string) $creator[0];
        }      
      }
      
      /** editor */
      if ($metadata->editors) {
        foreach ($metadata->editors->children() as $editor) {
          $editors[] = (string) $editor[0];
        }     
      }

      /** publishers */
      if ($metadata->publishers) {
        foreach ($metadata->publishers->children() as $publisher) {
          $publishers[] = (string) $publisher[0];
        }     
      }
      
      /** subjects */
      if ($metadata->subjects) {
        foreach ($metadata->subjects->children() as $subject) {
          $subjects[] = (string) $subject[0];
        }
      }

      /** isbns */
      if ($metadata->isbns) {
        foreach ($metadata->isbns->children() as $isbn) {
          $isbns[] = (string) $isbn[0];
        }     
      }
      
      /** other_versions */
      if ($metadata->other_versions) {
        foreach ($metadata->other_versions->children() as $other_version) {
          $other_versions[] = array(
            'url' => (string) $other_version[0]
          );
        }           
      }

      /** pdfs  */
      if ($metadata->pdf_files) {
        foreach ($metadata->pdf_files->children() as $pdf_file) {
          $pdf_files[] = (string) $pdf_file[0];
        }
      }	  

      $node = array(
        'title' => $book_title,
        'title_long' => (string)$metadata->title_long[0],        
        'node_language' => $node_language,
        'language' => (string) $metadata->language[0],
        'subtitle' => (string) $metadata->subtitle[0],
        'page_count' => (int) $metadata->page_count[0],
        'sequence_count' => (int) $metadata->sequence_count[0],
        'binding_orientation' => (int)$metadata->binding_orientation[0],
        'call_number' => (string)$metadata->call_number[0],
        'description' => (string)$metadata->description[0],
        'dimensions' => (string)$metadata->dimensions[0],
        'handle' => (string) $metadata->handle[0],
        'language_code' => (string)$metadata->language_code[0],
        'number' => (string)$metadata->number[0],
        'read_order' => (int)$metadata->read_order[0],
        'representative_image' => (string) $metadata->representative_image[0],
        'scan_order' => (int)$metadata->scan_order[0],
        'scanning_notes' => (string)$metadata->scanning_notes[0],
        'volume' => (string)$metadata->volume[0],
        'publication_date' => (string)$metadata->publication_date[0],
        'publication_location' => (string)$metadata->publication_location[0],
        'publication_date_text' => (string)$metadata->publication_date_text[0],
        'scan_date' => (string)$metadata->scan_date[0],
        'other_version' => $other_versions,
        'isbn' => $isbns,
        'identifier' => $book_identifier,
        'collections' => $collections,
        'author' => $authors,
        'creator' => $creators,
        'contributor' => $contributors,
        'editor' => $editors,
        'publisher' => $publishers,
        'subjects' => $subjects,
        'pdf_files' => $pdf_files,
        'pages' => $pages_array,
        'multivol' => $multivols,        
      );
      
      $created = save_dlts_book($node);
      
      if (
        $created && 
        isset($created->nid)
	  ) {

        drush_log(
          t('Book "@title" was successfully processed. (nid: @nid)', array('@nid' => $created->nid, '@title' => $created->title )), 'ok'
        );

        foreach($pages_array as $key => $book_page) {

          $title = (isset($book_page['title'])) ? $book_page['title'] : $book_title . ' ' . t('page @number', array('@number' => $book_page['sequence_number']));

          $page_node = array(
            'title' => $title,
            'is_part_of' => $book_identifier,
            'hand_side' => $book_page['hand_side'],
            'page_type' => $book_page['page_type'],
            'real_page_number' => $book_page['page'],
            'sequence_number' => $book_page['sequence'],
            'visible' => $book_page['visible'],
            'cropped_master' => $book_page['image'][0],  
            'service_copy' => $book_page['service'][0],          
          );

          $this_book_page = create_dlts_book_page( $page_node );

          if ($this_book_page->nid) {
             drush_log( t('Book page "@title" part of "@book" was successfully processed. (nid: @nid)', array('@nid' => $this_book_page->nid, '@title' => $this_book_page->title, '@book' => $created->title )), 'ok');
          }

        }

        foreach ($stitch_page_array as $key => $stitch_page) {

          $title = (isset($stitch_page['title'])) ? $stitch_page['title'] : $book_title . ' ' . t('page @number', array('@number' => $stitch_page['sequence_number_left'] . '-' . $stitch_page['sequence_number_right'] ) ) ;

          $stitch_page_node = array(
            'title' => $title,
            'is_part_of' => $book_identifier,
            'stitch_image' => $stitch_page['stitch_image'],
            'sequence_number_left' => $stitch_page['sequence_number_left'],
            'sequence_number_right' => $stitch_page['sequence_number_right'],
            'page_number_left' => $stitch_page['page_number_left'],
            'page_number_right' => $stitch_page['page_number_right'],
          );

          $this_stitch_page = create_dlts_book_stitched_page( $stitch_page_node );

          if ($this_stitch_page->nid) {
             drush_log( t('Book page "@title" part of "@book" was successfully processed. (nid: @nid)', array('@nid' => $this_stitch_page->nid, '@title' => $this_stitch_page->title, '@book' => $created->title )), 'ok');
          }

        }

      }
    }
  }
}