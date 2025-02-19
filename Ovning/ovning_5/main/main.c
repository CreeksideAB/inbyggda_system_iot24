#include <stdio.h>
#include "driver/uart.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "string.h"

#define UART_TX_PIN GPIO_NUM_13
#define UART_RX_PIN GPIO_NUM_12

#define UART_RECEIVE_BUFFER_SIZE 128

#define SEND_UART_PERIOD 2000

const char *TAG = "MAIN";

void app_main(void)
{
    const uart_port_t uart_num = UART_NUM_1;
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    };
    // Configure UART parameters
    ESP_ERROR_CHECK(uart_param_config(uart_num, &uart_config));

    // Set UART pins(TX: IO4, RX: IO5, RTS: IO18, CTS: IO19)
    ESP_ERROR_CHECK(uart_set_pin(uart_num, UART_TX_PIN, UART_RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));

    // Setup UART buffered IO with event queue
    const int uart_buffer_size = (1024 * 2);
    // Install UART driver using an event queue here
    ESP_ERROR_CHECK(uart_driver_install(uart_num, uart_buffer_size,
                                        uart_buffer_size, 0, NULL, 0));

    TickType_t sentTimer = xTaskGetTickCount();
    while (1)
    {
        TickType_t elapsedTime = xTaskGetTickCount() - sentTimer;
        if (elapsedTime > pdMS_TO_TICKS(SEND_UART_PERIOD))
        {
            sentTimer += pdMS_TO_TICKS(SEND_UART_PERIOD);
            // Write data to UART.
            char *test_str = "This is a test string.\n";
            int bytesSent = uart_write_bytes(uart_num, (const char *)test_str, strlen(test_str));
            if (bytesSent < 0)
            {
                ESP_LOGE(TAG, "Error writing uart");
            }
            else if (bytesSent > 0)
            {
                ESP_LOGI(TAG, "Sent %d bytes on uart %s", bytesSent, test_str);
            }
            else
            {
                ESP_LOGI(TAG, "Sent no bytes on uart");
            }
        }

        // Read data from UART.
        uint8_t data[UART_RECEIVE_BUFFER_SIZE] = {0};
        int length = 0;
        ESP_ERROR_CHECK(uart_get_buffered_data_len(uart_num, (size_t *)&length));
        if (length > UART_RECEIVE_BUFFER_SIZE - 1)
        {
            ESP_LOGI(TAG, "Bytes in buffer %d", length);
            length = uart_read_bytes(uart_num, data, UART_RECEIVE_BUFFER_SIZE - 1, pdMS_TO_TICKS(100));
        }
        else if (length > 0)
        {
            ESP_LOGI(TAG, "Bytes in buffer %d", length);
            length = uart_read_bytes(uart_num, data, length, pdMS_TO_TICKS(100));
        }
        else
        {
            vTaskDelay(pdMS_TO_TICKS(10));
            continue;
        }

        if (length < 0)
        {
            ESP_LOGE(TAG, "Error reading uart");
        }
        else if (length > 0)
        {
            data[length] = 0;
            ESP_LOGI(TAG, "Received %d bytes on uart %s", length, data);
        }
        else
        {
            ESP_LOGI(TAG, "Received no bytes on uart");
        }
    }
}