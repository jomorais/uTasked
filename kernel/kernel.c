#include "kernel.h"

static struct kernel_data_t kernel_data;
static struct kernel_t *kernel;

static int8_t __kernel_init(void)
{
    kernel_data.task_list = NULL;
    printf("%s\n", CONFIG_KERNEL_BANNER_MESSAGE);
    return SUCCESS;
}

static int8_t __kernel_is_task_paused(struct k_task_t *task)
{
    if (task->state == TASK_PAUSED)
        return true;
    return false;
}

static int8_t __kernel_is_task_waiting(struct k_task_t *task)
{
    if (task->state == TASK_WAITING)
        return true;
    return false;
}

static int8_t __kernel_is_task_pending(struct k_task_t *task)
{
    if (task->state == TASK_PENDING)
        return true;
    return false;
}

static int8_t __kernel_task_check_execution_time_overflow(struct k_task_t *task)
{
    if ((millis() - task->last_millis) >= task->execution_interval_ms)
    {
        task->state = TASK_PENDING;
        return true;
    }
    return false;
}

static int8_t __kernel_show_tasks(void)
{
    struct k_task_t *current_task = kernel_data.task_list;
    while (current_task != NULL)
    {
        printf("[ %s ]\n", current_task->name);
        printf("    routine_address: %p\n", current_task->routine);
        printf("    execution_interval_ms: %d ms\n", current_task->execution_interval_ms);
        printf("    execution_quantity: %d\n", current_task->execution_quantity);
        printf("    execution_type: %d\n\n", current_task->execution_type);
        current_task = current_task->next;
    }
    return SUCCESS;
}

static struct k_task_t *__kernel_get_task_by_name(const char *task_name)
{
    struct k_task_t *current_task = kernel_data.task_list;
    while (current_task != NULL)
    {
        if (strcmp(current_task->name, task_name) == 0)
        {
            return current_task;
        }
    }
    return NULL;
}

static struct k_task_t *__kernel_allocate_new_task(k_routine_t task,
                                                   const char *name,
                                                   uint32_t execution_interval_ms,
                                                   uint32_t execution_quantity,
                                                   uint8_t initial_state,
                                                   uint8_t *payload)
{
    struct k_task_t *new_task = (struct k_task_t *)malloc(sizeof(struct k_task_t));
    if (new_task == NULL)
        return NULL;
    *new_task = (struct k_task_t){
        .name = name,
        .routine = task,
        .execution_interval_ms = execution_interval_ms,
        .execution_quantity = execution_quantity,
        .state = initial_state,
        .last_millis = millis(),
        .execution_type = execution_quantity ? TASK_FINITE_EXEC : TASK_CONTINUOS_EXEC,
        .next = NULL};
    return new_task;
}

static struct k_task_t *__kernel_get_last_task(struct k_task_t *task_list)
{
    struct k_task_t *current_task;
    if (task_list == NULL)
        return task_list;

    current_task = kernel_data.task_list;
    while (current_task->next != NULL)
    {
        current_task = current_task->next;
    }
    return current_task;
}

static int8_t kernel_create_task(k_routine_t task,
                                 const char *name,
                                 uint32_t execution_interval_ms,
                                 uint32_t execution_quantity,
                                 uint8_t initial_state,
                                 uint8_t *payload)
{
    struct k_task_t *new_task = __kernel_allocate_new_task(task,
                                                           name,
                                                           execution_interval_ms,
                                                           execution_quantity,
                                                           initial_state,
                                                           payload);
    if (new_task == NULL)
        return ERROR;
    struct k_task_t *last_task = __kernel_get_last_task(kernel_data.task_list);
    if (last_task == NULL)
    {
        kernel_data.task_list = new_task;
        return SUCCESS;
    }
    last_task->next = new_task;
    return SUCCESS;
}

static int8_t kernel_edit_task(const char *task_name,
                               uint32_t execution_interval_ms,
                               uint32_t execution_quantity,
                               uint8_t state)
{
    struct k_task_t *task = __kernel_get_task_by_name(task_name);
    if (task == NULL)
        return ERROR;
    task->execution_interval_ms = execution_interval_ms;
    task->execution_quantity = execution_quantity;
    task->state = state;
    return SUCCESS;
}

static int8_t kernel_pause_task(const char *task_name)
{
    struct k_task_t *task = __kernel_get_task_by_name(task_name);
    if (task == NULL)
        return ERROR;
    task->state = TASK_PAUSED;
    return SUCCESS;
}

static int8_t kernel_resume_task(const char *task_name)
{
    struct k_task_t *task = __kernel_get_task_by_name(task_name);
    if (task == NULL)
        return ERROR;
    task->state = TASK_WAITING;
    return SUCCESS;
}

static int8_t kernel_destroy_task(const char *task_name)
{

    struct k_task_t *temp_task = kernel_data.task_list;
    struct k_task_t *previous_task = NULL;

    if (temp_task != NULL && strcmp(temp_task->name, task_name) == 0)
    {
        kernel_data.task_list = temp_task->next;
        free(temp_task);
        return SUCCESS;
    }

    while (temp_task != NULL && strcmp(temp_task->name, task_name) != 0)
    {
        previous_task = temp_task;
        temp_task = temp_task->next;
    }

    if (temp_task == NULL)
        return ERROR;

    previous_task->next = temp_task->next;

    free(temp_task);
    return SUCCESS;
}

static int8_t __kernel_task_callback_routine(struct k_task_t *task)
{
    if (task->state == TASK_PENDING)
    {
        task->routine(kernel, &task->payload);
        task->last_millis = millis();
        task->state = TASK_WAITING;

        if (task->execution_type != TASK_FINITE_EXEC)
            return true;

        task->execution_quantity--;
        if (task->execution_quantity <= 0)
            kernel_destroy_task(task->name);

        return true;
    }
    return false;
}

static int8_t __kernel_update(void)
{
    struct k_task_t *current_task = kernel_data.task_list;
    while (current_task != NULL)
    {
        if (__kernel_is_task_waiting(current_task) && __kernel_task_check_execution_time_overflow(current_task))
            __kernel_task_callback_routine(current_task);
        current_task = current_task->next;
    }
    return SUCCESS;
}

static void kernel_scheduler(void)
{
    __kernel_show_tasks();
    while (1)
    {
        __kernel_update();
    }
}

struct kernel_t *Kernel()
{
    if (kernel == NULL)
    {
        kernel = (struct kernel_t *)malloc(sizeof(struct kernel_t));
        *kernel = (struct kernel_t){
            .create_task = &kernel_create_task,
            .edit_task = &kernel_edit_task,
            .pause_task = &kernel_pause_task,
            .resume_task = &kernel_resume_task,
            .destroy_task = &kernel_destroy_task,
            .scheduler = &kernel_scheduler};
        __kernel_init();
    }
    return kernel;
}
