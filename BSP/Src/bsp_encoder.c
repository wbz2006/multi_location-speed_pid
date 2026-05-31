#include "bsp_encoder.h"
#include <stdint.h>

Encoder_HandleTypeDef hencoderA = {.htim = &htim2};

void Encoder_Init(Encoder_HandleTypeDef *hencoder)
{
    // 启动定时器编码器模式
    HAL_TIM_Encoder_Start(hencoder->htim, TIM_CHANNEL_ALL);
    
    // 清零计数器
    __HAL_TIM_SET_COUNTER(hencoder->htim, 0);
    
    hencoder->count = 0;
    hencoder->last_count = 0;
    hencoder->delta_count = 0;
    hencoder->total_count = 0;
    hencoder->speed_rpm = 0.0f;
}

/**
 * @brief  获取编码器的增量计数值
 * @retval 增量计数值
 */
int16_t Encoder_GetDeltaCount(Encoder_HandleTypeDef *hencoder)
{
    hencoder->last_count = hencoder->count;
    hencoder->count = (int16_t)__HAL_TIM_GET_COUNTER(hencoder->htim);
    hencoder->delta_count = hencoder->count - hencoder->last_count;
    return hencoder->delta_count;
}

/**
 * @brief  获取编码器速度（每10ms调用一次）
 * @retval 速度值（脉冲/10ms）
 */
float Encoder_GetSpeed_RPM(Encoder_HandleTypeDef *hencoder)
{
    int16_t delta;
    
    hencoder->count = (int16_t)__HAL_TIM_GET_COUNTER(hencoder->htim);
    delta = hencoder->count - hencoder->last_count;
    
    // 处理计数器溢出
    if (delta > 32767/2) delta -= 65536;
    if (delta < -32768/2) delta += 65536;
    
    hencoder->last_count = hencoder->count;
    hencoder->total_count += delta;
    
    // 计算速度（RPM）
    hencoder->speed_rpm = (float)delta * 60000.0f / (ENCODER_PPR * ENCODER_REDUCTION_RATIO * ENCODER_MULTIPLY * SPEED_MEASURE_PERIOD);
    return hencoder->speed_rpm;
}
