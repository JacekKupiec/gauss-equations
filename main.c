#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "solve.h"
#include "data_read.h"
#include "printing.h"


int main(int argc, char **argv) {
	double *solutions = NULL;
	double **equations = NULL;
	int rows, columns;

	if (argv[1][0] == 'r') 
		equations = LoadEquationMatrix(argv[2], &rows, &columns);
	else if (argv[1][0] == 'g')
		equations = LoadGraphMatrix(argv[2], &rows, &columns);

	if (equations == NULL) return EXIT_FAILURE;

	solutions = solve_with_full_choose(equations, rows, columns);

	for (int i = 0; i < rows; i++)
		printf("x%d = %f\n", i, solutions[i]);

	return EXIT_SUCCESS;
}