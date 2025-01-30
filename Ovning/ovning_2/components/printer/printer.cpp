#include <stdio.h>
#include "printer.h"
#include "freertos/FreeRTOS.h"
#include "helper_component.h"

#define PRINTER_PRINTF(format_string, ...) PRINT_GROUP_COLOR(ANSI_GREEN, "PRINTER", ANSI_RESET, format_string, ##__VA_ARGS__)
#define PRINTER_PRINTF_NL(format_string, ...) PRINT_GROUP_COLOR_NL(ANSI_GREEN, "PRINTER", ANSI_RESET, format_string, ##__VA_ARGS__)

namespace ovning_2
{
    int func(void)
    {
        return CONFIG_PRINT_LEVEL;
    }

    void printNumbers(int start, int end)
    {
        for (size_t i = start; i <= end; i++)
        {
            PRINTER_PRINTF("%d\n", i);
        }
    }

    void printEverySecond(int time)
    {
        int timeElapsed = 0;
        do
        {
            PRINTER_PRINTF("Time elapsed %d\n", timeElapsed);
            if (time - timeElapsed >= 1000)
            {
                vTaskDelay(pdMS_TO_TICKS(1000));
                timeElapsed += 1000;
            }
            else
            {
                vTaskDelay(pdMS_TO_TICKS(time - timeElapsed));
                timeElapsed += time - timeElapsed;
            }
        } while (timeElapsed < time);
        PRINTER_PRINTF("Time elapsed %d\n", timeElapsed);
    }

    int multiply(int a, int b)
    {
        return a * b;
    }
}