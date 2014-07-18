<?php

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