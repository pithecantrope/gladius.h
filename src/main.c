#include <stdio.h>
#define GLADIUS_IMPLEMENTATION
#include "gladius.h"

int
main(void) {
        arena* a = arena_create(KiB(16));
        printf(PRI_arena "\n", FMT_arena(a));

        arena_destroy(a);
        return 0;
}
