#include "Int_WS2812.h"

/* ���弸�ֳ�����ɫ��GRB˳����-��-���� */
uint8_t black[3] = {0, 0, 0};         // ��ɫ������ɫ��
uint8_t white[3] = {255, 255, 255};   // ��ɫ����ͨ��ȫ����
// ������ԭɫ
uint8_t red[3] = {0, 255, 0};         // ��ɫ������ɫͨ����
uint8_t green[3] = {255, 0, 0};       // ��ɫ������ɫͨ����
uint8_t blue[3] = {0, 0, 255};        // ��ɫ������ɫͨ����
// ����ɫ
uint8_t cyan[3] = {255, 0, 255};      // ��ɫ����+����
uint8_t purple[3] = {0, 255, 255};    // ��ɫ����+����
uint8_t yellow[3] = {255, 255, 0};    // ��ɫ����+�죩
// ����������ɫ
uint8_t orange[3] = {100, 255, 0};    // ��ɫ���϶��+�����̣�
uint8_t pink[3] = {0, 255, 100};      // ��ɫ����+��������
uint8_t magenta[3] = {0, 255, 200};   // Ʒ��ɫ����+�϶�����
uint8_t lime[3] = {255, 100, 0};      // ����ɫ����+�����죩
uint8_t sky_blue[3] = {200, 0, 255};  // ����ɫ���϶���+�����̣�
// ��ɫϵ�У���ͨ�����⣩
uint8_t dark_grey[3] = {50, 50, 50};  // ���ɫ
uint8_t grey[3] = {128, 128, 128};    // �л�ɫ
uint8_t light_grey[3] = {200, 200, 200}; // ǳ��ɫ

// LED�ƴ����
led_strip_handle_t led_strip;

/**
 * @brief ��ʼ�� �ƴ�WS2812
 *
 */
void Int_WS2812_Init(void)
{
    // LED �ƴ�����
    led_strip_config_t strip_config = {
        .strip_gpio_num = WS2812_GPIO_PIN,                           // ���ӵ� LED ���������ߵ� GPIO
        .max_leds = 12,                                              // �ƴ��е� LED ����
        .led_model = LED_MODEL_WS2812,                               // LED�ƴ��ͺ�
        .color_component_format = LED_STRIP_COLOR_COMPONENT_FMT_RGB, // ������ɫ˳��RGB
        .flags = {
            .invert_out = false, // ��Ҫ��ת����ź�
        }};

    //  LED�ƴ�������ã�RMT
    led_strip_rmt_config_t rmt_config = {
        .clk_src = RMT_CLK_SRC_DEFAULT,    // ��ͬ��ʱ��Դ�ᵼ�²�ͬ�Ĺ���
        .resolution_hz = 10 * 1000 * 1000, // RMT ������ʱ��Ƶ��
        .mem_block_symbols = 0,            //  RMT ͨ��ʹ�õ��ڴ���С ����Ϊ 0 ��������ʹ��Ĭ�ϴ�С
        .flags = {
            .with_dma = 0, // ʹ�� DMA ��������������� LED ʱ������
        }};

    // ���ر���LED�ƴ����
    led_strip_new_rmt_device(&strip_config, &rmt_config, &led_strip);
}

/**
 * @brief �򿪵��� �ƴ�WS2812
 *
 * @param index Ҫ�򿪵�LED����
 * @param color Ҫ�򿪵�LED��ɫ
 */
void Int_WS2812_OpenLED(uint32_t index, uint8_t color[3])
{
    led_strip_set_pixel(led_strip, index, color[0], color[1], color[2]);
    led_strip_refresh(led_strip);
}

/**
 * @brief �ر����� �ƴ�WS2812
 *
 */
void Int_WS2812_CloseLED()
{
    led_strip_clear(led_strip);
    led_strip_refresh(led_strip);
}
