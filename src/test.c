#define GLADIUS_IMPLEMENTATION
#include "gladius.h"

constexpr size_t ARENA_SIZE = KiB(16);
#define TEST(name) static void test_##name(Arena a)
#define test(expr) check(expr, "test failed")

TEST(arena_valid) {
        test(arena_valid(a));
        a = (Arena){};
        test(!arena_valid(a));
}

TEST(arena_reset) {
        *a.used = a.size;
        test(arena_valid(a));
        arena_reset(a);
        test(*a.used == 0);
}

TEST(arena_create_destroy) {
        a = arena_create(KiB(1));
        test(arena_valid(a));
        test(*a.used == 0 && a.size == KiB(1));
        arena_destroy(a);
        test(!arena_valid(a));
}

TEST(arena) {
        test(a.size == ARENA_SIZE - sizeof(size_t));
        test_arena_valid(a);
        test_arena_reset(a);
        test_arena_create_destroy(a);
}

int
main(void) {
        char memory[ARENA_SIZE];
        Arena a = arena_borrow(memory, ARENA_SIZE);

        test_arena(a);

        puts("Success!");
        return 0;
}
