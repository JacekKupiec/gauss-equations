#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <omp.h>
//#include <string.h>
#include "shared_consts.h"


double **LoadEquationMatrix(char *path, int *rows, int *columns) {
	double **matrix;
	int n, i, j;
	FILE *file;

	if ((file = fopen(path, "r")) == NULL) {
		printf("%s:%d Nie udalo sie otworzyc podanego pliku: %s\n", __FILE__, __LINE__, path);
		return NULL;
	}
	if (fscanf(file, "%d", &n) != 1) return NULL;

	*rows = n;
	*columns = n + 1;
	matrix = ALLOC_MEMORY(sizeof(double*)*n);

	for (i = 0; i < n; i++) {
		matrix[i] = (double*)ALLOC_MEMORY(sizeof(double)*(n + 1));

		for (j = 0; j <= n; j++)
			if (fscanf(file, "%lf", matrix[i] + j) != 1) return NULL;
	}

	fclose(file);

	return matrix;
}


double** DrawEquationMatrixParallel(int rows, int columns) {
	double **matrix = NULL;
	int i, j;

	matrix = ALLOC_MEMORY(sizeof(double*)*rows);
	if (matrix == NULL) return NULL;

	for (i = 0; i < rows; i++) {
		matrix[i] = ALLOC_MEMORY(sizeof(double)*columns);
		if (matrix[i] == NULL) return NULL;
	}
	
	srand(time(NULL));

#ifndef FALSE_SHARING
	__assume_aligned(matrix, ALIGNMENT_SIZE);
#endif

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

	matrix = ALLOC_MEMORY(sizeof(double*)*rows);
	if (matrix == NULL) return NULL;

	srand(time(NULL));

#ifndef FALSE_SHARING
	__assume_aligned(matrix, ALIGNMENT_SIZE);
#endif

	for (i = 0; i < rows; i++) {
		matrix[i] = ALLOC_MEMORY(sizeof(double)*columns);
		if (matrix[i] == NULL) return NULL;
		for (j = 0; j < columns; j++)
			matrix[i][j] = (double)rand() / RAND_MAX;
	}

	return matrix;
}