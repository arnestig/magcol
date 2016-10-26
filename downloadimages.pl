#!/usr/bin/perl

use warnings;
use strict;
use Data::Dumper;
use LWP::Simple;
use JSON;
use threads;

our @images;
$| = 1;

sub download_image
{
    my($multiverseid,$name) = @_;
    my $url = "http://gatherer.wizards.com/Handlers/Image.ashx?multiverseid=$multiverseid&type=card";
    print "Downloading $name\n";
    getstore( $url, "images/$name" );
}

sub fetch_img_refs
{
}

if ( ! -e "ISD.json" ) {
    print "Downloading json database\n";
    my $ff = File::Fetch->new( uri => "http://mtgjson.com/json/ISD.json" );
    $ff->fetch( to => "." ) or die $ff->error();
}

open( JSON_DB, "<ISD.json" ) or die( "Error: $!" );
my $json = JSON->new->allow_nonref;
my $json_ref = $json->decode( <JSON_DB> );
my %json_hash = %{ $json_ref };
close( JSON_DB );
foreach my $card ( @{ $json_ref->{ cards } } ) {
    my $name = $card->{ name };
    $name =~ s/[^a-zA-Z ]//g;
    $name =~ s/ /_/g;
    my $expansion = "ISD";
    if ( defined $card->{ multiverseid } ) {
        my $mid = $card->{ multiverseid };
        my %image;
        $image{ mid } = $mid;
        $image{ name } = "$name\_$expansion\_$mid.jpg";
        push( @images, \%image );
    }
}

foreach my $image ( @images ) {
    if ( ! -e "images/$image->{ name }" ) {
        download_image( $image->{ mid }, $image->{ name } );
        # now we should do all three calculations on the downloaded image.
        # Calling genimagehash with different arguments
        # also this is single thread right now, code below works but FF is not thread safe. Should give curl a try instead
    }
}

#while ( @images ) {
    #my @images_to_download;
    #my @thread_pool;
#
    #if ( $#images >= 19 ) {
        #@images_to_download = splice( @images, -20 );
    #} else {
        #@images_to_download = splice( @images, -$#images-1 );
    #}
#
    #foreach my $image ( @images_to_download ) {
        #if ( ! -e "images/$image->{ name }" ) {
            #my $thread = threads->new(\&download_image, $image->{ mid }, $image->{ name } );
            #push( @thread_pool, $thread );
        #}
    #}
#
    #foreach ( @thread_pool ) {
        #$_->join();
    #}
#}
