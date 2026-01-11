#ifndef GLADIUS_HEADER
#define GLADIUS_HEADER

#if defined(__cplusplus)
#error "Gladius does not support C++"
#elif (!defined(__STDC_VERSION__) || (__STDC_VERSION__ < 202311L))
#error "Gladius requires C23"
#endif

#include <stddef.h>

#ifndef GLADIUS_DEF
#define GLADIUS_DEF
#endif // GLADIUS_DEF

#ifndef GLADIUS_MALLOC
#include <stdlib.h>
#define GLADIUS_MALLOC malloc
#endif // GLADIUS_MALLOC

#ifndef GLADIUS_FREE
#include <stdlib.h>
#define GLADIUS_FREE free
#endif // GLADIUS_FREE

#ifndef GLADIUS_ASSERT
#include <assert.h>
#define GLADIUS_ASSERT assert
#endif // GLADIUS_ASSERT

// Arena declaration -------------------------------------------------------------------------------
typedef struct {
        char* buf;
        size_t len;
        size_t cap;
} gladius_arena;

#ifndef GLADIUS_PREFIXED
#define arena gladius_arena
#endif // GLADIUS_PREFIXED

#ifdef GLADIUS_IMPLEMENTATION
// Arena definition --------------------------------------------------------------------------------
#endif // GLADIUS_IMPLEMENTATION

#endif // GLADIUS_HEADER
