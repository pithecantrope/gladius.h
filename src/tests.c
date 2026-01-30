#include <stdio.h>
#define GLADIUS_IMPLEMENTATION
#include "gladius.h"

static arena* a;

void
test_arena(void) {
        allocn(a, int8_t, 3);
        alloc(a, uint64_t);
        assert(a->len == 16 && "arena_alloc");

        arena_reset(a);
        assert(a->len == 0 && "arena_reset");

        arena_mark m = arena_mark_begin(a);
        a->len += KiB(1);
        arena_mark_end(m);
        assert(a->len == 0 && "arena_mark");
}

int
main(void) {
        a = arena_create(KiB(16));

        test_arena();
        printf("\033[32mAll tests passed!\033[0m\n");

        arena_destroy(a);
        return 0;
}
