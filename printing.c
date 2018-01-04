#include <stdio.h>

void PrintMatrix(double **matrix, int rows, int columns) {
	int i, j;

	for (i = 0; i < rows; i++) {
		for (j = 0; j < columns; j++)
			printf("%6.4f ", matrix[i][j]);
		putchar('\n');
	}

	puts("************************************************");
}

void PrintMatrixPV(double **matrix, int rows, int columns, int *pv) {
	int i, j;

	for(i = 0; i < rows; i++) {
		for(j=0; j < columns; j++)
			printf("%5.3f ",matrix[i][pv[j]]);
		putchar('\n');
	}
}

void Print(double *t, int size) {
	int i;

	for (i = 0; i < size; i++)
		printf("%d: %0.12f\n", i+ 1, t[i]);
	putchar('\n');
}