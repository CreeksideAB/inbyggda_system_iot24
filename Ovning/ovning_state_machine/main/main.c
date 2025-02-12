#include <stdio.h>
#include "esp_log.h"

typedef enum
{
    STATE_ON,
    STATE_ON_DEBOUNCE,
    STATE_OFF,
    STATE_OFF_DEBOUNCE
} state_e;

const char *TAG = "STATE";

state_e currentState = STATE_OFF;
state_e previousState = STATE_OFF;
state_e nextState = STATE_OFF;

int buttonIsPressed = 0;
int level = 0;
int elapsedTime = 0;
int timeSwitchedState = 0;

void update_state_machine();

void app_main(void)
{

    while (1)
    {
        update_state_machine();

        // delay;
    }
}

void update_state_machine()
{
    level = 0;                                            // read level
    elapsedTime = /*Current time*/ 0 - timeSwitchedState; // calcualte elapsed
    switch (currentState)
    {
    case STATE_ON:
        if (level == 0)
        {
            nextState = STATE_ON_DEBOUNCE;
            // callback function that trigger on level going low
        }
        break;

    case STATE_ON_DEBOUNCE:
        if (currentState != previousState)
        {
            // kör kod som endast ska ske en gång
            buttonIsPressed = 0;
            // callback function that trigger on level going low
        }
        if (elapsedTime > 30)
        {
            nextState = STATE_OFF;
        }
        break;

    case STATE_OFF:
        if (level == 1)
        {
            nextState = STATE_OFF_DEBOUNCE;
        }
        break;

    case STATE_OFF_DEBOUNCE:
        if (currentState != previousState)
        {
            buttonIsPressed = 1;
        }
        if (elapsedTime > 30)
        {
            nextState = STATE_ON;
        }
        break;

    default:
        ESP_LOGE(TAG, "Unknown state");
        break;
    }

    if (nextState != currentState)
    {
        timeSwitchedState = /*Current time*/ 0; // set to current time
        elapsedTime = 0;                        // zero out elapsed
        switch (currentState)
        {

                default:
            break;
        }
    }

    previousState = currentState;
    currentState = nextState;
}