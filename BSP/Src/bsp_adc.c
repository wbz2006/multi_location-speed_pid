#include "bsp_adc.h"
#include "adc.h"

int16_t AD_GetValue(void)
{
    int32_t sum = 0;
    HAL_ADC_Start(&hadc1);
    for(uint8_t i = 0; i < 10; i++)
    {
        HAL_ADC_PollForConversion(&hadc1, 100);
        sum += HAL_ADC_GetValue(&hadc1);
        HAL_ADC_Start(&hadc1);
    }

    int16_t adc_avg = sum / 10;
    adc_avg = ((float) adc_avg / 4095.0f) * 100.0f;
    HAL_ADC_Stop(&hadc1);
    adc_avg = (int16_t) adc_avg*2;

    return adc_avg;
}