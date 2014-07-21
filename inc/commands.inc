<?php

foreach ( file_scan_directory( __DIR__, '/commands.(.*)\.inc$/', array('recurse' => FALSE )) as $file) {  
  include_once($file->uri);
}