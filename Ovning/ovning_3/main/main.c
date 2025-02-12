#include <stdio.h>
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "freertos/FreeRTOS.h"

typedef int (*callback_f)(int);
callback_f callback = NULL;

int callback_function(int value)
{
    return value * 2;
}

void set_callback(callback_f cb)
{
    callback = cb;
}

int *value = NULL;

int actual_value = 0;

void set_value(int *v)
{
    value = v;
}

void app_main(void)
{
    set_callback(callback_function);

    set_value(&actual_value);

    if (callback != NULL)
    {
        int result = callback(*value);
        printf("Result: %d\n", result);
    }

    gpio_config_t buttonConfig;
    buttonConfig.mode = GPIO_MODE_INPUT;
    buttonConfig.intr_type = GPIO_INTR_DISABLE;
    buttonConfig.pull_down_en = 0;
    buttonConfig.pull_up_en = 0;
    buttonConfig.pin_bit_mask = 1ULL << GPIO_NUM_12;
    esp_err_t error = gpio_config(&buttonConfig);

    ledc_timer_config_t ledcTimerConfig;
    ledcTimerConfig.clk_cfg = LEDC_AUTO_CLK;
    ledcTimerConfig.deconfigure = false;
    ledcTimerConfig.duty_resolution = LEDC_TIMER_12_BIT;
    ledcTimerConfig.freq_hz = 1000;
    ledcTimerConfig.speed_mode = LEDC_LOW_SPEED_MODE;
    ledcTimerConfig.timer_num = LEDC_TIMER_0;
    ledc_timer_config(&ledcTimerConfig);

    ledc_channel_config_t ledcChannelConfig;
    ledcChannelConfig.channel = LEDC_CHANNEL_0;
    ledcChannelConfig.duty = 0;
    ledcChannelConfig.flags.output_invert = 0;
    ledcChannelConfig.gpio_num = GPIO_NUM_13;
    ledcChannelConfig.hpoint = 0;
    ledcChannelConfig.intr_type = LEDC_INTR_DISABLE;
    ledcChannelConfig.speed_mode = LEDC_LOW_SPEED_MODE;
    ledcChannelConfig.timer_sel = LEDC_TIMER_0;

    ledc_channel_config(&ledcChannelConfig);

    int level = gpio_get_level(GPIO_NUM_12);
    TickType_t startTick = xTaskGetTickCount();
    int latch = 0;

    int duty = 0;
    int previousDuty = 0;

    ledc_set_duty_and_update(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 0b111111111111 / 2, 0);

    while (1)
    {
        level = gpio_get_level(GPIO_NUM_12);
        if (level == 1 && latch == 0)
        {
            TickType_t tickDifference = xTaskGetTickCount() - startTick;
            if (tickDifference >= pdMS_TO_TICKS(30))
            {
                startTick = xTaskGetTickCount();
                latch = 1;
                duty += 50;
            }
        }
        else if (level == 0 && latch == 1)
        {
            TickType_t tickDifference = xTaskGetTickCount() - startTick;
            if (tickDifference >= pdMS_TO_TICKS(30))
            {
                latch = 0;
            }
        }

        if (previousDuty != duty)
        {
            previousDuty = duty;
            ledc_set_duty_and_update(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, duty, 0);
        }
    }
}