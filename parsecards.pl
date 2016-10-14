#!/usr/bin/perl

use warnings;
use strict;
use WWW::Curl::Easy;
use Data::Dumper;
use DBI;
use utf8;
use Encode;
use threads;

our %CARDS;
$| = 1;

my $dbh = DBI->connect("DBI:Pg:dbname=magcol;host=localhost", "dbamagcol", "magcol", {'RaiseError' => 1});
$dbh->{ AutoCommit } = 0;
$dbh->{ pg_enable_utf8 } = 1;

sub parse_card
{
    my($cardid) = @_;
    my $curl = WWW::Curl::Easy::new();
    $curl->setopt(CURLOPT_HEADER,1);
    $curl->setopt(CURLOPT_URL, "http://www.svenskamagic.com/cardcollection/index.php?cardid=$cardid");
    my $response_body;
    $curl->setopt(CURLOPT_WRITEDATA,\$response_body);
    my $retcode = $curl->perform;

    $response_body =~ s/<br \/>//g;
    $response_body =~ s/<\/b>//g;
    $response_body =~ s/<\/a>//g;
    $response_body =~ s/<\/span>//g;
    $response_body =~ s/<span//g;
    $response_body =~ s/<img//g;
    my @lines = split('<', $response_body );

    my $cur_expansion;
    my $cur_card;
    my $rarity;
    my $img_ref;
    my %retval;

    foreach ( @lines ) {
        if ( /Zoom/ ) {
            $_ = encode( "UTF-8", $_ );
            ($img_ref) = $_ =~ /.*href="(.*)" target/;
        }

        if ( /rubrik" style/ ) {
            ($cur_card) = $_ =~ /.*>(.*)/;
            $cur_card = encode( "UTF-8", $cur_card );
        }

        if ( /text_brun noline/ ) {
            $_ = encode( "UTF-8", $_ );
            my @rarity_split = split(',', $_ );
            ($cur_expansion) = $rarity_split[0] =~ /.*>(.*)/;
            if ( $#rarity_split == 1 ) {
                $rarity = $rarity_split[1];
            }
            $retval{ $cur_expansion }{ $cur_card }{ rarity } = $rarity;
            $retval{ $cur_expansion }{ $cur_card }{ image_ref } = "http://www.svenskamagic.com$img_ref";
        }

        if ( /Korttyp/ ) {
            $_ = encode( "UTF-8", $_ );
            my $type = "";
            ($type) = $_ =~ /b>Korttyp:\t([A-z- ]+)/;
            if ( $type =~ /-/ ) {
                my($subtype) = $type =~ /.*- (.*)/;
                $type =~ s/ -.*//g;
                $retval{ $cur_expansion }{ $cur_card }{ subtype } = $subtype;
            }
            $retval{ $cur_expansion }{ $cur_card }{ cardtype } = $type;
        }

        if ( /CMC/ ) {
            ($retval{ $cur_expansion }{ $cur_card }{ CMC }) = $_ =~ /b>CMC:[\t ]([A-z0-9]*)/;
        }

        if ( /Manakostnad:/ ) {
            my @manacosttotal = split('.src', $_ );
            foreach my $manacost ( @manacosttotal ) {
                my($cur_cost) = $manacost =~ /.*([BGWUR0-9]).gif/;
                if ( $cur_cost ) {
                    $retval{ $cur_expansion }{ $cur_card }{ manacost } .= $cur_cost;
                }
            }
        }

        if ( /Nuvarande oracletext:/ ) {
            #$_ =~ s/\xc6/Ae/g;
            #$_ =~ s/\xe6/ae/g;
            #$_ =~ s/\x97/-/g;
            #$_ =~ s/\xe2/a/g;
            #$_ =~ s/\x80/-/g;
            $_ = encode( "UTF-8", $_ );
            ($retval{ $cur_expansion }{ $cur_card }{ oracle }) = $_ =~ /b>Nuvarande oracletext:\n\t+(.*)/;
        }
        #print "!$_!\n";
    }
    print ".";
    return \%retval;
}

sub push_to_db
{
    foreach my $expansion ( keys %::CARDS ) {
        foreach my $card ( keys %{ $::CARDS{ $expansion } } ) {
            print "pushing $expansion - $card to database\n";
            my $sth  = $dbh->prepare( 'SELECT add_card( ?, ?, ?, ?, ?, ?, ?, ? )' );
            $sth->bind_param( 1, $expansion );
            $sth->bind_param( 2, $card );
            $sth->bind_param( 3, $::CARDS{ $expansion }{ $card }{ rarity } );
            $sth->bind_param( 4, $::CARDS{ $expansion }{ $card }{ cardtype } );
            $sth->bind_param( 5, $::CARDS{ $expansion }{ $card }{ subtype } );
            $sth->bind_param( 6, $::CARDS{ $expansion }{ $card }{ oracle } );
            $sth->bind_param( 7, $::CARDS{ $expansion }{ $card }{ manacost } );
            $sth->bind_param( 8, $::CARDS{ $expansion }{ $card }{ image_ref } );
            $sth->execute();
            $sth->finish();
            delete $::CARDS{ $expansion }{ $card };
        }
    }
    $dbh->commit();
}

my @cards_to_scan = ( 1..40000 );
while ( @cards_to_scan ) {
    my $counter = 0;
    my @cards_to_add;
    my @thread_pool;
    # only 20 at a time
    if ( $#cards_to_scan >= 19 ) {
        @cards_to_add = splice( @cards_to_scan, -20 );
    } else {
        @cards_to_add = splice( @cards_to_scan, -$#cards_to_scan-1 );
    }

    # start thread for each card
    foreach my $card_id ( @cards_to_add ) {
        my $thread = threads->new( \&parse_card, $card_id );
        push( @thread_pool, $thread );
    }

    # join the threads, collect result
    foreach ( @thread_pool ) {
        my %retval = %{ $_->join() };
        foreach my $expansion ( keys %retval ) {
            foreach my $card ( keys %{ $retval{ $expansion } } ) {
                foreach my $keyvalue ( keys %{ $retval{ $expansion }{ $card } } ) {
                    $::CARDS{ $expansion }{ $card }{ $keyvalue } = $retval{ $expansion }{ $card }{ $keyvalue };
                }
            }
        }
    }
    push_to_db();
}

#print Dumper( %::CARDS );
