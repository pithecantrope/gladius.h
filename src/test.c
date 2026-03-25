#define GLADIUS_IMPLEMENTATION
#include "gladius.h"

constexpr size_t ARENA_SIZE = KiB(16);
#define TEST(name) static void test_##name(Arena a)
#define test(expr) check(expr, "test failed")

TEST(arena_valid) {
        a = (Arena){};
        test(!arena_valid(a));
}

TEST(arena) {
        test(arena_valid(a));
        test(a.size == ARENA_SIZE - sizeof(size_t));
        test_arena_valid(a);
}

int
main(void) {
        char memory[ARENA_SIZE];
        Arena a = arena_borrow(memory, ARENA_SIZE);

        test_arena(a);

        puts("Success!");
        return 0;
}
