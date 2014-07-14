<?php

function add_file($path) {

  global $user;

  $file = (object)array(
    'uid' => $user->uid,
    'uri' => $path,
    'filemime' => file_get_mimetype($path),
    'status' => 1,
  );

  $file = file_copy($file, 'public://', FILE_EXISTS_REPLACE);

  return $file;

}