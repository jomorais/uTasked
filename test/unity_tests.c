#include "kernel.h"
#include "unity.h"

void setUp(void)
{
    // set stuff up here
}

void tearDown(void)
{
    // clean stuff up here
}

void test_Kernel_Instance(void)
{
    struct kernel_t *kernel = Kernel(NULL);
    TEST_ASSERT_NOT_NULL(kernel);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_Kernel_Instance);
    return UNITY_END();
}