#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "printing.h"

double **LoadEquationMatrix(char *path, int *rows, int *columns) {
	double **matrix;
	int n, i, j;
	FILE *file;

	if ((file = fopen(path, "r")) == NULL) {
		printf("Nie udalo sie otworzyc podanego pliku: %s\n", path);
		return NULL;
	}

	assert(fscanf(file, "%d", &n) == 1);

	*rows = n;
	*columns = n + 1;

	matrix = (double**)malloc(sizeof(double*)*n);

	for (i = 0; i < n; i++) {
		matrix[i] = (double*)malloc(sizeof(double)*(n + 1));

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

#ifdef _DEBUG
	printf("Maks = %d\n", max + 1);
#endif

	matrix = (double**)malloc(sizeof(double*)*(max + 1));
	L = (int*)calloc((max + 1), sizeof(int));

	*rows = max + 1;
	*columns = max + 2;

	for (i = 0; i < *rows; i++)
		matrix[i] = (double*)calloc(*columns, sizeof(double));

	rewind(file);

	//zak�adam ,�e graf jest skierowany, �eby zrobic nieskierowany wystarczy nast�pna kraw�d�
	while (fscanf(file, "%d %d", &a, &b) == 2) {
		matrix[b][a] = 1.0;
		L[a]++;
	}


	//Sprawdzam ,kt�re strony nie linkuj� do niczego i zmieniam ich macierz s�siedztwa na tak� ,kt�ra m�wi ,�e linkuje do wszystkich element�w
	for (i = 0; i < *rows; i++) {
		if (!L[i]) {
			for (j = 0; j < *rows; j++)
				matrix[j][i] = i != j ? 1 : 0;

			L[i] = *rows - 1;
		}

		matrix[i][*columns - 1] = (1 - d) / *rows;
	}

#ifdef _DEBUG
	if (max <= 20) {
		puts("Macierz sasiedztwa");
		PrintMatrix(matrix, *rows, *rows);
	}
#endif

	//Teraz tworz� macierz (I - d*B*A)
	for (i = 0; i < *rows; i++)
	for (j = 0; j < *rows; j++) {
		if (i == j)
			matrix[i][i] = 1;
		else if (matrix[i][j] > 0)
			//Je�eli jest s�siedztwo to przypisze si� warto�� ,a jak nie to pozostania 0 jak by�o
			matrix[i][j] = -1.0 * d / (double)L[j];
	}

#ifdef _DEBUG
	if (max <= 20) {
		puts("Po wszystkich przekształceniach:");
		PrintMatrix(matrix, *rows, *columns);
	}
#endif

	fclose(file);
	free(L);

	return matrix;
}
