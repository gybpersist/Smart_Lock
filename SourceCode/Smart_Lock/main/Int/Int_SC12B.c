#include "Int_SC12B.h"

uint8_t IsTouch = 0;    //�Ƿ��±�־

/**
 * @brief SC12Bͨ��I2C��ȡ����
 *
 * @param reg_addr SC12B�еļĴ�����ַ
 * @param data ��ȡ������
 * @return esp_err_t ������Ϣ
 */
static esp_err_t Int_SC12B_read(uint8_t reg_addr, uint8_t *data)
{
    return i2c_master_write_read_device(0, SC12B_DEV_ADDR, &reg_addr, 1, data, 1, 1000);
}

/**
 * @brief SC12Bͨ��I2Cд������
 *
 * @param reg_addr SC12B��Ҫд��ļĴ���
 * @param data д�������
 * @return esp_err_t ������Ϣ
 */
static esp_err_t Int_SC12B_write(uint8_t reg_addr, uint8_t data)
{
    int ret;
    uint8_t write_buf[2] = {reg_addr, data};
    ret = i2c_master_write_to_device(0, SC12B_DEV_ADDR, write_buf, sizeof(write_buf), 1000);

    return ret;
}

/**
 * @brief INT ���жϷ������
 * 
 * @param arg 
 */
static void IRAM_ATTR gpio_isr_handler(void* arg)
{
    IsTouch = 1;
}

/**
 * @brief ����SC12B ��ʼ��
 *
 */
void Int_SC12B_Init(void)
{
    // 1 I2C����
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,             // ������ģʽ
        .sda_io_num = SC12B_SDA_PIN,         // SDA����
        .scl_io_num = SC12B_SCL_PIN,         // SCL����
        .sda_pullup_en = GPIO_PULLUP_ENABLE, // ����
        .scl_pullup_en = GPIO_PULLUP_ENABLE, // ����
        .master.clk_speed = 400000,          // �ٶ� 400KHZ
    };
    i2c_param_config(0, &conf);
    // 2 ��װ����I2C
    i2c_driver_install(0, conf.mode, 0, 0, 0);

    // 3 ��ʼ��ʱ�� 300ms
    vTaskDelay(300);

    // 4 ���ð�����������
    Int_SC12B_write(0x00, 0x15); // ����KEY_4��������
    Int_SC12B_write(0x01, 0x15); // ��������������������

    // 5 ���ÿ��ƼĴ���(����Ĭ��)

    // 6 ���� INT �ж�
    // ��ʼ�����ýṹ
    gpio_config_t io_conf = {};
    // GPIO �ж����� (������)
    io_conf.intr_type = GPIO_INTR_POSEDGE;
    // ���ŵ�λ����
    io_conf.pin_bit_mask = (1 << SC12B_INT_PIN);
    // ����Ϊ����ģʽ
    io_conf.mode = GPIO_MODE_INPUT;
    // ��������ģʽ
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    io_conf.pull_down_en = GPIO_PULLDOWN_ENABLE;
    gpio_config(&io_conf);
    // �����жϷ���(����0 ��ʾʹ��Ĭ������)
    gpio_install_isr_service(0);
    // Ϊ��Ӧ�� GPIO ������� ISR �жϴ������
    gpio_isr_handler_add(SC12B_INT_PIN, gpio_isr_handler, (void *)NULL);
}

/**
 * @brief ��ȡ���°�����ֵ
 *
 * @return Key_Event �ĸ���������
 */
Key_Event Int_SC12B_GetKey(void)
{
    // ���水����ֵ
    uint16_t key_value = 0;
    uint8_t output1 = 0; // CH0~CH7
    uint8_t output2 = 0; // CH8~CH11
    Int_SC12B_read(0x08, &output1);
    Int_SC12B_read(0x09, &output2);
    // �ϲ���������
    key_value = (output1 << 8) | output2;

    switch (key_value)
    {
    case 0x8000:
        return KEY_0;
        break;
    case 0x4000:
        return KEY_1;
        break;
    case 0x2000:
        return KEY_2;
        break;
    case 0x1000:
        return KEY_3;
        break;
    case 0x0800:
        return KEY_7;
        break;
    case 0x0400:
        return KEY_5;
        break;
    case 0x0200:
        return KEY_6;
        break;
    case 0x0100:
        return KEY_4;
        break;
    case 0x0080:
        return KEY_M;
        break;
    case 0x0040:
        return KEY_8;
    case 0x0020:
        return KEY_9;
    case 0x0010:
        return KEY_HASH;
        break;
    }
    return KEY_NONE;
}

/**
 * @brief ʹ���ж��Ż���ȡ����
 *
 * @return Key_Event �ĸ���������
 */
Key_Event Int_SC12B_TouchKey(void)
{
    Key_Event key_event = KEY_NONE;
    
    if (IsTouch == 1)
    {
        key_event = Int_SC12B_GetKey();
        IsTouch = 0;
    }
    return key_event;
}
