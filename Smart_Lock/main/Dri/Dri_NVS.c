#include "Dri_NVS.h"

nvs_handle_t my_handle;

/**
 * @brief 初始化 NVS
 *
 */
void Dri_NVS_Init(void)
{
    // 初始化 NVS
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        // NVS 分区被截断，需要擦除
        // 重试nvs_flash_init
        nvs_flash_erase();
        nvs_flash_init();
    }   
    nvs_open("lock", NVS_READWRITE, &my_handle);
}

/**
 * @brief 向NVS中添加一对键值对
 *
 * @param key 键值的名称
 * @param value 键值
 * @return esp_err_t 错误信息
 */
esp_err_t Dri_NVS_AddKeyValue(char *key, char *value)
{
    // 为给定键设置字符串
    return nvs_set_str(my_handle, key, value);
}

/**
 * @brief 查找具有给定键名称的键值对
 *
 * @param key 键的名称
 * @return esp_err_t 错误信息
 */
esp_err_t Dri_NVS_FindKey(char *key)
{
    // 查找具有给定键名称的键值对
    return nvs_find_key(my_handle, key, (nvs_type_t *)NULL);
}

/**
 * @brief 获取给定键的字符串值
 *
 * @param key 键的名称
 * @param out_value 获取到的键值
 * @param length out_value为零，将设置为保存该值所需的长度。out_value不为零，将设置为写入值的实际长度。
 * @return esp_err_t 错误信息
 */
esp_err_t Dri_NVS_GetKey(char *key, char *out_value, size_t *length)
{
    // 获取给定键的字符串值
    return nvs_get_str(my_handle, key, out_value, length);
}

/**
 * @brief 擦除具有给定键名称的键值对
 * 
 * @param key 键的名称
 * @return esp_err_t 错误信息
 */
esp_err_t  Dri_NVS_DeleteKeyValue(char *key)
{
    // 擦除具有给定键名称的键值对
    return nvs_erase_key(my_handle,key);
}

/**
 * @brief 擦除命名空间中的所有键值对
 * 
 * @return esp_err_t 错误信息
 */
esp_err_t Dri_NVS_DeleteKeyValueAll(void)
{
    // 擦除命名空间中的所有键值对
    return nvs_erase_all(my_handle);
}
