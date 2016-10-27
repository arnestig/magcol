#include <stdio.h>
#include <pHash.h>
#include <iostream>
#include <iomanip>
#include <fstream>

#include <glob.h>
#include <vector>
#include <algorithm>
#include <string> 

struct hashFile
{
    std::string filename;
    ulong64 dct;
    Digest dig;
    uint8_t *mh_hash;
    int distance;
};

static bool DCTSorter( hashFile l, hashFile r )
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

void findBestDCTMatch( hashFile targetfile, std::vector< hashFile > imageHashes )
{
    ulong64 hash;
    for( std::vector< hashFile >::iterator it = imageHashes.begin(); it != imageHashes.end(); ++it ) {
        (*it).distance = ph_hamming_distance(targetfile.dct, (*it).dct);
    }
    std::sort( imageHashes.begin(), imageHashes.end(), DCTSorter );

    for( int i = 0; i < 10; i++ ) {
        std::cout << imageHashes[ i ].filename << " - " << imageHashes[ i ].distance << std::endl;
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

hashFile loadHashFromFile( std::string filename )
{
    hashFile retval;
    retval.filename = filename;
    FILE *hash_file;
    hash_file = fopen( filename.c_str(), "r" );
    char buf[1024];

    /**
    // MH
    uint8_t *hash = ph_mh_imagehash( filename.c_str(), mh_hashlen, alpha, level );
    for ( int i = 0; i < mh_hashlen; i++ ) {
        fprintf(hash_results,"%02x", hash[ i ] );
    }
    **/
    fgets(buf, 1024, hash_file);

    // DCT
    ulong64 dct_hash;
    fgets(buf, 1024, hash_file);
    sscanf(buf,"DCT: %llx\n", &dct_hash );
    retval.dct = dct_hash;

    // radial
    Digest file_dig;
    /**fprintf( hash_results, "Radial: %d", file_dig.size );
    for ( int i = 0; i < file_dig.size; i++ ) {
        fprintf(hash_results, ",%02x", file_dig.coeffs[ i ] );
    }
    fprintf(hash_results, "\n");

    std::cout << "Created " << filename << ".hash" << std::endl;
    **/
    fclose( hash_file );
    return retval;
}


int main( int argc, char *argv[] )
{
    if ( argc == 3 ) {
        std::string directory = argv[1];
        directory += "/*.hash";
        hashFile targetfile = loadHashFromFile( argv[2] );
        std::vector< hashFile > imageHashes;
        std::vector< std::string > files = glob( directory );
        for( std::vector< std::string >::iterator it = files.begin(); it != files.end(); ++it ) {
            hashFile tmph = loadHashFromFile( (*it) );
            imageHashes.push_back( tmph );
        }
        findBestDCTMatch( targetfile, imageHashes );
    } else {
        std::cout << "Error! Usage: findsimilar <directory with hashes> <filename>" << std::endl;
        return 1;
    }
    return 0;
}

