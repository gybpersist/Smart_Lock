#include "App_Finger.h"

uint16_t min_id = 0;
uint16_t current_id = 0;

// ������ջ�����
uint32_t task_value = 0;

/**
 * @brief ��ʼ��ָ��ģ��
 *
 */
void App_Finger_Init(void)
{
    Int_FPM383_Init();
}

/**
 * @brief ָ�ƴ�����
 *
 */
void App_Finger_Handle(void)
{
    // ��������֪ͨ
    xTaskNotifyWait(UINT32_MAX, UINT32_MAX, &task_value, portMAX_DELAY);
    MY_LOGI("tak_value: %ld\r\n", task_value);

    // δ����������Ҫ�ر��ж�
    gpio_intr_disable(FPM383_INT_PIN);
    switch (task_value)
    {
    case 1: // ���ָ��
        // MY_LOGI("App1\r\n");
        sayWithoutInt();
        sayPlaceFinger(); // �����ָ��
        vTaskDelay(3000);

        // uint16_t min_id = 0;
        // ��ȡ��СID
        Int_FPM383_GetMinID(&min_id);
        MY_LOGE("min_id = %d\r\n", min_id);
        // �Զ�ע��
        if (Int_FPM383_AutoRegister(min_id) == Com_OK)
        {
            sayWithoutInt();
            sayFingerprintAddSucc(); // ָ����ӳɹ�
        }

        break;
    case 2: // ɾ��ָ��
        // MY_LOGI("App2\r\n");
        sayWithoutInt();
        sayPlaceFinger(); // �������ָ
        vTaskDelay(3000);

        // ����¼��ָ��
        if (Int_FPM383_NoFinger() == Com_OK)
        {
            sayWithoutInt();
            sayInvalid();
            // ��������
            Int_FPM383_Sleep();
            // ����������Ҫ�����ж�
            gpio_intr_enable(FPM383_INT_PIN);
            return;
        }

        // uint16_t current_id = 0;
        // ��ȡ��ǰָ��ID
        Int_FPM383_GetFingerID(&current_id);
        MY_LOGI("current_id = %d\r\n", current_id);

        // ɾ��ָ��
        if (Int_FPM383_DeleteFinger(current_id) == Com_OK)
        {
            sayWithoutInt();
            sayDelSucc(); // ɾ���ɹ�
        }
        else
        {
            sayWithoutInt();
            sayDelFail(); // ɾ��ʧ��
        }

        break;
    case 3: // ɾ������ָ��
        // MY_LOGI("App3\r\n");
        if (Int_FPM383_DeleteFingerAll() == Com_OK)
        {
            sayWithoutInt();
            sayDelSucc(); // ɾ���ɹ�
        }
        else
        {
            sayWithoutInt();
            sayDelFail(); // ɾ��ʧ��
        }

        break;
    case 4: // ָ����֤

        // MY_LOGI("App4\r\n");
        // �Զ���ָ֤��
        if (Int_FPM383_AutoIdentify() == Com_OK)
        {
            sayWithoutInt();
            sayFingerprintVerifySucc(); // ָ����֤�ɹ�
            sayWithoutInt();
            sayDoorOpen(); // ���Ѵ�

            Int_BDR6120_OpenLock();
        }
        else
        {
            sayWithoutInt();
            sayFingerprintVerifyFail(); // ָ����֤ʧ��
            sayWithoutInt();
            sayRetry(); // ������
        }

        break;

    default:
        break;
    }
    // ��������
    Int_FPM383_Sleep();
    // ����������Ҫ�����ж�
    gpio_intr_enable(FPM383_INT_PIN);
}
