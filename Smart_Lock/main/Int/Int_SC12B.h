#ifndef __INT_SC12B_H__
#define __INT_SC12B_H__

#include "driver/gpio.h"
#include "driver/i2c.h"
#include "esp_task.h"

#define SC12B_SCL_PIN GPIO_NUM_1
#define SC12B_SDA_PIN GPIO_NUM_2
#define SC12B_INT_PIN GPIO_NUM_0

#define SC12B_DEV_ADDR 0x42

// �����¼�:�����ĸ�����
typedef enum{
    KEY_0 = 0,  // 0
    KEY_1,      // 1
    KEY_2,      // 2
    KEY_3,      // 3
    KEY_4,      // 4
    KEY_5,      // 5
    KEY_6,      // 6
    KEY_7,      // 7
    KEY_8,      // 8
    KEY_9,      // 9
    KEY_HASH,   // #
    KEY_M,      // M 
    KEY_NONE    // �ް�������
} Key_Event;

/**
 * @brief ����SC12B ��ʼ��
 * 
 */
void Int_SC12B_Init(void);

/**
 * @brief ��ȡ���°�����ֵ
 * 
 * @return Key_Event �ĸ���������
 */
Key_Event Int_SC12B_GetKey(void);

/**
 * @brief ʹ���ж��Ż���ȡ����
 * 
 * @return Key_Event �ĸ���������
 */
Key_Event Int_SC12B_TouchKey(void);

#endif /* __INT_SC12B_H__ */
