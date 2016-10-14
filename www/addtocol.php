<?php

include_once( "include/collectionmanagement.php" );

echo '
<!DOCTYPE html>
<html>
<head>
<script src="https://ajax.googleapis.com/ajax/libs/jquery/1.12.4/jquery.min.js"></script>
<script>
';
?>

function populateCards() {
    var searchString = document.getElementById("cardSearchBox").value;
    if ( searchString.length < 3 ) {
        return;
    }
    $.ajax({
        url: 'cards.php',
        data: "getCards="+searchString,
        dataType: "json",
        method: "POST",
        success: function(data) {
            $('#cardList').empty();
            $.each(data, function( i, obj ) {
                $('<option />',
                    {
                        value:obj[ 'card_name' ] + ", " + obj[ 'card_expansion' ],
                    }
                    ).appendTo('#cardList');
            });
        },
        error: function( xhr, desc, err ) {
            console.log(xhr);
            console.log("Details: " + desc + "\nError: " + err);
        }
    });
}

<?php
echo '
</script>
</head>
<body>

<FORM action="addtocol.php" method="post">
    <input list="cardList" autofocus autocomplete=on id="cardSearchBox" name="cardSearchBox" oninput="populateCards()">
    <datalist id="cardList"></datalist>
</FORM>

</body>
</html>';

if (isset($_POST['cardSearchBox'])) {
    $selection = $_POST['cardSearchBox'];
    $last_comma = strrpos( $selection, "," );
    $cardname = substr( $selection, 0, $last_comma );
    $expansion = substr( $selection, $last_comma + 2 );
    $collection = new Collection();
    $collection->addCardToCollection( $cardname, $expansion );
}

?>
