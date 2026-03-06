// #define GLD_API static inline
// #define GLD_MALLOC(size)               custom_malloc(size)
// #define GLD_FREE(ptr)                  custom_free(ptr)
// #define GLD_ASSERT(condition, message) custom_assert(condition, message)
// #define GLADIUS_PREFIXED
#define GLADIUS_IMPLEMENTATION
#include "gladius.h"

#define TEST(name)       static void test_##name(void)
#define check(condition) GLD_ASSERT(condition, "Failure")

int
main(void) {
        check(!67);

        puts("Success!");
        return 0;
}
