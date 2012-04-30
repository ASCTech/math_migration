<?php

/**
 * @file
 * Importing math.osu.edu.
 */ 


/* 
 * hook_migrate_api 
 */
function math_migration_migrate_api() {
  $api = array(
    'api' => 2,
  );
  return $api;
}

/**
 * Pull in pages.
 */

class MathPageMigration extends Migration{
  public function __contstruct() {
    parent::__construct();

Database::addConnectionInfo('math_mig', 'default', array(
  'driver' => 'mysql',
  'database' => 'math-temp-04302012',
  'username' => 'mathtemp',
  'password' => 'UQSTJNqCB1Jb',
  'host' => 'ascdb-d.asc.ohio-state.edu',
  'prefix' => '',
  ));

    $this->description = t('Import math pages from temp math database.');

    $source_fields = array(
      'nid' => t('The node ID of the page'),
      //'linked_files' => t('The set of linked files'),
    );

    $query = Database::getConnection('default', 'math_mig')
      ->select('node', 'n')
      ->fields('n', array('nid', 'vid', 'type', 'title', 'uid', 'status', 'created', 'changed', 'comment', 'promote', 'moderate', 'sticky'))
      ->condition('n.type', 'page', '=');
    // $query->join('math-temp-04302012.node_revisions', 'nr', 'n.vid = nr.vid');
    //$query->addField('nr', 'body');
    //$query->addField('nr', 'teaser');
    //$query->join('math-temp-04302012.users', 'u', 'n.uid = u.uid');
    //$query->addField('u', 'name');
    //$query->orderBy('n.changed');

    $this->source = new MigrateSourceSQL($query, $source_fields);
    $this->destination = new MigrateDestinationNode('page');
         
    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'nid' => array(
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
          'description' => 'D6 Unique Node ID',
          'alias' => 'n',
          )
        ),
      MigrateDestinationNode::getKeySchema()
   );
  }
}
