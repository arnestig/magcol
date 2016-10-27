#!/usr/bin/perl

use warnings;
use strict;

$|=1;

my $cards_counted;
my $successful_cards;

print "actual card, found card, result\n";
foreach my $photo_hash ( glob "../resources/photos/*.hash" ) {
    my $fsoutput = qx{../client/findsimilar 1 ../resources/hashes $photo_hash};
    my ($cur_cardname) = $photo_hash =~ /.*\/(.*)_photo\.jpg.hash/;
    my ($best_match,$score) = lc($fsoutput) =~ /.*\/(.*)_.*_\d+\.jpg\.hash - (\d+)/;

    print "$cur_cardname,$best_match ($score), ";
    if ( $cur_cardname eq $best_match ) {
        print "OK!\n";
        $successful_cards++;
    } else {
        print "FAIL!\n";
    }
    $cards_counted++;
}

printf( "$successful_cards/$cards_counted (%2d%%)",($successful_cards/$cards_counted*100) );
