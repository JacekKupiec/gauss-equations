#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <omp.h>
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

	matrix = (double**)_mm_malloc(sizeof(double*)*n, ALIGNMENT_SIZE);

	for (i = 0; i < n; i++) {
		matrix[i] = (double*)_mm_malloc(sizeof(double)*(n + 1), ALIGNMENT_SIZE);

		for (j = 0; j <= n; j++)
			assert(fscanf(file, "%lf", matrix[i] + j) == 1);
	}

	fclose(file);

	return matrix;
}

//wczytuj pliki z których już usunięto 4 pierwsze linie z komentarzami
//dodatkowo tworzy odpowienią macierz dla Page Rank
//pliki przygotowane tak ,że numeracja od 0
double **LoadGraphMatrix(char *path, int *rows, int *columns) {
	FILE *file = fopen(path, "r");
	double **matrix;
	const double d = 0.85;
	int max = 0, a, b, i, *L, j;

	while (fscanf(file, "%d %d", &a, &b) == 2) {
		if (a > max)
			max = a;
		if (b > max)
			max = b;
	}

	matrix = (double**)_mm_malloc(sizeof(double*)*(max + 1), ALIGNMENT_SIZE);
	L = (int*)calloc((max + 1), sizeof(int));

	*rows = max + 1;
	*columns = max + 2;

	for (i = 0; i < *rows; i++)
		matrix[i] = (double*)calloc(*columns, sizeof(double));

	rewind(file);

	//zakładam ,że graf jest skierowany, żeby zrobic nieskierowany wystarczy następna krawędź
	while (fscanf(file, "%d %d", &a, &b) == 2) {
		matrix[b][a] = 1.0;
		L[a]++;
	}


	//Sprawdzam ,które strony nie linkują do niczego i zmieniam ich macierz sąsiedztwa na taką ,która mówi ,że linkuje do wszystkich elementów
	for (i = 0; i < *rows; i++) {
		if (!L[i]) {
			for (j = 0; j < *rows; j++)
				matrix[j][i] = i != j ? 1 : 0;

			L[i] = *rows - 1;
		}

		matrix[i][*columns - 1] = (1 - d) / *rows;
	}

	//Teraz tworzę macierz (I - d*B*A)
	for (i = 0; i < *rows; i++)
	for (j = 0; j < *rows; j++) {
		if (i == j)
			matrix[i][i] = 1;
		else if (matrix[i][j] > 0)
			//Jeżeli jest sąsiedztwo to przypisze się wartość ,a jak nie to pozostania 0 jak było
			matrix[i][j] = -1.0 * d / (double)L[j];
	}


	fclose(file);
	_mm_free(L);

	return matrix;
}


double** DrawEquationMatrixParallel(int rows, int columns) {
	double **matrix = NULL;
	int i, j;

	matrix = _mm_malloc(sizeof(double*)*rows, ALIGNMENT_SIZE);
	if (matrix == NULL) return NULL;

	for (i = 0; i < rows; i++) {
		matrix[i] = _mm_malloc(sizeof(double)*columns, ALIGNMENT_SIZE);
		if (matrix[i] == NULL) return NULL;
	}
	
	srand(time(NULL));

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

	matrix = _mm_malloc(sizeof(double*)*rows, ALIGNMENT_SIZE);
	if (matrix == NULL) return NULL;
	
	srand(time(NULL));

	for (i = 0; i < rows; i++) {
		matrix[i] = _mm_malloc(sizeof(double)*columns, ALIGNMENT_SIZE);
		if (matrix[i] == NULL) return NULL;
		for (j = 0; j < columns; j++)
			matrix[i][j] = (double)rand() / RAND_MAX;
	}

	return matrix;
}