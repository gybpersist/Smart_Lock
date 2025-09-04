#include "Dri_NVS.h"

nvs_handle_t my_handle;

/**
 * @brief ��ʼ�� NVS
 *
 */
void Dri_NVS_Init(void)
{
    // ��ʼ�� NVS
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        // NVS �������ضϣ���Ҫ����
        // ����nvs_flash_init
        nvs_flash_erase();
        nvs_flash_init();
    }   
    nvs_open("lock", NVS_READWRITE, &my_handle);
}

/**
 * @brief ��NVS�����һ�Լ�ֵ��
 *
 * @param key ��ֵ������
 * @param value ��ֵ
 * @return esp_err_t ������Ϣ
 */
esp_err_t Dri_NVS_AddKeyValue(char *key, char *value)
{
    // Ϊ�����������ַ���
    return nvs_set_str(my_handle, key, value);
}

/**
 * @brief ���Ҿ��и��������Ƶļ�ֵ��
 *
 * @param key ��������
 * @return esp_err_t ������Ϣ
 */
esp_err_t Dri_NVS_FindKey(char *key)
{
    // ���Ҿ��и��������Ƶļ�ֵ��
    return nvs_find_key(my_handle, key, (nvs_type_t *)NULL);
}

/**
 * @brief ��ȡ���������ַ���ֵ
 *
 * @param key ��������
 * @param out_value ��ȡ���ļ�ֵ
 * @param length out_valueΪ�㣬������Ϊ�����ֵ����ĳ��ȡ�out_value��Ϊ�㣬������Ϊд��ֵ��ʵ�ʳ��ȡ�
 * @return esp_err_t ������Ϣ
 */
esp_err_t Dri_NVS_GetKey(char *key, char *out_value, size_t *length)
{
    // ��ȡ���������ַ���ֵ
    return nvs_get_str(my_handle, key, out_value, length);
}

/**
 * @brief �������и��������Ƶļ�ֵ��
 * 
 * @param key ��������
 * @return esp_err_t ������Ϣ
 */
esp_err_t  Dri_NVS_DeleteKeyValue(char *key)
{
    // �������и��������Ƶļ�ֵ��
    return nvs_erase_key(my_handle,key);
}

/**
 * @brief ���������ռ��е����м�ֵ��
 * 
 * @return esp_err_t ������Ϣ
 */
esp_err_t Dri_NVS_DeleteKeyValueAll(void)
{
    // ���������ռ��е����м�ֵ��
    return nvs_erase_all(my_handle);
}
