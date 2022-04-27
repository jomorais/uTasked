#include <stdint.h>
#include <malloc.h>

typedef int8_t (*k_routine_t)(void *payload);

typedef int8_t (*k_init_t)(void);
typedef int8_t (*k_update_t)(void);
typedef int8_t (*k_create_task_t)(k_routine_t task, uint32_t execution_interval_ms, uint32_t execution_quantity, uint8_t initial_state);
typedef int8_t (*k_edit_task_t)(k_routine_t task, uint32_t execution_interval_ms, uint32_t execution_quantity, uint8_t initial_state);
typedef int8_t (*k_pause_task_t)(k_routine_t task);
typedef int8_t (*k_destroy_task_t)(k_routine_t task);
typedef int8_t (*k_scheduler_t)(void);

struct k_task_t
{
    k_routine_t routine;
    uint32_t last_millis;
    uint8_t initial_state;
    uint32_t execution_interval_ms;
    uint32_t execution_quantity;
    uint8_t execution_type;
    struct k_task_t *next;
};

struct kernel_data_t
{
    struct k_task_t *task_list;
};

struct kernel_t
{
    k_create_task_t create_task;
    k_edit_task_t edit_task;
    k_pause_task_t pause_task;
    k_destroy_task_t destroy_task;
    k_scheduler_t scheduler;
};

struct kernel_t *Kernel();