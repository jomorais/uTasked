#include "kernel.h"
#include "port.h"
#include "unity.h"

void * fake_malloc_failed(size_t size){
    return NULL;
}

void fake_free_ok(void * pointer){
    pointer=NULL;
}

void setUp(void)
{
    // set stuff up here
}

void tearDown(void)
{
    // clean stuff up here
}

void test_expect_kernel_instance_successfully(void)
{
    struct kernel_t *kernel = Kernel(NULL);
    TEST_ASSERT_NOT_NULL(kernel);
}

void test_expect_kernel_instance_not_successfully(void)
{
    struct port_t mocked_port = (struct port_t){
        .malloc = fake_malloc_failed,
        .free = fake_free_ok
    };

    struct kernel_t *kernel = Kernel(&mocked_port);
    TEST_ASSERT_NULL(kernel);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_expect_kernel_instance_successfully);
    RUN_TEST(test_expect_kernel_instance_not_successfully);
    return UNITY_END();
}