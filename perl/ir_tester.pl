#!/usr/bin/perl

use warnings;
use strict;

$|=1;

my $cards_counted;
my %successful_cards;

print "type, actual card, found card, score, result\n";
foreach my $photo_hash ( glob "../resources/photos/*.hash" ) {
    my @fsoutput = qx{../client/findsimilar 1 ../resources/hashes $photo_hash};
    foreach ( @fsoutput ) {
        my ($cur_cardname) = $photo_hash =~ /.*\/(.*)_photo\.jpg.hash/;
        my ($type,$best_match,$score) = lc($_) =~ /(.*):.*\/(.*)_.*_\d+\.jpg\.hash - (\d+)/;

        print "$type, $cur_cardname,$best_match,$score, ";
        if ( $cur_cardname eq $best_match ) {
            print "OK!\n";
            $successful_cards{$type}++;
        } else {
            print "FAIL!\n";
        }
    }
    $cards_counted++;
}

printf( "DCT: $successful_cards{'dct'}/$cards_counted (%2d%%)",($successful_cards{'dct'}/$cards_counted*100) );
printf( "Radial: $successful_cards{'radial'}/$cards_counted (%2d%%)",($successful_cards{'radial'}/$cards_counted*100) );
