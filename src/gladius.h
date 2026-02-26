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

// Linear allocator with fixed capacity
typedef struct {
        char* buf;
        size_t len;
        size_t cap;
} GldArena;

GLD_API void gld_arena_println(const GldArena* a);

#define GLD_KiB(x) ((size_t)(x) << 10)
#define GLD_MiB(x) ((size_t)(x) << 20)
#define GLD_GiB(x) ((size_t)(x) << 30)

// Prefer `KiB`, `MiB` or `GiB` for capacity
[[nodiscard]] GLD_API GldArena* gld_arena_create(size_t capacity);
GLD_API void gld_arena_reset(GldArena* a);
GLD_API void gld_arena_destroy(GldArena* a);

// Save and restore arena state
typedef struct {
        GldArena* a;
        size_t len;
} GldArenaScratch;

[[nodiscard]] GLD_API GldArenaScratch gld_arena_scratch_begin(GldArena* a);
GLD_API void gld_arena_scratch_end(GldArenaScratch sc);
#define gld_arena_scratch(arena)                                                                   \
        for (GldArenaScratch _sc = gld_arena_scratch_begin(arena); _sc.a;                          \
             gld_arena_scratch_end(_sc), _sc.a = nullptr)

#ifndef GLADIUS_PREFIXED
#define Arena               GldArena
#define arena_println       gld_arena_println
#define KiB                 GLD_KiB
#define MiB                 GLD_MiB
#define GiB                 GLD_GiB
#define arena_create        gld_arena_create
#define arena_reset         gld_arena_reset
#define arena_destroy       gld_arena_destroy
#define ArenaScratch        GldArenaScratch
#define arena_scratch_begin gld_arena_scratch_begin
#define arena_scratch_end   gld_arena_scratch_end
#define arena_scratch       gld_arena_scratch
#endif // GLADIUS_PREFIXED

// Arena Test --------------------------------------------------------------------------------------
#ifdef GLADIUS_TEST
static void
test_arena_reset(Arena* a) {
        a->len = MiB(1);
        arena_reset(a);
        GLD_CHECK(a->len == 0);
}

static void
test_arena_scratch(Arena* a) {
        size_t before = a->len;
        arena_scratch(a) { a->len += KiB(1); }
        GLD_CHECK(before == a->len);
}

static void
test_arena(Arena* a) {
        test_arena_reset(a);
        test_arena_scratch(a);
}
#endif // GLADIUS_TEST

// Arena Definition --------------------------------------------------------------------------------
#ifdef GLADIUS_IMPLEMENTATION
void
gld_arena_println(const GldArena* a) {
        GLD_ASSERT(a != nullptr && a->buf != nullptr && a->len <= a->cap, "Invalid Arena");
        printf("{buf:%p, len:%zu, cap:%zu}\n", a->buf, a->len, a->cap);
}

GldArena*
gld_arena_create(size_t capacity) {
        GLD_ASSERT(capacity > 0, "Invalid capacity");
        GldArena* a = GLD_MALLOC(sizeof(*a));
        GLD_ASSERT(a != nullptr, "GLD_MALLOC failed");
        *a = (GldArena){.buf = GLD_MALLOC(capacity), .len = 0, .cap = capacity};
        GLD_ASSERT(a->buf != nullptr, "GLD_MALLOC failed");
        return a;
}

void
gld_arena_reset(GldArena* a) {
        GLD_ASSERT(a != nullptr && a->buf != nullptr && a->len <= a->cap, "Invalid Arena");
        a->len = 0;
}

void
gld_arena_destroy(GldArena* a) {
        GLD_ASSERT(a != nullptr && a->buf != nullptr && a->len <= a->cap, "Invalid Arena");
        GLD_FREE(a->buf);
        GLD_FREE(a);
}

GldArenaScratch
gld_arena_scratch_begin(GldArena* a) {
        GLD_ASSERT(a != nullptr && a->buf != nullptr && a->len <= a->cap, "Invalid Arena");
        return (GldArenaScratch){.a = a, .len = a->len};
}

void
gld_arena_scratch_end(GldArenaScratch sc) {
        GLD_ASSERT(sc.a != nullptr && sc.len <= sc.a->cap, "Invalid scratch Arena");
        sc.a->len = sc.len;
}
#endif // GLADIUS_IMPLEMENTATION

#ifdef GLADIUS_TEST
[[maybe_unused]] static void
test(void) {
        Arena* a = arena_create(MiB(1));
        test_arena(a);

        puts("\033[32mSuccess!\033[0m");
        arena_destroy(a);
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
