#include <stdint.h>
#include <stdlib.h>
#include <time.h>

uint32_t millis();

typedef uint32_t (*millis_t)();

struct port_t
{
    millis_t millis;
};

struct port_t *Port();