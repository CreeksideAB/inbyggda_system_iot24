#include <stdio.h>
#include "printer.h"
#include "helper_component.h"
#include "esp_log.h"

#define PRINT_MAIN(format_string, ...) PRINT_GROUP_COLOR(ANSI_BLUE, "MAIN", ANSI_RESET, format_string, ##__VA_ARGS__)
#define PRINT_MAIN_NL(format_string, ...) PRINT_GROUP_COLOR_NL(ANSI_BLUE, "MAIN", ANSI_RESET, format_string, ##__VA_ARGS__)

extern "C" void app_main(void)
{
    int a = ovning_2::func();
    PRINT_MAIN_NL("got value %d", a);
    a = inbyggda_iot24::func();
    PRINT_MAIN_NL("got value %d", a);

    ovning_2::printNumbers(5, 10);

    ovning_2::printEverySecond(10500);

    int product = ovning_2::multiply(2, 5);
    PRINT_MAIN_NL("product: %d", product);
}
