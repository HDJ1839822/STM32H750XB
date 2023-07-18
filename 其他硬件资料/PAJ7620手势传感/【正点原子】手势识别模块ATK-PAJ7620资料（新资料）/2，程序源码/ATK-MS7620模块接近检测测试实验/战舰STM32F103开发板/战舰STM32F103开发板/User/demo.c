/**
 ****************************************************************************************************
 * @file        demo.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2022-06-21
 * @brief       ATK-MS7620ģ��ӽ�������ʵ��
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
    uint8_t brightness;
    uint16_t size;
    
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
    
    /* ����ATK-MS7620ģ��Ϊ�ӽ����ģʽ */
    ret = atk_ms7620_mode_config(ATK_MS7620_MODE_PS);
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
        /* ��ȡ�������Ⱥʹ�С */
        ret  = atk_ms7620_get_obj_brightness(&brightness);
        ret += atk_ms7620_get_obj_size(&size);
        if (ret == ATK_MS7620_EOK)
        {
            printf("Object brightness: %d, size: %d\r\n", brightness, size);
        }
    }
}
