#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <assert.h>
#include <omp.h>
#include "solve.h"
#include "data_read.h"
#include "shared_consts.h"


int main(int argc, char **argv) {
	double *solutions = NULL;
	double **equations = NULL;
	int i, rows, columns;

	if (argv[1][0] == 'h') {
		puts("HELP: program f|g|r s|p path");
		return EXIT_SUCCESS;
	}

	omp_set_dynamic(1);
	omp_set_nested(1);

	if (argv[1][0] == 'f') 
		equations = LoadEquationMatrix(argv[3], &rows, &columns);
	else if (argv[1][0] == 'r') {
		sscanf(argv[3], "%d", &rows);
		sscanf(argv[4], "%d", &columns);
		equations = DrawEquationMatrix(rows, columns);
	}

	if (equations == NULL) return EXIT_FAILURE;

	if (argv[2][0] == 's')
		solutions = solve_with_full_choose(equations, rows, columns);
	else if (argv[2][0] == 'p') 
		solutions = solve_with_full_choose_parallel(equations, rows, columns);

	if (solutions == NULL) puts("Brak rozwiązań układu równań");

	if (argv[argc - 1][0] == 'v')
		for (i = 0; i < rows; i++)
			printf("%f\n", solutions[i]);

	return EXIT_SUCCESS;
}