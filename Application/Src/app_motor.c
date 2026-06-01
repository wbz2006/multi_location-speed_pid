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






