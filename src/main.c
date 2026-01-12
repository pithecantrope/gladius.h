#include <stdio.h>
#define GLADIUS_IMPLEMENTATION
#include "gladius.h"

int
main(void) {
        arena* a = arena_create(KiB(16));
        printf(PRI_arena "\n", FMT_arena(a));

        arena_mark m = arena_mark_begin(a);
        a->len += sizeof(double);
        arena_mark_end(m);
        assert(a->len == 0);

        arena_destroy(a);
        return 0;
}
