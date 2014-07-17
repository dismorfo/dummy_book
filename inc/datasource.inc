<?php

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

function multivol_book_nid($identifier, $volume_page) {

  $query = new EntityFieldQuery;
  
  $result = $query
    ->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'dlts_multivol')
    ->fieldCondition('field_identifier', 'value', $identifier, '=')
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

function book_nid($identifier) {

  $query = new EntityFieldQuery;
  
  $result = $query
    ->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'dlts_book')
    ->fieldCondition('field_identifier', 'value', $identifier, '=')
    ->execute();
    
  if (empty($result['node'])) return FALSE;

  $keys = array_keys($result['node']);

  return (int)$keys[0];

}