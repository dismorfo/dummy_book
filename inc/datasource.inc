<?php

function series_nid($identifier) {

  $query = new EntityFieldQuery;
  
  $result = $query
    ->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'dlts_series')
    ->fieldCondition('field_identifier', 'value', $identifier, '=')
    ->execute();

  if (empty($result['node'])) return FALSE;

  $keys = array_keys($result['node']);

  return (int)$keys[0];

}

function find_nodes($type) {

  /** find all */
  $query = db_query("SELECT DISTINCT nid FROM {node} WHERE type = :type", array('type' => $type));

  /** return nids */
  return $query->fetchAll();

}

function stitched_page_nid($identifier, $left, $right) {
    
  $book_nid = book_nid($identifier);

  $query = new EntityFieldQuery;

  $result = $query
    ->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'dlts_book_stitched_page')
    ->fieldCondition('field_sequence_number_left', 'value', $left, '=')
    ->fieldCondition('field_sequence_number_right', 'value', $right, '=')
    ->fieldCondition('field_book', 'nid', $book_nid, '=')
    ->execute();

  if (empty($result['node'])) return FALSE;

  $keys = array_keys($result['node']);

  return (int)$keys[0];
}

function collection_nid($identifier) {

  $query = new EntityFieldQuery;
  
  $result = $query
    ->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'dlts_collection')
    ->fieldCondition('field_identifier', 'value', $identifier, '=')
    ->execute();
    
  if (empty($result['node'])) return FALSE;

  $keys = array_keys($result['node']);

  return (int)$keys[0];

}

function multivol_nid($identifier) {

  $query = new EntityFieldQuery;
  
  $result = $query
    ->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'dlts_multivol')
    ->fieldCondition('field_identifier', 'value', $identifier, '=')
    ->execute();
    
  if (empty($result['node'])) return FALSE;

  $keys = array_keys($result['node']);

  return (int)$keys[0];

}

function multivol_book_nid($multivol_identifier, $volume_page, $collection_identifier) {

  $query = new EntityFieldQuery;
  
  $multivol_nid = multivol_nid($multivol_identifier);
  
  $collection_nid = collection_nid($collection_identifier);  
  
  $result = $query
    ->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'dlts_multivol_book')
    ->fieldCondition('field_multivol', 'nid', $multivol_nid, '=')
    ->fieldCondition('field_collection', 'nid', $collection_nid, '=')    
	->fieldCondition('field_volume_number', 'value', $volume_page, '=')
    ->execute();
    
  if (empty($result['node'])) return FALSE;

  $keys = array_keys($result['node']);

  return (int)$keys[0];

}

function book_page_nid($book_nid, $sequence_number) {

  $query = new EntityFieldQuery;

  $result = $query
    ->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'dlts_book_page')
    ->fieldCondition('field_sequence_number', 'value', $sequence_number, '=')
    ->fieldCondition('field_book', 'nid', $book_nid, '=')    
    ->execute();
    
  if (empty($result['node'])) return FALSE;

  $keys = array_keys($result['node']);

  return (int)$keys[0];

}

function partner_nid($identifier) {

  $query = new EntityFieldQuery;

  $result = $query
    ->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'dlts_partner')
    ->fieldCondition('field_identifier', 'value', $identifier, '=')
    ->execute();
    
  if (empty($result['node'])) return FALSE;

  $keys = array_keys($result['node']);

  return (int)$keys[0];

}

function book_nid($identifier, $language = LANGUAGE_NONE) {

  $query = new EntityFieldQuery;
  
  $result = $query
    ->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'dlts_book') 
    ->propertyCondition('language', $language)  
    ->fieldCondition('field_identifier', 'value', $identifier, '=')
    ->execute();
    
  if (empty($result['node'])) return FALSE;

  $keys = array_keys($result['node']);

  return (int)$keys[0];

}