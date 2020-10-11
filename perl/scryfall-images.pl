#!/usr/bin/perl

use warnings;
use strict;
use DBI;
use Data::Dumper;
use File::Fetch;
use JSON;
use threads;
use utf8;

our @images;
$| = 1;

sub download_image
{
    my($id,$uri) = @_;
    if ( ! -e "../resources/images/$id.jpg" ) {
        print "Downloading $uri\n";
        system( "curl '$uri' -o ../resources/images/$id.jpg --silent" );
    }
}

# connect to our database
my $dbh = DBI->connect("DBI:Pg:dbname=magcol;host=localhost", "dbamagcol", "magcol", {'RaiseError' => 1});


# update database with cards
my $sth = $dbh->prepare( 'BEGIN; SELECT get_cards_image_uri()');
$sth->execute();
my $cursor = $sth->fetch;
$sth = $dbh->prepare( 'FETCH ALL IN "'.@{ $cursor }[ 0 ].'";' );
$sth->execute();
my $img_ref = $sth->fetchall_hashref( 'card_id' );
foreach my $ref ( keys %{ $img_ref } ) {
	print "$ref = $img_ref->{ $ref }{ card_image_uri }\n";
	download_image( $ref, $img_ref->{ $ref }{ card_image_uri } );
}

$sth = $dbh->prepare( 'COMMIT;' );
$sth->execute();
$sth->finish();

# clean up
$dbh->disconnect();

