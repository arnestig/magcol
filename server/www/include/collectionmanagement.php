<?php

include_once( "config.php" );

class Collection
{
    private $dbcon;
    public function __construct()
    {
        $this->dbcon = new PDO( "pgsql:host=" . DB_SERVER . ";dbname=" . DB_DATABASE . ";user=" . DB_USER . ";password=" . DB_PASSWORD . ";port=" . DB_PORT ) or die ("Could not connect to server\n"); 
        $this->dbcon->setAttribute( PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
    }

    public function addCardToCollection( $cardname, $cardexpansion )
    {
        $sth = $this->dbcon->prepare( "SELECT add_card_to_collection( ?, ?, ? )" );
        $sth->execute( array( 1, $cardname, $cardexpansion ) );
    }

    public function removeCardFromCollection( $cardid )
    {
        $sth = $this->dbcon->prepare( "SELECT remove_card_from_collection( ?, ? )" );
        $sth->execute( array( 0, $cardid ) );
    }

    public function getCollection( $search_string )
    {
        $this->dbcon->beginTransaction();
        $sth = $this->dbcon->prepare( "SELECT get_collection( ?, ? )" );
        $sth->execute( array( 0, $search_string ) );

        $cursors = $sth->fetch();
        $sth->closeCursor();

        // get each result set
        $results = array();
        $sth = $this->dbcon->query('FETCH ALL IN "'. $cursors['get_collection'] .'";');
        $results = $sth->fetchAll( PDO::FETCH_ASSOC );
        $sth->closeCursor();
        $this->dbcon->commit();
        unset($sth);

        return $results;
    }
}

?>
