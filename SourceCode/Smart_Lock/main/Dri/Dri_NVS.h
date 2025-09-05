#ifndef __DRI_NVS_H__
#define __DRI_NVS_H__

#include "driver/gpio.h"
#include "esp_task.h"
#include "nvs_flash.h"
#include "nvs.h"

/**
 * @brief ��ʼ�� NVS
 * 
 */
void Dri_NVS_Init(void);

/**
 * @brief ��NVS�����һ�Լ�ֵ��
 * 
 * @param key ��������
 * @param value ��ֵ
 * @return esp_err_t ������Ϣ
 */
esp_err_t Dri_NVS_AddKeyValue(char *key,char *value);

/**
 * @brief ���Ҿ��и��������Ƶļ�ֵ��
 * 
 * @param key ��������
 * @return esp_err_t ������Ϣ
 */
esp_err_t Dri_NVS_FindKey(char *key);

/**
 * @brief ��ȡ���������ַ���ֵ
 *
 * @param key ��������
 * @param out_value ��ȡ���ļ�ֵ
 * @param length out_valueΪ�㣬������Ϊ�����ֵ����ĳ��ȡ�out_value��Ϊ�㣬������Ϊд��ֵ��ʵ�ʳ��ȡ�
 * @return esp_err_t
 */
esp_err_t Dri_NVS_GetKey(char *key, char *out_value, size_t *length);

/**
 * @brief �������и��������Ƶļ�ֵ��
 * 
 * @param key ��������
 * @return esp_err_t ������Ϣ
 */
esp_err_t  Dri_NVS_DeleteKeyValue(char *key);

/**
 * @brief ���������ռ��е����м�ֵ��
 * 
 * @return esp_err_t ������Ϣ
 */
esp_err_t Dri_NVS_DeleteKeyValueAll(void);

#endif /* __DRI_NVS_H__ */
