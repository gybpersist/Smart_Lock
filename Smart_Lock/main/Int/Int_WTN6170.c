#include "Int_WTN6170.h"

/**
 * @brief 初始化 语言模块WTN6170
 *
 */
void Int_WTN6170_Init(void)
{
    // 1 GPIO 配置
    // 初始化GPIO配置结构
    gpio_config_t io_conf = {};
    // 禁用中断
    io_conf.intr_type = GPIO_INTR_DISABLE;
    // 设置为输出模式
    io_conf.mode = GPIO_MODE_OUTPUT;
    // 要设置的引脚的位掩码
    io_conf.pin_bit_mask = (1ULL << GPIO_WTN6170_DATA);
    // 禁用下拉模式
    io_conf.pull_down_en = 0;
    // 禁用上拉模式
    io_conf.pull_up_en = 0;
    // 配置GPIO与给定的设置
    gpio_config(&io_conf);

    // 2 初始化要延迟 100ms
    vTaskDelay(100);

    // 3 默认高电平
    GPIO_WTN6170_HIGH;
}

/**
 * @brief 发送命令
 *
 * @param cmd 命令
 */
void Int_WTN6170_SendCmd(uint8_t cmd)
{
    // 发命令前需要先拉低 10ms(至少5ms)
    GPIO_WTN6170_LOW;
    vTaskDelay(10);

    // 循环发送命令 (先发低位)
    for (uint8_t i = 0; i < 8; i++)
    {
        // 取出低位
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

        // 右移一位
        cmd >>= 1;
    }

    // 发送完成拉高
    GPIO_WTN6170_HIGH;

    // 命令间隔 5ms (至少2ms)
    vTaskDelay(5);
}