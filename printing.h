#ifndef PRINTING
#define PRINTING
#include <float.h>

#ifdef INFINITY
#undef INFINITY
#define INFINITY (DBL_MAX * DBL_MAX)
#else
#define INFINITY (DBL_MAX * DBL_MAX)
#endif

#endif