#ifndef __DRI_NVS_H__
#define __DRI_NVS_H__

#include "driver/gpio.h"
#include "esp_task.h"
#include "nvs_flash.h"
#include "nvs.h"

/**
 * @brief 初始化 NVS
 * 
 */
void Dri_NVS_Init(void);

/**
 * @brief 向NVS中添加一对键值对
 * 
 * @param key 键的名称
 * @param value 键值
 * @return esp_err_t 错误信息
 */
esp_err_t Dri_NVS_AddKeyValue(char *key,char *value);

/**
 * @brief 查找具有给定键名称的键值对
 * 
 * @param key 键的名称
 * @return esp_err_t 错误信息
 */
esp_err_t Dri_NVS_FindKey(char *key);

/**
 * @brief 获取给定键的字符串值
 *
 * @param key 键的名称
 * @param out_value 获取到的键值
 * @param length out_value为零，将设置为保存该值所需的长度。out_value不为零，将设置为写入值的实际长度。
 * @return esp_err_t
 */
esp_err_t Dri_NVS_GetKey(char *key, char *out_value, size_t *length);

/**
 * @brief 擦除具有给定键名称的键值对
 * 
 * @param key 键的名称
 * @return esp_err_t 错误信息
 */
esp_err_t  Dri_NVS_DeleteKeyValue(char *key);

/**
 * @brief 擦除命名空间中的所有键值对
 * 
 * @return esp_err_t 错误信息
 */
esp_err_t Dri_NVS_DeleteKeyValueAll(void);

#endif /* __DRI_NVS_H__ */
