//
// Created by wbz on 2026/5/18.
//

#ifndef AT8236_DRIVER_BSP_USER_KEY_H
#define AT8236_DRIVER_BSP_USER_KEY_H
#include <sys/types.h>

extern uint64_t global_count_tim;\
extern float Target;

typedef enum
{
    KEY_IDLE,       // 空闲
    KEY_DEBOUNCE,   // 消抖
    KEY_PRESSING,   // 按下中，计时
    KEY_RELEASE     // 释放判断
} KeyState;

extern KeyState key_state;


void Key_Process(void);

#endif //AT8236_DRIVER_BSP_USER_KEY_H
