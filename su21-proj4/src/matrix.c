#include "matrix.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

// Include SSE intrinsics
#if defined(_MSC_VER)
#include <intrin.h>
#elif defined(__GNUC__) && (defined(__x86_64__) || defined(__i386__))
#include <immintrin.h>
#include <x86intrin.h>
#endif

/* Below are some intel intrinsics that might be useful
 * void _mm256_storeu_pd (double * mem_addr, __m256d a)
 * __m256d _mm256_set1_pd (double a)
 * __m256d _mm256_set_pd (double e3, double e2, double e1, double e0)
 * __m256d _mm256_loadu_pd (double const * mem_addr)
 * __m256d _mm256_add_pd (__m256d a, __m256d b)
 * __m256d _mm256_sub_pd (__m256d a, __m256d b)
 * __m256d _mm256_fmadd_pd (__m256d a, __m256d b, __m256d c)
 * __m256d _mm256_mul_pd (__m256d a, __m256d b)
 * __m256d _mm256_cmp_pd (__m256d a, __m256d b, const int imm8)
 * __m256d _mm256_and_pd (__m256d a, __m256d b)
 * __m256d _mm256_max_pd (__m256d a, __m256d b)
*/

/* Generates a random double between low and high */
double rand_double(double low, double high) {
    double range = (high - low);
    double div = RAND_MAX / range;
    return low + (rand() / div);
}

/* Generates a random matrix */
void rand_matrix(matrix *result, unsigned int seed, double low, double high) {
    srand(seed);
    for (int i = 0; i < result->rows; i++) {
        for (int j = 0; j < result->cols; j++) {
            set(result, i, j, rand_double(low, high));
        }
    }
}

/*
 * Allocates space for a matrix struct pointed to by the double pointer mat with
 * `rows` rows and `cols` columns. You should also allocate memory for the data array
 * and initialize all entries to be zeros. `parent` should be set to NULL to indicate that
 * this matrix is not a slice. You should also set `ref_cnt` to 1.
 * You should return -1 if either `rows` or `cols` or both have invalid values. Return -2 if any
 * call to allocate memory in this function fails. Remember to set the error messages in numc.c.
 * Return 0 upon success.
 */
int allocate_matrix(matrix **mat, int rows, int cols) {
    /* TODO: YOUR CODE HERE */
    if (rows <= 0 || cols <= 0) {
        return -1;
    }
    *mat = malloc(sizeof(matrix));
    if (mat == NULL || *mat == NULL) {
        return -2;
    }
    (*mat)->rows = rows;
    (*mat)->cols = cols;
    // Allocate memory for ref_cnt 
    (*mat)->ptr_ref_cnt = malloc(sizeof(int));
    *((*mat)->ptr_ref_cnt) = 1;
    (*mat)->ref_cnt = 1;
    (*mat)->parent = NULL;
    (*mat)->data = calloc(rows * cols, sizeof(double));
    (*mat)->par_data = (*mat)->data;
    if ((*mat)->data == NULL) {
        free(mat);
        return -2;
    }
    return 0;
}

/*
 * Allocates space for a matrix struct pointed to by `mat` with `rows` rows and `cols` columns.
 * Its data should point to the `offset`th entry of `from`'s data (you do not need to allocate memory)
 * for the data field. `parent` should be set to `from` to indicate this matrix is a slice of `from`.
 * You should return -1 if either `rows` or `cols` or both have invalid values. Return -2 if any
 * call to allocate memory in this function fails.
 * Remember to set the error messages in numc.c.
 * Return 0 upon success.
 */
int allocate_matrix_ref(matrix **mat, matrix *from, int offset, int rows, int cols) {
    /* TODO: YOUR CODE HERE */
    if (rows <= 0 || cols <= 0) {
        return -1;
    }
    *mat = malloc(sizeof(matrix));
    if (mat == NULL || *mat == NULL) {
        return -2;
    }
    (*mat)->rows = rows;
    (*mat)->cols = cols;
    (*mat)->data = from->data + offset;
    from->ref_cnt += 1;
    *(from->ptr_ref_cnt) += 1;
    (*mat)->ptr_ref_cnt = from->ptr_ref_cnt;
    (*mat)->ref_cnt = from->ref_cnt;
    (*mat)->parent = from;
    (*mat)->par_data = from->par_data;
    return 0;
}

/*
 * You need to make sure that you only free `mat->data` if `mat` is not a slice and has no existing slices,
 * or that you free `mat->parent->data` if `mat` is the last existing slice of its parent matrix and its parent matrix has no other references
 * (including itself). You cannot assume that mat is not NULL.
 */
void deallocate_matrix(matrix *mat) {
    /* TODO: YOUR CODE HERE */
    if (mat == NULL) {
        return;
    }
    if (mat->ref_cnt > 1) {
        // Free matrix
        mat->ref_cnt -= 1;
        // *(mat->ptr_ref_cnt) -= 1;
        free(mat);
        return;
    }
    if (mat->ref_cnt == 1) {
        free(mat->par_data);
        free(mat->ptr_ref_cnt);
        free(mat);
    } 
    return;

}

/*
 * Returns the double value of the matrix at the given row and column.
 * You may assume `row` and `col` are valid.
 */
double get(matrix *mat, int row, int col) {
    /* TODO: YOUR CODE HERE */
    return mat->data[row * mat->cols + col];
}

/*
 * Sets the value at the given row and column to val. You may assume `row` and
 * `col` are valid
 */
void set(matrix *mat, int row, int col, double val) {
    /* TODO: YOUR CODE HERE */
    mat->data[row * mat->cols + col] = val;
    return;
}

/*
 * Sets all entries in mat to val
 */
void fill_matrix(matrix *mat, double val) {
    /* TODO: YOUR CODE HERE */
    for (int i = 0; i < mat->rows * mat->cols; i++) {
        mat->data[i] = val;
    }
}

/*
 * Store the result of adding mat1 and mat2 to `result`.
 * Return 0 upon success and a nonzero value upon failure.
 */
int add_matrix(matrix *result, matrix *mat1, matrix *mat2) {
    // Length of all matrices should be equal (checked in caller of this func)
    int mat_length = mat1->rows * mat1->cols;
    if (mat_length > 0 && mat_length < 100) {
        for (int i = 0; i < mat_length; i++) {
            result->data[i] = mat1->data[i] + mat2->data[i];
        }
        return 0;
    } else {
        #pragma omp parallel for
        for (int i = 0; i < mat_length; i++) {
            result->data[i] = mat1->data[i] + mat2->data[i];
        }
        return 0;
    }
    return -3;
}

/*
 * (OPTIONAL)
 * Store the result of subtracting mat2 from mat1 to `result`.
 * Return 0 upon success and a nonzero value upon failure.
 */
int sub_matrix(matrix *result, matrix *mat1, matrix *mat2) {
    /* TODO: YOUR CODE HERE */
    for (int i = 0; i < mat1->rows; i++) {
        for (int j = 0; j < mat1->cols; j++) {
            set(result, i, j, get(mat1, i, j) - get(mat2, i, j));
            if (get(result, i, j) != get(mat1, i, j) - get(mat2, i, j)) {
                return -3;
            }
        }
    }
    return 0;
}

/*
 * Store the result of multiplying mat1 and mat2 to `result`.
 * Return 0 upon success and a nonzero value upon failure.
 * Remember that matrix multiplication is not the same as multiplying individual elements.
 */
int mul_matrix(matrix *result, matrix *mat1, matrix *mat2) {
    int rows = mat1->rows;
    int cols = mat2->cols;
    int mat_length = rows * cols;
    if (mat1->cols != mat2->rows) {
        return -3;
    }

    double *data2T = malloc(mat2->rows * mat2->cols * sizeof(double));

    // Use simd for all, pragma omp for if size > 100, 1000, 10000 (compare)
    // Maybe: pointer arithmetic
    #pragma omp parallel for if (mat_length > 100)
    for (int i = 0; i < mat2->cols; i++) {
        // #pragma omp parallel for if (mat_length > 100)
        for (int j = 0; j < mat2->rows / 4 * 4; j += 4) {
            data2T[i * mat2->rows + j] = mat2->data[i + j * mat2->cols];
            data2T[i * mat2->rows + j + 1] = mat2->data[i + (j + 1) * mat2->cols];
            data2T[i * mat2->rows + j + 2] = mat2->data[i + (j + 2) * mat2->cols];
            data2T[i * mat2->rows + j + 3] = mat2->data[i + (j + 3) * mat2->cols];
        }

        // COVER THE EDGE CASE
        for (int j = mat2->rows / 4 * 4; j < mat2->rows; j++) {
            data2T[i * mat2->rows + j] = mat2->data[i + j * mat2->cols];
        }
    }

    #pragma omp parallel for if (mat_length > 100)
    for (int i = 0; i < rows; i++) {
        // #pragma omp parallel for if (mat_length > 100)
        for (int j = 0; j < cols; j++) {
            __m256d sum_vec = _mm256_setzero_pd();
            __m256d temp1;
            __m256d temp2;
            int k = 0;
            double sum = 0;

            for (; k < mat1->cols / 16 * 16; k += 16) {
                temp1 = _mm256_loadu_pd((mat1->data) + i * mat1->cols + k);
                temp2 = _mm256_loadu_pd(data2T + j * mat2->rows + k);
                sum_vec = _mm256_fmadd_pd(temp1, temp2, sum_vec);

                temp1 = _mm256_loadu_pd((mat1->data) + i * mat1->cols + k + 4);
                temp2 = _mm256_loadu_pd(data2T + j * mat2->rows + k + 4);
                sum_vec = _mm256_fmadd_pd(temp1, temp2, sum_vec);

                temp1 = _mm256_loadu_pd((mat1->data) + i * mat1->cols + k + 8);
                temp2 = _mm256_loadu_pd(data2T + j * mat2->rows + k + 8);
                sum_vec = _mm256_fmadd_pd(temp1, temp2, sum_vec);

                temp1 = _mm256_loadu_pd((mat1->data) + i * mat1->cols + k + 12);
                temp2 = _mm256_loadu_pd(data2T + j * mat2->rows + k + 12);
                sum_vec = _mm256_fmadd_pd(temp1, temp2, sum_vec);
            }

            double temp_arr[4];
            _mm256_storeu_pd(temp_arr, sum_vec);
            sum += sum_vec[0] + sum_vec[1] + sum_vec[2] + sum_vec[3];

            for (; k < mat1->cols; k++) {
                sum += mat1->data[i * mat1->cols + k] * data2T[j * mat2->rows + k];
            }
            result->data[i * result->cols + j] = sum;
        }
    }
    free(data2T);
    return 0;
}

/*
 * Store the result of raising mat to the (pow)th power to `result`.
 * Return 0 upon success and a nonzero value upon failure.
 * Remember that pow is defined with matrix multiplication, not element-wise multiplication.
 */
int pow_matrix(matrix *result, matrix *mat, int pow) {
    /* TODO: YOUR CODE HERE */
    if (pow < 0) {
        return -4;
    }

    if (mat->rows != mat->cols) {
        return -3;
    }

    result->rows = mat->rows;
    result->cols = mat->cols;

    if (pow == 0) {
        // Store the identity matrix in result->data for 0th power
        int n = mat->rows;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (i == j) {
                    result->data[i * result->cols + j] = 1;
                } else {
                    result->data[i * result->cols + j] = 0;
                }
            }
        }
        return 0;
    }

    int max = 1; // 2 ^ (max - 1) should be greater than pow
    for (; 1 << max <= pow; max++) {
        continue;
    }
    int binaryExpansion[max]; // binaryExpansion[i] will be 1 if 2 ^ i is in the expansion and 0 otherwise
    max -= 1;
    int tempPow = pow;
    for (int i = max; i >= 0; i--) {
        if (1 << i <= tempPow) {
            binaryExpansion[i] = 1;
            tempPow -= 1 << i;
        } else {
            binaryExpansion[i] = 0;
        }
    }
    max -= 1; // This is done because binaryExpansion[max] has to be 1

    matrix *temp = NULL;
    allocate_matrix(&temp, mat->rows, mat->cols);
    #pragma omp parallel for
    for (int i = 0; i < mat->rows; i++) { // Sets temp to equal mat
        #pragma omp parallel for
        for (int j = 0; j < mat->cols; j++) {
            temp->data[i * mat->cols + j] = mat->data[i * mat->cols + j];
        }
    }
    int counter = 0;
    for (int i = max; i >= 0; i--) {
        if (counter == 0) {
            mul_matrix(result, temp, temp);
            if (binaryExpansion[i] == 1) {
                mul_matrix(temp, result, mat);
            } else {
                counter = 1;
            }
        } else {
            mul_matrix(temp, result, result);
            if (binaryExpansion[i] == 1) {
                mul_matrix(result, temp, mat);
            } else {
                counter = 0;
            }
        }
    }
    if (counter == 0) { // Result is in temp if counter is 0
        for (int i = 0; i < mat->rows; i++) {
            for (int j = 0; j < mat->cols; j++) {
                result->data[i * mat->cols + j] = temp->data[i * mat->cols + j];
            }
        }
    }
    deallocate_matrix(temp);
    return 0;
}

/*
 * (OPTIONAL)
 * Store the result of element-wise negating mat's entries to `result`.
 * Return 0 upon success and a nonzero value upon failure.
 */
int neg_matrix(matrix *result, matrix *mat) {
    /* TODO: YOUR CODE HERE */
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            set(result, i, j, -1 * get(mat, i, j));
            if (get(result, i, j) != -1 * get(mat, i, j)) {
                return -3;
            }
        }
    }
    return 0;
}

/*
 * Store the result of taking the absolute value element-wise to `result`.
 * Return 0 upon success and a nonzero value upon failure.
 */
int abs_matrix(matrix *result, matrix *mat) {
    int mat_length = mat->rows*mat->cols;

    /* SIMD CODE*/
    if (mat_length > 0 && mat_length < 100) {
        for (int i = 0; i < mat_length; i++) {
            // If neg
            double temp = mat->data[i];
            if (temp < 0) {
                result->data[i] = -1 * temp;
            }
            else {
                result->data[i] = temp;
            }
        }
        return 0;
    } else {
        __m256d neg =_mm256_set1_pd(-1.0);
        #pragma omp parallel for
        for (int i = 0; i < mat_length / 4 *4; i += 4) {
            __m256d orig_vec = _mm256_loadu_pd(mat->data + i);
            __m256d neg_vec = _mm256_mul_pd(orig_vec, neg);
            _mm256_storeu_pd(result->data + i, _mm256_max_pd(orig_vec, neg_vec));
        }

        for(int i = mat_length / 4 * 4; i < mat_length; i++) {
            // If neg
            double temp = mat->data[i];
            if (temp < 0) {
                result->data[i] = -1 * temp;
            }
            else {
                result->data[i] = temp;
            }
        }
        return 0;
    }
}
