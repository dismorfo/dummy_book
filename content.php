<?php

/**
 * A command-line Drupal script to create a dummy book
 * drush -r --user=1 --uri=http://localhost/book_test scr content.php
 */
 
function show_help(array $commands = array()) {

  drush_print('');

  foreach ($commands as $key => $option) {
    drush_print('[' . $key . '] ' . $option['label']);
  }

  drush_print('');

}

function show_options (array $commands = array()) {

  drush_print(t('Please type one of the following options to continue:'));

  show_help($commands);

  $handle = fopen ("php://stdin","r");

  $line = fgets($handle);

  run(trim($line), $commands);

}

function run ($task, array $commands = array()) {
  if (isset($commands[$task]) && isset($commands[$task]['callback'])) {
    foreach ($commands[$task]['callback'] as $callback) {
      if (function_exists($callback)) {
        $callback();
      }
    }
  }
  else {
    drush_print('');
    drush_print(t('ERROR: Unable to perform task'));
    drush_print('');
    show_options($commands);
  }
}

function init ( array $options = array() ) {

  ini_set('memory_limit', '512MM');

  include_once('inc/common.inc');

  include_once('inc/datasource.inc');

  include_once('inc/create.inc');

  include_once('inc/default_commands.inc');

  $trace = debug_backtrace();

  $caller = (isset($trace[0]['file']) ? $trace[0]['file'] : __FILE__);

  $default_commands = default_commands();

  $commands = array_merge($default_commands, $options['commands']);

  // Add exit command to end of the commands options  
  $commands[] = array(
    'label' => t('Exit'),
    'callback' => array(
      'drush_exit',
    ),
  );

  if (isset($args[1])) {
    run($args[1], $commands);
  }
  
  else {
    show_options($commands);
  }

}

init( array( 'commands' => array() ) );