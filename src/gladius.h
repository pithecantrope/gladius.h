/* gladius.h - v0.0.0 - MIT - https://github.com/pithecantrope/gladius.h
 *
 *                 +------------------------------------------+
 *                 | Gladius - The Sword of Arena Allocation. |
 *                 +------------------------------------------+
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
 *      - Asserts for error handling. No return values need checking.
 *      - Short aliases by default. Define GLADIUS_PREFIXED before including to disable.
*/

#ifndef GLADIUS_HEADER
#define GLADIUS_HEADER

#if defined(__cplusplus)
#error "Gladius does not support C++"
#elif (!defined(__STDC_VERSION__) || (__STDC_VERSION__ < 202000L))
#error "Gladius requires C23"
#endif

// Symbol visibility control
#ifndef GLD_API
#define GLD_API extern
#endif // GLD_API

#ifndef GLD_ASSERT
#include <assert.h>
#define GLD_ASSERT assert
#endif // GLD_ASSERT

#ifndef GLD_MALLOC
#include <stdlib.h>
#define GLD_MALLOC malloc
#endif // GLD_MALLOC

#ifndef GLD_FREE
#include <stdlib.h>
#define GLD_FREE free
#endif // GLD_FREE

#include <stddef.h>
#include <stdint.h>

typedef ptrdiff_t gld_isize;

#ifndef GLADIUS_PREFIXED
#define isize gld_isize
#endif // GLADIUS_PREFIXED

// Arena Declaration -------------------------------------------------------------------------------
#define GLD_KiB(x) ((size_t)(x) << 10)
#define GLD_MiB(x) ((size_t)(x) << 20)
#define GLD_GiB(x) ((size_t)(x) << 30)

// Linear allocator with fixed capacity
typedef struct {
        char* buf;
        size_t len;
        size_t cap;
} GldArena;

#define GLD_PRIArena    "{buf: %p, len:%zu, cap:%zu}"
#define GLD_FMTArena(a) (a)->buf, (a)->len, (a)->cap
// printf(PRIArena "\n", FMTArena(a));

[[nodiscard]] GLD_API GldArena* gld_arena_create(size_t capacity);
GLD_API void gld_arena_reset(GldArena* a);
GLD_API void gld_arena_destroy(GldArena* a);

// Use `gld_alloc` and `gld_allocn` instead!
[[nodiscard]] GLD_API void* gld_arena_alloc(GldArena* a, size_t count, size_t size, size_t align);
#define gld_alloc(a, type)       (type*)gld_arena_alloc(a, 1, sizeof(type), alignof(type))
#define gld_allocn(a, type, num) (type*)gld_arena_alloc(a, num, sizeof(type), alignof(type))

// Save and restore arena state
typedef struct {
        GldArena* a;
        size_t len;
} GldArenaMark;

[[nodiscard]] GLD_API GldArenaMark gld_arena_mark_begin(GldArena* a);
GLD_API void gld_arena_mark_end(GldArenaMark m);

#ifndef GLADIUS_PREFIXED
#define KiB              GLD_KiB
#define MiB              GLD_MiB
#define GiB              GLD_GiB
#define Arena            GldArena
#define PRIArena         GLD_PRIArena
#define FMTArena         GLD_FMTArena
#define arena_create     gld_arena_create
#define arena_reset      gld_arena_reset
#define arena_destroy    gld_arena_destroy
#define arena_alloc      gld_arena_alloc
#define alloc            gld_alloc
#define allocn           gld_allocn
#define ArenaMark        GldArenaMark
#define arena_mark_begin gld_arena_mark_begin
#define arena_mark_end   gld_arena_mark_end
#endif // GLADIUS_PREFIXED

#ifdef GLADIUS_IMPLEMENTATION
// Arena Definition --------------------------------------------------------------------------------
[[nodiscard]] GLD_API GldArena*
gld_arena_create(size_t capacity) {
        GLD_ASSERT(capacity > 0 && "Invalid capacity");
        GldArena* a = GLD_MALLOC(sizeof(*a));
        GLD_ASSERT(a != nullptr);
        *a = (GldArena){.buf = GLD_MALLOC(capacity), .len = 0, .cap = capacity};
        GLD_ASSERT(a->buf != nullptr);
        return a;
}

GLD_API void
gld_arena_reset(GldArena* a) {
        GLD_ASSERT(a != nullptr && a->buf != nullptr && "Invalid arena");
        a->len = 0;
}

GLD_API void
gld_arena_destroy(GldArena* a) {
        GLD_ASSERT(a != nullptr && a->buf != nullptr && "Invalid arena");
        GLD_FREE(a->buf);
        *a = (GldArena){0};
        GLD_FREE(a);
}

[[nodiscard]] GLD_API void*
gld_arena_alloc(GldArena* a, size_t count, size_t size, size_t align) {
        GLD_ASSERT(a != nullptr && a->buf != nullptr && "Invalid arena");
        GLD_ASSERT(size > 0 && "Invalid size");
        GLD_ASSERT((align & (align - 1)) == 0 && "Invalid align");
        GLD_ASSERT(align <= alignof(max_align_t) && "Invalid align");

        size_t padding = -(uintptr_t)(a->buf + a->len) & (align - 1);
        GLD_ASSERT(a->cap - a->len >= padding && "Increase arena capacity");
        GLD_ASSERT(count <= (a->cap - a->len - padding) / size && "Increase arena capacity");
        void* ptr = a->buf + a->len + padding;
        a->len += padding + count * size;
        return ptr;
}

[[nodiscard]] GLD_API GldArenaMark
gld_arena_mark_begin(GldArena* a) {
        GLD_ASSERT(a != nullptr && a->buf != nullptr && "Invalid arena");
        return (GldArenaMark){.a = a, .len = a->len};
}

GLD_API void
gld_arena_mark_end(GldArenaMark m) {
        GLD_ASSERT(m.a != nullptr && m.len <= m.a->cap && "Invalid arena mark");
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
