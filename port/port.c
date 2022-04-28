#include "port.h"

static struct port_t port;

uint32_t port_millis()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (ts.tv_sec * 1000 + ts.tv_nsec / 1000000L);
}


void * port_malloc(size_t size)
{
    return malloc(size);
}


void port_free(void * pointer)
{
    free(pointer);
}


struct port_t *Port(malloc_t malloc, free_t free)
{
    if (port.malloc == NULL && port.free == NULL)
    {
        port = (struct port_t){
            .millis = &port_millis,
            .malloc = malloc ? malloc : &port_malloc,
            .free = free ? free : &port_free
        };
    }
    return &port;
}