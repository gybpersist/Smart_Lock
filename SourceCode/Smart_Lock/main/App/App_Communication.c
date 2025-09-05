#include "App_Communication.h"

/**
 * @brief ������ʼ��
 *
 */
void App_Communication_Init(void)
{
    Dri_BLE_Init();
}

/**
 * @brief ����esp32�յ��ֻ�����ʱ�Ļص�������
 */
void App_Communication_ReceDataCb(uint8_t *data, uint16_t datalen)
{
    char flag = data[0];
    uint8_t password[100] = {0};
    memcpy(password, &data[2], datalen - 2);

    switch (flag)
    {
    case '0': // �������뿪��
        if (Dri_NVS_FindKey((char *)password) == ESP_OK)
        {
            MY_LOGI("Open Door...\r\n");
            sayWithoutInt();
            sayPasswordVerifySucc(); // ������֤�ɹ�
            sayWithoutInt();
            sayDoorOpen(); // ��Ч+���ѿ�
            Int_BDR6120_OpenLock();
        }else{
            sayWithoutInt();
            sayPasswordVerifyFail(); // ������֤ʧ��
        }

        break;
    case '1': // ����û�����
        // �����Ƿ���ӹ�����
        if (Dri_NVS_FindKey((char *)password) == ESP_OK)
        {
            MY_LOGI("Add User Error...\r\n");
            sayWithoutInt();
            sayAddFail(); // ���ʧ��
            return;
        }
        // ��ӳɹ�
        if (Dri_NVS_AddKeyValue((char *)password, "0") == ESP_OK)
        {
            MY_LOGI("Add User Ok...\r\n");
            sayWithoutInt();
            sayAddSucc(); // ��ӳɹ�
        }

        break;
    case '2': // ɾ���û�����
        // �����Ƿ���ӹ�����
        if (Dri_NVS_FindKey((char *)password) != ESP_OK)
        {
            MY_LOGI("Delete User Error...\r\n");
            sayWithoutInt();
            sayDelInvalid(); // ɾ��ʧ��
            return;
        }
        // ��ӳɹ�
        if (Dri_NVS_DeleteKeyValue((char *)password) == ESP_OK)
        {
            MY_LOGI("Add User Ok...\r\n");
            sayWithoutInt();
            sayDelSucc(); // ɾ���ɹ�
        }
        break;

    default:
        break;
    }
}

#define HASH_LEN 32
static void get_sha256_of_partitions(void)
{
    uint8_t sha_256[HASH_LEN] = {0};
    esp_partition_t partition;

    // get sha256 digest for bootloader
    partition.address = ESP_BOOTLOADER_OFFSET;
    partition.size = ESP_PARTITION_TABLE_OFFSET;
    partition.type = ESP_PARTITION_TYPE_APP;
    esp_partition_get_sha256(&partition, sha_256);

    // get sha256 digest for running partition
    esp_partition_get_sha256(esp_ota_get_running_partition(), sha_256);
}

#define TAG "ota"
/// ����һϵ�е�HTTP�¼�
esp_err_t _http_event_handler(esp_http_client_event_t *evt)
{
    switch (evt->event_id)
    {
    case HTTP_EVENT_ERROR:
        ESP_LOGD(TAG, "HTTP_EVENT_ERROR");
        break;
    case HTTP_EVENT_ON_CONNECTED:
        ESP_LOGD(TAG, "HTTP_EVENT_ON_CONNECTED");
        break;
    case HTTP_EVENT_HEADER_SENT:
        ESP_LOGD(TAG, "HTTP_EVENT_HEADER_SENT");
        break;
    case HTTP_EVENT_ON_HEADER:
        ESP_LOGD(TAG, "HTTP_EVENT_ON_HEADER, key=%s, value=%s", evt->header_key, evt->header_value);
        break;
    case HTTP_EVENT_ON_DATA:
        ESP_LOGD(TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
        break;
    case HTTP_EVENT_ON_FINISH:
        ESP_LOGD(TAG, "HTTP_EVENT_ON_FINISH");
        break;
    case HTTP_EVENT_DISCONNECTED:
        ESP_LOGD(TAG, "HTTP_EVENT_DISCONNECTED");
        break;
    case HTTP_EVENT_REDIRECT:
        ESP_LOGD(TAG, "HTTP_EVENT_REDIRECT");
        break;
    }
    return ESP_OK;
}

/**
 * @description: ����ota�õĶ������ļ�
 * @return {*}
 */
static void App_Communication_OTADownloadBin(void)
{
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        nvs_flash_erase();
        err = nvs_flash_init();
    }

    /* 1. ��ȡ������Ϣ */
    get_sha256_of_partitions();

    /* 2. ��ʼ������ */
    esp_netif_init();

    /* 3. �����ͳ�ʼ��Ĭ���¼�ѭ�� */
    esp_event_loop_create_default();

    esp_http_client_config_t config = {
        .url = "http://192.168.243.229:8080/01_Test.bin",
        .crt_bundle_attach = esp_crt_bundle_attach,
        .event_handler = NULL,
        .keep_alive_enable = true,
    };

    esp_https_ota_config_t ota_config = {
        .http_config = &config,
    };

    esp_https_ota(&ota_config);
}

/**
 * @description: ʵ��OTA����
 * @return {*}
 */
void App_Communication_OTA(void)
{
    /* 1. ����wifi */
    Dri_WIFI_Init();

    /* 2. ota����   ʹ��python����������http-server ����
          C:\esp\tools\idf-python\3.11.2\python -m http.server 8080
    */
    printf("ota start...\r\n");
    App_Communication_OTADownloadBin();
    printf("ota end...\r\n");

    /* 3. �ر�wifi */
    esp_wifi_stop();

    /* 4. ����esp32 */
    esp_restart();
}
