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
  )
  return $api;
}

/**
 * Pull in users.
 */
class MathUserMigration extends Migration {
  public function __construct() {
    parent::__construct();
    $this->description = t('Migrate Math users');
 
    $source_fields = array(
      'uid' => t('User ID'),
      'roles' => t('The set of roles assigned to a user.'),
    );
 
    $query = db_select(MathDBName .'.users', 'u')
      ->fields('u', array('uid', 'name', 'pass', 'mail', 'created', 'access', 'login', 'status', 'init'))
      ->condition('u.name', array('admin', 'rgates'), 'NOT IN')
      ->condition('u.uid', 0, '>');
    $this->source = new MigrateSourceSQL($query, $source_fields);
    $this->destination = new MigrateDestinationUser();
 
    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'uid' => array(
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
          'description' => 'D6 Unique User ID',
          'alias' => 'u',
        )
      ),
      MigrateDestinationUser::getKeySchema()
    );
 
    // Make the mappings
    $this->addFieldMapping('name', 'name')->dedupe('users', 'name');
    $this->addFieldMapping('pass', 'pass');
    $this->addFieldMapping('mail', 'mail')->dedupe('users', 'mail');
    $this->addFieldMapping('language')->defaultValue('');
    $this->addFieldMapping('theme')->defaultValue('');
    $this->addFieldMapping('signature')->defaultValue('');
    $this->addFieldMapping('signature_format')->defaultValue('filtered_html');
    $this->addFieldMapping('created', 'created');
    $this->addFieldMapping('access', 'access');
    $this->addFieldMapping('login', 'login');
    $this->addFieldMapping('status', 'status');
    $this->addFieldMapping('picture')->defaultValue(0);
    $this->addFieldMapping('init', 'init');
    $this->addFieldMapping('timezone')->defaultValue(NULL);
    $this->addFieldMapping('path')->issueGroup(t('DNM'));
    $this->addFieldMapping('pathauto_perform_alias')->defaultValue('1');
    $this->addFieldMapping('roles', 'roles');
  }
 
  public function prepareRow($current_row) {
    $source_id = $current_row->uid;
    $query = db_select(REDCAT_MIGRATION_DATABASE_NAME .'.users_roles', 'r')
      ->fields('r', array('uid', 'rid'))
      ->condition('r.uid', $source_id, '=');
    $results = $query->execute();
    $roles = array('2' => '2');
    foreach ($results as $row) {
      $roles[$row->rid] = $row->rid;
    }
    $current_row->roles = $roles;
    return TRUE;
    // return FALSE if you wish to skip a particular row
  }
}

/**
 * Pull in pages.
 */

class MathPageMigration extends Migration{
  public function __contstruct() {
    parent::__construct();

    $this->description = t('Import math scores from temp math database.');

    $source_fields = array(
      'nid' => t('The node ID of the page'),
      'linked_files' => t('The set of linked files'),
    );

    $query = db_select(MathDBName . '.node', 'n')
      ->fields('n', array('nid', 'vid', 'type', 'title', 'uid', 'status', 'created', 'changed', 'comment', 'promote', 'moderate', 'sticky'))
      ->condition('n.type', 'page', '=');
    $query->join(MathDBName .'.node_revisions', 'nr', 'n.vid = nr.vid');
    $query->addField('nr', 'body');
    $query->addField('nr', 'teaser');
    $query->join(MathDBName .'.users', 'u', 'n.uid = u.uid');
    $query->addField('u', 'name');
    $query->orderBy('n.changed');

    $this->highwaterField = array(
      'name' => 'changed', // Column to be used as highwater mark
      'alias' => 'n',           // Table alias containing that column
    );

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
