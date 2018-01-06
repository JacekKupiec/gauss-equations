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
	clock_t t;

	if (argv[1][0] == 'h') {
		puts("HELP: program r|g s|p path");
		return EXIT_SUCCESS;
	}

	

	if (argv[1][0] == 'r') 
		equations = LoadEquationMatrix(argv[4], &rows, &columns);
	else if (argv[1][0] == 'g')
		equations = LoadGraphMatrix(argv[4], &rows, &columns);

	if (equations == NULL) return EXIT_FAILURE;

	if (argv[2][0] == 's') {
		t = clock();
		solutions = solve_with_full_choose(equations, rows, columns);
		t = clock() - t;
	}
	else if (argv[2][0] == 'p') {
		t = clock();
		solutions = solve_with_full_choose_parallel(equations, rows, columns);
		t = clock() - t;
	}

	assert(solutions != NULL);
	printf("Czas przetwarzania: %lf\n", ((double)t) / CLOCKS_PER_SEC);

	if (argv[3][0] == 'v')
		for (int i = 0; i < rows; i++)
			printf("%f\n", solutions[i]);

	return EXIT_SUCCESS;
}