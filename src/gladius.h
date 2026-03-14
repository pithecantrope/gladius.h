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
#include <stdint.h>
#include <stdio.h>
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

#define GLD_CONCAT_(left, right) left##right
#define GLD_CONCAT(left, right)  GLD_CONCAT_(left, right)
#define GLD_UNIQUE(name)         GLD_CONCAT(name, __LINE__)

// Arena -------------------------------------------------------------------------------------------

// Linear allocator with fixed capacity
typedef struct {
        size_t cap;
        size_t* len;
} GldArena;

[[nodiscard]] GLD_API bool gld_arena_valid(GldArena a);
GLD_API void gld_arena_println(GldArena a);

#define GLD_KiB(x) ((size_t)(x) << 10)
#define GLD_MiB(x) ((size_t)(x) << 20)
#define GLD_GiB(x) ((size_t)(x) << 30)

[[nodiscard]] GLD_API GldArena gld_arena_alloc(size_t capacity);
GLD_API void gld_arena_reset(GldArena a);
GLD_API void gld_arena_free(GldArena a);

[[nodiscard]] GLD_API void* gld_arena_new(GldArena a, size_t count, size_t size, size_t align);
#define GLD_NEW(a, type, num) (type*)gld_arena_new(a, num, sizeof(type), alignof(type))
#define GLD_SCOPE(a)                                                                               \
        for (size_t GLD_UNIQUE(len) = *a.len, GLD_UNIQUE(once) = 1; GLD_UNIQUE(once);              \
             GLD_UNIQUE(once) = 0, *a.len = GLD_UNIQUE(len))                                       \
                for (int GLD_UNIQUE(break) = 1; GLD_UNIQUE(break); GLD_UNIQUE(break) = 0)

[[nodiscard]] GLD_API bool gld_arena_contains(GldArena a, void* ptr);
[[nodiscard]] GLD_API bool gld_arena_newest(GldArena a, void* ptr, size_t size);

#ifndef GLADIUS_PREFIXED
#define Arena          GldArena
#define arena_valid    gld_arena_valid
#define arena_println  gld_arena_println
#define KiB            GLD_KiB
#define MiB            GLD_MiB
#define GiB            GLD_GiB
#define arena_alloc    gld_arena_alloc
#define arena_reset    gld_arena_reset
#define arena_free     gld_arena_free
#define arena_new      gld_arena_new
#define NEW            GLD_NEW
#define SCOPE          GLD_SCOPE
#define arena_contains gld_arena_contains
#define arena_newest   gld_arena_newest
#endif // GLADIUS_PREFIXED

#ifdef GLADIUS_IMPLEMENTATION
bool
gld_arena_valid(GldArena a) {
        return a.cap > 0 && a.len != nullptr && *a.len <= a.cap;
}

void
gld_arena_println(GldArena a) {
        assert(gld_arena_valid(a) && "Invalid GldArena");
        printf("GldArena{cap:%zu, len:%zu}\n", a.cap, *a.len);
}

GldArena
gld_arena_alloc(size_t capacity) {
        assert(capacity > 0 && "Invalid capacity");
        assert(capacity <= SIZE_MAX - sizeof(size_t) && "Invalid capacity");
        size_t total = sizeof(size_t) + capacity;
        GldArena a = {
                .cap = capacity,
                .len = GLD_MALLOC(total),
        };
        GLD_PANIC(a.len == nullptr, "GLD_MALLOC failed");
        *a.len = 0;
        return a;
}

void
gld_arena_reset(GldArena a) {
        assert(gld_arena_valid(a) && "Invalid GldArena");
        *a.len = 0;
}

void
gld_arena_free(GldArena a) {
        assert(gld_arena_valid(a) && "Invalid GldArena");
        *a.len = SIZE_MAX;
        GLD_FREE(a.len);
}

void*
gld_arena_new(GldArena a, size_t count, size_t size, size_t align) {
        assert(gld_arena_valid(a) && "Invalid GldArena");
        assert(size > 0 && "Invalid size");
        assert(align > 0 && (align & (align - 1)) == 0 && "Invalid alignment");

        char* mem = (char*)a.len + sizeof(size_t);
        size_t padding = -(uintptr_t)(mem + *a.len) & (align - 1);
        assert(a.cap - *a.len >= padding && "Increase GldArena capacity");
        assert(count <= (a.cap - *a.len - padding) / size && "Increase GldArena capacity");
        void* ptr = mem + *a.len + padding;
        *a.len += padding + count * size;
        return ptr;
}

bool
gld_arena_contains(GldArena a, void* ptr) {
        assert(gld_arena_valid(a) && "Invalid GldArena");
        assert(ptr != nullptr && "Invalid pointer");
        char* mem = (char*)a.len + sizeof(size_t);
        return mem <= (char*)ptr && (char*)ptr <= mem + *a.len;
}

bool
gld_arena_newest(GldArena a, void* ptr, size_t size) {
        assert(gld_arena_valid(a) && "Invalid GldArena");
        assert(ptr != nullptr && "Invalid pointer");
        char* mem = (char*)a.len + sizeof(size_t);
        return mem + *a.len == (char*)ptr + size;
}
#endif // GLADIUS_IMPLEMENTATION

// String ------------------------------------------------------------------------------------------
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
