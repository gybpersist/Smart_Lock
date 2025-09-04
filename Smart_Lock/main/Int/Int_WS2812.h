#ifndef __INT_WS2812_H__
#define __INT_WS2812_H__

#include "driver/gpio.h"
#include "led_strip.h"
#include "esp_task.h"

#define WS2812_GPIO_PIN GPIO_NUM_6

/* ��ɫ������ⲿ������GRB˳����-��-���� */
extern uint8_t black[3];         // ��ɫ������ɫ��
extern uint8_t white[3];         // ��ɫ����ͨ��ȫ����
// ������ԭɫ
extern uint8_t red[3];           // ��ɫ������ɫͨ����
extern uint8_t green[3];         // ��ɫ������ɫͨ����
extern uint8_t blue[3];          // ��ɫ������ɫͨ����
// ����ɫ
extern uint8_t cyan[3];          // ��ɫ����+����
extern uint8_t purple[3];        // ��ɫ����+����
extern uint8_t yellow[3];        // ��ɫ����+�죩
// ����������ɫ
extern uint8_t orange[3];        // ��ɫ���϶��+�����̣�
extern uint8_t pink[3];          // ��ɫ����+��������
extern uint8_t magenta[3];       // Ʒ��ɫ����+�϶�����
extern uint8_t lime[3];          // ����ɫ����+�����죩
extern uint8_t sky_blue[3];      // ����ɫ���϶���+�����̣�
// ��ɫϵ�У���ͨ�����⣩
extern uint8_t dark_grey[3];     // ���ɫ
extern uint8_t grey[3];          // �л�ɫ
extern uint8_t light_grey[3];    // ǳ��ɫ

/**
 * @brief ��ʼ�� �ƴ�WS2812
 * 
 */
void Int_WS2812_Init(void);

/**
 * @brief �򿪵��� �ƴ�WS2812
 * 
 * @param index Ҫ�򿪵�LED����
 * @param color Ҫ�򿪵�LED��ɫ���飬RGB��ʽΪ{��ɫ,��ɫ,��ɫ}
 */
void Int_WS2812_OpenLED(uint32_t index,uint8_t color[3]);

/**
 * @brief �ر����� �ƴ�WS2812
 * 
 */
void Int_WS2812_CloseLED();

#endif /* __INT_WS2812_H__ */
