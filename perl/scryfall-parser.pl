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
        print "Downloading $id\n";
        system( "curl '$uri' -o ../resources/images/$id.jpg --silent" );
    }
}

open( JSON_DB, "<scryfall-default-cards.json" ) or die( "Error: $!" );
my @jsstr = <JSON_DB>;
close( JSON_DB );
my $json_ref = decode_json(join('',@jsstr));
my $cou = 0;
foreach my $card ( @{ $json_ref } ) {
    if ( $card->{ lang } eq "en" ) {
        my $str = "$card->{ name }¤$card->{ id }¤$card->{ mana_cost }¤$card->{ set }¤$card->{ type_line }¤$card->{ image_uris }->{ normal }¤$card->{ oracle_text }";
        foreach my $key ( qw{name id mana_cost set type_line oracle_text} ) {
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
        # my $success = utf8::downgrade($str, 1 );
        # if ( ! $success ) {
        #     print "FAIL: $str\n";
        #     exit;
        # }
        download_image($card->{ id }, $card->{ image_uris }->{ normal });
        $::CARDS{ $card->{ id } }{ exp } = $card->{ set };
        $::CARDS{ $card->{ id } }{ rarity } = $card->{ rarity };
        $::CARDS{ $card->{ id } }{ name } = $card->{ name };
        $::CARDS{ $card->{ id } }{ type } = $card->{ type_line };
        $::CARDS{ $card->{ id } }{ subtype } = "";
        $::CARDS{ $card->{ id } }{ oracle } = $card->{ oracle_text };
        $::CARDS{ $card->{ id } }{ cost } = $card->{ mana_cost };
        $::CARDS{ $card->{ id } }{ img_ref } = "$card->{ id }.jpg";
        # print CSV_DUMP "$str\n";
        if ( $cou++ > 100 ) {
            last;
        }
    }
}


# connect to our database
my $dbh = DBI->connect("DBI:Pg:dbname=magcol;host=localhost", "dbamagcol", "magcol", {'RaiseError' => 1});

my %hostinfo;
my @hosts_to_scan;
$dbh->{ AutoCommit } = 0;

# update database with current status of the hosts
my $sth = $dbh->prepare( 'BEGIN;' );
$sth->execute();
$sth = $dbh->prepare( 'COMMIT;' );

foreach ( keys %::CARDS ) {
    $sth = $dbh->prepare( 'SELECT add_card( ?, ?, ?, ?, ?, ?, ?, ? )' );
    $sth->bind_param( 1, $::CARDS{ $_ }{ exp } );
    $sth->bind_param( 2, $::CARDS{ $_ }{ name } );
    $sth->bind_param( 3, $::CARDS{ $_ }{ rarity } );
    $sth->bind_param( 4, $::CARDS{ $_ }{ type } );
    $sth->bind_param( 5, $::CARDS{ $_ }{ subtype } );
    $sth->bind_param( 6, $::CARDS{ $_ }{ oracle } );
    $sth->bind_param( 7, $::CARDS{ $_ }{ cost } );
    $sth->bind_param( 8, $::CARDS{ $_ }{ img_ref } );
    $sth->execute();
}
# cleanup database transaction
$sth = $dbh->prepare( 'COMMIT;' );
$sth->execute();
$sth->finish();

$dbh->commit();
#
# clean up
$dbh->disconnect();

