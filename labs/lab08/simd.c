#include <time.h>
#include <stdio.h>
#include <x86intrin.h>
#include "simd.h"

long long int sum(int vals[NUM_ELEMS]) {
    clock_t start = clock();

    long long int sum = 0;
    for(unsigned int w = 0; w < OUTER_ITERATIONS; w++) {
        for(unsigned int i = 0; i < NUM_ELEMS; i++) {
            if(vals[i] >= 128) {
                sum += vals[i];
            }
        }
    }
    clock_t end = clock();
    printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
    return sum;
}

long long int sum_unrolled(int vals[NUM_ELEMS]) {
    clock_t start = clock();
    long long int sum = 0;

    for(unsigned int w = 0; w < OUTER_ITERATIONS; w++) {
        for(unsigned int i = 0; i < NUM_ELEMS / 4 * 4; i += 4) {
            if(vals[i] >= 128) sum += vals[i];
            if(vals[i + 1] >= 128) sum += vals[i + 1];
            if(vals[i + 2] >= 128) sum += vals[i + 2];
            if(vals[i + 3] >= 128) sum += vals[i + 3];
        }

        // TAIL CASE, for when NUM_ELEMS isn't a multiple of 4
        // NUM_ELEMS / 4 * 4 is the largest multiple of 4 less than NUM_ELEMS
        // Order is important, since (NUM_ELEMS / 4) effectively rounds down first
        for(unsigned int i = NUM_ELEMS / 4 * 4; i < NUM_ELEMS; i++) {
            if (vals[i] >= 128) {
                sum += vals[i];
            }
        }
    }
    clock_t end = clock();
    printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
    return sum;
}

long long int sum_simd(int vals[NUM_ELEMS]) {
    clock_t start = clock();
    __m128i _127 = _mm_set1_epi32(127); // This is a vector with 127s in it... Why might you need this?
    long long int result = 0;                   // This is where you should put your final result!
    /* DO NOT MODIFY ANYTHING ABOVE THIS LINE (in this function) */

    for(unsigned int w = 0; w < OUTER_ITERATIONS; w++) {
        /* YOUR CODE GOES HERE */
        __m128i sum_vec = _mm_setzero_si128();
        __m128i temp;
        __m128i check;
        int i;

        for (i = 0; i < NUM_ELEMS / 4 * 4; i += 4) {
            temp = _mm_loadu_si128((__m128i *) (vals + i));
            check = _mm_cmpgt_epi32(temp, _127);
            sum_vec = _mm_add_epi32(sum_vec, _mm_and_si128(temp, check));
        }

        for (; i < NUM_ELEMS; i++) {
            if (vals[i] >= 128) {
                result += vals[i];
            }
        }

        int temp_arr[4];
        _mm_storeu_si128((__m128i *) temp_arr, sum_vec);
        result += temp_arr[0] + temp_arr[1] + temp_arr[2] + temp_arr[3];
        /* Hint: you'll need a tail case. */
    }

    /* DO NOT MODIFY ANYTHING BELOW THIS LINE (in this function) */
    clock_t end = clock();
    printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
    return result;
}

long long int sum_simd_unrolled(int vals[NUM_ELEMS]) {
    clock_t start = clock();
    __m128i _127 = _mm_set1_epi32(127);
    long long int result = 0;
    /* DO NOT MODIFY ANYTHING ABOVE THIS LINE (in this function) */

    for(unsigned int w = 0; w < OUTER_ITERATIONS; w++) {
        /* YOUR CODE GOES HERE */
        /* Copy your sum_simd() implementation here, and unroll it */
        int size = 16;
        __m128i sum_vec = _mm_setzero_si128();
        __m128i temp0, temp1, temp2, temp3;
        __m128i check0, check1, check2, check3;
        int i;

        for (i = 0; i < NUM_ELEMS / size * size; i += size) {
            int *ptr = vals + i;

            temp0 = _mm_loadu_si128((__m128 *) ptr);
            check0 = _mm_cmpgt_epi32(temp0, _127);
            sum_vec = _mm_add_epi32(sum_vec, _mm_and_si128(temp0, check0));

            temp1 = _mm_loadu_si128((__m128 *) (ptr + 4));
            check1 = _mm_cmpgt_epi32(temp1, _127);
            sum_vec = _mm_add_epi32(sum_vec, _mm_and_si128(temp1, check1));

            temp2 = _mm_loadu_si128((__m128 *) (ptr + 8));
            check2 = _mm_cmpgt_epi32(temp2, _127);
            sum_vec = _mm_add_epi32(sum_vec, _mm_and_si128(temp2, check2));

            temp3 = _mm_loadu_si128((__m128 *) (ptr + 12));
            check3 = _mm_cmpgt_epi32(temp3, _127);
            sum_vec = _mm_add_epi32(sum_vec, _mm_and_si128(temp3, check3));
        }       


        /* Hint: you'll need 1 or maybe 2 tail cases here. */
        for (; i < NUM_ELEMS; i++) {
            if (vals[i] >= 128) {
                result += vals[i];
            }
        }

        int temp_arr[4];
        _mm_storeu_si128((__m128i *) temp_arr, sum_vec);
        result += temp_arr[0] + temp_arr[1] + temp_arr[2] + temp_arr[3];
    }

    /* DO NOT MODIFY ANYTHING BELOW THIS LINE (in this function) */
    clock_t end = clock();
    printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
    return result;
}
