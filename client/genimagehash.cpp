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

// MH
void genHashFromFile( std::string filename )
{
    int alpha = 2;
    int level = 1;
    int mh_hashlen = 0;
    uint8_t *hash = ph_mh_imagehash( filename.c_str(), mh_hashlen, alpha, level );
    std::ofstream hashmh_file;
    hashmh_file.open( filename + ".hashmh" );
    for ( int i = 0; i < mh_hashlen; i++ ) {
        //std::cout << std::hex << std::setw(2) << std::setfill( '0' ) << (int)hash[i];
        hashmh_file << std::hex << std::setw(2) << std::setfill( '0' ) << (int)hash[i];
    }
    hashmh_file.close();
    //std::cout << std::endl;
    std::cout << "Created " << filename << ".hashmh" << std::endl;
}

int main( int argc, char *argv[] )
{
    
    if ( argc == 2 ) {
        genHashFromFile( argv[1] );
    } else if ( argc == 3 ) {
        findBestDCTMatch( argv[1], argv[2] );
    } else {
        std::cout << "Error! Usage: genimagehash <filename>" << std::endl;
        return 1;
    }
    return 0;
}

