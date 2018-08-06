#include <stdio.h>

void mul_matrices( float A[ ][ 3 ], float B[ ][ 3 ], float C[ ][ 3 ] )
{
int i, j, k;

float sum;

for( i = 0; i < 3; i++ )
    for( j = 0; j < 3; j++ ) {
        sum = 0;
        for( k = 0; k < 3; k++ )
            sum = sum + A[ i ][ k ] * B[ k ][ j ];
        C[ i ][ j ] = sum;
    }
}

void print_matrix( float A[ ][ 3 ] ) {
    int i, j;
    for ( i = 0; i < 3; i++ ) {
        for( j = 0; j < 3; j++ )
            printf( "%.2lf\t", A[ i ][ j ] );
        printf("\n");
    }
}

int main( ) {

    float A[ 3 ][ 3 ] = { { 1, 2, 3 }, { 4, 5, 6 }, { 7, 8, 9 } };
    float B[ 3 ][ 3 ] = { { 10, 11, 12 }, { 13, 14, 15 }, { 16, 17, 18 } };
    float C[ 3 ][ 3 ];

    mul_matrices( A, B, C );

    print_matrix( C );

    return 0;
}
