#ifndef PRINTING
#define PRINTING
#include <float.h>

#if defined(__INTEL_COMPILER)
#include <malloc.h>
#else
#include <mm_malloc.h>
#endif

#ifdef INFINITY
#undef INFINITY
#endif
#define INFINITY (DBL_MAX * DBL_MAX)

#define ALIGNMENT_SIZE 64
#define ROUND_UP_ALIGN(x) ((((x) + ALIGNMENT_SIZE - 1) / ALIGNMENT_SIZE) * ALIGNMENT_SIZE)

#ifdef FALSE_SHARING
#define ALLOC_MEMORY(n) malloc(n)
#define FREE_MEMORY(ptr) free(ptr)
#else
#define ALLOC_MEMORY(n) _mm_malloc(ROUND_UP_ALIGN(n), ALIGNMENT_SIZE)
#define FREE_MEMORY(ptr) _mm_free(ptr)
#endif

#endif