#include <stdio.h>
#include "nvs_flash.h"
#include "esp_log.h"

const char *TAG = "NVS_TEST";

void app_main(void)
{
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    nvs_handle_t handle;

    err = nvs_open("Test_namespace", NVS_READWRITE, &handle);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to open namespace");
        return;
    }

    int32_t counter = 0;
    err = nvs_get_i32(handle, "counter", &counter);
    if (err != ESP_OK)
    {
        counter = 0;
        ESP_LOGW(TAG, "DIdnt find counter");
    }
    else
    {
        counter += 1;
    }

    err = nvs_set_i32(handle, "counter", counter);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to write counter to nvs");
    }

    err = nvs_commit(handle);

    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to commit to nvs");
    }

    nvs_close(handle);

    ESP_LOGI(TAG, "Counter: %ld", counter);
}