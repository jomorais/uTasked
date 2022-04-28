#ifndef PORT_H
#define PORT_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

typedef uint32_t (*millis_t)();
typedef void * (*malloc_t)(size_t size);
typedef void (*free_t)(void * pointer);


struct port_t
{
    millis_t millis;
    malloc_t malloc;
    free_t free;
};

struct port_t *Port(malloc_t malloc, free_t free);

#endif