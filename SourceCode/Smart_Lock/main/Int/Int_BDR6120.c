#include "Int_BDR6120.h"

/**
 * @brief ��ʼ�� ���BDR6120
 * 
 */
void Int_BDR6120_Init(void)
{
    // 1 GPIO ����
    // ��ʼ��GPIO���ýṹ
    gpio_config_t io_conf = {};
    // �����ж�
    io_conf.intr_type = GPIO_INTR_DISABLE;
    // ����Ϊ���ģʽ
    io_conf.mode = GPIO_MODE_OUTPUT;
    // Ҫ���õ����ŵ�λ����
    io_conf.pin_bit_mask = (1ULL << BDR6120_INA_PIN) | (1ULL << BDR6120_INB_PIN);
    // ��������ģʽ
    io_conf.pull_down_en = 0;
    // ��������ģʽ
    io_conf.pull_up_en = 0;
    // ����GPIO�����������
    gpio_config(&io_conf);

    // 2 ��ʼ��INA INBΪ�͵�ƽ
    gpio_set_level(BDR6120_INA_PIN, 0);
    gpio_set_level(BDR6120_INB_PIN, 0);
}

/**
 * @brief ����
 * 
 * @note ��,����5s�ر�
 */
void Int_BDR6120_OpenLock(void)
{
    // 1 ���� 200ms
    gpio_set_level(BDR6120_INA_PIN, 1);
    gpio_set_level(BDR6120_INB_PIN, 0);
    vTaskDelay(200);

    // 2 ���� 5s
    gpio_set_level(BDR6120_INA_PIN, 1);
    gpio_set_level(BDR6120_INB_PIN, 1);
    vTaskDelay(5000);

    // 3 �ر� 200ms
    gpio_set_level(BDR6120_INA_PIN, 0);
    gpio_set_level(BDR6120_INB_PIN, 1);
    vTaskDelay(200);

    // 4 �ر� INA INB
    gpio_set_level(BDR6120_INA_PIN, 1);
    gpio_set_level(BDR6120_INB_PIN, 1);
}
