#include "App_Communication.h"

/**
 * @brief 蓝牙初始化
 *
 */
void App_Communication_Init(void)
{
    Dri_BLE_Init();
}

/**
 * @brief 定义esp32收到手机数据时的回调弱函数
 */
void App_Communication_ReceDataCb(uint8_t *data, uint16_t datalen)
{
    char flag = data[0];
    uint8_t password[100] = {0};
    memcpy(password, &data[2], datalen - 2);

    switch (flag)
    {
    case '0': // 输入密码开门
        if (Dri_NVS_FindKey((char *)password) == ESP_OK)
        {
            MY_LOGI("Open Door...\r\n");
            sayWithoutInt();
            sayPasswordVerifySucc(); // 密码验证成功
            sayWithoutInt();
            sayDoorOpen(); // 音效+门已开
            Int_BDR6120_OpenLock();
        }else{
            sayWithoutInt();
            sayPasswordVerifyFail(); // 密码验证失败
        }

        break;
    case '1': // 添加用户密码
        // 查找是否添加过密码
        if (Dri_NVS_FindKey((char *)password) == ESP_OK)
        {
            MY_LOGI("Add User Error...\r\n");
            sayWithoutInt();
            sayAddFail(); // 添加失败
            return;
        }
        // 添加成功
        if (Dri_NVS_AddKeyValue((char *)password, "0") == ESP_OK)
        {
            MY_LOGI("Add User Ok...\r\n");
            sayWithoutInt();
            sayAddSucc(); // 添加成功
        }

        break;
    case '2': // 删除用户密码
        // 查找是否添加过密码
        if (Dri_NVS_FindKey((char *)password) != ESP_OK)
        {
            MY_LOGI("Delete User Error...\r\n");
            sayWithoutInt();
            sayDelInvalid(); // 删除失败
            return;
        }
        // 添加成功
        if (Dri_NVS_DeleteKeyValue((char *)password) == ESP_OK)
        {
            MY_LOGI("Add User Ok...\r\n");
            sayWithoutInt();
            sayDelSucc(); // 删除成功
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
/// 处理一系列的HTTP事件
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
 * @description: 下载ota用的二进制文件
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

    /* 1. 获取分区信息 */
    get_sha256_of_partitions();

    /* 2. 初始化网络 */
    esp_netif_init();

    /* 3. 创建和初始化默认事件循环 */
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
 * @description: 实现OTA操作
 * @return {*}
 */
void App_Communication_OTA(void)
{
    /* 1. 连接wifi */
    Dri_WIFI_Init();

    /* 2. ota升级   使用python启动个本地http-server 命令
          C:\esp\tools\idf-python\3.11.2\python -m http.server 8080
    */
    printf("ota start...\r\n");
    App_Communication_OTADownloadBin();
    printf("ota end...\r\n");

    /* 3. 关闭wifi */
    esp_wifi_stop();

    /* 4. 重启esp32 */
    esp_restart();
}
