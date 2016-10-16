#include <stdio.h>
#include <pHash.h>

/**
typedef struct ph_digest {
    char *id;                   //hash id
    uint8_t *coeffs;            //the head of the digest integer coefficient array
    int size;                   //the size of the coeff array
} Digest;
**/

int main()
{
    // DCT hashing
    ulong64 orig_hash, photo_hash;
    ph_dct_imagehash("data/sc.jpg", orig_hash);
    ph_dct_imagehash("data/sc_photo.jpg", photo_hash);
    int distance = ph_hamming_distance(orig_hash, photo_hash);
    printf( "Orig hash: 0x%llx, photo hash: 0x%llx. Distance: %d\n", orig_hash, photo_hash, distance );

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
    return 0;
}

