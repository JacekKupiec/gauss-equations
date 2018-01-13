#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <omp.h>
#if defined(__INTEL_COMPILER)
#include <malloc.h>
#else
#include <mm_malloc.h>
#endif
#include "shared_consts.h"


double **LoadEquationMatrix(char *path, int *rows, int *columns) {
	double **matrix;
	int n, i, j;
	FILE *file;

	if ((file = fopen(path, "r")) == NULL) {
		printf("%s:%d Nie udalo sie otworzyc podanego pliku: %s\n", __FILE__, __LINE__, path);
		return NULL;
	}

	assert(fscanf(file, "%d", &n) == 1);

	*rows = n;
	*columns = n + 1;

	matrix = (double**)_mm_malloc(ROUND_UP_ALIGN(sizeof(double*)*n), ALIGNMENT_SIZE);

	for (i = 0; i < n; i++) {
		matrix[i] = (double*)_mm_malloc(ROUND_UP_ALIGN(sizeof(double)*(n + 1)), ALIGNMENT_SIZE);

		for (j = 0; j <= n; j++)
			assert(fscanf(file, "%lf", matrix[i] + j) == 1);
	}

	fclose(file);

	return matrix;
}


double** DrawEquationMatrixParallel(int rows, int columns) {
	double **matrix = NULL;
	int i, j;

	matrix = _mm_malloc(ROUND_UP_ALIGN(sizeof(double*)*rows), ALIGNMENT_SIZE);
	if (matrix == NULL) return NULL;

	for (i = 0; i < rows; i++) {
		matrix[i] = _mm_malloc(ROUND_UP_ALIGN(sizeof(double)*columns), ALIGNMENT_SIZE);
		if (matrix[i] == NULL) return NULL;
	}
	
	srand(time(NULL));
	__assume_aligned(matrix, ALIGNMENT_SIZE);
#pragma omp parallel for default(none) schedule(static) private(i, j) shared(matrix, rows, columns) collapse(2)
	for (i = 0; i < rows; i++) {
		for (j = 0; j < columns; j++)
			matrix[i][j] = (double)rand() / RAND_MAX;
	}

	return matrix;
}


double** DrawEquationMatrix(int rows, int columns) {
	double **matrix = NULL;
	int i, j;

	matrix = _mm_malloc(ROUND_UP_ALIGN(sizeof(double*)*rows), ALIGNMENT_SIZE);
	if (matrix == NULL) return NULL;
	__assume_aligned(matrix, ALIGNMENT_SIZE);
	srand(time(NULL));

	for (i = 0; i < rows; i++) {
		matrix[i] = _mm_malloc(ROUND_UP_ALIGN(sizeof(double)*columns), ALIGNMENT_SIZE);
		if (matrix[i] == NULL) return NULL;
		for (j = 0; j < columns; j++)
			matrix[i][j] = (double)rand() / RAND_MAX;
	}

	return matrix;
}