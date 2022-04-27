#include "kernel.h"
#include "unity.h"

int main(void)
{
    struct kernel_t *kernel = Kernel();
    kernel->init();
    kernel->create_task();
    kernel->scheduler();
    return 0;
}