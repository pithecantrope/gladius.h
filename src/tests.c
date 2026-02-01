#include <stdio.h>
// #define GLADIUS_PREFIXED
#define GLADIUS_IMPLEMENTATION
#include "gladius.h"

void
test_arena(Arena* a) {
        int8_t* arr = allocn(a, int8_t, 3);
        arr[0] = arr[1] = arr[2] = -3;
        (void)alloc(a, uint64_t);
        assert(*arr == -3 && a->len == 2 * 8 && "arena_alloc");

        arena_reset(a);
        assert(a->len == 0 && "arena_reset");

        ArenaMark m = arena_mark_begin(a);
        a->len += KiB(1);
        arena_mark_end(m);
        assert(a->len == 0 && "arena_mark");
}

void
test_string(Arena* a) {
        String Hello = SL("Hello, World!");
        printf("String: " PRIString "\n", FMTString(Hello));

        size_t before = a->len;
        String author = SA(a, "Egor Afanasin");
        assert(before + (size_t)author.len == a->len && "SA");

        before = a->len;
        String s = string_dup(a, author);
        assert(before + (size_t)s.len == a->len && s.len == author.len && "string_dup");
        assert(memcmp(s.data, author.data, (size_t)s.len) == 0 && "string_dup");
}

int
main(void) {
        Arena* a = arena_create(KiB(16)); // MiB, GiB

        test_arena(a);
        test_string(a);
        printf("Arena: " PRIArena "\n", FMTArena(a));
        printf("\n\033[32mAll tests have been passed!\033[0m\n");

        arena_destroy(a);
        return 0;
}
