#include <stdio.h>
#include <pHash.h>
#include <iostream>
#include <iomanip>
#include <fstream>

#include <glob.h>
#include <vector>
#include <algorithm>
#include <string>

struct dctResult
{
    std::string filename;
    ulong64 hashval;
    int distance;
};

std::vector< dctResult > DCTvector;

static bool DCTSorter( dctResult l, dctResult r )
{
    return l.distance < r.distance;
}

inline std::vector< std::string > glob( const std::string& directory )
{
    glob_t glob_result;
    glob( directory.c_str(), GLOB_TILDE, NULL, &glob_result );
    std::vector< std::string > retval;
    for( unsigned int i = 0; i < glob_result.gl_pathc; ++i ) {
        retval.push_back( std::string( glob_result.gl_pathv[ i ] ) );
    }
    globfree( &glob_result );

    return retval;
}

void findBestDCTMatch( std::string basefile, std::string directory )
{
    ulong64 hash;
    ph_dct_imagehash(basefile.c_str(), hash);
    std::vector< std::string > files = glob( directory );
    for( std::vector< std::string >::iterator it = files.begin(); it != files.end(); ++it ) {
        dctResult res;
        res.filename = (*it);
        ph_dct_imagehash((*it).c_str(), res.hashval);
        res.distance = ph_hamming_distance(hash, res.hashval);
        DCTvector.push_back( res );
    }
    std::sort( DCTvector.begin(), DCTvector.end(), DCTSorter );

    for( int i = 0; i < 5; i++ ) {
        std::cout << DCTvector[ i ].filename << " - " << DCTvector[ i ].distance << std::endl;
    }
}

void hashes()
{
    // radial hashing
    Digest orig_dig, photo_dig;
    ph_image_digest("data/sc.jpg", 1.0, 1.0, orig_dig, 180);
    ph_image_digest("data/sc_photo.jpg", 1.0, 1.0, photo_dig, 180);
    double correlation;
    if ( ph_crosscorr(orig_dig, photo_dig, correlation, 0.90) ) {
        printf( "Correlation: %f\n", correlation );
        printf( " -- \n" );
        printf( "orig size: %d\n", orig_dig.size );
        for ( int i = 0; i < orig_dig.size; i++ ) {
            printf( " %d: %d", i, orig_dig.coeffs[ i ] );
        }
        printf("\n");
        printf( "photo size: %d\n", photo_dig.size );
        for ( int i = 0; i < photo_dig.size; i++ ) {
            printf( " %d: %d", i, photo_dig.coeffs[ i ] );
        }
        printf("\n");
    }
}

void genHashFromFile( std::string filename, std::string expansion, std::string manaCost )
{
    std::string outputfilename = filename + ".hash";
    FILE *hash_results;
    hash_results = fopen( outputfilename.c_str(), "w" );

    // MH
    int alpha = 2;
    int level = 1;
    int mh_hashlen = 0;
    uint8_t *hash = ph_mh_imagehash( filename.c_str(), mh_hashlen, alpha, level );
    fprintf(hash_results,"MH: ");
    for ( int i = 0; i < mh_hashlen; i++ ) {
        fprintf(hash_results,"%02x", hash[ i ] );
    }
    fprintf(hash_results,"\n");

    // DCT
    ulong64 dct_hash;
    ph_dct_imagehash(filename.c_str(), dct_hash);
    fprintf(hash_results,"DCT: %llx\n", dct_hash );

    // radial
    Digest file_dig;
    ph_image_digest(filename.c_str(), 1.0, 1.0, file_dig, 180);
    fprintf( hash_results, "Radial: %d", file_dig.size );
    for ( int i = 0; i < file_dig.size; i++ ) {
        fprintf(hash_results, ",%02x", file_dig.coeffs[ i ] );
    }
    fprintf(hash_results, "\n");

    fprintf(hash_results, "Expansion: %s\nManacost: %s\n",expansion.c_str(), manaCost.c_str() );

    std::cout << "Created " << filename << ".hash" << std::endl;
    fclose( hash_results );
}


int main( int argc, char *argv[] )
{
    std::string expansion;
    std::string manaCost;
    if ( argc == 3 ) { expansion = argv[2]; }
    if ( argc == 4 ) { expansion = argv[2]; manaCost = argv[3]; }
    if ( argc >= 2 && argc <= 4 ) {
        genHashFromFile( argv[1], expansion, manaCost );
    } else {
        std::cout << "Error! Usage: genimagehash <filename>" << std::endl;
        return 1;
    }
    return 0;
}

