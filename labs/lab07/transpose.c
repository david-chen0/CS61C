#include "transpose.h"
#include <math.h>

/* The naive transpose function as a reference. */
void transpose_naive(int n, int blocksize, int *dst, int *src) {
    for (int x = 0; x < n; x++) {
        for (int y = 0; y < n; y++) {
            dst[y + x * n] = src[x + y * n];
        }
    }
}

/* Implement cache blocking below. You should NOT assume that n is a
 * multiple of the block size. */
void transpose_blocking(int n, int blocksize, int *dst, int *src) {
    // YOUR CODE HERE
    int subBlocks = floor(n / blocksize) + 1;
    for (int i = 0; i < subBlocks + 1; i++) { // block count of x
        for (int j = 0; j < subBlocks + 1; j++) { // block count of y
            for (int a = 0; a < blocksize; a++) { // index count of y
                for (int b = 0; b < blocksize; b++) { // index count of x
                    int y = a + (j * blocksize);
                    int x = b + (i * blocksize);
                    if (x >= n || y >= n) {
                        continue;
                    }
                    dst[y + x * n] = src[x + y * n];
                }
            }
        }
    }
}
