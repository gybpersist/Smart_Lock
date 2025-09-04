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
 * @brief ��ʼ��ָ��ģ��
 * 
 */
void App_Finger_Init(void);

/**
 * @brief ָ�ƴ�����
 * 
 */
void App_Finger_Handle(void);

#endif /* __APP_FINGER_H__ */
