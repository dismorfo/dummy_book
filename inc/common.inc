<?php

function add_language($requested_language) {
        
  $list_enabled_languages = locale_language_list();

  $enabled_languages = array_keys( $list_enabled_languages );
  
  if ( in_array( $requested_language, $enabled_languages ) ) return TRUE;
  
  require_once DRUPAL_ROOT . '/includes/iso.inc';
   
  // https://api.drupal.org/api/drupal/includes%21iso.inc/function/_locale_get_predefined_list/7
  $predefined_list = _locale_get_predefined_list();

  // https://api.drupal.org/api/drupal/includes%21locale.inc/function/locale_add_language/7    
  $language_to_install = $predefined_list[$requested_language];

  if (!$language_to_install) {
    drush_log( t( 'At this moment there is only support for auto-install languages defined by Drupal iso.inc' ), 'error' ) ;
    return FALSE;
  }

  locale_add_language($requested_language);

  return TRUE;

}

function image_to_jpg( $sourse, $overwrite = TRUE ) {
    
  if (!file_exists($sourse)) return FALSE;

  /**
   * Files folder real path
   */
  $dupal_file_path = drupal_realpath('public://');
  
  $pathInfo = pathinfo( $sourse );

  $localFile = $dupal_file_path . '/' . $pathInfo['filename'] . '.jpg';
  
  if ( file_exists($localFile) && !$overwrite ) return $localFile;

  $im = new Imagick($sourse);
  
  // convert to jpg 
  $im->setImageColorspace(255);

  $im->setCompression(Imagick::COMPRESSION_JPEG);

  $im->setCompressionQuality(60);

  $im->setImageFormat('jpeg');

  $jp2_support = $im->queryFormats( 'JP2' );

  $tiff_support = $im->queryFormats( 'TIFF' );

  if ( !empty($tiff_support) && !empty($jp2_support) ) {

    // write image on server 
    $im->writeImage($localFile);

    $im->clear();

    $im->destroy();

  }
  
  return file_exists($localFile) ? $localFile : FALSE;

}

function import_get_content($URL) {
   
   drush_log( t('Requesting external content @url', array('@url' => $URL) ), 'ok') ;

   $ch = curl_init();
   curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
   curl_setopt($ch, CURLOPT_URL, $URL);
   $data = curl_exec($ch);
   curl_close($ch);

   if ($data) {
     drush_log( t('External content was successfully requested from @url', array('@url' => $URL) ), 'ok') ;
   }

   else {
   	drush_log( t('Unable to get content requested from @url', array('@url' => $URL) ), 'fail') ;
   }

   return $data;

}

function import_add_file($path, $overwrite = TRUE) {

  global $user;
  
  $file = FALSE;
  
  /**
   * Files folder real path
   */
  $dupal_file_path = drupal_realpath('public://');

  $localFile = $dupal_file_path . '/' . basename( $path );
  
  if ( $overwrite || !file_exists($localFile) ) {
  	$put_success = file_put_contents($localFile, import_get_content($path));
  }
  
  else {
  		
  	drush_log( t('Skip request from external content from @url. File exist and was flag as do not overwrite; see: @localFile', array('@url' => $path, '@localFile' => $localFile) ), 'ok') ;

  	$put_success = TRUE;

  }
  
  if ($put_success) {
  	
	  $filemime = file_get_mimetype($localFile);

      $file = (object) array(
        'uid' =>  $user->uid,
        'uri' => $localFile,
        'filemime' => $filemime,
        'status' => 1,
      );

      $file = file_copy($file, 'public://');

  }
  
  return $file;
 
}