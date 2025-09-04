#include "Int_BDR6120.h"

/**
 * @brief 初始化 电机BDR6120
 * 
 */
void Int_BDR6120_Init(void)
{
    // 1 GPIO 配置
    // 初始化GPIO配置结构
    gpio_config_t io_conf = {};
    // 禁用中断
    io_conf.intr_type = GPIO_INTR_DISABLE;
    // 设置为输出模式
    io_conf.mode = GPIO_MODE_OUTPUT;
    // 要设置的引脚的位掩码
    io_conf.pin_bit_mask = (1ULL << BDR6120_INA_PIN) | (1ULL << BDR6120_INB_PIN);
    // 禁用下拉模式
    io_conf.pull_down_en = 0;
    // 禁用上拉模式
    io_conf.pull_up_en = 0;
    // 配置GPIO与给定的设置
    gpio_config(&io_conf);

    // 2 初始化INA INB为低电平
    gpio_set_level(BDR6120_INA_PIN, 0);
    gpio_set_level(BDR6120_INB_PIN, 0);
}

/**
 * @brief 打开锁
 * 
 * @note 打开,保持5s关闭
 */
void Int_BDR6120_OpenLock(void)
{
    // 1 开启 200ms
    gpio_set_level(BDR6120_INA_PIN, 1);
    gpio_set_level(BDR6120_INB_PIN, 0);
    vTaskDelay(200);

    // 2 保持 5s
    gpio_set_level(BDR6120_INA_PIN, 1);
    gpio_set_level(BDR6120_INB_PIN, 1);
    vTaskDelay(5000);

    // 3 关闭 200ms
    gpio_set_level(BDR6120_INA_PIN, 0);
    gpio_set_level(BDR6120_INB_PIN, 1);
    vTaskDelay(200);

    // 4 关闭 INA INB
    gpio_set_level(BDR6120_INA_PIN, 1);
    gpio_set_level(BDR6120_INB_PIN, 1);
}
