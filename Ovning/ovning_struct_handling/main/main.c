#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "counter.h"

void app_main(void)
{
    counter_t *counter_1 = init(0, 2);
    counter_t *counter_2 = init(10, 3);

    while (1)
    {
        count(counter_1);
        count(counter_2);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }

    destroy(counter_1);
    destroy(counter_2);
}