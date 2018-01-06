#ifndef SOLVE
#define SOLVE

void partial_choose(double **matrix, int rows, int column, int sub_matrix_size);

void full_choose(double **matrix, int rows, int columns, int sub_matrix_size, int *pv);

double* solve_naive(double **matrix, int rows, int columns);

double* solve_with_partial_choose(double **matrix, int rows, int columns);

double* solve_with_full_choose(double **matrix, int rows, int columns);

double* solve_with_full_choose_parallel(double **matrix, int rows, int columns);

#endif