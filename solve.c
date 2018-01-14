#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <omp.h>
#include "shared_consts.h"


void partial_choose(double **matrix, int rows, int column, int sub_matrix_size) {
	int i, max_i, start = rows - sub_matrix_size;
	double max_v;

#ifndef FALSE_SHARING
	__assume_aligned(matrix, ALIGNMENT_SIZE);
#endif

#ifdef NO_DEFAULT_VECTORIZATION
#pragma novector
#endif
	for (i = start, max_v = matrix[start][start]; i < rows; i++)
		if (matrix[i][start] > max_v) {
			max_v = matrix[i][start];
			max_i = i;
		}

	if (matrix[start][start] < max_v) {
		double *tmp = matrix[max_i];

		matrix[max_i] = matrix[start];
		matrix[start] = tmp;
	}
}


double* solve_with_partial_choose(double **matrix, int rows, int columns) {
	int i, j, k;
	double c, *R = ALLOC_MEMORY(sizeof(double)*rows);
	char nieskonczenie_wiele = 0;

	if (R == NULL) return NULL;

#ifndef FALSE_SHARING
	__assume_aligned(R, ALIGNMENT_SIZE);
	__assume_aligned(matrix, ALIGNMENT_SIZE);
#endif

	//postępowanie proste
	for (i = 0; i < rows; i++) {
			partial_choose(matrix, rows, columns, rows - i);

			if (matrix[i][i] != 0) {
				for (j = i + 1; j < rows; j++) {
#ifdef MANUAL_VECTORIZATION
#pragma ivdep
#endif
					for (k = i, c = matrix[j][i] / matrix[i][i]; k < columns; k++) {
						matrix[j][k] -= c*matrix[i][k];
					}
				}
			}
	}

	//sprawdzam czy istnieją rozwiązania
	for (i = rows - 1; i >= 0; i--)
	if (matrix[i][i] == 0 && matrix[i][columns - 1] != 0)
		return NULL; //brak rozwiązań
	else if (matrix[i][i] == 0 && matrix[i][columns - 1] == 0)
		nieskonczenie_wiele = 1;

	if (nieskonczenie_wiele) {
		R[0] = INFINITY;
		return R;
	}

	//Postępowanie odwrtone
	for (i = rows - 1; i >= 0; i--) {
		for (R[i] = matrix[i][columns - 1], j = i + 1; j < columns - 1; j++)
			R[i] -= matrix[i][j] * R[j];

		assert(matrix[i][i] != 0);
		R[i] /= matrix[i][i];
	}

	return R;
}


void partial_choose_parallel(double **matrix, int rows, int columns, int sub_matrix_size) {
	int i, j, start = rows - sub_matrix_size,
        max_i = start, 
        priv_max_i = start;
	double max_v = matrix[start][start], priv_max_v = matrix[start][start];

	if (start >= rows) return;

#ifndef FALSE_SHARING
	__assume_aligned(matrix, ALIGNMENT_SIZE);
#endif

#pragma omp parallel default(none) shared(matrix, rows, columns, sub_matrix_size, max_v, max_i, start) firstprivate(priv_max_i, priv_max_v) private(i)
	{
#pragma omp for schedule(static)
#ifdef NO_DEFAULT_VECTORIZATION
#pragma novector
#endif
		for (i = start; i < rows; i++) {
			if (priv_max_v < matrix[i][start]) {
				priv_max_i = i;
				priv_max_v = matrix[i][start];
			}
		}

#pragma omp flush (max_v) 
		{
            if (priv_max_v > max_v) {
#pragma omp critical
                {
                    if (priv_max_v > max_v) {
                        max_v = priv_max_v;
                        max_i = priv_max_i;
                    }
                }
            }
		}
	}

	if (matrix[start][start] < matrix[max_i][start]) {
		//przesuwam wiersz z maximum na górę
		if (matrix[start] != matrix[max_i]) {
			double *tmp = matrix[start];

			matrix[start] = matrix[max_i];
			matrix[max_i] = tmp;
		}
	}
}


double* solve_with_partial_choose_parallel(double **matrix, int rows, int columns) {
	int i, j, k;
	double *R = ALLOC_MEMORY(sizeof(double)*rows);
	char infinitely_many = 0, illegal = 0;

	if (R == NULL) return NULL;

#ifndef FALSE_SHARING
	__assume_aligned(R, ALIGNMENT_SIZE);
	__assume_aligned(matrix, ALIGNMENT_SIZE);
#endif

	//postępowanie proste
#pragma omp parallel for schedule(static) num_threads(1) default(none) private(i, j, k) shared(matrix, rows, columns)
	for (i = 0; i < rows; i++) {
		partial_choose(matrix, rows, columns, rows - i);

		if (matrix[i][i] != 0) {
#pragma omp parallel for schedule(static) default(none) private(j, k) shared(matrix, rows, columns, i)
			for (j = i + 1; j < rows; j++) {
				double c = matrix[j][i] / matrix[i][i];
#ifdef MANUAL_VECTORIZATION
#pragma ivdep
#endif
				for (k = i; k < columns; k++) {
					matrix[j][k] -= c*matrix[i][k];
				}
			}
		}
	}

	//sprawdzam czy istnieją rozwiązania
#pragma omp parallel for schedule(static) private(i)
#ifdef NO_DEFAULT_VECTORIZATION
#pragma novector
#endif
	for (i = rows - 1; i >= 0; i--)
		if (matrix[i][i] == 0 && matrix[i][columns - 1] != 0)
			illegal = 1; //brak rozwiązań
		else if (matrix[i][i] == 0 && matrix[i][columns - 1] == 0)
			infinitely_many = 1;

	if (illegal) return NULL;

	if (infinitely_many) {
		R[0] = INFINITY;
		return R;
	}

	//Postępowanie odwrtone
	for (i = rows - 1; i >= 0; i--) {
		double sum = 0;

		R[i] = matrix[i][columns - 1];
#pragma omp parallel for schedule(static) private(j) reduction(+:sum)
#ifdef NO_DEFAULT_VECTORIZATION
#pragma novector
#endif
		for (j = i + 1; j < columns - 1; j++) {
			sum += matrix[i][j] * R[j];
		}

		R[i] -= sum;
		assert(matrix[i][i]);
		R[i] /= matrix[i][i];
	}

	return R;
}