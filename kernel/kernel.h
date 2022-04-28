#ifndef KERNEL_H
#define KERNEL_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "config.h"
#include "port.h"

enum kernel_task_states_t
{
    TASK_PAUSED = 0,
    TASK_PENDING,
    TASK_WAITING
};

enum kernel_error_t
{
    ERROR = -1,
    SUCCESS,
};

enum boolean_t
{
    false = 0,
    true,
};

enum kernel_execution_type_t
{
    TASK_CONTINUOS_EXEC = 0,
    TASK_FINITE_EXEC,
};

typedef int8_t (*k_routine_t)(void *kernel, void *payload);

typedef int8_t (*k_init_t)(void);
typedef int8_t (*k_update_t)(void);
typedef int8_t (*k_create_task_t)(k_routine_t task, const char *name, uint32_t execution_interval_ms, uint32_t execution_quantity, uint8_t initial_state, uint8_t *payload);
typedef int8_t (*k_edit_task_t)(const char *name, uint32_t execution_interval_ms, uint32_t execution_quantity, uint8_t state);
typedef int8_t (*k_pause_task_t)(const char *name);
typedef int8_t (*k_resume_task_t)(const char *name);
typedef int8_t (*k_destroy_task_t)(const char *name);
typedef void (*k_scheduler_t)(void);

struct k_task_t
{
    const char *name;
    k_routine_t routine;
    uint32_t last_millis;
    uint8_t state;
    uint32_t execution_interval_ms;
    uint32_t execution_quantity;
    uint8_t execution_type;
    uint8_t payload;
    struct k_task_t *next;
};

struct kernel_data_t
{
    struct k_task_t *task_list;
    struct port_t *port;
};

struct kernel_t
{
    k_create_task_t create_task;
    k_edit_task_t edit_task;
    k_pause_task_t pause_task;
    k_resume_task_t resume_task;
    k_destroy_task_t destroy_task;
    k_scheduler_t scheduler;
};

struct kernel_t *Kernel(struct port_t *port);

#endif