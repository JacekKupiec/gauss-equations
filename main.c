#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <assert.h>
#include <omp.h>
#include "solve.h"
#include "data_read.h"
#include "printing.h"


int main(int argc, char **argv) {
	double *solutions = NULL;
	double **equations = NULL;
	int i, rows, columns;
	clock_t t;

	if (argv[1][0] == 'h') {
		puts("HELP: program f|g|r s|p path");
		return EXIT_SUCCESS;
	}

	omp_set_dynamic(1);
	omp_set_nested(1);
	omp_set_num_threads(3);

	if (argv[1][0] == 'f') 
		equations = LoadEquationMatrix(argv[3], &rows, &columns);
	else if (argv[1][0] == 'g')
		equations = LoadGraphMatrix(argv[3], &rows, &columns);
	else if (argv[1][0] == 'r') {
		sscanf(argv[3], "%d", &rows);
		sscanf(argv[4], "%d", &columns);
		equations = DrawEquationMatrix(rows, columns);

		return 0;
	}

	if (equations == NULL) return EXIT_FAILURE;

	if (argv[2][0] == 's') {
		t = clock();
		solutions = solve_with_full_choose(equations, rows, columns);
		t = clock() - t;
	}
	else if (argv[2][0] == 'p') {
		printf("OMP thread num: %d\nOMP Dynamic: %d\nOMP nested: %d\n", 
			omp_get_num_threads(), 
			omp_get_dynamic(),
			omp_get_nested());
		t = clock();
		solutions = solve_with_full_choose_parallel(equations, rows, columns);
		t = clock() - t;
	}

	assert(solutions != NULL);
	printf("Czas przetwarzania: %lf\n", ((double)t) / CLOCKS_PER_SEC);

	if (argv[argc - 1][0] == 'v')
		for (i = 0; i < rows; i++)
			printf("%f\n", solutions[i]);

	return EXIT_SUCCESS;
}