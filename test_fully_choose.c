#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <omp.h>

void max_cell(double **matrix, int start, int rows, int columns) {
	double max_v = matrix[start][start];
	int max_i = start, max_j = start;

	for (int i = start; i < rows; i++) {
		for (int j = start; j < columns - 1; j++) { //wyraz wolny nie jest brany pod uwagę
			if (max_v < matrix[i][j]) {
				max_i = i;
				max_j = j;
				max_v = matrix[i][j];
			}
		}
	}

	//printf("Sekwencyjnie:\nmax_v: %f\nmax_i: %d\nmax_j: %d\n", matrix[max_i][max_j], max_i, max_j);
}

void max_cell_reduction(double **matrix, int start, int rows, int columns) {
    double max_v = matrix[start][start];
	int max_i = start, max_j = start;

#pragma omp parallel for schedule(guided) collapse(2) reduction(max: max_v)
	for (int i = start; i < rows; i++) {
		for (int j = start; j < columns - 1; j++) { //wyraz wolny nie jest brany pod uwagę
			if (max_v < matrix[i][j]) {
				//max_i = i;
				//max_j = j;
				max_v = matrix[i][j];
			}
		}
	}

	//printf("Redukcja:\nmax_v: %f\nmax_i: %d\nmax_j: %d\n", max_v, max_i, max_j);
}


void full_choose(double **matrix, int rows, int columns, int sub_matrix_size, int *pv) {
	int start = rows - sub_matrix_size,
        max_i = start, 
        max_j = start, 
        priv_max_j = start, 
        priv_max_i = start;
	double max_v = matrix[start][start], priv_max_v = matrix[start][start];

	if (start >= rows) return;

#pragma omp parallel default(none) shared(matrix, rows, columns, sub_matrix_size, pv, max_v, max_i, max_j, start) firstprivate(priv_max_i,priv_max_j,priv_max_v)
	{
#pragma omp for schedule(static) collapse(2)
		for (int i = start; i < rows; i++) {
			for (int j = start; j < columns - 1; j++) { //wyraz wolny nie jest brany pod uwagę
				if (priv_max_v < matrix[i][j]) {
					priv_max_i = i;
					priv_max_j = j;
					priv_max_v = matrix[i][j];
				}
			}
		}

#pragma omp flush (max_v) 
		{
            if (priv_max_v > max_v) {
#pragma omp critical
                {
                    if (priv_max_v > max_v) {
                        //printf("%f > %f\n", priv_max_v, max_v);
                        max_v = priv_max_v;
                        max_i = priv_max_i;
                        max_j = priv_max_j;
                    }
                }
            }
		}
	}

    //printf("Równolegle:\nmax_v: %f\nmax_i: %d\nmax_j: %d\n", matrix[max_i][max_j], max_i, max_j);

	if (matrix[start][start] < matrix[max_i][max_j]) {
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


int main(int argc, char **argv) {
	double **equations = NULL;
	int i, j, rows = 10000, columns = 10001;
	int *pv = malloc(sizeof(int)*columns);
	clock_t t;

    srand(time(NULL));

	equations = (double**)malloc(sizeof(equations)*rows);
	for (i = 0; i < rows; i++)
        equations[i] = (double*)malloc(sizeof(double)*columns);

    for (i = 0; i < rows; i++)
        for (j = 0; j < columns; j++)
            equations[i][j] = (double)rand() / RAND_MAX;

printf("%d\n",omp_get_num_threads());
	t = clock();
	full_choose(equations, rows, columns, rows, pv);
	t = clock() - t;
	printf("Czas przetworzenia z OpenMP: %f\n", ((double)t) / CLOCKS_PER_SEC);

    t = clock();
	max_cell_reduction(equations, 0, rows, columns);
	t = clock() - t;
	printf("Czas przetworzenia Redukcja: %f\n", ((double)t) / CLOCKS_PER_SEC);

	t = clock();
	max_cell(equations, 0, rows, columns);
	t = clock() - t;
	printf("Czas przetworzenia SEKWENCYJNIE: %f\n", ((double)t) / CLOCKS_PER_SEC);
    
	for (i = 0; i < rows; i++)
        free(equations[i]);
    free(equations);
    free(pv);

	return EXIT_SUCCESS;
}