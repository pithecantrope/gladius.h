# gladius.h
Header-only C23 arena-owned containers.

> *Gladius - The Sword of Arena Allocation.*

### Features

Idea:
// Appends '\0', runs your block, then removes it TEMP_CSTR_READ(s) { printf("%s\n", string_data_ptr(s)); // safe to use as cstr here } // '\0' removed here

https://pkg.odin-lang.org/core/strings/
https://github.com/GNOME/glib/blob/main/glib/gstring.h
https://pkg.go.dev/strings
https://github.com/scott-ainsworth/go-ascii/blob/master/ascii.go

String pre:
https://github.com/pithecantrope/gladius.h/blob/1949051c03dfc019d68c129afdc90229e2918652/src/tests.c

String pre pre:
https://github.com/pithecantrope/just/blob/cbb35a8659d7568fb43b4d28403f3b6cc0801890/src/just.h
https://github.com/pithecantrope/just/blob/cbb35a8659d7568fb43b4d28403f3b6cc0801890/src/just.c

- No hidden allocations. Zero runtime overhead. Type safety. Cache-friendly
- There is no shared ownership, no reference counting, no implicit heap allocation behind your back
- No manual memory management. Arena owns everything
- Zero leaks. Fail fast on errors
- Leaking memory is impossible. Allocations die with their arena
- Modern, user friendly, type safe and blazingly fast
- Asserts for error handling. No return values need checking.
- **Python**'s comfort, **C**'s performance

## Usage
In exactly one `.c` file:
```c
#define GLADIUS_IMPLEMENTATION
#include "gladius.h"
```

Elsewhere:
```c
#include "gladius.h"
```

## Arena
Linear allocator with fixed capacity.
```c
Arena* a = arena_create(KiB(16)); // MiB, GiB
        
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

printf("Arena: " PRIArena "\n", FMTArena(a));
arena_destroy(a);
```

## Thanks
- https://github.com/tsoding/nob.h
- https://nullprogram.com
