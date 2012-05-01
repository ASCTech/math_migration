<?php

/**
 * @file
 * Importing math.osu.edu.
 */ 

/**
 * Pull in pages.
 */

class MathPageMigration extends Migration {
  public function __contstruct() {
    parent::__construct();

    $this->description = t('Migrate legacy pages.');

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
          'nid' => array('type' => 'int',
                           'length' => 8,
                           'not null' => TRUE,
                           'description' => 'Node ID',
                          )
        ),
        MigrateDestinationTerm::getKeySchema(),
        'for_migration'
      );

    $query = Database::getConnection('default', 'for_migration')
           ->select('node', 'n')
           ->fields('n', array('title'));

    $this->source = new MigrateSourceSQL($query);

    $this->destination = new MigrateDestinationNode('page');

    $this->addFieldMapping('nid', 'nid');

  }
}

?>
