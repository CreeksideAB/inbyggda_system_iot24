#include <stdio.h>
#include "printer.h"
#include "helper_component.h"
#include "esp_log.h"

extern "C" void app_main(void)
{
    int a = func();
    ESP_LOGI("MAIN", "got value %d", a);
}
