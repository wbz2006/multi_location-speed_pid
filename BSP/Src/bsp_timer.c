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
    static uint16_t count = 0;
    if (htim->Instance == TIM12)
    {
        global_count_tim ++;
        count = global_count_tim;
    }
    if (count % SPEED_MEASURE_PERIOD == 0)
    {
        count = 0;
        pid_control_flag = 1;
    }
}