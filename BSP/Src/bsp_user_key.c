//
// Created by wbz on 2026/5/18.
//
#include "gpio.h"
#include "bsp_user_key.h"

uint8_t key_GetNum(void)
{
    return  HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_0) == GPIO_PIN_SET;
}

void Key_Process(void)
{
    static uint32_t delay_time = 0;
    static uint32_t key_press_time = 0;
    switch (key_state)
    {
        case KEY_IDLE:

            if (key_GetNum() == 0)
            {
                delay_time =  global_count_tim;
                key_state = KEY_DEBOUNCE;

            }
            break;

        case KEY_DEBOUNCE:

            if ((global_count_tim - delay_time) >= 10)
            {
                if (key_GetNum() == 0)
                {
                    key_press_time = global_count_tim;
                    key_state = KEY_PRESSING;
                }
                else
                {
                    key_state = KEY_IDLE;
                }
            }

            break;

        case KEY_PRESSING:

            if (key_GetNum() == 1)
            {
                key_state = KEY_RELEASE;
                delay_time = global_count_tim;
            }
            break;

        case KEY_RELEASE:

            if ((global_count_tim - delay_time) >= 10)
            {
                if (key_GetNum() == 1)
                {
                    if ((global_count_tim -key_press_time) <= 1000)
                    {
                        Target += 10;
                    }
                    //if ((global_count_tim - key_press_time) >= 2000)
                    //else if ((global_count_tim - key_press_time) >= 1000)
                }
                key_state = KEY_IDLE;
            }
            break;
        default:
            key_state = KEY_IDLE;
            break;
    }
}
