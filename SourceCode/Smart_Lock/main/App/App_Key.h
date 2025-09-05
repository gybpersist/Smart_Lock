#ifndef __APP_KEY_H__
#define __APP_KEY_H__

#include "Com_Define.h"
#include "string.h"
#include "Int_BDR6120.h"    //���ģ��
#include "Int_SC12B.h"      //����ģ��
#include "Int_WS2812.h"     //�ƴ�ģ��
#include "Int_WTN6170.h"    //����ģ��
#include "Dri_NVS.h"        //�洢ģ��
#include "esp_task.h"
#include "App_Communication.h"

#define AddFinger 1

/**
 * @brief ���������ʼ��
 * 
 */
void App_Key_Init(void);

/**
 * @brief ѭ����ȡ�ĸ���������
 * 
 * @param key �����ȡ���İ������� (�ַ�����ʽ)
 * @return Com_Status ����״̬
 */
Com_Status App_Key_ReadKey(uint8_t *key);

/**
 * @brief ������#��Ĵ���
 * 
 * @param key ���밴�µİ�������
 * @note ����key����ĳ���������Ӧ���ж�
 */
void App_Key_HandleKey(uint8_t *key);

#endif /* __APP_KEY_H__ */
