#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <sys/time.h>
#include "solve.h"
#include "data_read.h"
#include "shared_consts.h"

double dtime()
{
	double tseconds = 0.0;
	struct timeval mytime;

	gettimeofday(&mytime,(struct timezone*)0);
	tseconds = (double)(mytime.tv_sec +mytime.tv_usec*1.0e-6);

	return tseconds;
}


int main(int argc, char **argv) {
	double 	*solutions = NULL,
			**equations = NULL, 
			t_start,
			t_stop;
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

	if (argv[2][0] == 's') {
		t_start = dtime();
		solutions = solve_with_partial_choose(equations, rows, columns);
		t_stop = dtime();
	}
	else if (argv[2][0] == 'p') {
		t_start = dtime();
		solutions = solve_with_partial_choose_parallel(equations, rows, columns);
		t_stop = dtime();
	}

	if (solutions == NULL) puts("Brak rozwiązań układu równań");

	printf("%.8f\n", t_stop - t_start);

	if (argv[argc - 1][0] == 'v')
		for (i = 0; i < rows; i++)
			printf("%f\n", solutions[i]);

	return EXIT_SUCCESS;
}