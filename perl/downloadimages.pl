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
    my($multiverseid,$name,$expansion,$manacost) = @_;
    my $url = "http://gatherer.wizards.com/Handlers/Image.ashx?multiverseid=$multiverseid&type=card";
    if ( ! -e "../resources/hashes/$name.hash" ) {
        print "Downloading $name\n";
        system( "curl '$url' -o ../resources/hashes/$name --silent" );
        my $is_png = qx{file ../resources/hashes/$name | grep -c PNG};
        if ( $is_png ) {
            system( "convert ../resources/hashes/$name ../resources/hashes/$name" );
        }
        system( "../client/genimagehash ../resources/hashes/$name $expansion $manacost" );
        unlink( "../resources/hashes/$name" );
    } else {
        print "Updating $name\n";
        open( HASH_FILE, ">>../resources/hashes/$name.hash" ) or die( "$!" );
        print HASH_FILE "Expansion: $expansion\n";
        print HASH_FILE "Manacost: $manacost\n";
        close( HASH_FILE );
    }
}

if ( ! -e "AllSets.json" ) {
    print "Downloading json database\n";
    my $ff = File::Fetch->new( uri => "http://mtgjson.com/json/AllSets.json" );
    $ff->fetch( to => "." ) or die $ff->error();
}

open( JSON_DB, "<AllSets.json" ) or die( "Error: $!" );
my $json = JSON->new->allow_nonref; my $json_ref = $json->decode( <JSON_DB> ); close( JSON_DB );
foreach my $expansion ( sort {$a cmp $b} keys %{ $json_ref } ) {
    foreach my $card ( @{ $json_ref->{ $expansion }{ cards } } ) {
        my $name = $card->{ name };
        $name =~ s/[^a-zA-Z ]//g;
        $name =~ s/ /_/g;
        if ( defined $card->{ multiverseid } ) {
            my $mid = $card->{ multiverseid };
            my %image;
            $image{ mid } = $mid;
            $image{ expansion } = $expansion;
            if ( $card->{ manaCost } ) {
                $image{ manaCost } = $card->{ manaCost };
            } else {
                $image{ manaCost } = "";
            }
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
        my $thread = threads->new(\&download_image, $image->{ mid }, $image->{ name }, $image->{ expansion }, $image->{ manaCost } );
        push( @thread_pool, $thread );
    }

    foreach ( @thread_pool ) {
        $_->join();
    }
}

