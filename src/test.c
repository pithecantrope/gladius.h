// #define GLADIUS_PREFIXED
#define GLADIUS_IMPLEMENTATION
// #define GLD_API static inline
// #define GLD_ASSERT(condition, message) custom_assert(condition, message)
// #define GLD_MALLOC(size) custom_malloc(size)
// #define GLD_FREE(ptr) custom_free(ptr)
#include "gladius.h"

// *      - Tests are embedded executable docs. Use them for reference.
#define check(condition)                                                                           \
        do {                                                                                       \
                if (!(condition)) {                                                                \
                        fprintf(stderr,                                                            \
                                "\033[31m"                                                         \
                                "Failure: %s:%d in %s:\n"                                          \
                                "\033[34m"                                                         \
                                "\t%s\n"                                                           \
                                "\033[0m",                                                         \
                                __FILE__, __LINE__, __func__, #condition);                         \
                        exit(EXIT_FAILURE);                                                        \
                }                                                                                  \
        } while (0)

static Arena* a;
#define TEST(name) static void test_##name(void)

TEST(arena_reset) {
        a->len = MiB(1);
        arena_reset(a);
        check(a->len == 0);
}

TEST(arena_alloc) {
        check(allocn(a, long, 0) != nullptr);
        uint8_t* arr = allocn(a, uint8_t, 3);
        arr[0] = arr[1] = arr[2] = 7;
        (void)alloc(a, int32_t);
        check(*arr == 7 && a->len == 8);
}

TEST(arena_scratch) {
        size_t before = a->len;
        arena_scratch(a) { a->len += KiB(1); }
        check(before == a->len);
}

TEST(arena) {
        test_arena_reset();
        test_arena_alloc();
        test_arena_scratch();
}

int
main(void) {
        a = arena_create(MiB(1));
        test_arena();

        puts("\033[32mSuccess!\033[0m");
        arena_destroy(a);
        return 0;
}
