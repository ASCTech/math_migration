<?php

/**
 * @file
 * Importing math.osu.edu.
 */

/**
 * Pull in users.
 */

class MathUserMigration extends Migration {
  public function __construct() {
    parent::__construct();

    $this->description = t('Migrate legacy users.');

    Database::addConnectionInfo('for_migration', 'default', array(
      'driver' => 'mysql',
      'database' => 'math-temp-04302012',
      'username' => 'mathtemp',
      'password' => 'UQSTJNqCB1Jb',
      'host' => 'ascdb-d.asc.ohio-state.edu',
      'prefix' => '',
    ));

    $this->map = new MigrateSQLMap($this->machineName,
        array(
          'uid' => array('type' => 'int',
                         'unsigned' => 'true',
                         'not null' => TRUE,
                         'description' => 'User ID',
                         'alias' => 'u',
                          )
        ),
        MigrateDestinationTerm::getKeySchema(),
        'for_migration'
      );

    $query = Database::getConnection('default', 'for_migration')
           ->select('users', 'u')
           ->fields('n', array('uid','mail','name','pass','created','access','login','status','init'))
           ->condition('u.uid', '3', '>');

    $this->source = new MigrateSourceSQL($query, array(), NULL, array('map_joinable' => FALSE));

    $this->destination = new MigrateDestinationUser();

    $this->addFieldMapping('uid', 'uid');

    $this->addFieldMapping('is_new')->defaultValue(TRUE);

    $this->addFieldMapping('name', 'name')->dedupe('users', 'name');
    $this->addFieldMapping('pass', 'pass');
    $this->addFieldMapping('mail', 'mail')->dedupe('users', 'mail');
    $this->addFieldMapping('language')->defaultValue('');
    $this->addFieldMapping('created', 'created');
    $this->addFieldMapping('access', 'access');
    $this->addFieldMapping('login', 'login');
    $this->addFieldMapping('status', 'status');
    $this->addFieldMapping('picture')->defaultValue(0);
    $this->addFieldMapping('init', 'init');
    $this->addFieldMapping('timezone')->defaultValue(NULL);
  }
}

?>
