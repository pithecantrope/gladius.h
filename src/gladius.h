/* gladius.h - v0.0.0 - MIT License - https://github.com/pithecantrope/gladius.h
 *
 * Usage:
 *      In exactly one .c file:
 *              #define GLADIUS_IMPLEMENTATION
 *              #include "gladius.h"
 *      Elsewhere:
 *              #include "gladius.h"
 *
 * Customization (define BEFORE including):
 *      - GLADIUS_PREFIXED : disable short aliases (e.g., 'arena' → 'gladius_arena')
 *      - GLADIUS_API      : function declaration prefix (default: extern)
*/

#ifndef GLADIUS_HEADER
#define GLADIUS_HEADER

#if defined(__cplusplus)
#error "Gladius does not support C++"
#elif (!defined(__STDC_VERSION__) || (__STDC_VERSION__ < 202000L))
#error "Gladius requires C23"
#endif

#ifndef GLADIUS_API
#define GLADIUS_API extern
#endif // GLADIUS_API

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

// Arena Declaration -------------------------------------------------------------------------------
#define GLADIUS_KiB(x) ((size_t)(x) << 10)
#define GLADIUS_MiB(x) ((size_t)(x) << 20)
#define GLADIUS_GiB(x) ((size_t)(x) << 30)

typedef struct {
        char* buf;
        size_t len;
        size_t cap;
} gladius_arena;

#define GLADIUS_PRI_arena    "{buf: %p, len:%zu, cap:%zu}"
#define GLADIUS_FMT_arena(a) (a)->buf, (a)->len, (a)->cap
// printf(PRI_arena "\n", FMT_arena(a));

[[nodiscard]] GLADIUS_API gladius_arena* gladius_arena_create(size_t capacity);
GLADIUS_API void gladius_arena_reset(gladius_arena* a);
GLADIUS_API void gladius_arena_destroy(gladius_arena* a);

// Use gladius_alloc and gladius_allocn instead!
[[nodiscard]] GLADIUS_API void* gladius_arena_alloc(gladius_arena* a, size_t count, size_t size,
                                                    size_t align);
#define gladius_alloc(a, type)       (type*)gladius_arena_alloc(a, 1, sizeof(type), alignof(type))
#define gladius_allocn(a, type, num) (type*)gladius_arena_alloc(a, num, sizeof(type), alignof(type))

typedef struct {
        gladius_arena* a;
        size_t len;
} gladius_arena_mark;

[[nodiscard]] GLADIUS_API gladius_arena_mark gladius_arena_mark_begin(gladius_arena* a);
GLADIUS_API void gladius_arena_mark_end(gladius_arena_mark m);

#ifndef GLADIUS_PREFIXED
#define KiB              GLADIUS_KiB
#define MiB              GLADIUS_MiB
#define GiB              GLADIUS_GiB
#define arena            gladius_arena
#define PRI_arena        GLADIUS_PRI_arena
#define FMT_arena        GLADIUS_FMT_arena
#define arena_create     gladius_arena_create
#define arena_reset      gladius_arena_reset
#define arena_destroy    gladius_arena_destroy
#define arena_alloc      gladius_arena_alloc
#define alloc            gladius_alloc
#define allocn           gladius_allocn
#define arena_mark       gladius_arena_mark
#define arena_mark_begin gladius_arena_mark_begin
#define arena_mark_end   gladius_arena_mark_end
#endif // GLADIUS_PREFIXED

#ifdef GLADIUS_IMPLEMENTATION
// Arena Definition --------------------------------------------------------------------------------
[[nodiscard]] GLADIUS_API gladius_arena*
gladius_arena_create(size_t capacity) {
        assert(capacity > 0 && "Invalid capacity");
        gladius_arena* a = malloc(sizeof(*a));
        assert(a != nullptr);
        *a = (gladius_arena){.buf = malloc(capacity), .len = 0, .cap = capacity};
        assert(a->buf != nullptr);
        return a;
}

GLADIUS_API void
gladius_arena_reset(gladius_arena* a) {
        assert(a != nullptr && a->buf != nullptr && "Invalid arena");
        a->len = 0;
}

GLADIUS_API void
gladius_arena_destroy(gladius_arena* a) {
        assert(a != nullptr && a->buf != nullptr && "Invalid arena");
        free(a->buf);
        *a = (gladius_arena){.buf = NULL, .len = 0, .cap = 0};
        free(a);
}

[[nodiscard]] GLADIUS_API void*
gladius_arena_alloc(gladius_arena* a, size_t count, size_t size, size_t align) {
        assert(a != nullptr && a->buf != nullptr && "Invalid arena");
        assert(size > 0 && "Invalid size");
        assert((align & (align - 1)) == 0 && "Invalid align");

        size_t padding = -(uintptr_t)(a->buf + a->len) & (align - 1);
        assert(count <= (a->cap - a->len - padding) / size && "Increase arena capacity");
        void* ptr = a->buf + a->len + padding;
        a->len += padding + count * size;
        return ptr;
}

[[nodiscard]] GLADIUS_API gladius_arena_mark
gladius_arena_mark_begin(gladius_arena* a) {
        assert(a != nullptr && a->buf != nullptr && "Invalid arena");
        return (gladius_arena_mark){.a = a, .len = a->len};
}

GLADIUS_API void
gladius_arena_mark_end(gladius_arena_mark m) {
        assert(m.a != nullptr && m.len <= m.a->cap && "Invalid arena mark");
        m.a->len = m.len;
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
