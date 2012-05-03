<?php

/**
 * @file
 * Importing math.osu.edu.
 */ 

/**
 * Pull in pages.
 */

class MathPageMigration extends Migration {
  public function __construct() {
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
                           'length' => 10,
                           'not null' => TRUE,
                           'description' => 'Node ID',
                          )
        ),
        MigrateDestinationTerm::getKeySchema(),
        'for_migration'
      );

    $query = Database::getConnection('default', 'for_migration')
           ->select('node', 'n')
           ->fields('n', array('nid','vid','type','title','uid','status','created','body','changed','promote','teaser'))
           ->condition('n.type', 'page', '=');
    $query->leftJoin('term_node', 'tn', 'n.nid = tn.nid');
    $query->addField('tn', 'tid');
    $query->leftJoin('node_revisions', 'nr', 'n.nid = nr.nid AND n.changed = nr.timestamp');
    $query->addField('nr', 'body');
    $query->addField('nr', 'teaser');

    $this->source = new MigrateSourceSQL($query, array(), NULL, array('map_joinable' => FALSE));

    $this->destination = new MigrateDestinationNode('page');

    $this->addSimpleMappings(array('nid', 'title', 'uid', 'status', 'created', 'changed', 'promote'));

    $this->addFieldMapping('body', 'nr_body');
    $this->addFieldMapping('teaser', 'nr_teaser');

    $this->addFieldMapping('field_section', 'tid')
           ->sourceMigration('MathTermSection')
           ->arguments(array('source_type' => 'tid'));

  }
}
?>
