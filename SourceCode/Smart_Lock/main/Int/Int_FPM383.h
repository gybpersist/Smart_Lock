#ifndef __INT_FPM383_H__
#define __INT_FPM383_H__

#include "driver/uart.h"
#include "string.h"
#include "driver/gpio.h"
#include "esp_task.h"
#include "Com_Define.h"
#include "Com_Debug.h"

#define FPM383_TX_PIN GPIO_NUM_21
#define FPM383_RX_PIN GPIO_NUM_20
#define FPM383_INT_PIN GPIO_NUM_10 // ָ��ģ����������

// �����Ƿ��ǰ���
extern uint8_t IsFinger;

/**
 * @brief ָ��FPM383 ��ʼ��
 *
 */
void Int_FPM383_Init(void);

/**
 * @brief ��ȡ��Сָ�Ƶ�ID
 *
 * @param id ��ȡ������СID
 * @return Com_Status ��ȡ״̬
 */
Com_Status Int_FPM383_GetMinID(uint16_t *id);

/**
 * @brief ָ��FPM383 ����˯��ģʽ
 *
 */
void Int_FPM383_Sleep(void);

/**
 * @brief �Զ�ע��ָ��
 *
 * @param id ָ��ID
 * @return Com_Status ע��״̬
 */
Com_Status Int_FPM383_AutoRegister(uint16_t id);

/**
 * @brief �Զ���ָ֤��
 *
 * @return Com_Status ��֤״̬
 */
Com_Status Int_FPM383_AutoIdentify(void);

/**
 * @brief ɾ��ָ��
 *
 * @return Com_Status
 */
Com_Status Int_FPM383_DeleteFinger(uint16_t id);

/**
 * @brief ɾ������ָ��
 * 
 * @return Com_Status ״̬��Ϣ
 */
Com_Status Int_FPM383_DeleteFingerAll(void);

/**
 * @brief ��ȡ������ָ��ָ��ID
 * 
 * @param id ָ��ID
 * @return Com_Status 
 */
Com_Status Int_FPM383_GetFingerID(uint16_t *id);

/**
 * @brief û����������ָ
 *
 * @return Com_Status ��֤״̬
 */
Com_Status Int_FPM383_NoFinger(void);

#endif /* __INT_FPM383_H__ */
