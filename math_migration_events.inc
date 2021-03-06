<?php

/**
 * @file
 * Importing math.osu.edu.
 */ 

/**
 * Pull in stories.
 */

class MathEventsMigration extends Migration {
  public function __construct() {
    parent::__construct();

    $this->description = t('Migrate legacy events.');

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
                           'length' => 10,
                           'not null' => TRUE,
                           'description' => 'Node ID',
                          )
        ),
        MigrateDestinationNode::getKeySchema(),
        'for_migration'
      );

    $or_statement = db_or()
           ->condition('n.type', 'flexinode-1', '=')
           ->condition('n.type', 'flexinode-2', '=')
           ->condition('n.type', 'flexinode-8', '=');

    $query = Database::getConnection('default', 'for_migration')
           ->select('node', 'n')
           ->fields('n', array('nid','type','title','uid','status','created','changed','promote'))
           ->condition($or_statement);
    $query->leftJoin('node_revisions', 'nr', 'n.nid = nr.nid AND n.changed = nr.timestamp');
    $query->addField('nr', 'body');

    $this->source = new MigrateSourceSQL($query, array(), NULL, array('map_joinable' => FALSE));

    $this->destination = new MigrateDestinationNode('asc_event');

    $this->addSimpleMappings(array('title', 'uid', 'status', 'created', 'changed', 'promote'));

    $this->addFieldMapping('is_new')->defaultValue(TRUE);
    $this->addFieldMapping('revision')->defaultValue(TRUE);
    $this->addFieldMapping('language')->defaultValue('und');

    $this->addFieldMapping('path')->issueGroup(t('DNM'));
    $this->addFieldMapping('pathauto_perform_alias')->defaultValue('1');

    $body_arguments = MigrateTextFieldHandler::arguments(NULL, filter_default_format(), NULL);
    $this->addFieldMapping('body', 'body')
            ->arguments($body_arguments);
    $this->addFieldMapping('body:format')->defaultValue('html');

  }
}
?>
