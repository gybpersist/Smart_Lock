#ifndef __APP_FINGER_H__
#define __APP_FINGER_H__

#include "driver/gpio.h"
#include "esp_task.h"
#include "Com_Define.h"
#include "Com_Debug.h"
#include "Int_FPM383.h"
#include "Int_WTN6170.h"
#include "Int_BDR6120.h"

/**
 * @brief 初始化指纹模块
 * 
 */
void App_Finger_Init(void);

/**
 * @brief 指纹处理函数
 * 
 */
void App_Finger_Handle(void);

#endif /* __APP_FINGER_H__ */
