#include "Int_FPM383.h"

// 定义接收缓冲区
uint8_t rx_buff[100] = {1};
extern TaskHandle_t finger_scan_handle;
// // 定义是否是按下
// uint8_t IsFinger = 0;

/**
 * @brief 使用串口发送数据
 *
 * @param data 要发送的数据
 * @param len 发送数据的长度
 * @return Com_Status 发送状况
 */
static Com_Status Int_FPM383_SendData(uint8_t *data, uint32_t len)
{
    // 从给定的缓冲区和长度将数据发送到 UART 端口 ,返回发送数据的长度
    int tx_len = uart_write_bytes(UART_NUM_1, data, len);
    // 判断 发送数据的长度(tx_len) 和 写入的长度(len) 是否相同
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
 * @brief 通过串口接收数据
 *
 * @param data 接收到的数据
 * @param len 要接收数据的长度
 * @return Com_Status 接收状况
 */
static Com_Status Int_FPM383_ReceiveData(uint8_t *data, uint32_t len)
{
    // 接收前清空缓冲区
    memset(rx_buff, 1, 100);

    // UART 从 UART 缓冲区读取字节
    int rx_len = uart_read_bytes(UART_NUM_1, data, len, 1000);
    // 判断 接收到数据的长度(rx_len) 和 写入的长度(len) 是否相同
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
 * @brief 指纹FPM383 进入睡眠模式
 *
 */
void Int_FPM383_Sleep(void)
{
    // 整合要发送的指令
    uint8_t cmd[12] = {
        0xEF, 0x01,             // 包头
        0xFF, 0xFF, 0xFF, 0xFF, // 设备地址
        0x01,                   // 包标识
        0x00, 0x03,             // 包长度
        0x33,                   // 指令码
        0x00, 0x37              // 校验和
    };

    // 判断确认码
    do
    {
        // 发送指令
        Int_FPM383_SendData(cmd, 12);

        // 接收数据
        Int_FPM383_ReceiveData(rx_buff, 12);

        // 正在进入休眠
        // MY_LOGI("sleeping ......\r\n");
    } while (rx_buff[9] != 0x00);

    // 进入休眠开启中断
    gpio_intr_enable(FPM383_INT_PIN);

    // 进入休眠
    // MY_LOGI("sleep success\r\n");
}

/**
 * @brief 获取指纹FPM383 模块ID
 *
 */
static void Int_FPM383_GetID(void)
{
    // 整合要发送的指令
    uint8_t cmd[13] = {
        0xEF, 0x01,             // 包头
        0xFF, 0xFF, 0xFF, 0xFF, // 设备地址
        0x01,                   // 包标识
        0x00, 0x04,             // 包长度
        0x34,                   // 指令码
        0x00,                   // 参数
        0x00, 0x39              // 校验和
    };

    // 发送指令
    Int_FPM383_SendData(cmd, 13);

    // 接收数据
    Int_FPM383_ReceiveData(rx_buff, 44);

    MY_LOGI("ID: %.32s\r\n", rx_buff + 10);
}

/**
 * @brief 计算校验和,并把校验和放到cmd的后两位
 *
 * @param cmd 要计算校验和的指令
 * @param len 指令长度
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
 * @brief 获取最小指纹的ID
 *
 * @param id 获取到的最小ID
 * @return Com_Status 获取状态
 */
Com_Status Int_FPM383_GetMinID(uint16_t *id)
{
    // 整合要发送的指令
    uint8_t cmd[13] = {
        0xEF, 0x01,             // 包头
        0xFF, 0xFF, 0xFF, 0xFF, // 设备地址
        0x01,                   // 包标识
        0x00, 0x04,             // 包长度
        0x1F,                   // 指令码
        0x00,                   // 页码0
        '\0', '\0'              // 校验和
    };

    // 计算校验和
    Int_FPM383_CheackSum(cmd, 13);

    // 发送指令
    Int_FPM383_SendData(cmd, 13);

    // 接收数据
    Int_FPM383_ReceiveData(rx_buff, 44);

    // 判断确认码
    if (rx_buff[9] == 0x00)
    {
        // 根据索引码找到最小ID
        // 遍历每一字节 索引码在10-42字节
        for (uint8_t i = 10; i < 44; i++)
        {
            // 遍历字节中每一bit
            for (uint8_t j = 0; j < 8; j++)
            {
                // 判断是否为0
                if (rx_buff[i] & 0x01)
                {
                    // 右移一位
                    rx_buff[i] >>= 1;
                }
                else
                {
                    // 最小ID
                    *id = (i - 10) * 8 + j;
                    return Com_OK;
                }
            }
        }
    }

    // 没有最小ID
    MY_LOGW("no min ID!!!\r\n");
    return Com_ERROR;
}

/**
 * @brief INT 的中断服务程序
 *
 * @param arg
 */
static void IRAM_ATTR gpio_isr_handler(void *arg)
{
    // IsFinger = 1;
    // 发送任务通知
    xTaskNotifyFromISR(finger_scan_handle, 4, eSetValueWithOverwrite, NULL);
}

/**
 * @brief 指纹FPM383 初始化
 *
 */
void Int_FPM383_Init(void)
{
    // 串口配置
    const uart_config_t uart_config = {
        .baud_rate = 57600,                    // FPM383指纹模块默认波特率
        .data_bits = UART_DATA_8_BITS,         // 数据位长度
        .parity = UART_PARITY_DISABLE,         // 无校验
        .stop_bits = UART_STOP_BITS_1,         // 1停止位
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE, // 禁用硬件流控制
        .source_clk = UART_SCLK_DEFAULT,       // UART 源时钟默认选择是 APB
    };
    uart_driver_install(UART_NUM_1, 1024, 0, 0, NULL, 0);
    uart_param_config(UART_NUM_1, &uart_config);
    uart_set_pin(UART_NUM_1, FPM383_TX_PIN, FPM383_RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

    // 6 配置 INT 中断
    // 初始化配置结构
    gpio_config_t io_conf = {};
    // GPIO 中断类型 (上升沿)
    io_conf.intr_type = GPIO_INTR_POSEDGE;
    // 引脚的位掩码
    io_conf.pin_bit_mask = (1 << FPM383_INT_PIN);
    // 设置为输入模式
    io_conf.mode = GPIO_MODE_INPUT;
    // 设置下拉模式
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    io_conf.pull_down_en = GPIO_PULLDOWN_ENABLE;
    gpio_config(&io_conf);
    // 启动中断服务(参数0 表示使用默认配置)
    gpio_install_isr_service(0);
    // 为相应的 GPIO 引脚添加 ISR 中断处理程序
    gpio_isr_handler_add(FPM383_INT_PIN, gpio_isr_handler, (void *)NULL);

    // 未进入休眠需要关闭中断
    gpio_intr_disable(FPM383_INT_PIN);

    // 进入休眠
    Int_FPM383_Sleep();

    // 获取ID
    Int_FPM383_GetID();
}

/**
 * @brief 自动注册指纹
 *
 * @param id 指纹ID
 * @return Com_Status 注册状态
 */
Com_Status Int_FPM383_AutoRegister(uint16_t id)
{
    // 自动注册命令 '\0'为占位符
    uint8_t cmd[17] = {
        0xEF, 0x01,             // 包头
        0xFF, 0xFF, 0xFF, 0xFF, // 设备地址
        0x01,                   // 包标识
        0x00, 0x08,             // 包长度
        0x31,                   // 指令码
        '\0', '\0',             // ID
        0x01,                   // 录入次数
        0x00, 0x37,             // 参数
        '\0', '\0'              // 校验和
    };

    // 放置ID
    cmd[10] = (id >> 8) & 0xFF;
    cmd[11] = (id >> 0) & 0xFF;

    // 计算校验和
    Int_FPM383_CheackSum(cmd, 17);

    // 发送指令
    Int_FPM383_SendData(cmd, 17);

    // 接收确认
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
 * @brief 自动验证指纹
 *
 * @return Com_Status 验证状态
 */
Com_Status Int_FPM383_AutoIdentify(void)
{
    // 整合要发送的指令
    uint8_t cmd[17] = {
        0xEF, 0x01,             // 包头
        0xFF, 0xFF, 0xFF, 0xFF, // 设备地址
        0x01,                   // 包标识
        0x00, 0x08,             // 包长度
        0x32,                   // 指令码
        0x03,                   // 分数等级
        0xFF, 0xFF,             // ID(一对N)
        0x00, 0x06,             // 参数
        '\0', '\0'              // 校验和
    };

    // 计算校验和
    Int_FPM383_CheackSum(cmd, 17);

    // 发送指令
    Int_FPM383_SendData(cmd, 17);

    // 接收
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
 * @brief 删除指纹
 *
 * @return Com_Status 状态信息
 */
Com_Status Int_FPM383_DeleteFinger(uint16_t id)
{
    // 整合要发送的指令
    uint8_t cmd[16] = {
        0xEF, 0x01,             // 包头
        0xFF, 0xFF, 0xFF, 0xFF, // 设备地址
        0x01,                   // 包标识
        0x00, 0x07,             // 包长度
        0x0C,                   // 指令码
        '\0', '\0',             // 页码ID
        0x00, 0x01,             // 删除个数
        '\0', '\0'              // 校验和
    };

    // 添加ID
    cmd[10] = (id >> 8) & 0xFF;
    cmd[11] = (id >> 0) & 0xFF;

    // 计算校验和
    Int_FPM383_CheackSum(cmd, 16);

    // 发送指令
    Int_FPM383_SendData(cmd, 16);

    // 接收
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
 * @brief 删除所有指纹
 *
 * @return Com_Status 状态信息
 */
Com_Status Int_FPM383_DeleteFingerAll(void)
{
    // 整合要发送的指令
    uint8_t cmd[12] = {
        0xEF, 0x01,             // 包头
        0xFF, 0xFF, 0xFF, 0xFF, // 设备地址
        0x01,                   // 包标识
        0x00, 0x03,             // 包长度
        0x0D,                   // 指令码
        0x00, 0x11              // 校验和
    };

    // 发送指令
    Int_FPM383_SendData(cmd, 12);

    // 接收
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
 * @brief 获取按下手指的指纹ID
 *
 * @param id 指纹ID
 * @return Com_Status
 */
Com_Status Int_FPM383_GetFingerID(uint16_t *id)
{
    // 整合要发送的指令
    uint8_t cmd[17] = {
        0xEF, 0x01,             // 包头
        0xFF, 0xFF, 0xFF, 0xFF, // 设备地址
        0x01,                   // 包标识
        0x00, 0x08,             // 包长度
        0x32,                   // 指令码
        0x03,                   // 分数等级
        0xFF, 0xFF,             // ID(一对N)
        0x00, 0x06,             // 参数
        '\0', '\0'              // 校验和
    };

    // 计算校验和
    Int_FPM383_CheackSum(cmd, 17);

    // 发送指令
    Int_FPM383_SendData(cmd, 17);

    // 接收
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
 * @brief 没有搜索到手指
 *
 * @return Com_Status 验证状态
 */
Com_Status Int_FPM383_NoFinger(void)
{
    // 整合要发送的指令
    uint8_t cmd[17] = {
        0xEF, 0x01,             // 包头
        0xFF, 0xFF, 0xFF, 0xFF, // 设备地址
        0x01,                   // 包标识
        0x00, 0x08,             // 包长度
        0x32,                   // 指令码
        0x03,                   // 分数等级
        0xFF, 0xFF,             // ID(一对N)
        0x00, 0x06,             // 参数
        '\0', '\0'              // 校验和
    };

    // 计算校验和
    Int_FPM383_CheackSum(cmd, 17);

    // 发送指令
    Int_FPM383_SendData(cmd, 17);

    // 接收
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
