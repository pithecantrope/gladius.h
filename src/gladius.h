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
 *      - Asserts for error handling. No return values need checking.
 *      - Short aliases by default. Define GLADIUS_PREFIXED to disable.
*/

#ifndef GLADIUS_HEADER
#define GLADIUS_HEADER

#if defined(__cplusplus)
#error "Gladius does not support C++"
#elif (!defined(__STDC_VERSION__) || (__STDC_VERSION__ < 202000L))
#error "Gladius requires C23"
#endif

// Symbol visibility
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

#include <limits.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

// Arena Declaration -------------------------------------------------------------------------------

// Linear allocator with fixed capacity
typedef struct {
        char* buf;
        size_t len;
        size_t cap;
} GldArena;

// printf(PRIArena "\n", FMTArena(a));
#define GLD_PRIArena    "{buf:%p, len:%zu, cap:%zu}"
#define GLD_FMTArena(a) (a)->buf, (a)->len, (a)->cap

#define GLD_KiB(x)      ((size_t)(x) << 10)
#define GLD_MiB(x)      ((size_t)(x) << 20)
#define GLD_GiB(x)      ((size_t)(x) << 30)

// Use `KiB`, `MiB` or `GiB` for capacity
[[nodiscard]] GLD_API GldArena* gld_arena_create(size_t capacity);
GLD_API void gld_arena_reset(GldArena* a);
GLD_API void gld_arena_destroy(GldArena* a);

// Use `alloc` and `allocn` macros instead! Zero count is valid
GLD_API void* gld_arena_alloc(GldArena* a, size_t count, size_t size, size_t align);
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
#define Arena            GldArena
#define PRIArena         GLD_PRIArena
#define FMTArena         GLD_FMTArena
#define KiB              GLD_KiB
#define MiB              GLD_MiB
#define GiB              GLD_GiB
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

// CharClass Declaration ---------------------------------------------------------------------------

// ASCII character classification
typedef bool GldCharClass[UCHAR_MAX + 1];

#define GLD_DIGIT  "0123456789"
#define GLD_XDIGIT "0123456789abcdefABCDEF"
#define GLD_LOWER  "abcdefghijklmnopqrstuvwxyz"
#define GLD_UPPER  "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define GLD_ALPHA  "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define GLD_ALNUM  "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define GLD_WORD   "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_"
#define GLD_SPACE  " \t\n\r\f\v"
#define GLD_PUNCT  "!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~"

[[nodiscard]] GLD_API GldCharClass* gld_charclass(GldArena* a, const char* chars);
// Is character `c` in class `cc`
#define gld_cc_in(c, cc) ((*cc)[(unsigned char)(c)])

#ifndef GLADIUS_PREFIXED
#define CharClass GldCharClass
#define DIGIT     GLD_DIGIT
#define XDIGIT    GLD_XDIGIT
#define LOWER     GLD_LOWER
#define UPPER     GLD_UPPER
#define ALPHA     GLD_ALPHA
#define ALNUM     GLD_ALNUM
#define WORD      GLD_WORD
#define SPACE     GLD_SPACE
#define PUNCT     GLD_PUNCT
#define charclass gld_charclass
#define cc_in     gld_cc_in
#endif // GLADIUS_PREFIXED

// String Declaration ------------------------------------------------------------------------------

// Arena-owned string without null terminator
typedef struct {
        char* data;
        int len;
} GldString;

// printf(PRIString "\n", FMTString(s));
#define GLD_PRIString    "{data:%.*s, len:%d}"
#define GLD_FMTString(s) (s).len, (s).data, (s).len

// Returns exactly -1, 0 or 1
[[nodiscard]] GLD_API int gld_string_cmp(GldString s1, GldString s2);
#define gld_string_eq(s1, s2) (gld_string_cmp(s1, s2) == 0)

// Read-only
#define GLD_SL(literal)                                                                            \
        (GldString) { .data = literal, .len = (int)(sizeof(literal) - 1) }
// Arena-owned
#define GLD_SA(a, literal) gld_string_new(a, literal, sizeof(literal) - 1)

[[nodiscard]] GLD_API GldString gld_string_new(GldArena* a, const char* s, size_t len);
[[nodiscard]] GLD_API GldString gld_string_dup(GldArena* a, GldString s);
#if __has_c_attribute(gnu::format)
[[gnu::format(printf, 2, 3)]]
#endif
[[nodiscard]] GLD_API GldString gld_string_fmt(GldArena* a, const char* fmt, ...);
[[nodiscard]] GLD_API GldString gld_string_read_file(GldArena* a, const char* path);
// [start, stop)
[[nodiscard]] GLD_API GldString gld_string_slice(GldString s, int start, int stop);

#ifndef GLADIUS_PREFIXED
#define String           GldString
#define PRIString        GLD_PRIString
#define FMTString        GLD_FMTString
#define string_cmp       gld_string_cmp
#define string_eq        gld_string_eq
#define SL               GLD_SL
#define SA               GLD_SA
#define string_new       gld_string_new
#define string_dup       gld_string_dup
#define string_fmt       gld_string_fmt
#define string_read_file gld_string_read_file
#define string_slice     gld_string_slice
#endif // GLADIUS_PREFIXED

#ifdef GLADIUS_IMPLEMENTATION
// Arena Definition --------------------------------------------------------------------------------

//
GldArena*
gld_arena_create(size_t capacity) {
        GLD_ASSERT(capacity > 0 && "Invalid capacity");
        GldArena* a = GLD_MALLOC(sizeof(*a));
        GLD_ASSERT(a != nullptr && "GLD_MALLOC failed");
        *a = (GldArena){.buf = GLD_MALLOC(capacity), .len = 0, .cap = capacity};
        GLD_ASSERT(a->buf != nullptr && "GLD_MALLOC failed");
        return a;
}

void
gld_arena_reset(GldArena* a) {
        GLD_ASSERT(a != nullptr && a->buf != nullptr && a->len <= a->cap && "Invalid arena");
        a->len = 0;
}

void
gld_arena_destroy(GldArena* a) {
        GLD_ASSERT(a != nullptr && a->buf != nullptr && a->len <= a->cap && "Invalid arena");
        GLD_FREE(a->buf);
        *a = (GldArena){0};
        GLD_FREE(a);
}

void*
gld_arena_alloc(GldArena* a, size_t count, size_t size, size_t align) {
        GLD_ASSERT(a != nullptr && a->buf != nullptr && a->len <= a->cap && "Invalid arena");
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

GldArenaMark
gld_arena_mark_begin(GldArena* a) {
        GLD_ASSERT(a != nullptr && a->buf != nullptr && a->len <= a->cap && "Invalid arena");
        return (GldArenaMark){.a = a, .len = a->len};
}

void
gld_arena_mark_end(GldArenaMark m) {
        GLD_ASSERT(m.a != nullptr && m.len <= m.a->cap && "Invalid arena mark");
        m.a->len = m.len;
}

// CharClass Definition ----------------------------------------------------------------------------

//
GldCharClass*
gld_charclass(GldArena* a, const char* chars) {
        GLD_ASSERT(chars != nullptr && "Invalid characters");
        GldCharClass* cc = gld_alloc(a, GldCharClass);
        memset(cc, false, sizeof(*cc));
        for (const char* p = chars; *p != '\0'; ++p) {
                unsigned char c = (unsigned char)*p;
                GLD_ASSERT(c < 128 && "Non-ASCII character");
                (*cc)[c] = true;
        }
        return cc;
}

// String Definition -------------------------------------------------------------------------------

//
int
gld_string_cmp(GldString s1, GldString s2) {
        GLD_ASSERT(s1.len >= 0 && "Invalid first string");
        GLD_ASSERT(s2.len >= 0 && "Invalid second string");
        if (s1.len != s2.len) {
                return s1.len < s2.len ? -1 : 1;
        }
        if (s1.len == 0) {
                return 0;
        }
        int cmp = memcmp(s1.data, s2.data, (size_t)s2.len);
        return cmp == 0 ? 0 : (cmp < 0 ? -1 : 1);
}

GldString
gld_string_new(GldArena* a, const char* s, size_t len) {
        GLD_ASSERT(s != nullptr && "Invalid string");
        GLD_ASSERT(len <= INT_MAX && "Length is too big");
        GldString res = {.data = gld_allocn(a, char, len), .len = (int)len};
        memcpy(res.data, s, len);
        return res;
}

GldString
gld_string_dup(GldArena* a, GldString s) {
        GLD_ASSERT(s.len >= 0 && "Invalid string");
        if (s.len == 0) {
                return (GldString){.data = nullptr, .len = 0};
        }
        GldString res = {.data = gld_allocn(a, char, (size_t)s.len), .len = s.len};
        memcpy(res.data, s.data, (size_t)s.len);
        return res;
}

GldString
gld_string_fmt(GldArena* a, const char* fmt, ...) {
        va_list args;
        va_start(args, fmt);
        int len = vsnprintf(NULL, 0, fmt, args);
        va_end(args);
        GLD_ASSERT(len >= 0 && "Invalid format string");

        va_start(args, fmt);
        GldString res = {.data = gld_allocn(a, char, (size_t)len + 1), .len = len};
        vsnprintf(res.data, (size_t)len + 1, fmt, args);
        --a->len; // Hide null terminator
        va_end(args);
        return res;
}

GldString
gld_string_read_file(GldArena* a, const char* path) {
        GLD_ASSERT(path != nullptr && "Invalid path");
        FILE* file = fopen(path, "rb");
        GLD_ASSERT(file != nullptr && "Failed to open file");

        int err = fseek(file, 0, SEEK_END);
        GLD_ASSERT(err == 0 && "fseek failed");
        long len = ftell(file);
        GLD_ASSERT(len >= 0 && "ftell failed");
        GLD_ASSERT(len <= INT_MAX && "File is too big");
        rewind(file);

        char* data = gld_allocn(a, char, (size_t)len);
        size_t read = fread(data, sizeof(char), (size_t)len, file);
        GLD_ASSERT(read == (size_t)len && "fread failed");
        err = fclose(file);
        GLD_ASSERT(err == 0 && "fclose failed");
        return (GldString){.data = data, .len = (int)len};
}

GldString
gld_string_slice(GldString s, int start, int stop) {
        GLD_ASSERT(s.len >= 0 && "Invalid string");
        GLD_ASSERT(0 <= start && 0 <= stop && "Negative indexes");
        GLD_ASSERT(start <= stop && "start > stop");
        GLD_ASSERT(stop <= s.len && "Index out of bounds");
        return (GldString){.data = s.data + start, .len = stop - start};
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
