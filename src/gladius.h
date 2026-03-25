/* gladius.h - v0.0 - MIT - https://github.com/pithecantrope/gladius.h
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
*/

#ifndef GLADIUS_HEADER
#define GLADIUS_HEADER
#define GLADIUS_VERSION_MAJOR 0
#define GLADIUS_VERSION_MINOR 0
#define GLADIUS_VERSION       "0.0"

#if !(defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 202000L))
#error "Gladius requires C23 or later"
#endif

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef NDEBUG
#define check(expression, message, ...)                                                            \
        do {                                                                                       \
                if (!(expression)) {                                                               \
                        unreachable();                                                             \
                }                                                                                  \
        } while (0)
#else
#define check(expression, message, ...)                                                            \
        do {                                                                                       \
                if (!(expression)) {                                                               \
                        gld__check_fail(#expression, __FILE__, __LINE__, __func__,                 \
                                        message __VA_OPT__(, ) __VA_ARGS__);                       \
                }                                                                                  \
        } while (0)
#endif

typedef struct {
        size_t size;
        size_t* used;
} Arena;

#define PRIArena    "{size:%zu, used:%zu}"
#define FMTArena(a) (a).size, ((a).used ? *(a).used : SIZE_MAX)
[[nodiscard]] bool arena_valid(Arena a);
void arena_dump(Arena a);

#if __has_c_attribute(gnu::format)
[[gnu::format(printf, 5, 6)]]
#endif
[[noreturn]] void gld__check_fail(const char* expression, const char* file, int line,
                                  const char* func, const char* format, ...);

#ifdef GLADIUS_IMPLEMENTATION
bool
arena_valid(Arena a) {
        return a.size > 0 && a.used != nullptr && a.size >= *a.used;
}

void
arena_dump(Arena a) {
        check(arena_valid(a), "Invalid Arena " PRIArena, FMTArena(a));
        printf("Arena" PRIArena "\n", FMTArena(a));
}

void
gld__check_fail(const char* expression, const char* file, int line, const char* func,
                const char* format, ...) {
        va_list args;
        va_start(args, format);
        fprintf(stderr, "File \"%s\", line %d, in %s:\n", file, line, func);

        fprintf(stderr, "\tcheck(%s);\n", expression);

        fputs("\t      ", stderr);
        for (size_t i = 0; i < strlen(expression); ++i) {
                fputc('^', stderr);
        }
        fputc('\n', stderr);

        fputs("Failure: ", stderr);
        vfprintf(stderr, format, args);
        fputc('\n', stderr);
        va_end(args);
        exit(EXIT_FAILURE);
}
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
