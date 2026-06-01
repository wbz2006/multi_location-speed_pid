#include "app_motor.h"
#include "bsp_motor.h"
#include "bsp_encoder.h"
#include "bsp_adc.h"
#include "usart.h"
#include <stdio.h>
#include <math.h>

/*pid参数*/
PID_t inner_pid = 
{
    .Kp = 0.4f,
    .Ki = 0.2f,
    .Kd = 0.0f,
    .OutMax = 100.0f,
    .OutMin = -100.0f,
};

PID_t outer_pid =
{
    .Kp = 0.2f,
    .Ki = 0.0f,
    .Kd = 0.1f,
    .OutMax = 100.0f,
    .OutMin = -100.0f,
};

/*前馈参数*/
float Kf_v = 0.9f;  // 速度前馈系数
float Kf_a = 0.05f; // 加速度前馈系数



uint8_t inner_pid_control_flag = 0;
uint8_t outer_pid_control_flag = 0;

void Motor_PID_Init(void)
{
    /*外设初始化*/
    Motor_Init(&hmotorA);
    Encoder_Init(&hencoderA);
}

void Motor_PID_calc(PID_t *pid)
{
    pid->Error1 = pid->Error0;			
	pid->Error0 = pid->Target - pid->Actual;
    
    if (fabs(pid->Error0) < 10)
    {
        pid->Out = 0;
    }

    if (pid->Ki != 0)				
	{
		pid->ErrorInt += pid->Error0;		
	}
	else						
	{
		pid->ErrorInt = 0;			
	}

	pid->Out = pid->Kp * pid->Error0 + pid->Ki * pid->ErrorInt + pid->Kd * (pid->Error0 - pid->Error1);
    
    
    if (pid->Out > 100) {pid->Out = 100;}
    if (pid->Out < -100) {pid->Out = -100;}

}

void Motor_PID_Task(void)
{
    static float speed = 0, location = 0;

    if (inner_pid_control_flag == 1)
    {
        inner_pid_control_flag  = 0;
        
        outer_pid.Target = AD_GetValue()* 1874 / 100 - 937;//将ADC值映射到-937~937范围
        
        speed = Encoder_GetDeltaCount(&hencoderA);

        location += speed;

        inner_pid.Actual = speed;

        Motor_PID_calc(&inner_pid);

        Motor_SetSpeed(&hmotorA, inner_pid.Out);

    }
    if (outer_pid_control_flag == 1)
    {
        outer_pid_control_flag = 0;
        
        outer_pid.Actual = location;

        Motor_PID_calc(&outer_pid);
        /*
        static float last_target_location = 0;
        static float last_speed_ff = 0;
        
        float speed_ff = (outer_pid.Target - last_target_location) / 0.04f; // 1. 计算目标速度前馈 = 目标位置变化量 / 控制周期
        // 2. 计算目标加速度前馈 = 目标速度变化量 / 控制周期
        float acc_ff = (speed_ff - last_speed_ff) / 0.04f;
        
        // 位置环PID输出 + 速度前馈 + 加速度前馈 = 速度环最终目标
        inner_pid.Target = outer_pid.Out + Kf_v * speed_ff + Kf_a * acc_ff;
        
        // 保存上一次的值，用于下次计算
        last_target_location = outer_pid.Target;
        last_speed_ff = speed_ff;
        */
        inner_pid.Target = outer_pid.Out;
        
    }
}

void Motor_PID_Data_Transmit(void)
{
    char uart_buf[40];
    int len = sprintf(uart_buf, "%d,%d,%d\r\n",(int16_t)outer_pid.Target, (int16_t)outer_pid.Actual, (int16_t)outer_pid.Out);
    HAL_UART_Transmit(&huart5, (uint8_t *)uart_buf, len, HAL_MAX_DELAY);
    HAL_Delay(SPEED_MEASURE_PERIOD);
}






