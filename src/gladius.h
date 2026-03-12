/* gladius.h - v0.0.0 - MIT - https://github.com/pithecantrope/gladius.h
 *
 * Gladius: C, Sharpened.
 *
 * About:
 *      Header-only C23 arena-owned containers.
 *
 * Usage:
 *      In exactly one .c file:
 *              #define GLADIUS_IMPLEMENTATION
 *              #include "gladius.h"
 *      Elsewhere:
 *              #include "gladius.h"
 *
 * Note:
 *      - No need to check return values. Panics on error by default.
 *      - Define NDEBUG to disable it and optimize for release builds.
 *      - Short aliases by default. Define GLADIUS_PREFIXED to disable.
*/

#ifndef GLADIUS_HEADER
#define GLADIUS_HEADER

#if defined(__cplusplus)
#error "Gladius does not support C++."
#elif (!defined(__STDC_VERSION__) || (__STDC_VERSION__ < 202000L))
#error "Gladius requires C23 or later."
#endif

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>

#ifndef GLD_API
#define GLD_API extern
#endif // GLD_API

#ifndef GLD_MALLOC
#define GLD_MALLOC(size) malloc(size)
#endif // GLD_MALLOC

#ifndef GLD_FREE
#define GLD_FREE(ptr) free(ptr)
#endif // GLD_FREE

#ifdef NDEBUG
#define GLD_PANIC(cond, msg)                                                                       \
        do {                                                                                       \
                if (cond)                                                                          \
                        unreachable();                                                             \
        } while (0)
#else
#define GLD_PANIC(cond, msg)                                                                       \
        do {                                                                                       \
                if (cond) {                                                                        \
                        fprintf(stderr,                                                            \
                                "GLD_PANIC: " msg "\n"                                             \
                                "File \"%s\", line %d, in %s\n"                                    \
                                "\t" #cond "\n",                                                   \
                                __FILE__, __LINE__, __func__);                                     \
                        exit(EXIT_FAILURE);                                                        \
                }                                                                                  \
        } while (0)
#endif // NDEBUG

// Arena -------------------------------------------------------------------------------------------

#ifdef GLADIUS_IMPLEMENTATION
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
