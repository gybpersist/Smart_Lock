#ifndef __APP_COMMUNICATION_H__
#define __APP_COMMUNICATION_H__

#include "Dri_BLE.h"
#include "Int_BDR6120.h"
#include "Int_WTN6170.h"
#include "Dri_NVS.h"
#include "Com_Debug.h"

#include "esp_system.h"
#include "esp_mac.h"
#include "esp_ota_ops.h"
#include "esp_http_client.h"
#include "esp_https_ota.h"
#include "esp_crt_bundle.h"
#include "esp_log.h"
#include "Dri/Dri_Wifi.h"
#include "Com_Debug.h"
#include "App_Key.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_event.h"
#include "string.h" 
#include "nvs.h"
#include "nvs_flash.h"
#include <sys/socket.h>
#include "esp_wifi.h"

/**
 * @brief 蓝牙初始化
 * 
 */
void App_Communication_Init(void);

/**
 * @brief 定义esp32收到手机数据时的回调弱函数
 */
void App_Communication_ReceDataCb(uint8_t *data, uint16_t datalen);

/**
 * @brief 执行OTA
 */
void App_Communication_OTA(void);

#endif /* __APP_COMMUNICATION_H__ */
