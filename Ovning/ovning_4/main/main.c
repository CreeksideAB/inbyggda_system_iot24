#include <stdio.h>
#include "esp_task_wdt.h"
#include "driver/gpio.h"

#define IDLE_CORE_0_BIT 0b01
#define IDLE_CORE_1_BIT 0b10
#define WATCHDOG_TIMEOUT 3000
#define WATCHDOG_TRIGGER_PANIC true
#define BUTTON_PIN GPIO_NUM_13

void button_interrupt_handler(void *arg);

void app_main(void)
{
    bool pressed = false;
    bool interrupt_is_configured = false;
    TickType_t button_pressed_time;

    gpio_config_t buttonConfig = {
        .mode = GPIO_MODE_INPUT,
        .pin_bit_mask = ((uint64_t)1) << BUTTON_PIN,
        .pull_down_en = 0,
        .pull_up_en = 1,
        .intr_type = GPIO_INTR_NEGEDGE,
    };

    ESP_ERROR_CHECK_WITHOUT_ABORT(gpio_config(&buttonConfig));

    ESP_ERROR_CHECK_WITHOUT_ABORT(gpio_install_isr_service(0));

    ESP_ERROR_CHECK_WITHOUT_ABORT(gpio_isr_handler_add(BUTTON_PIN, button_interrupt_handler, &pressed));
    interrupt_is_configured = true;

    esp_task_wdt_config_t wdtConfig = {
        .idle_core_mask = IDLE_CORE_0_BIT | IDLE_CORE_1_BIT,
        .trigger_panic = WATCHDOG_TRIGGER_PANIC,
        .timeout_ms = WATCHDOG_TIMEOUT,
    };
    ESP_ERROR_CHECK_WITHOUT_ABORT(esp_task_wdt_reconfigure(&wdtConfig));

    esp_task_wdt_add(NULL);

    while (1)
    {
        if (pressed)
        {
            pressed = false;
            button_pressed_time = xTaskGetTickCount();
            interrupt_is_configured = false;
            esp_task_wdt_reset();
        }

        if (!interrupt_is_configured)
        {
            TickType_t timeElapsed = xTaskGetTickCount() - button_pressed_time;
            if (timeElapsed > pdMS_TO_TICKS(40))
            {
                ESP_ERROR_CHECK_WITHOUT_ABORT(gpio_isr_handler_add(BUTTON_PIN, button_interrupt_handler, &pressed));
                interrupt_is_configured = true;
            }
        }

        vTaskDelay(pdMS_TO_TICKS(10));
    }

    esp_task_wdt_delete(NULL);
}

void button_interrupt_handler(void *arg)
{
    gpio_isr_handler_remove(BUTTON_PIN);
    bool *button = (bool *)arg;
    *button = true;
}