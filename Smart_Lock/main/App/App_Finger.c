#include "App_Finger.h"

uint16_t min_id = 0;
uint16_t current_id = 0;

// 任务接收缓冲区
uint32_t task_value = 0;

/**
 * @brief 初始化指纹模块
 *
 */
void App_Finger_Init(void)
{
    Int_FPM383_Init();
}

/**
 * @brief 指纹处理函数
 *
 */
void App_Finger_Handle(void)
{
    // 接收任务通知
    xTaskNotifyWait(UINT32_MAX, UINT32_MAX, &task_value, portMAX_DELAY);
    MY_LOGI("tak_value: %ld\r\n", task_value);

    // 未进入休眠需要关闭中断
    gpio_intr_disable(FPM383_INT_PIN);
    switch (task_value)
    {
    case 1: // 添加指纹
        // MY_LOGI("App1\r\n");
        sayWithoutInt();
        sayPlaceFinger(); // 请放置指纹
        vTaskDelay(3000);

        // uint16_t min_id = 0;
        // 获取最小ID
        Int_FPM383_GetMinID(&min_id);
        MY_LOGE("min_id = %d\r\n", min_id);
        // 自动注册
        if (Int_FPM383_AutoRegister(min_id) == Com_OK)
        {
            sayWithoutInt();
            sayFingerprintAddSucc(); // 指纹添加成功
        }

        break;
    case 2: // 删除指纹
        // MY_LOGI("App2\r\n");
        sayWithoutInt();
        sayPlaceFinger(); // 请放置手指
        vTaskDelay(3000);

        // 检查非录入指纹
        if (Int_FPM383_NoFinger() == Com_OK)
        {
            sayWithoutInt();
            sayInvalid();
            // 进入休眠
            Int_FPM383_Sleep();
            // 进入休眠需要开启中断
            gpio_intr_enable(FPM383_INT_PIN);
            return;
        }

        // uint16_t current_id = 0;
        // 获取当前指纹ID
        Int_FPM383_GetFingerID(&current_id);
        MY_LOGI("current_id = %d\r\n", current_id);

        // 删除指纹
        if (Int_FPM383_DeleteFinger(current_id) == Com_OK)
        {
            sayWithoutInt();
            sayDelSucc(); // 删除成功
        }
        else
        {
            sayWithoutInt();
            sayDelFail(); // 删除失败
        }

        break;
    case 3: // 删除所有指纹
        // MY_LOGI("App3\r\n");
        if (Int_FPM383_DeleteFingerAll() == Com_OK)
        {
            sayWithoutInt();
            sayDelSucc(); // 删除成功
        }
        else
        {
            sayWithoutInt();
            sayDelFail(); // 删除失败
        }

        break;
    case 4: // 指纹验证

        // MY_LOGI("App4\r\n");
        // 自动验证指纹
        if (Int_FPM383_AutoIdentify() == Com_OK)
        {
            sayWithoutInt();
            sayFingerprintVerifySucc(); // 指纹验证成功
            sayWithoutInt();
            sayDoorOpen(); // 门已打开

            Int_BDR6120_OpenLock();
        }
        else
        {
            sayWithoutInt();
            sayFingerprintVerifyFail(); // 指纹验证失败
            sayWithoutInt();
            sayRetry(); // 请重试
        }

        break;

    default:
        break;
    }
    // 进入休眠
    Int_FPM383_Sleep();
    // 进入休眠需要开启中断
    gpio_intr_enable(FPM383_INT_PIN);
}
