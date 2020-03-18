#!/usr/bin/perl

use strict;
use warnings;

use DBI;
use HTTP::Date qw(str2time);
use Data::Dumper;
use Net::Ping;
use Scalar::Util qw(looks_like_number);
use POSIX qw(strftime);
use Net::SMTP;

our %CARDS;

$::CARDS{ snappy }{ exp } = "A";
$::CARDS{ snappy }{ name } = "Snapcaster Mage";
$::CARDS{ snappy }{ rarity } = "Rare";
$::CARDS{ snappy }{ type } = "Creature";
$::CARDS{ snappy }{ subtype } = "Wizard";
$::CARDS{ snappy }{ oracle } = "When Snapcaster Mage enters the battlefield, target an instant or sorcery card in your graveyard. Until the end of turn you may play that card for it's manacost.";
$::CARDS{ snappy }{ cost } = "1B";
$::CARDS{ snappy }{ img_ref } = "snapcastermage.jpg";

$| = 1;

# connect to our database
my $dbh = DBI->connect("DBI:Pg:dbname=magcol;host=localhost", "dbamagcol", "magcol", {'RaiseError' => 1});

my %hostinfo;
my @hosts_to_scan;
$dbh->{ AutoCommit } = 0;

# update database with current status of the hosts
my $sth = $dbh->prepare( 'BEGIN;' );
$sth->execute();
$sth = $dbh->prepare( 'COMMIT;' );
# create or replace function add_card(
    # c_expansion varchar,
    # c_name varchar,
    # c_rarity varchar,
    # c_type varchar,
    # c_subtype varchar,
    # c_oracle varchar,
    # c_cost varchar,
    # c_image_ref varchar )

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
exit;
