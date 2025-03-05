#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"

const char *TAG = "MAIN";

TaskHandle_t task_1_handle;
TaskHandle_t task_2_handle;
TaskHandle_t task_3_handle;

QueueHandle_t queue_handle;

void task_1_function(void *arg);
void task_3_function(void *arg);

void app_main(void)
{
    int arg = 0;
    ESP_LOGI(TAG, "app_main Starting");

    queue_handle = xQueueCreate(10, sizeof(int));

    if (xTaskCreatePinnedToCore(task_1_function, "Task 1", 3000, &arg, 1, &task_1_handle, 0) != pdPASS)
    {
        ESP_LOGE(TAG, "failed to create task 1");
    }
    if (xTaskCreatePinnedToCore(task_1_function, "Task 2", 3000, &arg, 1, &task_2_handle, 0) != pdPASS)
    {
        ESP_LOGE(TAG, "failed to create task 2");
    }
    if (xTaskCreatePinnedToCore(task_3_function, "Task 3", 3000, &arg, 2, &task_3_handle, 0) != pdPASS)
    {
        ESP_LOGE(TAG, "failed to create task 3");
    }
    while (true)
    {
        int item = 0;
        if (xQueueReceive(queue_handle, &item, pdMS_TO_TICKS(1000)) == pdTRUE)
        {
            ESP_LOGI(TAG, "Item received %d", item);
        }
        else
        {
            ESP_LOGI(TAG, "Received no item");
        }
    }
}

void task_1_function(void *arg)
{
    int *counter = (int *)arg;
    ESP_LOGI(TAG, "task_1_function running");

    while (true)
    {
        *counter = *counter + 1;
        ESP_LOGI(TAG, "Counter: %d", *counter);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }

    ESP_LOGI(TAG, "task_1_function returning");
    vTaskDelete(NULL);
}

void task_3_function(void *arg)
{
    int *counter = (int *)arg;
    int item = 5;
    ESP_LOGI(TAG, "task_3_function starting");

    while (true)
    {
        if (*counter >= 10)
        {
            printf("-\n");
            for (size_t i = 0; i < 10000000; i++)
            {
                *counter = *counter + 1;
                if (i % 10000 == 0)
                {
                    ESP_LOGI(TAG, "i = %d", i);
                }
            }
            printf("- %d -", *counter);
            printf("\n");
            *counter = 0;
            if (xQueueSend(queue_handle, &item, pdMS_TO_TICKS(10)) == pdTRUE)
            {
                item++;
                // lyckades skicka
            }
            else
            {
                // lyckades inte skicka
            }
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }

    ESP_LOGI(TAG, "task_3_function returning");
    vTaskDelete(NULL);
}