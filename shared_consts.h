#ifndef PRINTING
#define PRINTING
#include <float.h>

#ifdef INFINITY
#undef INFINITY
#define INFINITY (DBL_MAX * DBL_MAX)
#else
#define INFINITY (DBL_MAX * DBL_MAX)
#endif

#define ALIGNMENT_SIZE 64
#define ROUND_UP_ALIGN(x) ((((x) + ALIGNMENT_SIZE - 1) / ALIGNMENT_SIZE) * ALIGNMENT_SIZE)

#endif