//
// Created by wbz on 2026/5/18.
//
#include "bsp_timer.h"
#include "bsp_encoder.h"
#include "bsp_motor.h"
#include "tim.h"

// 定时器更新中断回调函数
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    static uint16_t count1 = 0;
    static uint16_t count2 = 0;
    if (htim->Instance == TIM12)
    {
        global_count_tim ++;
        count1 = global_count_tim;
        count2 = global_count_tim;
    }
    if (count1 % SPEED_MEASURE_PERIOD == 0)
    {
        count1 = 0;
        inner_pid_control_flag = 1;
    }
    if (count2 % LOCATION_MEASURE_PERIOD == 0)
    {
        count2 = 0;
        outer_pid_control_flag = 1;
    }
}