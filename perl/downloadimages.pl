#!/usr/bin/perl

use warnings;
use strict;
use Data::Dumper;
use File::Fetch;
use JSON;
use threads;

our @images;
$| = 1;

sub download_image
{
    my($multiverseid,$name) = @_;
    my $url = "http://gatherer.wizards.com/Handlers/Image.ashx?multiverseid=$multiverseid&type=card";
    print "Downloading $name\n";
    system( "curl '$url' -o hashes/$name --silent" );
    my $is_png = qx{file hashes/$name | grep -c PNG};
    if ( $is_png ) {
        system( "convert hashes/$name hashes/$name" );
    }
    system( "client/genimagehash hashes/$name" );
    unlink( "hashes/$name" );
}

if ( ! -e "AllSets.json" ) {
    print "Downloading json database\n";
    my $ff = File::Fetch->new( uri => "http://mtgjson.com/json/AllSets.json" );
    $ff->fetch( to => "." ) or die $ff->error();
}

open( JSON_DB, "<AllSets.json" ) or die( "Error: $!" );
my $json = JSON->new->allow_nonref; my $json_ref = $json->decode( <JSON_DB> ); close( JSON_DB );
foreach my $expansion ( keys %{ $json_ref } ) {
    foreach my $card ( @{ $json_ref->{ $expansion }{ cards } } ) {
        my $name = $card->{ name };
        $name =~ s/[^a-zA-Z ]//g;
        $name =~ s/ /_/g;
        if ( defined $card->{ multiverseid } ) {
            my $mid = $card->{ multiverseid };
            my %image;
            $image{ mid } = $mid;
            $image{ name } = "$name\_$expansion\_$mid.jpg";
            push( @images, \%image );
        }
    }
}

while ( @images ) {
    my @images_to_download;
    my @thread_pool;

    if ( $#images >= 19 ) {
        @images_to_download = splice( @images, -20 );
    } else {
        @images_to_download = splice( @images, -$#images-1 );
    }

    foreach my $image ( @images_to_download ) {
        if ( ! -e "hashes/$image->{ name }.hash" ) {
            my $thread = threads->new(\&download_image, $image->{ mid }, $image->{ name } );
            push( @thread_pool, $thread );
        }
    }

    foreach ( @thread_pool ) {
        $_->join();
    }
}

