#include "port.h"

struct port_t *port;

uint32_t port_millis()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (ts.tv_sec * 1000 + ts.tv_nsec / 1000000L);
}

struct port_t *Port()
{
    if (port == NULL)
    {
        port = (struct port_t *)malloc(sizeof(struct port_t));
        *port = (struct port_t){
            .millis = &port_millis};
    }
    return port;
}