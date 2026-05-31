#include "bsp_motor.h"
#include "tim.h"
//motorA句柄
Motor_HandleTypeDef hmotorA = {
    .htim_pwm1 = &htim10,
    .ch_pwm1 = TIM_CHANNEL_1,
    .htim_pwm2 = &htim11,
    .ch_pwm2 = TIM_CHANNEL_1
};

//motorB句柄
/*
Motor_HandleTypeDef hmotorB = {
    .htim_pwm1 = &htim9,
    .ch_pwm1 = TIM_CHANNEL_1,
    .htim_pwm2 = &htim9,
    .ch_pwm2 = TIM_CHANNEL_2
};
*/

/**
 * @brief  电机初始化
 */
void Motor_Init(Motor_HandleTypeDef *hmotor)
{
    // 启动两路PWM输出
    HAL_TIM_PWM_Start(hmotor->htim_pwm1, hmotor->ch_pwm1);
    HAL_TIM_PWM_Start(hmotor->htim_pwm2, hmotor->ch_pwm2);
    
    // 初始状态：刹车
    Motor_Brake(hmotor);
}

/**
 * @brief  设置电机速度（双PWM模式）
 * @param  speed -100~100
 */
void Motor_SetSpeed(Motor_HandleTypeDef *hmotor, float speed)
{
    // 速度限幅
    if (speed > 100)
    {
        speed = 100;
    }
    if (speed < -100)
    {
        speed = -100;
    }
    
    hmotor->speed = speed;

    int16_t pwm_out;

    if (speed > 0)
    {
        // 正转：PWM1输出PWM，PWM2输出0
        pwm_out = MOTOR_FORWARD_DEAD_ZONE + (speed * (MOTOR_MAX_PWM - MOTOR_FORWARD_DEAD_ZONE)) / 100;
        __HAL_TIM_SET_COMPARE(hmotor->htim_pwm1, hmotor->ch_pwm1, pwm_out);
        __HAL_TIM_SET_COMPARE(hmotor->htim_pwm2, hmotor->ch_pwm2, 0);
        hmotor->state = MOTOR_FORWARD;
    }
    else if (speed < 0)
    {
        // 反转：PWM1输出0，PWM2输出PWM
        pwm_out = MOTOR_REVERSE_DEAD_ZONE + ((-speed) * (MOTOR_MAX_PWM - MOTOR_REVERSE_DEAD_ZONE)) / 100;
        __HAL_TIM_SET_COMPARE(hmotor->htim_pwm1, hmotor->ch_pwm1, 0);
        __HAL_TIM_SET_COMPARE(hmotor->htim_pwm2, hmotor->ch_pwm2, pwm_out);
        hmotor->state = MOTOR_REVERSE;
    }
    else
    {
        // 速度为0时刹车
        Motor_Brake(hmotor);
    }
}

/**
 * @brief  电机刹车
 */
void Motor_Brake(Motor_HandleTypeDef *hmotor)
{
    __HAL_TIM_SET_COMPARE(hmotor->htim_pwm1, hmotor->ch_pwm1, 1000);
    __HAL_TIM_SET_COMPARE(hmotor->htim_pwm2, hmotor->ch_pwm2, 1000);
    
    hmotor->state = MOTOR_BRAKE;
    hmotor->speed = 0;
}

/**
 * @brief  电机滑行
 */
void Motor_Coast(Motor_HandleTypeDef *hmotor)
{
    __HAL_TIM_SET_COMPARE(hmotor->htim_pwm1, hmotor->ch_pwm1, 0);
    __HAL_TIM_SET_COMPARE(hmotor->htim_pwm2, hmotor->ch_pwm2, 0);
    
    hmotor->state = MOTOR_STOP;
    hmotor->speed = 0;
}
