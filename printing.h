#ifndef PRINTING
#define PRINTING
#include <float.h>

void PrintMatrix(double **matrix, int rows, int columns);
void PrintMatrixPV(double **matrix, int rows, int columns, int *pv);
void Print(double *t, int size);

#ifdef INFINITY
#undef INFINITY
#define INFINITY (DBL_MAX * DBL_MAX)
#else
#define INFINITY (DBL_MAX * DBL_MAX)
#endif

#endif