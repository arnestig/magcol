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

open( JSON_DB, "<scryfall-default-cards.json" ) or die( "Error: $!" );
my @jsstr = <JSON_DB>;
close( JSON_DB );
my $json_ref = decode_json(join('',@jsstr));
my $cou = 0;
printf "Parsing cards";
foreach my $card ( @{ $json_ref } ) {
    if ( $card->{ lang } eq "en" ) {
        my $str = "$card->{ name }¤$card->{ id }¤$card->{ mana_cost }¤$card->{ set }¤$card->{ type_line }¤$card->{ image_uris }->{ normal }¤$card->{ oracle_text }";
        foreach my $key ( qw{name id mana_cost set type_line oracle_text} ) {
            if ( defined $card->{ $key } ) {
                $card->{ $key } =~ s/—/-/g;
                $card->{ $key } =~ s/−/-/g;
                $card->{ $key } =~ s/•/-/g;
                $card->{ $key } =~ s/☐/-/g;
                $card->{ $key } =~ s/’/'/g;
                $card->{ $key } =~ s/π/pi/g;
                $card->{ $key } =~ s/∞/infinite/g;
                $card->{ $key } =~ s/®//g;
                $card->{ $key } =~ s/™//g;
                $card->{ $key } =~ s/\n/<br>/g;
            }
        }
        # my $success = utf8::downgrade($str, 1 );
        # if ( ! $success ) {
        #     print "FAIL: $str\n";
        #     exit;
        # }
        # download_image($card->{ id }, $card->{ image_uris }->{ normal });
        $::CARDS{ $card->{ id } }{ exp } = $card->{ set };
        $::CARDS{ $card->{ id } }{ rarity } = $card->{ rarity };
        $::CARDS{ $card->{ id } }{ name } = $card->{ name };
        $::CARDS{ $card->{ id } }{ type } = $card->{ type_line };
        $::CARDS{ $card->{ id } }{ subtype } = "";
        $::CARDS{ $card->{ id } }{ oracle } = $card->{ oracle_text };
        $::CARDS{ $card->{ id } }{ cost } = $card->{ mana_cost };
        $::CARDS{ $card->{ id } }{ img_ref } = "$card->{ id }.jpg";
        $::PRICES{ $card->{ id } }{ usd } = $card->{ prices }->{ usd };
        $::PRICES{ $card->{ id } }{ usd_foil } = $card->{ prices }->{ usd_foil };
        # print CSV_DUMP "$str\n";
        if ( ($cou++ % 1000) == 0 ) {
            printf ".";
        }
    }
}

printf "\nStoring in database\n";

# connect to our database
my $dbh = DBI->connect("DBI:Pg:dbname=magcol;host=localhost", "dbamagcol", "magcol", {'RaiseError' => 1});

$dbh->{ AutoCommit } = 0;

# update database with cards
my $sth = $dbh->prepare( 'BEGIN;' );

foreach ( keys %::CARDS ) {
    $sth = $dbh->prepare( 'SELECT add_card( ?, ?, ?, ?, ?, ?, ?, ?, ? )' );
    $sth->bind_param( 1, $_ );
    $sth->bind_param( 2, $::CARDS{ $_ }{ exp } );
    $sth->bind_param( 3, $::CARDS{ $_ }{ name } );
    $sth->bind_param( 4, $::CARDS{ $_ }{ rarity } );
    $sth->bind_param( 5, $::CARDS{ $_ }{ type } );
    $sth->bind_param( 6, $::CARDS{ $_ }{ subtype } );
    $sth->bind_param( 7, $::CARDS{ $_ }{ oracle } );
    $sth->bind_param( 8, $::CARDS{ $_ }{ cost } );
    $sth->bind_param( 9, $::CARDS{ $_ }{ img_ref } );
    $sth->execute();
}
# cleanup database transaction
$sth = $dbh->prepare( 'COMMIT;' );
$sth->execute();
$sth->finish();

$dbh->commit();

# update database with prices
$sth = $dbh->prepare( 'BEGIN;' );

foreach ( keys %::PRICES ) {
    $sth = $dbh->prepare( 'SELECT add_price( ?, ?, ? )' );
    $sth->bind_param( 1, $_ );
    $sth->bind_param( 2, $::PRICES{ $_ }{ usd } );
    $sth->bind_param( 3, $::PRICES{ $_ }{ usd_foil } );
    $sth->execute();
}
# cleanup database transaction
$sth = $dbh->prepare( 'COMMIT;' );
$sth->execute();
$sth->finish();

$dbh->commit();

# clean up
$dbh->disconnect();

