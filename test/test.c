#include "kernel.h"
#include "unity.h"

int8_t task_1(void *kernel, void *payload)
{
    printf("task 1\n");
    return 0;
}

int8_t task_2(void *kernel, void *payload)
{
    printf("task 22\n");
    return 0;
}

int8_t task_3(void *kernel, void *payload)
{
    printf("task 333\n");
    return 0;
}

int8_t task_4(void *kernel, void *payload)
{
    struct kernel_t *k = (struct kernel_t *)kernel;
    printf("task 4444\n");
    k->pause_task("task_2");
    k->create_task(task_1, "task_1", 50, 5, TASK_WAITING, 0);
    return 0;
}

int main(void)
{
    struct kernel_t *kernel = Kernel();
    if (kernel->create_task(task_1, "task_1", 100, 5, TASK_WAITING, 0) != SUCCESS)
        printf("kernel->create_task:: error\n");
    if (kernel->create_task(task_2, "task_2", 1000, 0, TASK_WAITING, 0) != SUCCESS)
        printf("kernel->create_task:: error\n");
    if (kernel->create_task(task_3, "task_3", 3000, 0, TASK_WAITING, 0) != SUCCESS)
        printf("kernel->create_task:: error\n");
    if (kernel->create_task(task_4, "task_4", 4000, 0, TASK_WAITING, 0) != SUCCESS)
        printf("kernel->create_task:: error\n");

    kernel->scheduler();
    return 0;
}

// todo: messageQueue(struct kernel_t *kernel);