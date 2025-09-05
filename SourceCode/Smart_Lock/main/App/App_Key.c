#include "App_Key.h"

// 指纹任务的句柄
extern TaskHandle_t finger_scan_handle;

// 密码缓冲区
uint8_t input1[32] = {0};
uint8_t input2[32] = {0};

/**
 * @brief 清除缓冲区
 *
 */
static void App_Key_ClearBuff(void)
{
    memset(input1, 0, 32);
    memset(input2, 0, 32);
}

/**
 * @brief 添加管理员
 *
 */
static void App_Key_AddAdmin(void)
{
    sayWithoutInt();
    sayAddAdmin(); // 添加管理员
    // 判断是否有管理员
    // 管理员已满
    esp_err_t err = Dri_NVS_FindKey("admin");
    if (err == ESP_OK)
    {
        sayWithoutInt();
        sayAdmIntull(); // 管理员已满
        return;
    }

    // 可以添加管理员 判断两次输入是否一致
    sayWithoutInt();
    sayInputAdminPassword(); // 请输入管理员密码
    // 保存第一个管理员密码
    Com_Status status1 = App_Key_ReadKey(input1); // 保存读取到的按键数组到input1
    if (status1 != Com_OK)
    {
        sayWithoutInt();
        sayOperateFail(); // 操作失败
        // 清空缓冲区
        App_Key_ClearBuff();
        return;
    }

    sayWithoutInt();
    sayInputAdminPasswordAgain(); // 请再次输入管理员密码
    // 保存第二个管理员密码
    Com_Status status2 = App_Key_ReadKey(input2); // 保存读取到的按键数组到input2
    if (status2 != Com_OK)
    {
        sayWithoutInt();
        sayOperateFail(); // 操作失败
        // 清空缓冲区
        App_Key_ClearBuff();
        return;
    }

    // 判断两次输入是否一致
    if (strcmp((char *)input1, (char *)input2) == 0)
    {
        sayAddSucc(); // 添加成功
        Dri_NVS_AddKeyValue("admin", (char *)input2);
        // 清空缓冲区
        App_Key_ClearBuff();
    }
    else
    {
        sayAddFail(); // 添加失败
    }
}

/**
 * @brief 判断管理员是否存在 及 验证管理员身份
 */
static bool App_Key_CheckAdmin(void)
{
    // 1. 判断管理员是否存在
    if (Dri_NVS_FindKey("admin") != ESP_OK)
    {
        sayWithoutInt();
        sayInvalid();
        return false;
    }
    // 2. 语音播报验证管理员 请输入管理员密码
    sayWithoutInt();
    sayCheckAdmin();
    sayWithoutInt();
    sayInputAdminPassword();

    // 3.验证管理员身份
    if (App_Key_ReadKey(input1) != Com_OK)
    {
        sayWithoutInt();
        sayInvalid();
        // 清空缓冲区
        App_Key_ClearBuff();
        return false;
    }
    // 4. 判断管理员密码是否一致
    size_t input2_size = 32;
    Dri_NVS_GetKey("admin", (char *)input2, &input2_size);

    bool is_OK = strcmp((char *)input1, (char *)input2) == 0;
    // 清空缓冲区
    App_Key_ClearBuff();
    return is_OK;
}

/**
 * @brief 删除管理员
 *
 */
static void App_Key_Deleteadmin(void)
{
    // 1. 语音播报删除管理员
    sayWithoutInt();
    sayDelAdmin();

    // 2. 验证管理员身份
    if (App_Key_CheckAdmin())
    {
        sayWithoutInt();
        sayDelSucc(); // 删除成功
        Dri_NVS_DeleteKeyValue("admin");
        // 清空缓冲区
        App_Key_ClearBuff();
    }
    else
    {
        sayWithoutInt();
        sayDelFail(); // 添加失败
        // 清空缓冲区
        App_Key_ClearBuff();
    }
}

/**
 * @brief 添加用户
 *
 */
static void App_Key_AddUser(void)
{
    sayWithoutInt();
    sayAddUser(); // 添加用户

    // 验证管理员
    if (!App_Key_CheckAdmin())
    {
        sayWithoutInt();
        sayAddFail();
        return;
    }

    // 用户密码配置
    sayWithoutInt();
    sayInputUserPassword();  // 请输入用户密码
    App_Key_ReadKey(input2); // 记录输入的密码
    // 查找刚刚输入的是否存在
    esp_err_t err2 = Dri_NVS_FindKey((char *)input2);
    if (err2 == ESP_OK) // 代表已经存在了
    {
        sayWithoutInt();
        sayInvalid(); // 操作无效
        App_Key_ClearBuff();
        return;
    }

    // 不存在
    sayWithoutInt();
    sayInputUserPasswordAgain(); // 请再次输入用户密码
    App_Key_ReadKey(input1);     // 记录输入的密码

    // 判断两次输入是否一致
    if (strcmp((char *)input1, (char *)input2) == 0)
    {
        sayWithoutInt();
        sayAddSucc(); // 添加成功
        Dri_NVS_AddKeyValue((char *)input2, "0");
        // 清空缓冲区
        App_Key_ClearBuff();
    }
    else
    {
        sayWithoutInt();
        sayAddFail(); // 添加失败
        // 清空缓冲区
        App_Key_ClearBuff();
    }
}

/**
 * @brief 删除用户
 *
 */
static void App_Key_DeleteUser(void)
{
    sayWithoutInt();
    sayDelUser(); // 删除用户

    //  验证管理员
    if (!App_Key_CheckAdmin())
    {
        sayWithoutInt();
        sayDelFail();
        return;
    }

    // 用户密码配置
    sayWithoutInt();
    sayInputUserPassword();  // 请输入用户密码
    App_Key_ReadKey(input2); // 记录输入的密码
    // 查找刚刚输入的是否存在
    esp_err_t err2 = Dri_NVS_FindKey((char *)input2);
    if (err2 != ESP_OK) // 代表现在还没有用户
    {
        sayWithoutInt();
        sayDelFail(); // 删除失败
        App_Key_ClearBuff();
        return;
    }

    // 存在 (有用户)
    sayWithoutInt();
    sayInputUserPasswordAgain(); // 请再次输入用户密码
    App_Key_ReadKey(input1);     // 记录输入的密码

    // 判断两次输入是否一致
    if (strcmp((char *)input1, (char *)input2) == 0)
    {
        sayWithoutInt();
        sayDelSucc(); // 删除成功
        Dri_NVS_DeleteKeyValue((char *)input2);
        // 清空缓冲区
        App_Key_ClearBuff();
    }
    else
    {
        sayWithoutInt();
        sayDelFail(); // 删除失败
        // 清空缓冲区
        App_Key_ClearBuff();
    }
}

/**
 * @brief 按键处理初始化
 *
 */
void App_Key_Init(void)
{
    Int_BDR6120_Init(); // 电机模块
    Int_SC12B_Init();   // 按键模块
    Int_WS2812_Init();  // 灯带模块
    Int_WTN6170_Init(); // 语音模块
    Dri_NVS_Init();     // 存储模块

    sayCtrlVolume(0xE5); // 调节音量
}

/**
 * @brief 循环读取哪个按键按下
 *
 * @param key 保存读取到的按键数组 (字符串格式)
 * @return Com_Status 操作状态
 */
Com_Status App_Key_ReadKey(uint8_t *key)
{
    Key_Event key_event;     // 按键事件
    uint8_t key_timeout = 0; // 超时时间
    uint8_t key_index = 0;   // 按键数组索引

    while (1)
    {
        // 获取按键事件
        key_event = Int_SC12B_TouchKey();

        // 如果没有按键按下,记录超时时间
        if (key_event == KEY_NONE)
        {
            // 超时 5s 退出
            key_timeout++;
            if (key_timeout > 100)
            {
                // 超时关灯
                Int_WS2812_CloseLED();
                return Com_TIMEOUT;
            }
        }
        else // 有按键按下
        {
            // 关闭上次按的灯
            Int_WS2812_CloseLED();
            // 有按键按下就清空超时时间
            key_timeout = 0;
            // 按键亮起并语音提示
            Int_WS2812_OpenLED(key_event, pink);
            sayWaterDrop(); // 水滴声

            // 判断 #:确认 M:取消 数字
            switch (key_event)
            {
            case KEY_HASH: // 按键 #
                return Com_OK;

            case KEY_M: // 按键 M
                return Com_ERROR;

            default:                                // 数字
                key[key_index++] = key_event + '0'; // 转换为字符型
                break;
            }
        }

        vTaskDelay(50);
    }
}

/**
 * @brief 处理按下#后的处理
 *
 * @param key 传入按下的按键数组
 * @note 根据key数组的长度作出响应的判断
 */
void App_Key_HandleKey(uint8_t *key)
{
    // 计算 key数组的长度
    size_t key_size = strlen((char *)key);

    if (key_size < 2) // 操作无效
    {
        // 语音提示 操作无效
        sayWithoutInt();
        sayInvalid();
        return;
    }

    if (key_size == 2) // 操作有效
    {
        if (key[0] == '0' && key[1] == '0') // 删除所有
        {
            // 测试
            Dri_NVS_DeleteKeyValueAll();
        }
        else if (key[0] == '1' && key[1] == '1') // 添加管理员
        {
            // 添加管理员
            App_Key_AddAdmin();
        }
        else if (key[0] == '2' && key[1] == '2') // 删除管理员
        {
            // 删除管理员
            App_Key_Deleteadmin();
        }
        else if (key[0] == '3' && key[1] == '3') // 添加用户
        {
            // 添加用户
            App_Key_AddUser();
        }
        else if (key[0] == '4' && key[1] == '4') // 删除用户
        {
            // 删除用户
            App_Key_DeleteUser();
        }
        else if (key[0] == '5' && key[1] == '5') // 添加指纹
        {
            sayWithoutInt();
            sayAddUserFingerprint(); // 添加用户指纹

            // 验证管理员
            if (!App_Key_CheckAdmin())
            {
                sayWithoutInt();
                sayAddFail();
                return;
            }

            xTaskNotify(finger_scan_handle, 1, eSetValueWithOverwrite);
        }
        else if (key[0] == '6' && key[1] == '6') // 删除单个指纹
        {
            sayWithoutInt();
            sayDelUserFingerprint(); // 删除用户指纹
            // 验证管理员
            if (!App_Key_CheckAdmin())
            {
                sayWithoutInt();
                sayAddFail();
                return;
            }

            xTaskNotify(finger_scan_handle, 2, eSetValueWithOverwrite);
        }
        else if (key[0] == '7' && key[1] == '7') // 删除所有指纹
        {
            sayWithoutInt();
            sayDelAll(); // 全部删除
            // 验证管理员
            if (!App_Key_CheckAdmin())
            {
                sayWithoutInt();
                sayAddFail();
                return;
            }

            xTaskNotify(finger_scan_handle, 3, eSetValueWithOverwrite);
        }
        else if (key[0] == '8' && key[1] == '8') // OTA 升级
        {
            // OTA 升级
            App_Communication_OTA();
        }
        else
        {
            sayWithoutInt();
            sayInvalid(); // 操作无效
            return;
        }
    }

    if (key_size > 2) // 用户输入密码
    {
        // 判断输入密码是否正确
        esp_err_t err = Dri_NVS_FindKey((char *)key);
        if (err != ESP_OK)
        {
            sayWithoutInt();
            sayPasswordVerifyFail(); // 密码验证失败
            sayWithoutInt();
            sayRetry(); // 请重试
        }
        else
        {
            sayWithoutInt();
            sayPasswordVerifySucc(); // 密码验证成功
            sayWithoutInt();
            sayDoorOpen(); // 音效+门已开
            Int_BDR6120_OpenLock();
        }
    }
}
