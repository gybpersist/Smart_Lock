#include "Int_FPM383.h"

// ������ջ�����
uint8_t rx_buff[100] = {1};
extern TaskHandle_t finger_scan_handle;
// // �����Ƿ��ǰ���
// uint8_t IsFinger = 0;

/**
 * @brief ʹ�ô��ڷ�������
 *
 * @param data Ҫ���͵�����
 * @param len �������ݵĳ���
 * @return Com_Status ����״��
 */
static Com_Status Int_FPM383_SendData(uint8_t *data, uint32_t len)
{
    // �Ӹ����Ļ������ͳ��Ƚ����ݷ��͵� UART �˿� ,���ط������ݵĳ���
    int tx_len = uart_write_bytes(UART_NUM_1, data, len);
    // �ж� �������ݵĳ���(tx_len) �� д��ĳ���(len) �Ƿ���ͬ
    if (tx_len == len)
    {
        return Com_OK;
    }
    else
    {
        return Com_ERROR;
    }
}

/**
 * @brief ͨ�����ڽ�������
 *
 * @param data ���յ�������
 * @param len Ҫ�������ݵĳ���
 * @return Com_Status ����״��
 */
static Com_Status Int_FPM383_ReceiveData(uint8_t *data, uint32_t len)
{
    // ����ǰ��ջ�����
    memset(rx_buff, 1, 100);

    // UART �� UART ��������ȡ�ֽ�
    int rx_len = uart_read_bytes(UART_NUM_1, data, len, 1000);
    // �ж� ���յ����ݵĳ���(rx_len) �� д��ĳ���(len) �Ƿ���ͬ
    if (rx_len == len)
    {
        return Com_OK;
    }
    else
    {
        return Com_ERROR;
    }
}

/**
 * @brief ָ��FPM383 ����˯��ģʽ
 *
 */
void Int_FPM383_Sleep(void)
{
    // ����Ҫ���͵�ָ��
    uint8_t cmd[12] = {
        0xEF, 0x01,             // ��ͷ
        0xFF, 0xFF, 0xFF, 0xFF, // �豸��ַ
        0x01,                   // ����ʶ
        0x00, 0x03,             // ������
        0x33,                   // ָ����
        0x00, 0x37              // У���
    };

    // �ж�ȷ����
    do
    {
        // ����ָ��
        Int_FPM383_SendData(cmd, 12);

        // ��������
        Int_FPM383_ReceiveData(rx_buff, 12);

        // ���ڽ�������
        // MY_LOGI("sleeping ......\r\n");
    } while (rx_buff[9] != 0x00);

    // �������߿����ж�
    gpio_intr_enable(FPM383_INT_PIN);

    // ��������
    // MY_LOGI("sleep success\r\n");
}

/**
 * @brief ��ȡָ��FPM383 ģ��ID
 *
 */
static void Int_FPM383_GetID(void)
{
    // ����Ҫ���͵�ָ��
    uint8_t cmd[13] = {
        0xEF, 0x01,             // ��ͷ
        0xFF, 0xFF, 0xFF, 0xFF, // �豸��ַ
        0x01,                   // ����ʶ
        0x00, 0x04,             // ������
        0x34,                   // ָ����
        0x00,                   // ����
        0x00, 0x39              // У���
    };

    // ����ָ��
    Int_FPM383_SendData(cmd, 13);

    // ��������
    Int_FPM383_ReceiveData(rx_buff, 44);

    MY_LOGI("ID: %.32s\r\n", rx_buff + 10);
}

/**
 * @brief ����У���,����У��ͷŵ�cmd�ĺ���λ
 *
 * @param cmd Ҫ����У��͵�ָ��
 * @param len ָ���
 */
static void Int_FPM383_CheackSum(uint8_t *cmd, uint16_t len)
{
    uint16_t sum = 0;
    for (uint16_t i = 6; i < len - 2; i++)
    {
        sum += cmd[i];
    }
    cmd[len - 2] = (sum >> 8) & 0xFF;
    cmd[len - 1] = (sum >> 0) & 0xFF;
}

/**
 * @brief ��ȡ��Сָ�Ƶ�ID
 *
 * @param id ��ȡ������СID
 * @return Com_Status ��ȡ״̬
 */
Com_Status Int_FPM383_GetMinID(uint16_t *id)
{
    // ����Ҫ���͵�ָ��
    uint8_t cmd[13] = {
        0xEF, 0x01,             // ��ͷ
        0xFF, 0xFF, 0xFF, 0xFF, // �豸��ַ
        0x01,                   // ����ʶ
        0x00, 0x04,             // ������
        0x1F,                   // ָ����
        0x00,                   // ҳ��0
        '\0', '\0'              // У���
    };

    // ����У���
    Int_FPM383_CheackSum(cmd, 13);

    // ����ָ��
    Int_FPM383_SendData(cmd, 13);

    // ��������
    Int_FPM383_ReceiveData(rx_buff, 44);

    // �ж�ȷ����
    if (rx_buff[9] == 0x00)
    {
        // �����������ҵ���СID
        // ����ÿһ�ֽ� ��������10-42�ֽ�
        for (uint8_t i = 10; i < 44; i++)
        {
            // �����ֽ���ÿһbit
            for (uint8_t j = 0; j < 8; j++)
            {
                // �ж��Ƿ�Ϊ0
                if (rx_buff[i] & 0x01)
                {
                    // ����һλ
                    rx_buff[i] >>= 1;
                }
                else
                {
                    // ��СID
                    *id = (i - 10) * 8 + j;
                    return Com_OK;
                }
            }
        }
    }

    // û����СID
    MY_LOGW("no min ID!!!\r\n");
    return Com_ERROR;
}

/**
 * @brief INT ���жϷ������
 *
 * @param arg
 */
static void IRAM_ATTR gpio_isr_handler(void *arg)
{
    // IsFinger = 1;
    // ��������֪ͨ
    xTaskNotifyFromISR(finger_scan_handle, 4, eSetValueWithOverwrite, NULL);
}

/**
 * @brief ָ��FPM383 ��ʼ��
 *
 */
void Int_FPM383_Init(void)
{
    // ��������
    const uart_config_t uart_config = {
        .baud_rate = 57600,                    // FPM383ָ��ģ��Ĭ�ϲ�����
        .data_bits = UART_DATA_8_BITS,         // ����λ����
        .parity = UART_PARITY_DISABLE,         // ��У��
        .stop_bits = UART_STOP_BITS_1,         // 1ֹͣλ
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE, // ����Ӳ��������
        .source_clk = UART_SCLK_DEFAULT,       // UART Դʱ��Ĭ��ѡ���� APB
    };
    uart_driver_install(UART_NUM_1, 1024, 0, 0, NULL, 0);
    uart_param_config(UART_NUM_1, &uart_config);
    uart_set_pin(UART_NUM_1, FPM383_TX_PIN, FPM383_RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

    // 6 ���� INT �ж�
    // ��ʼ�����ýṹ
    gpio_config_t io_conf = {};
    // GPIO �ж����� (������)
    io_conf.intr_type = GPIO_INTR_POSEDGE;
    // ���ŵ�λ����
    io_conf.pin_bit_mask = (1 << FPM383_INT_PIN);
    // ����Ϊ����ģʽ
    io_conf.mode = GPIO_MODE_INPUT;
    // ��������ģʽ
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    io_conf.pull_down_en = GPIO_PULLDOWN_ENABLE;
    gpio_config(&io_conf);
    // �����жϷ���(����0 ��ʾʹ��Ĭ������)
    gpio_install_isr_service(0);
    // Ϊ��Ӧ�� GPIO ������� ISR �жϴ������
    gpio_isr_handler_add(FPM383_INT_PIN, gpio_isr_handler, (void *)NULL);

    // δ����������Ҫ�ر��ж�
    gpio_intr_disable(FPM383_INT_PIN);

    // ��������
    Int_FPM383_Sleep();

    // ��ȡID
    Int_FPM383_GetID();
}

/**
 * @brief �Զ�ע��ָ��
 *
 * @param id ָ��ID
 * @return Com_Status ע��״̬
 */
Com_Status Int_FPM383_AutoRegister(uint16_t id)
{
    // �Զ�ע������ '\0'Ϊռλ��
    uint8_t cmd[17] = {
        0xEF, 0x01,             // ��ͷ
        0xFF, 0xFF, 0xFF, 0xFF, // �豸��ַ
        0x01,                   // ����ʶ
        0x00, 0x08,             // ������
        0x31,                   // ָ����
        '\0', '\0',             // ID
        0x01,                   // ¼�����
        0x00, 0x37,             // ����
        '\0', '\0'              // У���
    };

    // ����ID
    cmd[10] = (id >> 8) & 0xFF;
    cmd[11] = (id >> 0) & 0xFF;

    // ����У���
    Int_FPM383_CheackSum(cmd, 17);

    // ����ָ��
    Int_FPM383_SendData(cmd, 17);

    // ����ȷ��
    Int_FPM383_ReceiveData(rx_buff, 14);

    if (rx_buff[9] == 0x00)
    {
        MY_LOGI("Auto Register Succes...\r\n");
        return Com_OK;
    }
    else
    {
        MY_LOGE("Auto Register Error!!!\r\n");
        return Com_ERROR;
    }
}

/**
 * @brief �Զ���ָ֤��
 *
 * @return Com_Status ��֤״̬
 */
Com_Status Int_FPM383_AutoIdentify(void)
{
    // ����Ҫ���͵�ָ��
    uint8_t cmd[17] = {
        0xEF, 0x01,             // ��ͷ
        0xFF, 0xFF, 0xFF, 0xFF, // �豸��ַ
        0x01,                   // ����ʶ
        0x00, 0x08,             // ������
        0x32,                   // ָ����
        0x03,                   // �����ȼ�
        0xFF, 0xFF,             // ID(һ��N)
        0x00, 0x06,             // ����
        '\0', '\0'              // У���
    };

    // ����У���
    Int_FPM383_CheackSum(cmd, 17);

    // ����ָ��
    Int_FPM383_SendData(cmd, 17);

    // ����
    Int_FPM383_ReceiveData(rx_buff, 17);

    if (rx_buff[9] == 0x00)
    {
        MY_LOGI("Auto Identify Succes...\r\n");
        return Com_OK;
    }
    else
    {
        MY_LOGE("Auto Identify Error!!!\r\n");
        return Com_ERROR;
    }
}

/**
 * @brief ɾ��ָ��
 *
 * @return Com_Status ״̬��Ϣ
 */
Com_Status Int_FPM383_DeleteFinger(uint16_t id)
{
    // ����Ҫ���͵�ָ��
    uint8_t cmd[16] = {
        0xEF, 0x01,             // ��ͷ
        0xFF, 0xFF, 0xFF, 0xFF, // �豸��ַ
        0x01,                   // ����ʶ
        0x00, 0x07,             // ������
        0x0C,                   // ָ����
        '\0', '\0',             // ҳ��ID
        0x00, 0x01,             // ɾ������
        '\0', '\0'              // У���
    };

    // ���ID
    cmd[10] = (id >> 8) & 0xFF;
    cmd[11] = (id >> 0) & 0xFF;

    // ����У���
    Int_FPM383_CheackSum(cmd, 16);

    // ����ָ��
    Int_FPM383_SendData(cmd, 16);

    // ����
    Int_FPM383_ReceiveData(rx_buff, 12);

    if (rx_buff[9] == 0x00)
    {
        MY_LOGI("Delete Finger Succes...\r\n");
        return Com_OK;
    }
    else
    {
        MY_LOGE("Delete Finger Error!!!\r\n");
        return Com_ERROR;
    }
}

/**
 * @brief ɾ������ָ��
 *
 * @return Com_Status ״̬��Ϣ
 */
Com_Status Int_FPM383_DeleteFingerAll(void)
{
    // ����Ҫ���͵�ָ��
    uint8_t cmd[12] = {
        0xEF, 0x01,             // ��ͷ
        0xFF, 0xFF, 0xFF, 0xFF, // �豸��ַ
        0x01,                   // ����ʶ
        0x00, 0x03,             // ������
        0x0D,                   // ָ����
        0x00, 0x11              // У���
    };

    // ����ָ��
    Int_FPM383_SendData(cmd, 12);

    // ����
    Int_FPM383_ReceiveData(rx_buff, 12);

    if (rx_buff[9] == 0x00)
    {
        MY_LOGI("Delete All Finger Succes...\r\n");
        return Com_OK;
    }
    else
    {
        MY_LOGE("Delete All Finger Error!!!\r\n");
        return Com_ERROR;
    }
}

/**
 * @brief ��ȡ������ָ��ָ��ID
 *
 * @param id ָ��ID
 * @return Com_Status
 */
Com_Status Int_FPM383_GetFingerID(uint16_t *id)
{
    // ����Ҫ���͵�ָ��
    uint8_t cmd[17] = {
        0xEF, 0x01,             // ��ͷ
        0xFF, 0xFF, 0xFF, 0xFF, // �豸��ַ
        0x01,                   // ����ʶ
        0x00, 0x08,             // ������
        0x32,                   // ָ����
        0x03,                   // �����ȼ�
        0xFF, 0xFF,             // ID(һ��N)
        0x00, 0x06,             // ����
        '\0', '\0'              // У���
    };

    // ����У���
    Int_FPM383_CheackSum(cmd, 17);

    // ����ָ��
    Int_FPM383_SendData(cmd, 17);

    // ����
    Int_FPM383_ReceiveData(rx_buff, 17);

    if (rx_buff[9] == 0x00)
    {
        *id = (rx_buff[11] << 8) | rx_buff[12];
        MY_LOGI("Get Finger ID: %hu ...\r\n", *id);
        return Com_OK;
    }
    else
    {
        MY_LOGE("Get Finger Error!!!\r\n");
        return Com_ERROR;
    }
}

/**
 * @brief û����������ָ
 *
 * @return Com_Status ��֤״̬
 */
Com_Status Int_FPM383_NoFinger(void)
{
    // ����Ҫ���͵�ָ��
    uint8_t cmd[17] = {
        0xEF, 0x01,             // ��ͷ
        0xFF, 0xFF, 0xFF, 0xFF, // �豸��ַ
        0x01,                   // ����ʶ
        0x00, 0x08,             // ������
        0x32,                   // ָ����
        0x03,                   // �����ȼ�
        0xFF, 0xFF,             // ID(һ��N)
        0x00, 0x06,             // ����
        '\0', '\0'              // У���
    };

    // ����У���
    Int_FPM383_CheackSum(cmd, 17);

    // ����ָ��
    Int_FPM383_SendData(cmd, 17);

    // ����
    Int_FPM383_ReceiveData(rx_buff, 17);

    if (rx_buff[9] == 0x09)
    {
        MY_LOGI("No Finger...\r\n");
        return Com_OK;
    }
    else
    {
        MY_LOGE("Have Finger!!!\r\n");
        return Com_ERROR;
    }
}
