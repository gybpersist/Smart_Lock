#include "Int_WS2812.h"

/* 定义几种常见颜色（GRB顺序：绿-红-蓝） */
uint8_t black[3] = {0, 0, 0};         // 黑色（无颜色）
uint8_t white[3] = {255, 255, 255};   // 白色（三通道全开）
// 基础三原色
uint8_t red[3] = {0, 255, 0};         // 红色（仅红色通道）
uint8_t green[3] = {255, 0, 0};       // 绿色（仅绿色通道）
uint8_t blue[3] = {0, 0, 255};        // 蓝色（仅蓝色通道）
// 二次色
uint8_t cyan[3] = {255, 0, 255};      // 青色（绿+蓝）
uint8_t purple[3] = {0, 255, 255};    // 紫色（红+蓝）
uint8_t yellow[3] = {255, 255, 0};    // 黄色（绿+红）
// 其他常用颜色
uint8_t orange[3] = {100, 255, 0};    // 橙色（较多红+少量绿）
uint8_t pink[3] = {0, 255, 100};      // 粉色（红+少量蓝）
uint8_t magenta[3] = {0, 255, 200};   // 品红色（红+较多蓝）
uint8_t lime[3] = {255, 100, 0};      // 青柠色（绿+少量红）
uint8_t sky_blue[3] = {200, 0, 255};  // 天蓝色（较多蓝+少量绿）
// 灰色系列（三通道均衡）
uint8_t dark_grey[3] = {50, 50, 50};  // 深灰色
uint8_t grey[3] = {128, 128, 128};    // 中灰色
uint8_t light_grey[3] = {200, 200, 200}; // 浅灰色

// LED灯带句柄
led_strip_handle_t led_strip;

/**
 * @brief 初始化 灯带WS2812
 *
 */
void Int_WS2812_Init(void)
{
    // LED 灯带配置
    led_strip_config_t strip_config = {
        .strip_gpio_num = WS2812_GPIO_PIN,                           // 连接到 LED 灯条数据线的 GPIO
        .max_leds = 12,                                              // 灯带中的 LED 数量
        .led_model = LED_MODEL_WS2812,                               // LED灯带型号
        .color_component_format = LED_STRIP_COLOR_COMPONENT_FMT_RGB, // 条带颜色顺序：RGB
        .flags = {
            .invert_out = false, // 不要反转输出信号
        }};

    //  LED灯带后端配置：RMT
    led_strip_rmt_config_t rmt_config = {
        .clk_src = RMT_CLK_SRC_DEFAULT,    // 不同的时钟源会导致不同的功耗
        .resolution_hz = 10 * 1000 * 1000, // RMT 计数器时钟频率
        .mem_block_symbols = 0,            //  RMT 通道使用的内存块大小 设置为 0 将回退以使用默认大小
        .flags = {
            .with_dma = 0, // 使用 DMA 可以提高驱动更多 LED 时的性能
        }};

    // 返回保存LED灯带句柄
    led_strip_new_rmt_device(&strip_config, &rmt_config, &led_strip);
}

/**
 * @brief 打开单个 灯带WS2812
 *
 * @param index 要打开的LED索引
 * @param color 要打开的LED颜色
 */
void Int_WS2812_OpenLED(uint32_t index, uint8_t color[3])
{
    led_strip_set_pixel(led_strip, index, color[0], color[1], color[2]);
    led_strip_refresh(led_strip);
}

/**
 * @brief 关闭所有 灯带WS2812
 *
 */
void Int_WS2812_CloseLED()
{
    led_strip_clear(led_strip);
    led_strip_refresh(led_strip);
}
