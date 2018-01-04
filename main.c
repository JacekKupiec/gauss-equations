#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "solve.h"
#include "data_read.h"
#include "printing.h"


int main(int argc, char **argv) {
	double *solutions = NULL;
	double **equations = NULL;
	int i, j, rows, columns;
	double sum;
	int pv[16] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };

	if (argv[1][0] == 'r') 
		equations = LoadEquationMatrix(argv[3], &rows, &columns);
	else if (argv[1][0] == 'g')
		equations = LoadGraphMatrix(argv[3], &rows, &columns);

	if (equations == NULL) return EXIT_FAILURE;

	full_choose(equations, rows, columns, 15, pv);

	return EXIT_SUCCESS;
}