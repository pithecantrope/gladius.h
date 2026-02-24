# gladius.h
Header-only C23 arena-owned containers.

> *Gladius - The Sword of Arena Allocation.*

### Features
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
