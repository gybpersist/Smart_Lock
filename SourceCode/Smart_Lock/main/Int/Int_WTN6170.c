#include "Int_WTN6170.h"

/**
 * @brief ��ʼ�� ����ģ��WTN6170
 *
 */
void Int_WTN6170_Init(void)
{
    // 1 GPIO ����
    // ��ʼ��GPIO���ýṹ
    gpio_config_t io_conf = {};
    // �����ж�
    io_conf.intr_type = GPIO_INTR_DISABLE;
    // ����Ϊ���ģʽ
    io_conf.mode = GPIO_MODE_OUTPUT;
    // Ҫ���õ����ŵ�λ����
    io_conf.pin_bit_mask = (1ULL << GPIO_WTN6170_DATA);
    // ��������ģʽ
    io_conf.pull_down_en = 0;
    // ��������ģʽ
    io_conf.pull_up_en = 0;
    // ����GPIO�����������
    gpio_config(&io_conf);

    // 2 ��ʼ��Ҫ�ӳ� 100ms
    vTaskDelay(100);

    // 3 Ĭ�ϸߵ�ƽ
    GPIO_WTN6170_HIGH;
}

/**
 * @brief ��������
 *
 * @param cmd ����
 */
void Int_WTN6170_SendCmd(uint8_t cmd)
{
    // ������ǰ��Ҫ������ 10ms(����5ms)
    GPIO_WTN6170_LOW;
    vTaskDelay(10);

    // ѭ���������� (�ȷ���λ)
    for (uint8_t i = 0; i < 8; i++)
    {
        // ȡ����λ
        if (cmd & 0x01)
        {
            GPIO_WTN6170_HIGH;
            usleep(600);
            GPIO_WTN6170_LOW;
            usleep(200);
        }
        else
        {
            GPIO_WTN6170_HIGH;
            usleep(200);
            GPIO_WTN6170_LOW;
            usleep(600);
        }

        // ����һλ
        cmd >>= 1;
    }

    // �����������
    GPIO_WTN6170_HIGH;

    // ������ 5ms (����2ms)
    vTaskDelay(5);
}