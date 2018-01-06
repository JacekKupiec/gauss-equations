#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <assert.h>
#include "solve.h"
#include "data_read.h"
#include "printing.h"


int main(int argc, char **argv) {
	double *solutions = NULL;
	double **equations = NULL;
	int rows, columns;

	if (argv[1][0] == 'h') {
		puts("HELP: program r|g s|p path");
		return EXIT_SUCCESS;
	}

	if (argv[1][0] == 'r') 
		equations = LoadEquationMatrix(argv[3], &rows, &columns);
	else if (argv[1][0] == 'g')
		equations = LoadGraphMatrix(argv[3], &rows, &columns);

	if (equations == NULL) return EXIT_FAILURE;

	if (argv[2][0] == 's')
		solutions = solve_with_full_choose(equations, rows, columns);
	else if (argv[2][0] == 'p')
		solutions = solve_with_full_choose_parallel(equations, rows, columns);

	assert(solutions != NULL);

	for (int i = 0; i < rows; i++)
		printf("%f\n", solutions[i]);

	return EXIT_SUCCESS;
}