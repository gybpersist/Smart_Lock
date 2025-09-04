#ifndef __INT_BDR6120_H__
#define __INT_BDR6120_H__

#include "driver/gpio.h"
#include "esp_task.h"

#define BDR6120_INA_PIN    GPIO_NUM_4
#define BDR6120_INB_PIN    GPIO_NUM_5

/**
 * @brief ��ʼ�� ���BDR6120
 * 
 */
void Int_BDR6120_Init(void);

/**
 * @brief ����
 * 
 * @note ��,����5s�ر�
 */
void Int_BDR6120_OpenLock(void);


#endif /* __INT_BDR6120_H__ */
