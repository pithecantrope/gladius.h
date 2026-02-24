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
 *      - Tests are embedded executable docs. Use them for reference.
 *      - Short aliases by default. Define GLADIUS_PREFIXED to disable.
 *      - Assertions for error handling. No return values need checking.
*/

#ifndef GLADIUS_HEADER
#define GLADIUS_HEADER

#if defined(__cplusplus)
#error "Gladius does not support C++."
#elif (!defined(__STDC_VERSION__) || (__STDC_VERSION__ < 202000L))
#error "Gladius requires C23 or later."
#endif

#ifndef GLD_API
#define GLD_API extern
#endif // GLD_API

#ifndef GLD_ASSERT
#include <assert.h>
#define GLD_ASSERT(condition, message) assert((condition) && (message))
#endif // GLD_ASSERT

#ifndef GLD_MALLOC
#define GLD_MALLOC(size) malloc(size)
#endif // GLD_MALLOC

#ifndef GLD_FREE
#define GLD_FREE(ptr) free(ptr)
#endif // GLD_FREE

#include <stdio.h>
#include <stdlib.h>

#ifdef GLADIUS_TEST
#define GLD_CHECK(condition)                                                                       \
        do {                                                                                       \
                if (!(condition)) {                                                                \
                        fprintf(stderr,                                                            \
                                "\033[31m"                                                         \
                                "Failure: %s:%d in %s:\n"                                          \
                                "\033[34m"                                                         \
                                "\t%s\n"                                                           \
                                "\033[0m",                                                         \
                                __FILE__, __LINE__, __func__, #condition);                         \
                        exit(EXIT_FAILURE);                                                        \
                }                                                                                  \
        } while (0)
#endif // GLADIUS_TEST

// Arena Declaration -------------------------------------------------------------------------------

//

#ifdef GLADIUS_TEST
#endif // GLADIUS_TEST

#ifndef GLADIUS_PREFIXED
#endif // GLADIUS_PREFIXED

#ifdef GLADIUS_TEST
#endif // GLADIUS_TEST

#ifdef GLADIUS_IMPLEMENTATION
// Arena Definition --------------------------------------------------------------------------------

//

#endif // GLADIUS_IMPLEMENTATION

#ifdef GLADIUS_TEST
static void
GLADIUS_TEST_ALL(void) {
        GLD_CHECK(false && "Hello, World!");
        puts("\033[32m"
             "Success!"
             "\033[0m");
}
#endif // GLADIUS_TEST

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
