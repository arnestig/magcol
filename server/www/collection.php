<?php

include_once( "include/collectionmanagement.php" );
include_once( "include/tablegenerator.php" );

function displayCollection( $search_string ) {
    $collection = new Collection();
    $cardsincollection = $collection->getCollection( $search_string );

    $tableGenerator = new TableGenerator();
    $tableGenerator->addColumn( 'Card', '%s (%s)', array( 'card_name', 'card_expansion' ) );
    $tableGenerator->addColumn( 'Rarity', '%s', array( 'card_rarity' ) );
    $tableGenerator->addColumn( 'Type', '%s', array( 'card_type' ) );
    $tableGenerator->addColumn( '', '<a href="collection.php?remove_card_from_collection=%d">-</a>', array( 'card_id' ) );
    $tableGenerator->setData( $cardsincollection );
    echo $tableGenerator->generateHTML();
}

echo '<html>
        <head>
            <link rel="stylesheet" href="qtip2/jquery.qtip.min.css">
            <link rel="stylesheet" href="magcol.css">
            <script src="https://ajax.googleapis.com/ajax/libs/jquery/1.11.3/jquery.min.js"></script>
            <script type="text/javascript" src="qtip2/jquery.qtip.min.js"></script>
            <script type="text/javascript" src="js/sorttable.js"></script>
            <script type="text/javascript" src="js/scripts.js"></script>
        </head>';

/* Handle our different GET's on this page */
if ($_SERVER[ 'REQUEST_METHOD' ] === 'GET') {
    if ( isset( $_GET[ 'add_card' ] ) ) {
        $collection = new Collection();
        $collection->addCardToCollection( $_GET[ 'add_card'], "NM", 1 );
    }
}

/* Handle our different POST's on this page */
if ( $_SERVER[ 'REQUEST_METHOD'] === 'POST' ) {
}

$search_for = "";


/* Display a list of our hosts */
displayCollection( $search_for );

echo '</html>';

?>
