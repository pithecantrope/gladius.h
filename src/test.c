// #define NDEBUG
// #define GLD_API static inline
// #define GLD_MALLOC(size) custom_malloc(size)
// #define GLD_FREE(ptr)    custom_free(ptr)
// #define GLADIUS_PREFIXED
#include <stdint.h>
#define GLADIUS_IMPLEMENTATION
#include "gladius.h"

#define TEST(name)       static void test_##name(Arena a)
#define check(condition) GLD_PANIC(!(condition), "Test failed")
static constexpr size_t ARENA_CAPACITY = MiB(1);

TEST(arena_reset) {
        *a.len += ARENA_CAPACITY;
        arena_reset(a);
        check(*a.len == 0);
}

TEST(arena_new) {
        check(NEW(a, long, 0) != nullptr);
        uint8_t* arr = NEW(a, uint8_t, 3);
        arr[0] = arr[1] = arr[2] = 7;
        (void)NEW(a, int32_t, 1);
        check(*arr == 7 && *a.len == 8);
        check((0 & (0 - 1)) == 0);
}

TEST(arena) {
        check(a.cap == ARENA_CAPACITY);
        test_arena_reset(a);
        test_arena_new(a);
}

int
main(void) {
        Arena a = arena_alloc(ARENA_CAPACITY);

        test_arena(a);
        arena_println(a);

        puts("\nSuccess!");
        arena_free(a);
        return 0;
}
