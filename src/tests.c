#include <stdio.h>
#define GLADIUS_IMPLEMENTATION
#include "gladius.h"

static arena* a;

void
test_arena(void) {
        int32_t* arr = allocn(a, int32_t, 2);
        arr[0] = arr[1] = 1;
        assert(a->len == 8 && "arena_alloc");

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
        printf("All tests passed!\n");

        arena_destroy(a);
        return 0;
}
