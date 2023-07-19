/**
 ****************************************************************************************************
 * @file        demo.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2022-06-21
 * @brief       ATK-MS7620ģ�����Ƽ�����ʵ��
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� STM32F103������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
 *
 ****************************************************************************************************
 */

#include "demo.h"
#include "./BSP/ATK_MS7620/atk_ms7620.h"
#include "./BSP/LED/led.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"

/**
 * @brief       ������ʾ��ں���
 * @param       ��
 * @retval      ��
 */
void demo_run(void)
{
    uint8_t ret;
    atk_ms7620_gesture_t gesture;
    
    /* ��ʼ��ATK-MS7620ģ�� */
    ret = atk_ms7620_init();
    if (ret != 0)
    {
        printf("ATK-MS7620 init failed!\r\n");
        while (1)
        {
            LED0_TOGGLE();
            delay_ms(200);
        }
    }
    
    /* ����ATK-MS7620ģ��Ϊ���Ƽ��ģʽ */
    ret = atk_ms7620_mode_config(ATK_MS7620_MODE_GESTURE);
    if (ret != 0)
    {
        printf("ATK_MS7620 config failed!\r\n");
        while (1)
        {
            LED0_TOGGLE();
            delay_ms(200);
        }
    }
    
    printf("ATK-MS7620 config succedded!\r\n");
    
    while (1)
    {
        /* ��ȡ���� */
        ret = atk_ms7620_get_gesture(&gesture);
        if (ret == ATK_MS7620_EOK)
        {
            switch (gesture)
            {
                case ATK_MS7620_GESTURE_UP:
                {
                    printf("Gesture: Up\r\n");
                    break;
                }
                case ATK_MS7620_GESTURE_DOWN:
                {
                    printf("Gesture: Down\r\n");
                    break;
                }
                case ATK_MS7620_GESTURE_LEFT:
                {
                    printf("Gesture: Left\r\n");
                    break;
                }
                case ATK_MS7620_GESTURE_RIGHT:
                {
                    printf("Gesture: Right\r\n");
                    break;
                }
                case ATK_MS7620_GESTURE_FORWARD:
                {
                    printf("Gesture: Forward\r\n");
                    break;
                }
                case ATK_MS7620_GESTURE_BACKWARD:
                {
                    printf("Gesture: Backward\r\n");
                    break;
                }
                case ATK_MS7620_GESTURE_CLOCKWISE:
                {
                    printf("Gesture: Clockwise\r\n");
                    break;
                }
                case ATK_MS7620_GESTURE_ANTICLOCKWISE:
                {
                    printf("Gesture: Anticlockwise\r\n");
                    break;
                }
                case ATK_MS7620_GESTURE_WAVE:
                {
                    printf("Gesture: Wave\r\n");
                    break;
                }
                default:
                {
                    break;
                }
            }
        }
    }
}
