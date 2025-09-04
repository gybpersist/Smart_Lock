#include "Int_SC12B.h"

uint8_t IsTouch = 0;    //是否按下标志

/**
 * @brief SC12B通过I2C读取数据
 *
 * @param reg_addr SC12B中的寄存器地址
 * @param data 读取的数据
 * @return esp_err_t 错误信息
 */
static esp_err_t Int_SC12B_read(uint8_t reg_addr, uint8_t *data)
{
    return i2c_master_write_read_device(0, SC12B_DEV_ADDR, &reg_addr, 1, data, 1, 1000);
}

/**
 * @brief SC12B通过I2C写入数据
 *
 * @param reg_addr SC12B中要写入的寄存器
 * @param data 写入的数据
 * @return esp_err_t 错误信息
 */
static esp_err_t Int_SC12B_write(uint8_t reg_addr, uint8_t data)
{
    int ret;
    uint8_t write_buf[2] = {reg_addr, data};
    ret = i2c_master_write_to_device(0, SC12B_DEV_ADDR, write_buf, sizeof(write_buf), 1000);

    return ret;
}

/**
 * @brief INT 的中断服务程序
 * 
 * @param arg 
 */
static void IRAM_ATTR gpio_isr_handler(void* arg)
{
    IsTouch = 1;
}

/**
 * @brief 按键SC12B 初始化
 *
 */
void Int_SC12B_Init(void)
{
    // 1 I2C配置
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,             // 配置主模式
        .sda_io_num = SC12B_SDA_PIN,         // SDA引脚
        .scl_io_num = SC12B_SCL_PIN,         // SCL引脚
        .sda_pullup_en = GPIO_PULLUP_ENABLE, // 上拉
        .scl_pullup_en = GPIO_PULLUP_ENABLE, // 上拉
        .master.clk_speed = 400000,          // 速度 400KHZ
    };
    i2c_param_config(0, &conf);
    // 2 安装启动I2C
    i2c_driver_install(0, conf.mode, 0, 0, 0);

    // 3 初始化时间 300ms
    vTaskDelay(300);

    // 4 配置按键的灵敏度
    Int_SC12B_write(0x00, 0x15); // 设置KEY_4的灵敏度
    Int_SC12B_write(0x01, 0x15); // 设置其他按键的灵敏度

    // 5 配置控制寄存器(保持默认)

    // 6 配置 INT 中断
    // 初始化配置结构
    gpio_config_t io_conf = {};
    // GPIO 中断类型 (上升沿)
    io_conf.intr_type = GPIO_INTR_POSEDGE;
    // 引脚的位掩码
    io_conf.pin_bit_mask = (1 << SC12B_INT_PIN);
    // 设置为输入模式
    io_conf.mode = GPIO_MODE_INPUT;
    // 设置下拉模式
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    io_conf.pull_down_en = GPIO_PULLDOWN_ENABLE;
    gpio_config(&io_conf);
    // 启动中断服务(参数0 表示使用默认配置)
    gpio_install_isr_service(0);
    // 为相应的 GPIO 引脚添加 ISR 中断处理程序
    gpio_isr_handler_add(SC12B_INT_PIN, gpio_isr_handler, (void *)NULL);
}

/**
 * @brief 获取按下按键的值
 *
 * @return Key_Event 哪个按键按下
 */
Key_Event Int_SC12B_GetKey(void)
{
    // 保存按键的值
    uint16_t key_value = 0;
    uint8_t output1 = 0; // CH0~CH7
    uint8_t output2 = 0; // CH8~CH11
    Int_SC12B_read(0x08, &output1);
    Int_SC12B_read(0x09, &output2);
    // 合并按键数据
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
 * @brief 使用中断优化获取按键
 *
 * @return Key_Event 哪个按键按下
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
