#!/usr/bin/perl

use warnings;
use strict;
use Data::Dumper;
use DBI;
use File::Fetch;

our %IMAGES;
$| = 1;

my $dbh = DBI->connect("DBI:Pg:dbname=magcol;host=localhost", "dbamagcol", "magcol", {'RaiseError' => 1});

sub download_image
{
    my($url,$path) = @_;
    my $ff = File::Fetch->new( uri => $url );
    $ff->fetch( to => "images/$path" ) or die $ff->error();
}

sub fetch_img_refs
{
    $dbh->{ AutoCommit } = 1;
    # select our stored procedure for hosts to scan
    my $sth = $dbh->prepare( 'SELECT card_image_ref,card_expansion FROM cards;' );
    $sth->execute();

    # get all objects to scan
    my $hash_dump = $sth->fetchall_hashref( 'card_image_ref' );
    return $hash_dump;
}

my $images = fetch_img_refs();
foreach ( keys %{ $images } ) {
    print "Download $_ to $images->{ $_ }{ 'card_expansion' }\n";
    download_image( $_, $images->{ $_ }{ 'card_expansion' } );
}
