#ifndef __INT_BDR6120_H__
#define __INT_BDR6120_H__

#include "driver/gpio.h"
#include "esp_task.h"

#define BDR6120_INA_PIN    GPIO_NUM_4
#define BDR6120_INB_PIN    GPIO_NUM_5

/**
 * @brief 初始化 电机BDR6120
 * 
 */
void Int_BDR6120_Init(void);

/**
 * @brief 打开锁
 * 
 * @note 打开,保持5s关闭
 */
void Int_BDR6120_OpenLock(void);


#endif /* __INT_BDR6120_H__ */
