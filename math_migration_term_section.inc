<?php

/**
 * @file
 * Importing math.osu.edu.
 */

/**
 * Pull in terms..
 */

class MathTermSeminarMigration extends Migration {
  public function __construct() {
    parent::__construct();

    $this->description = t('Migrate legacy taxonomy section term.');

    Database::addConnectionInfo('for_term_section_migration', 'default', array(
      'driver' => 'mysql',
      'database' => 'math-temp-04302012',
      'username' => 'mathtemp',
      'password' => 'UQSTJNqCB1Jb',
      'host' => 'ascdb-d.asc.ohio-state.edu',
      'prefix' => '',
    ));

    $this->map = new MigrateSQLMap($this->machineName,
        array(
          'tid' => array('type' => 'int',
                         'unsigned' => 'true',
                         'not null' => TRUE,
                         'description' => 'Term ID',
                         'alias' => 't',
                          )
        ),
        MigrateDestinationTerm::getKeySchema(),
        'for_term_section_migration'
      );

    $query = Database::getConnection('default', 'for_term_section_migration')
           ->select('term_data', 'td')
           ->fields('td', array('tid','vid','name','description','weight'))
           ->condition('td.vid', '8', '=');
    $query->join('term_hierarchy', 'th', 'th.tid = td.tid');
    $query->addField('th', 'parent');

    $this->source = new MigrateSourceSQL($query, array(), NULL, array('map_joinable' => FALSE));

    $this->destination = new MigrateDestinationTerm('section');

    $this->addSimpleMappings(array('tid', 'name', 'description', 'parent', 'weight'));

  }

  public function preImport() {
    parent::preImport();

    if (!taxonomy_vocabulary_load_multiple(array(), array('machine_name' => 'section'))) {
      // Create a vocabulary named "Section".
      $description = "Which section of the website this goes in";
      $help = "Select the appropriate section of the website for this page";
      $vocabulary = (object) array(
        'vid' => 8,
        'name' => 'Section',
        'description' => $description,
        'machine_name' => 'section',
        'help' => $help,
        'hierarchy'=> 1,
      );

      taxonomy_vocabulary_save($vocabulary);
    }

    }
  }
}

?>
