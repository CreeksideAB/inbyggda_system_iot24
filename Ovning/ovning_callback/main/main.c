#include <stdio.h>
#include "esp_log.h"

typedef int (*my_function_pointer_f)(void *user_data);

typedef struct
{
    int a;
    int b;
    char text[10];
} function_3_user_data_t;

// Callback
const char *TAG = "CALLBACK";

int function_1(void *user_data);
int function_2(void *user_data);
int function_3(void *user_data);

void set_my_function_pointer(my_function_pointer_f f, void *user_data);
void run_my_function_pointer();

my_function_pointer_f my_function_pointer = NULL;
void *my_function_pointer_user_data;

void app_main(void)
{
    int a = 0;
    char *text = "Hello";
    function_3_user_data_t data = {
        .a = 0,
        .b = 0,
        .text = {0}};

    sprintf(data.text, "Hello 3");

    printf("Starting Main\n");
    run_my_function_pointer();
    set_my_function_pointer(function_1, &a);
    run_my_function_pointer();
    set_my_function_pointer(function_2, text);
    run_my_function_pointer();
    set_my_function_pointer(function_3, &data);
    run_my_function_pointer();

    function_1(&a);
    function_2(text);
    function_3(&data);
}

void set_my_function_pointer(my_function_pointer_f f, void *user_data)
{
    my_function_pointer = f;
    my_function_pointer_user_data = user_data;
}

void run_my_function_pointer()
{
    if (my_function_pointer != NULL)
    {
        int a = my_function_pointer(my_function_pointer_user_data);
        printf("My function pointer returned %d\n", a);
    }
}

int function_1(void *user_data)
{
    int *integer = (int *)user_data;
    ESP_LOGI(TAG, "Function 1 %d", (*integer)++);
    return 1;
}

int function_2(void *user_data)
{
    char *text = (char *)user_data;
    ESP_LOGE(TAG, "Function 2 %s", text);
    return 2;
}

int function_3(void *user_data)
{
    function_3_user_data_t *data = (function_3_user_data_t *)user_data;
    data->a = data->a + 1;
    data->b = data->a * 2;
    ESP_LOGW(TAG, "Function 3 %s, a:%d, b:%d", data->text, data->a, data->b);
    return 3;
}