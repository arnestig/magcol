<?php

include_once( "include/carddatabase.php" );
include_once( "include/tablegenerator.php" );

function displayCards( $search_string ) {
    $carddb = new CardDatabase();
    $cards = $carddb->getCards( $search_string );

    $tableGenerator = new TableGenerator();
    $tableGenerator->addColumn( 'Card', '<a class="thumbnail">%s (%s)<span><img src="resources/images/%s"/></span></a>', array( 'card_name', 'card_expansion','card_image_ref' ) );
    $tableGenerator->addColumn( 'Rarity', '%s', array( 'card_rarity' ) );
    $tableGenerator->addColumn( 'Type', '%s', array( 'card_type' ) );
    $tableGenerator->addColumn( '', '<a href="collection.php?add_card=%s">+</a>', array( 'card_id' ) );
    $tableGenerator->setData( $cards );
    echo $tableGenerator->generateHTML();
}

echo '<html>
        <head>
            <link rel="stylesheet" href="magcol.css">
            <script type="text/javascript" src="js/sorttable.js"></script>
        </head>';

echo '<form method="post" action="cards.php""><input type="text" value="'.$_POST[ 'getCards' ].'" name="getCards"></form>';

/* Handle our different GET's on this page */
if ($_SERVER[ 'REQUEST_METHOD' ] === 'GET') {
    if ( isset( $_GET[ 'getCards' ] ) ) {
    }
}

/* Handle our different POST's on this page */
if ( $_SERVER[ 'REQUEST_METHOD'] === 'POST' ) {
    if ( isset( $_POST[ 'getCards' ] ) ) {
        displayCards( $_POST[ 'getCards' ] );
    }
}

echo '</html>';

?>
