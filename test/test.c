#include "kernel.h"
#include "unity.h"


int8_t task_1(void *payload)
{
    printf("task 1");
    return 0;
}

int main(void)
{
    struct kernel_t *kernel = Kernel();
    if(kernel->create_task(task_1, "task_i", 1000, 0, 0) != 0)
        printf("kernel->create_task:: error\n");
    kernel->scheduler();
    return 0;
}