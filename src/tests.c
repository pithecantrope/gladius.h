// #define GLADIUS_PREFIXED
#define GLADIUS_IMPLEMENTATION
#include "gladius.h"

void
test_arena(Arena* a) {
        int8_t* arr = allocn(a, int8_t, 3);
        arr[0] = arr[1] = arr[2] = -3;
        (void)alloc(a, uint64_t);
        assert(allocn(a, long, 0) != nullptr && "arena_alloc");
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
        printf("Author: " PRIString "\n", FMTString(SL("Egor Afanasin")));

        String hello = SL("hello, world!"), Hello = SL("Hello, World!");
        String l = SL("l"), W = SL("W");
        assert(string_cmp(Hello, hello) == -1 && "string_cmp");
        assert(string_cmp(l, W) == 1 && "string_cmp");

        String empty = {0}, s;
        assert(string_eq(empty, empty) && "string_eq");

        size_t before = a->len;
        String desc = SA(a, "Arena-owned string");
        assert(before + (size_t)desc.len == a->len && "string_new");

        before = a->len;
        s = string_dup(a, desc);
        assert(before + (size_t)s.len == a->len && string_eq(s, desc) && "string_dup");

        assert(string_eq(hello, string_fmt(a, "%s, %s!", "hello", "world")) && "string_fmt");

        String file = string_read_file(a, "LICENSE");
        assert(file.len == 1070 && "string_read_file");
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
