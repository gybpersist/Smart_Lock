#include "App_Key.h"

// ָ������ľ��
extern TaskHandle_t finger_scan_handle;

// ���뻺����
uint8_t input1[32] = {0};
uint8_t input2[32] = {0};

/**
 * @brief ���������
 *
 */
static void App_Key_ClearBuff(void)
{
    memset(input1, 0, 32);
    memset(input2, 0, 32);
}

/**
 * @brief ��ӹ���Ա
 *
 */
static void App_Key_AddAdmin(void)
{
    sayWithoutInt();
    sayAddAdmin(); // ��ӹ���Ա
    // �ж��Ƿ��й���Ա
    // ����Ա����
    esp_err_t err = Dri_NVS_FindKey("admin");
    if (err == ESP_OK)
    {
        sayWithoutInt();
        sayAdmIntull(); // ����Ա����
        return;
    }

    // ������ӹ���Ա �ж����������Ƿ�һ��
    sayWithoutInt();
    sayInputAdminPassword(); // ���������Ա����
    // �����һ������Ա����
    Com_Status status1 = App_Key_ReadKey(input1); // �����ȡ���İ������鵽input1
    if (status1 != Com_OK)
    {
        sayWithoutInt();
        sayOperateFail(); // ����ʧ��
        // ��ջ�����
        App_Key_ClearBuff();
        return;
    }

    sayWithoutInt();
    sayInputAdminPasswordAgain(); // ���ٴ��������Ա����
    // ����ڶ�������Ա����
    Com_Status status2 = App_Key_ReadKey(input2); // �����ȡ���İ������鵽input2
    if (status2 != Com_OK)
    {
        sayWithoutInt();
        sayOperateFail(); // ����ʧ��
        // ��ջ�����
        App_Key_ClearBuff();
        return;
    }

    // �ж����������Ƿ�һ��
    if (strcmp((char *)input1, (char *)input2) == 0)
    {
        sayAddSucc(); // ��ӳɹ�
        Dri_NVS_AddKeyValue("admin", (char *)input2);
        // ��ջ�����
        App_Key_ClearBuff();
    }
    else
    {
        sayAddFail(); // ���ʧ��
    }
}

/**
 * @brief �жϹ���Ա�Ƿ���� �� ��֤����Ա���
 */
static bool App_Key_CheckAdmin(void)
{
    // 1. �жϹ���Ա�Ƿ����
    if (Dri_NVS_FindKey("admin") != ESP_OK)
    {
        sayWithoutInt();
        sayInvalid();
        return false;
    }
    // 2. ����������֤����Ա ���������Ա����
    sayWithoutInt();
    sayCheckAdmin();
    sayWithoutInt();
    sayInputAdminPassword();

    // 3.��֤����Ա���
    if (App_Key_ReadKey(input1) != Com_OK)
    {
        sayWithoutInt();
        sayInvalid();
        // ��ջ�����
        App_Key_ClearBuff();
        return false;
    }
    // 4. �жϹ���Ա�����Ƿ�һ��
    size_t input2_size = 32;
    Dri_NVS_GetKey("admin", (char *)input2, &input2_size);

    bool is_OK = strcmp((char *)input1, (char *)input2) == 0;
    // ��ջ�����
    App_Key_ClearBuff();
    return is_OK;
}

/**
 * @brief ɾ������Ա
 *
 */
static void App_Key_Deleteadmin(void)
{
    // 1. ��������ɾ������Ա
    sayWithoutInt();
    sayDelAdmin();

    // 2. ��֤����Ա���
    if (App_Key_CheckAdmin())
    {
        sayWithoutInt();
        sayDelSucc(); // ɾ���ɹ�
        Dri_NVS_DeleteKeyValue("admin");
        // ��ջ�����
        App_Key_ClearBuff();
    }
    else
    {
        sayWithoutInt();
        sayDelFail(); // ���ʧ��
        // ��ջ�����
        App_Key_ClearBuff();
    }
}

/**
 * @brief ����û�
 *
 */
static void App_Key_AddUser(void)
{
    sayWithoutInt();
    sayAddUser(); // ����û�

    // ��֤����Ա
    if (!App_Key_CheckAdmin())
    {
        sayWithoutInt();
        sayAddFail();
        return;
    }

    // �û���������
    sayWithoutInt();
    sayInputUserPassword();  // �������û�����
    App_Key_ReadKey(input2); // ��¼���������
    // ���Ҹո�������Ƿ����
    esp_err_t err2 = Dri_NVS_FindKey((char *)input2);
    if (err2 == ESP_OK) // �����Ѿ�������
    {
        sayWithoutInt();
        sayInvalid(); // ������Ч
        App_Key_ClearBuff();
        return;
    }

    // ������
    sayWithoutInt();
    sayInputUserPasswordAgain(); // ���ٴ������û�����
    App_Key_ReadKey(input1);     // ��¼���������

    // �ж����������Ƿ�һ��
    if (strcmp((char *)input1, (char *)input2) == 0)
    {
        sayWithoutInt();
        sayAddSucc(); // ��ӳɹ�
        Dri_NVS_AddKeyValue((char *)input2, "0");
        // ��ջ�����
        App_Key_ClearBuff();
    }
    else
    {
        sayWithoutInt();
        sayAddFail(); // ���ʧ��
        // ��ջ�����
        App_Key_ClearBuff();
    }
}

/**
 * @brief ɾ���û�
 *
 */
static void App_Key_DeleteUser(void)
{
    sayWithoutInt();
    sayDelUser(); // ɾ���û�

    //  ��֤����Ա
    if (!App_Key_CheckAdmin())
    {
        sayWithoutInt();
        sayDelFail();
        return;
    }

    // �û���������
    sayWithoutInt();
    sayInputUserPassword();  // �������û�����
    App_Key_ReadKey(input2); // ��¼���������
    // ���Ҹո�������Ƿ����
    esp_err_t err2 = Dri_NVS_FindKey((char *)input2);
    if (err2 != ESP_OK) // �������ڻ�û���û�
    {
        sayWithoutInt();
        sayDelFail(); // ɾ��ʧ��
        App_Key_ClearBuff();
        return;
    }

    // ���� (���û�)
    sayWithoutInt();
    sayInputUserPasswordAgain(); // ���ٴ������û�����
    App_Key_ReadKey(input1);     // ��¼���������

    // �ж����������Ƿ�һ��
    if (strcmp((char *)input1, (char *)input2) == 0)
    {
        sayWithoutInt();
        sayDelSucc(); // ɾ���ɹ�
        Dri_NVS_DeleteKeyValue((char *)input2);
        // ��ջ�����
        App_Key_ClearBuff();
    }
    else
    {
        sayWithoutInt();
        sayDelFail(); // ɾ��ʧ��
        // ��ջ�����
        App_Key_ClearBuff();
    }
}

/**
 * @brief ���������ʼ��
 *
 */
void App_Key_Init(void)
{
    Int_BDR6120_Init(); // ���ģ��
    Int_SC12B_Init();   // ����ģ��
    Int_WS2812_Init();  // �ƴ�ģ��
    Int_WTN6170_Init(); // ����ģ��
    Dri_NVS_Init();     // �洢ģ��

    sayCtrlVolume(0xE5); // ��������
}

/**
 * @brief ѭ����ȡ�ĸ���������
 *
 * @param key �����ȡ���İ������� (�ַ�����ʽ)
 * @return Com_Status ����״̬
 */
Com_Status App_Key_ReadKey(uint8_t *key)
{
    Key_Event key_event;     // �����¼�
    uint8_t key_timeout = 0; // ��ʱʱ��
    uint8_t key_index = 0;   // ������������

    while (1)
    {
        // ��ȡ�����¼�
        key_event = Int_SC12B_TouchKey();

        // ���û�а�������,��¼��ʱʱ��
        if (key_event == KEY_NONE)
        {
            // ��ʱ 5s �˳�
            key_timeout++;
            if (key_timeout > 100)
            {
                // ��ʱ�ص�
                Int_WS2812_CloseLED();
                return Com_TIMEOUT;
            }
        }
        else // �а�������
        {
            // �ر��ϴΰ��ĵ�
            Int_WS2812_CloseLED();
            // �а������¾���ճ�ʱʱ��
            key_timeout = 0;
            // ��������������ʾ
            Int_WS2812_OpenLED(key_event, pink);
            sayWaterDrop(); // ˮ����

            // �ж� #:ȷ�� M:ȡ�� ����
            switch (key_event)
            {
            case KEY_HASH: // ���� #
                return Com_OK;

            case KEY_M: // ���� M
                return Com_ERROR;

            default:                                // ����
                key[key_index++] = key_event + '0'; // ת��Ϊ�ַ���
                break;
            }
        }

        vTaskDelay(50);
    }
}

/**
 * @brief ������#��Ĵ���
 *
 * @param key ���밴�µİ�������
 * @note ����key����ĳ���������Ӧ���ж�
 */
void App_Key_HandleKey(uint8_t *key)
{
    // ���� key����ĳ���
    size_t key_size = strlen((char *)key);

    if (key_size < 2) // ������Ч
    {
        // ������ʾ ������Ч
        sayWithoutInt();
        sayInvalid();
        return;
    }

    if (key_size == 2) // ������Ч
    {
        if (key[0] == '0' && key[1] == '0') // ɾ������
        {
            // ����
            Dri_NVS_DeleteKeyValueAll();
        }
        else if (key[0] == '1' && key[1] == '1') // ��ӹ���Ա
        {
            // ��ӹ���Ա
            App_Key_AddAdmin();
        }
        else if (key[0] == '2' && key[1] == '2') // ɾ������Ա
        {
            // ɾ������Ա
            App_Key_Deleteadmin();
        }
        else if (key[0] == '3' && key[1] == '3') // ����û�
        {
            // ����û�
            App_Key_AddUser();
        }
        else if (key[0] == '4' && key[1] == '4') // ɾ���û�
        {
            // ɾ���û�
            App_Key_DeleteUser();
        }
        else if (key[0] == '5' && key[1] == '5') // ���ָ��
        {
            sayWithoutInt();
            sayAddUserFingerprint(); // ����û�ָ��

            // ��֤����Ա
            if (!App_Key_CheckAdmin())
            {
                sayWithoutInt();
                sayAddFail();
                return;
            }

            xTaskNotify(finger_scan_handle, 1, eSetValueWithOverwrite);
        }
        else if (key[0] == '6' && key[1] == '6') // ɾ������ָ��
        {
            sayWithoutInt();
            sayDelUserFingerprint(); // ɾ���û�ָ��
            // ��֤����Ա
            if (!App_Key_CheckAdmin())
            {
                sayWithoutInt();
                sayAddFail();
                return;
            }

            xTaskNotify(finger_scan_handle, 2, eSetValueWithOverwrite);
        }
        else if (key[0] == '7' && key[1] == '7') // ɾ������ָ��
        {
            sayWithoutInt();
            sayDelAll(); // ȫ��ɾ��
            // ��֤����Ա
            if (!App_Key_CheckAdmin())
            {
                sayWithoutInt();
                sayAddFail();
                return;
            }

            xTaskNotify(finger_scan_handle, 3, eSetValueWithOverwrite);
        }
        else if (key[0] == '8' && key[1] == '8') // OTA ����
        {
            // OTA ����
            App_Communication_OTA();
        }
        else
        {
            sayWithoutInt();
            sayInvalid(); // ������Ч
            return;
        }
    }

    if (key_size > 2) // �û���������
    {
        // �ж����������Ƿ���ȷ
        esp_err_t err = Dri_NVS_FindKey((char *)key);
        if (err != ESP_OK)
        {
            sayWithoutInt();
            sayPasswordVerifyFail(); // ������֤ʧ��
            sayWithoutInt();
            sayRetry(); // ������
        }
        else
        {
            sayWithoutInt();
            sayPasswordVerifySucc(); // ������֤�ɹ�
            sayWithoutInt();
            sayDoorOpen(); // ��Ч+���ѿ�
            Int_BDR6120_OpenLock();
        }
    }
}
