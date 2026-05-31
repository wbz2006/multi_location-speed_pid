#ifndef __BSP_ENCODER_H
#define __BSP_ENCODER_H

#include "stm32f4xx_hal.h"
#include "tim.h"
#include <stdint.h>

#define ENCODER_PPR 11          // 基础脉冲数
#define ENCODER_REDUCTION_RATIO 21.3 // 减速比
#define ENCODER_MULTIPLY 4      // STM32 4倍频计数
#define SPEED_MEASURE_PERIOD 40 // 测速周期，单位ms

// 编码器句柄结构体
typedef struct
{
    TIM_HandleTypeDef *htim;
    int16_t count;      // 当前计数值
    int16_t last_count; // 上一次计数值
    int16_t delta_count; // 增量计数值
    int32_t total_count;// 累计计数值
    float speed_rpm;   // 速度，单位RPM
} Encoder_HandleTypeDef;

// 全局编码器句柄声明
extern Encoder_HandleTypeDef hencoderA;
//extern Encoder_HandleTypeDef hencoderB;
//extern Encoder_HandleTypeDef hencoderC;
//extern Encoder_HandleTypeDef hencoderD;

// 函数接口
void Encoder_Init(Encoder_HandleTypeDef *hencoder);
int16_t Encoder_GetDeltaCount(Encoder_HandleTypeDef *hencoder);
float Encoder_GetSpeed_RPM(Encoder_HandleTypeDef *hencoder);


#endif /* __BSP_ENCODER_H */

