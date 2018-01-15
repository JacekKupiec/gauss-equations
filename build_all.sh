icc main.c solve.c data_read.c -O2 -mmic -openmp -no-vec -D FALSE_SHARING -o basic
icc main.c solve.c data_read.c -O2 -mmic -openmp -D MANUAL_VECTORIZATION -o optimal
icc main.c solve.c data_read.c -O2 -mmic -openmp -D MANUAL_VECTORIZATION -o sequential
icc main.c solve.c data_read.c -O2 -mmic -openmp -D FALSE_SHARING -o basic_def_vec #Basic + def_vec
icc main.c solve.c data_read.c -O2 -mmic -openmp -D NO_DEFAULT_VECTORIZATION -D MANUAL_VECTORIZATION -o optimal_def_vec #optimal - def_vec
icc main.c solve.c data_read.c -O2 -mmic -openmp -D NO_DEFAULT_VECTORIZATION -D MANUAL_VECTORIZATION -D FALSE_SHARING -o basic_man_vec #basic + man_vec
icc main.c solve.c data_read.c -O2 -mmic -openmp -o optimal_man_vec #optimal - man_vec
icc main.c solve.c data_read.c -O2 -mmic -openmp -no-vec -o basic_fsp # basic + fsp
icc main.c solve.c data_read.c -O2 -mmic -openmp -D FALSE_SHARING -D MANUAL_VECTORIZATION -o optimal_fsp #optimal - fsp
