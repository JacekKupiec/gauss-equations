#ifndef DATA_READ
#define DATA_READ

double **LoadEquationMatrix(char *path, int *rows, int *columns);
double **LoadGraphMatrix(char *path, int *rows, int *columns);
double** DrawEquationMatrixParallel(int rows, int columns);
double** DrawEquationMatrix(int rows, int columns);

#endif