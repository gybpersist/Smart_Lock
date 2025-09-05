#ifndef __APP_KEY_H__
#define __APP_KEY_H__

#include "Com_Define.h"
#include "string.h"
#include "Int_BDR6120.h"    //电机模块
#include "Int_SC12B.h"      //按键模块
#include "Int_WS2812.h"     //灯带模块
#include "Int_WTN6170.h"    //语音模块
#include "Dri_NVS.h"        //存储模块
#include "esp_task.h"
#include "App_Communication.h"

#define AddFinger 1

/**
 * @brief 按键处理初始化
 * 
 */
void App_Key_Init(void);

/**
 * @brief 循环读取哪个按键按下
 * 
 * @param key 保存读取到的按键数组 (字符串格式)
 * @return Com_Status 操作状态
 */
Com_Status App_Key_ReadKey(uint8_t *key);

/**
 * @brief 处理按下#后的处理
 * 
 * @param key 传入按下的按键数组
 * @note 根据key数组的长度作出响应的判断
 */
void App_Key_HandleKey(uint8_t *key);

#endif /* __APP_KEY_H__ */
