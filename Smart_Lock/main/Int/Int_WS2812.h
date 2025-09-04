#ifndef __INT_WS2812_H__
#define __INT_WS2812_H__

#include "driver/gpio.h"
#include "led_strip.h"
#include "esp_task.h"

#define WS2812_GPIO_PIN GPIO_NUM_6

/* 颜色定义的外部声明（GRB顺序：绿-红-蓝） */
extern uint8_t black[3];         // 黑色（无颜色）
extern uint8_t white[3];         // 白色（三通道全开）
// 基础三原色
extern uint8_t red[3];           // 红色（仅红色通道）
extern uint8_t green[3];         // 绿色（仅绿色通道）
extern uint8_t blue[3];          // 蓝色（仅蓝色通道）
// 二次色
extern uint8_t cyan[3];          // 青色（绿+蓝）
extern uint8_t purple[3];        // 紫色（红+蓝）
extern uint8_t yellow[3];        // 黄色（绿+红）
// 其他常用颜色
extern uint8_t orange[3];        // 橙色（较多红+少量绿）
extern uint8_t pink[3];          // 粉色（红+少量蓝）
extern uint8_t magenta[3];       // 品红色（红+较多蓝）
extern uint8_t lime[3];          // 青柠色（绿+少量红）
extern uint8_t sky_blue[3];      // 天蓝色（较多蓝+少量绿）
// 灰色系列（三通道均衡）
extern uint8_t dark_grey[3];     // 深灰色
extern uint8_t grey[3];          // 中灰色
extern uint8_t light_grey[3];    // 浅灰色

/**
 * @brief 初始化 灯带WS2812
 * 
 */
void Int_WS2812_Init(void);

/**
 * @brief 打开单个 灯带WS2812
 * 
 * @param index 要打开的LED索引
 * @param color 要打开的LED颜色数组，RGB格式为{红色,绿色,蓝色}
 */
void Int_WS2812_OpenLED(uint32_t index,uint8_t color[3]);

/**
 * @brief 关闭所有 灯带WS2812
 * 
 */
void Int_WS2812_CloseLED();

#endif /* __INT_WS2812_H__ */
