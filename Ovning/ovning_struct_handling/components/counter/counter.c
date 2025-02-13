#include <stdio.h>
#include "counter.h"
#include "freertos/FreeRTOS.h"

counter_t *init(int start_value, int increment)
{
    counter_t *new_counter = pvPortMalloc(sizeof(counter_t));
    new_counter->i = start_value;
    new_counter->increment = increment;
    return new_counter;
}

void destroy(counter_t *counter)
{
    vPortFree(counter);
}

void count(counter_t *counter)
{
    counter->i = counter->i + counter->increment;

    printf("i = %d\n", counter->i);
}
