#ifndef BSP_MOTOR_H
#define BSP_MOTOR_H

#include "stm32f4xx_hal.h"

//电机死区值
#define MOTOR_FORWARD_DEAD_ZONE 522
#define MOTOR_REVERSE_DEAD_ZONE 525
// PWM最大输出值
#define MOTOR_MAX_PWM 1000

// 电机状态枚举
typedef enum
{
    MOTOR_STOP = 0,
    MOTOR_FORWARD,
    MOTOR_REVERSE,
    MOTOR_BRAKE
} Motor_StateTypeDef;

// 电机句柄结构体（双PWM模式）
typedef struct
{
    // PWM硬件资源
    TIM_HandleTypeDef *htim_pwm1;
    uint32_t ch_pwm1;
    TIM_HandleTypeDef *htim_pwm2;
    uint32_t ch_pwm2;
    
    // 电机状态
    Motor_StateTypeDef state;
    float speed;
} Motor_HandleTypeDef;

// 全局电机句柄声明
extern Motor_HandleTypeDef hmotorA;
//extern Motor_HandleTypeDef hmotorB;



// 函数接口
void Motor_Init(Motor_HandleTypeDef *hmotor);
void Motor_SetSpeed(Motor_HandleTypeDef *hmotor, float speed);
void Motor_Brake(Motor_HandleTypeDef *hmotor);
void Motor_Coast(Motor_HandleTypeDef *hmotor);



#endif /* BSP_MOTOR_H */
