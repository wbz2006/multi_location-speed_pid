#ifndef APP_MOTOR_H
#define APP_MOTOR_H

#include "stm32f4xx_hal.h"
#include <sys/types.h>

extern uint8_t inner_pid_control_flag;
extern uint8_t outer_pid_control_flag;

typedef struct 
{
	float Target;
	float Actual;
	float Out;
	
	float Kp;
	float Ki;
	float Kd;
	
	float Error0;
	float Error1;
	float ErrorInt;
	
	float OutMax;
	float OutMin;
} PID_t;



void Motor_PID_Init(void);
void Motor_PID_Task(void);
void Motor_PID_Data_Transmit(void);

#endif /* APP_MOTOR_H */