#include <stdio.h>
#define GLADIUS_IMPLEMENTATION
#include "gladius.h"

int
main(void) {
        arena* a = &(arena){.buf = malloc(KiB(67))};
        printf(PRIa "\n", FMTa(a));

        free(a->buf);
        return 0;
}
