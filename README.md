# gladius.h

> A blade forged for the Arena - not of combat, but of code: where objects sharing a lifetime are grouped and freed with a single blow.

 * About:
 *
## Usage
In **one** `.c` file:
```c
#define GLADIUS_IMPLEMENTATION
#include "gladius.h"
```

Elsewhere:
```c
#include "gladius.h"
```

## Customization
Define before including:
- GLADIUS_PREFIXED : disable short aliases (e.g., 'arena' → 'gladius_arena') 
- GLADIUS_API      : function declaration prefix (default: extern)

## Note
- Uses `assert` for error handling. No return values need checking.

 Header only C23 containers backed by arena.  * Containers (String, Vector, Hashmap) do not own their data □~@~T the arena does.
 * This eliminates manual memory management and makes leaks impossible.

 * Since the arena must be explicitly provided to each allocation function,
 * callers are forced to consider object lifetime from the start □~@~T making it
 * trivial to track where allocations occur and who owns them.
 *
 * Because allocations are simple pointer bumps inside a lifetime-bound
 * memory region, it is significantly faster than traditional malloc/free.
 *
 *          |   - Arena: linear allocator with fixed capacity, bound to a lifetime.   |
 *          |   - Containers (String, Vector, Map) do NOT own data — the arena does.  |
 *          |   - No malloc/free per object → faster, no leaks, no manual cleanup.    |
 *          |   - Explicit arena passing forces lifetime awareness at every call.     |

 ## Special thanks
 - https://github.com/tsoding/nob.h
 - https://nullprogram.com
