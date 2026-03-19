// #define API static inline
// #define MALLOC(size) custom_malloc(size)
// #define FREE(ptr)    custom_free(ptr)
#define GLADIUS_IMPLEMENTATION
#include "gladius.h"

#define TEST(name)       static void test_##name(Arena a)
#define check(condition) assert((condition) && "Test failed")

int
main(void) {
        puts("\nSuccess!");
        return 0;
}
