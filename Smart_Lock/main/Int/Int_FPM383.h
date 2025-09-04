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
#define FPM383_INT_PIN GPIO_NUM_10 // 指纹模块的输出引脚

// 定义是否是按下
extern uint8_t IsFinger;

/**
 * @brief 指纹FPM383 初始化
 *
 */
void Int_FPM383_Init(void);

/**
 * @brief 获取最小指纹的ID
 *
 * @param id 获取到的最小ID
 * @return Com_Status 获取状态
 */
Com_Status Int_FPM383_GetMinID(uint16_t *id);

/**
 * @brief 指纹FPM383 进入睡眠模式
 *
 */
void Int_FPM383_Sleep(void);

/**
 * @brief 自动注册指纹
 *
 * @param id 指纹ID
 * @return Com_Status 注册状态
 */
Com_Status Int_FPM383_AutoRegister(uint16_t id);

/**
 * @brief 自动验证指纹
 *
 * @return Com_Status 验证状态
 */
Com_Status Int_FPM383_AutoIdentify(void);

/**
 * @brief 删除指纹
 *
 * @return Com_Status
 */
Com_Status Int_FPM383_DeleteFinger(uint16_t id);

/**
 * @brief 删除所有指纹
 * 
 * @return Com_Status 状态信息
 */
Com_Status Int_FPM383_DeleteFingerAll(void);

/**
 * @brief 获取按下手指的指纹ID
 * 
 * @param id 指纹ID
 * @return Com_Status 
 */
Com_Status Int_FPM383_GetFingerID(uint16_t *id);

/**
 * @brief 没有搜索到手指
 *
 * @return Com_Status 验证状态
 */
Com_Status Int_FPM383_NoFinger(void);

#endif /* __INT_FPM383_H__ */
