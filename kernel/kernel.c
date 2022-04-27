#include "kernel.h"

static struct kernel_data_t kernel_data;
static struct kernel_t *kernel;

int8_t __kernel_init(void)
{
    kernel_data.task_list = NULL;
    return 0;
}

int8_t __kernel_update(void)
{
    return 0;
}

struct k_task_t *__kernel_allocate_new_task(k_routine_t task, uint32_t execution_interval_ms, uint32_t execution_quantity, uint8_t initial_state)
{
    struct k_task_t *new_task = (struct k_task_t *)malloc(sizeof(struct k_task_t));
    if (new_task == NULL)
        return NULL;
    *new_task = {
        .task = task,
        .execution_interval_ms = execution_interval_ms,
        .execution_quantity = execution_quantity,
        .initial_state = initial_state,
        .last_millis = 0,
        .execution_type = 0,
        .next = NULL};
    return new_task;
}

struct k_task_t *__kernel_get_last_task(struct k_task_t *task_list)
{
    struct k_task_t *current_task;
    if (task_list == NULL)
        return task_list;

    current_task = list;
    while (current_task->next != NULL)
    {
        current_task = current_task->next;
    }
    return current_task;
}

int8_t kernel_create_task(k_routine_t task, uint32_t execution_interval_ms, uint32_t execution_quantity, uint8_t initial_state)
{
    struct k_task_t *new_task = __kernel_allocate_new_task(task, execution_interval_ms, execution_quantity, initial_state);
    if (new_task == NULL)
        return -1;
    struct k_task_t *last_task = __kernel_get_last_task(kernel_data.task_list);
    last_task->next = new_task;
    return 0;
}

int8_t kernel_edit_task(k_routine_t task, uint32_t execution_interval_ms, uint32_t execution_quantity, uint8_t initial_state)
{
    // todo
    return 0;
}

int8_t kernel_pause_task(k_routine_t task)
{
    // todo
    return 0;
}

int8_t kernel_destroy_task(k_routine_t task)
{
    // todo
    return 0;
}

int8_t kernel_scheduler(void)
{
    while (1)
    {
        __kernel_update();
        // todo:
        /*

        struct k_task_t *current_task = kernel_data.task_list;
        while (current_task != NULL)
        {

            printf("data = %d\n", currNodePtr->data);
            currNodePtr = currNodePtr->nextPtr;
        }
        */
    }
    return 0;
}

struct kernel_t *Kernel()
{
    if (kernel == NULL)
    {
        kernel = (struct kernet_t)malloc(sizeof(struct kernet_t));
        *kernel = {
            .create_task = &kernel_create_task,
            .edit_task = &kernel_edit_task,
            .pause_task = &kernel_pause_task,
            .destroy_task = &kernel_destroy_task,
            .scheduler = &kernel_scheduler};
        __kernel_init();
    }
    return kernel;
}
