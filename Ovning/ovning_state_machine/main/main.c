#include <stdio.h>
#include "esp_log.h"

typedef enum
{
    STATE_ON,
    STATE_OFF
} state_e;

typedef enum
{
    DEBOUNCE,
    DONE
} sub_state_e;

const char *TAG = "STATE";

state_e currentState = STATE_OFF;
state_e previousState = STATE_OFF;
state_e nextState = STATE_OFF;

sub_state_e currentSubState = DEBOUNCE;

int buttonIsPressed = 0;
int level = 0;
int elapsedTime = 0;
int timeSwitchedState = 0;

void update_state_machine();

void update_sub_state_machine();

void app_main(void)
{

    while (1)
    {
        update_state_machine();
    }
}

void update_state_machine()
{
    level = 0;                                            // read level
    elapsedTime = /*Current time*/ 0 - timeSwitchedState; // calcualte elapsed
    switch (currentState)
    {
    case STATE_ON:
        if (currentState != previousState)
        {
            currentSubState = DEBOUNCE;
        }
        update_sub_state_machine();
        if (level == 0 && currentSubState == DONE)
        {
            nextState = STATE_OFF;
        }
        break;

    case STATE_OFF:
        if (currentState != previousState)
        {
            currentSubState = DEBOUNCE;
        }
        update_sub_state_machine();
        if (level == 1 && currentSubState == DONE)
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

void update_sub_state_machine()
{
    elapsedTime = /*Current time*/ 0 - timeSwitchedState; // calcualte elapsed
    switch (currentSubState)
    {
    case DEBOUNCE:
        if (elapsedTime > 500)
        {
            currentSubState = DONE;
        }
        break;

    case DONE:
        break;

    default:
        break;
    }
}