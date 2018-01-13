#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <omp.h>
#if defined(__INTEL_COMPILER)
#include <malloc.h>
#else
#include <mm_malloc.h>
#endif
#include "shared_consts.h"

void partial_choose(double **matrix, int rows, int column, int sub_matrix_size) {
	int i, max_i, start = rows - sub_matrix_size;
	double max_v;

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
	double *R = (double*)malloc(sizeof(double)*rows);
	char nieskonczenie_wiele = 0;

	if (R == NULL)
		return NULL;

	//postępowanie proste
	for (i = 0; i < rows; i++) {
		partial_choose(matrix, rows, columns, rows - i);

		if (matrix[i][i] != 0) {
			for (j = i + 1; j < rows; j++) {
				double c = matrix[j][i] / matrix[i][i];

				for (k = i; k < columns; k++) {
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
		R = (double*)realloc(R, sizeof(double)); //bo po co mi więcej ? :)
		assert(R != NULL);
		R[0] = INFINITY;
		return R;
	}

	//Postępowanie odwrtone
	for (i = rows - 1; i >= 0; i--) {
		//columns - 1 poniewaz nie chcę odjemować od ... = "liczba" tejże "liczby" ,która jest w matrix[i][columns - 1]
		for (R[i] = matrix[i][columns - 1], j = i + 1; j < columns - 1; j++)
			R[i] -= matrix[i][j] * R[j];

		assert(matrix[i][i] != 0);
		R[i] /= matrix[i][i];
	}

	return R;
}


//Wybiera największy element z całej podmacierzy i przestawia wiersz i kolumnę
void full_choose(double **matrix, int rows, int columns, int sub_matrix_size, int *pv) {
	int i, j, max_i, max_j, start = rows - sub_matrix_size;
	double max_v;

	if (start >= rows) return;

	__assume_aligned(matrix, ALIGNMENT_SIZE);
	__assume_aligned(pv, ALIGNMENT_SIZE);

	for (i = start, max_v = matrix[start][pv[start]], max_i = start, max_j = start; i < rows; i++) {
		for (j = start; j < columns - 1; j++) { //wyraz wolny nie jest brany pod uwagę
			if (max_v < matrix[i][pv[j]]) {
				max_i = i;
				max_j = j;
				max_v = matrix[i][pv[j]];
			}
		}
	}

	if (matrix[start][pv[start]] < matrix[max_i][pv[max_j]]) {
		//przesuwam wiersz z maximum na górę
		if (matrix[start] != matrix[max_i]) {
			double *tmp = matrix[start];

			matrix[start] = matrix[max_i];
			matrix[max_i] = tmp;
		}

		//przesuwam kolumnę zawierającą maksimum na początek
		if (start != max_j) {
			int tmp = pv[start];

			pv[start] = pv[max_j];
			pv[max_j] = tmp;
		}
	}
}


void full_choose_parallel(double **matrix, int rows, int columns, int sub_matrix_size, int *pv) {
	int i, j, start = rows - sub_matrix_size,
        max_i = start, 
        max_j = start, 
        priv_max_j = start, 
        priv_max_i = start;
	double max_v = matrix[start][pv[start]], priv_max_v = matrix[start][pv[start]];

	if (start >= rows) return;
	__assume_aligned(matrix, ALIGNMENT_SIZE);
	__assume_aligned(pv, ALIGNMENT_SIZE);

#pragma omp parallel default(none) shared(matrix, rows, columns, sub_matrix_size, pv, max_v, max_i, max_j, start) firstprivate(priv_max_i,priv_max_j,priv_max_v) private(i, j)
	{
#pragma omp for schedule(static) collapse(2)
		for (i = start; i < rows; i++) {
			for (j = start; j < columns - 1; j++) { //wyraz wolny nie jest brany pod uwagę
				if (priv_max_v < matrix[i][pv[j]]) {
					priv_max_i = i;
					priv_max_j = j;
					priv_max_v = matrix[i][pv[j]];
				}
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
                        max_j = priv_max_j;
                    }
                }
            }
		}
	}

	if (matrix[start][pv[start]] < matrix[max_i][pv[max_j]]) {
		//przesuwam wiersz z maximum na górę
		if (matrix[start] != matrix[max_i]) {
			double *tmp = matrix[start];

			matrix[start] = matrix[max_i];
			matrix[max_i] = tmp;
		}

		//przesuwam kolumnę zawierającą maksimum na początek
		if (start != max_j) {
			int tmp = pv[start];

			pv[start] = pv[max_j];
			pv[max_j] = tmp;
		}
	}
}


double* solve_with_full_choose(double **matrix, int rows, int columns) {
	int i, j, k, *pv = (int*)_mm_malloc(sizeof(int) * columns, ALIGNMENT_SIZE); //pv to permutation_vector
	double c, *R = (double*)_mm_malloc(sizeof(double) * rows, ALIGNMENT_SIZE);
	char infinitely_many = 0;

	if (R == NULL || pv == NULL)
		return NULL;

	__assume_aligned(pv, ALIGNMENT_SIZE);
	__assume_aligned(R, ALIGNMENT_SIZE);
	__assume_aligned(matrix, ALIGNMENT_SIZE);

	//inicjalizacja wektora permutacji
	for(i = 0; i < columns; i++)
		pv[i] = i;

	//postępowanie proste
	for (i = 0; i < rows; i++) {
			full_choose(matrix, rows, columns, rows - i, pv);

			if (matrix[i][pv[i]] != 0)
			for (j = i + 1; j < rows; j++) {
				for (k = i, c = matrix[j][pv[i]] / matrix[i][pv[i]]; k < columns; k++) {
					matrix[j][pv[k]] -= c*matrix[i][pv[k]];
				}
			}
	}

	//sprawdzam czy istnieja rozwiązania
	for (i = rows - 1; i >= 0; i--)
		if (matrix[i][pv[i]] == 0 && matrix[i][pv[columns - 1]] != 0)
			return NULL; //brak rozwiązań
		else if (matrix[i][pv[i]] == 0 && matrix[i][pv[columns - 1]] == 0)
			infinitely_many = 1;

	if (infinitely_many) {
		R[0] = INFINITY;
		return R;
	}

	//Post�powanie odwrtone
	for (i = rows - 1; i >= 0; i--) {
		//columns - 1 poniewaz nie chc� odjemowa� od ... = "liczba" tej�e "liczby" ,kt�ra jest w matrix[i][columns - 1]
		for (R[i] = matrix[i][pv[columns - 1]], j = i + 1; j < columns - 1; j++)
			R[i] -= matrix[i][pv[j]] * R[j];

		assert(matrix[i][pv[i]]);
		R[i] /= matrix[i][pv[i]];
	}

	_mm_free(pv);

	return R;
}


double* solve_with_full_choose_parallel(double **matrix, int rows, int columns) {
	int *pv = (int*)_mm_malloc(sizeof(int) * columns, ALIGNMENT_SIZE); //pv to permutation_vector
	int i, j, k;
	double *R = (double*)_mm_malloc(sizeof(double) * rows, ALIGNMENT_SIZE);
	char infinitely_many = 0, illegal = 0;

	assert(R != NULL);
	assert(pv != NULL);

	__assume_aligned(pv, ALIGNMENT_SIZE);
	__assume_aligned(R, ALIGNMENT_SIZE);
	__assume_aligned(matrix, ALIGNMENT_SIZE);
	
	//inicjalizacja wektora permutacji
#pragma omp parallel for schedule(static) private(i)
	for(i = 0; i < columns; i++)
		pv[i] = i;

#pragma omp parallel for default(none) schedule(static) num_threads(1) private(i, j, k) shared(matrix, rows, columns, pv)
	for (i = 0; i < rows; i++) {
			full_choose_parallel(matrix, rows, columns, rows - i, pv);	

			if (matrix[i][pv[i]] != 0) {
#pragma omp parallel for default(none) schedule(static) private(j, k) shared(matrix, rows, columns, pv, i)
				for (j = i + 1; j < rows; j++) {
					double c = matrix[j][pv[i]] / matrix[i][pv[i]];

					for (k = i; k < columns; k++) {
						matrix[j][pv[k]] -= c*matrix[i][pv[k]];
					}
				}
			}
	}

#pragma omp parallel for schedule(static) private(i)
	for (i = rows - 1; i >= 0; i--)
		if (matrix[i][pv[i]] == 0 && matrix[i][pv[columns - 1]] != 0)
			illegal = 1; //brak rozwiązań
		else if (matrix[i][pv[i]] == 0 && matrix[i][pv[columns - 1]] == 0)
			infinitely_many = 1;

	if (illegal) return NULL;

	if (infinitely_many) {
		R[0] = INFINITY;

		return R;
	}

	for (i = rows - 1; i >= 0; i--) {
		double sum = 0;

		R[i] = matrix[i][pv[columns - 1]];
		//columns - 1 poniewaz nie chcę odjemować od ... = "liczba" tejże "liczby" ,która jest w matrix[i][columns - 1]
#pragma omp parallel for schedule(static) private(j) reduction(+:sum)
		for (j = i + 1; j < columns - 1; j++) {
			sum += matrix[i][pv[j]] * R[j];
		}

		R[i] -= sum;
		assert(matrix[i][pv[i]]);
		R[i] /= matrix[i][pv[i]];
	}

	_mm_free(pv);

	return R;
}