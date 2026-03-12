// #define NDEBUG
// #define GLD_API static inline
// #define GLD_MALLOC(size) custom_malloc(size)
// #define GLD_FREE(ptr)    custom_free(ptr)
// #define GLADIUS_PREFIXED
#define GLADIUS_IMPLEMENTATION
#include "gladius.h"

#define TEST(name)       static void test_##name(Arena a)
#define check(condition) GLD_PANIC(!(condition), "Test failed")
static constexpr size_t ARENA_CAPACITY = MiB(1);

TEST(arena) { check(a.cap == ARENA_CAPACITY); }

int
main(void) {
        Arena a = arena_alloc(ARENA_CAPACITY);

        test_arena(a);
        arena_println(a);

        puts("\nSuccess!");
        arena_free(a);
        return 0;
}
