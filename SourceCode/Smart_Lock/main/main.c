#include <stdio.h>
#include "Com_Debug.h"
#include "App_Key.h"
#include "Int_FPM383.h"
#include "App_Finger.h"
#include "App_Communication.h"

// 按键处理的任务配置
#define KEY_SCAN_TASK_NAME "key_scan_task"
#define KEY_SCAN_TASK_STACK 4096
#define KEY_SCAN_TASK_PRIORITY 10
TaskHandle_t key_scan_handle;
void key_scan_task(void *pvParameters);
#define KEY_SCAN_EXEC_CYCLE 10

// 指纹处理的任务配置
#define FINGER_SCAN_TASK_NAME "finger_scan_task"
#define FINGER_SCAN_TASK_STACK 2048
#define FINGER_SCAN_TASK_PRIORITY 10
TaskHandle_t finger_scan_handle;
void finger_scan_task(void *pvParameters);

void app_main(void)
{
    App_Communication_Init();
    // 创建按键处理任务
    xTaskCreate(
        key_scan_task,
        KEY_SCAN_TASK_NAME, /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
        KEY_SCAN_TASK_STACK,
        NULL,
        KEY_SCAN_TASK_PRIORITY,
        &key_scan_handle);

    xTaskCreate(
        finger_scan_task,
        FINGER_SCAN_TASK_NAME, /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
        FINGER_SCAN_TASK_STACK,
        NULL,
        FINGER_SCAN_TASK_PRIORITY,
        &finger_scan_handle);

    // Int_FPM383_Init();

    // while (1)
    // {
    //     // 指纹模块是否按下
    //     if (IsFinger == 1)
    //     {
    //         MY_LOGI("Finger is passed...\r\n");
    //         // 清除标志位
    //         IsFinger = 0;
    //         // 未进入休眠需要关闭中断
    //         gpio_intr_disable(FPM383_INT_PIN);
    //         // 进入休眠
    //         Int_FPM383_Sleep();
    //     }

    //     vTaskDelay(100);
    // }
}

// 按键任务函数
void key_scan_task(void *pvParameters)
{
    uint8_t key[32] = {0};
    App_Key_Init();
    while (1)
    {
        // 如果等于 Com_OK ,需要调用 # 处理函数
        switch (App_Key_ReadKey(key))
        {
        case Com_OK:
            // 调用 # 处理函数
            App_Key_HandleKey(key);
            // MY_LOGI("确认!!!");
            break;
        case Com_ERROR:
            sayWithoutInt();
            sayInvalid(); // 操作无效
            // MY_LOGE("取消!!!");
            break;
        case Com_TIMEOUT:
            // MY_LOGW("超时!!!");
            break;
        default:
            break;
        }

        // 当前处理结束把key清0
        memset(key, 0, sizeof(key));
        vTaskDelay(10);
    }
}

// 指纹任务函数
void finger_scan_task(void *pvParameters)
{
    // 初始化
    App_Finger_Init();
    while (1)
    { 
        // 指纹处理函数
        App_Finger_Handle();
        vTaskDelay(100);
    }
}
