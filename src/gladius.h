/* Gladius - TODO: (v0.0.0) - MIT License - https://github.com/pithecantrope/gladius.h
 *
 * Header only library for arena-based containers in C23:
 *      -> TODO: string
 *      -> TODO: vector
 *      -> TODO: hashmap
 *
 * Usage:
 *      In exactly ONE source file, define GLADIUS_IMPLEMENTATION before including:
 *              #define GLADIUS_IMPLEMENTATION
 *              #include "gladius.h"
 *      In all other files, just include normally:
 *              #include "gladius.h"
 *
 * Customization (define before including):
 *      -> GLADIUS_PREFIXED
 *              Disables short name aliases (e.g., `arena` <=> `gladius_arena`).
 *      -> GLADIUS_DEF (default: extern)
 *              Controls linkage of implementation functions.
 *      -> GLADIUS_MALLOC(size)
 *              Override `malloc` from <stdlib.h>.
 *      -> GLADIUS_FREE(ptr)
 *              Override `free` from <stdlib.h>.
 *      -> GLADIUS_ASSERT(condition)
 *              Override `assert` from <assert.h>.
*/

#ifndef GLADIUS_HEADER
#define GLADIUS_HEADER

#if defined(__cplusplus)
#error "Gladius does not support C++"
#elif (!defined(__STDC_VERSION__) || (__STDC_VERSION__ < 202311L))
#error "Gladius requires C23"
#endif

#include <stddef.h>

#ifndef GLADIUS_DEF
#define GLADIUS_DEF extern
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

/*
 MIT License

 Copyright (c) 2026 Egor Afanasin

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */
