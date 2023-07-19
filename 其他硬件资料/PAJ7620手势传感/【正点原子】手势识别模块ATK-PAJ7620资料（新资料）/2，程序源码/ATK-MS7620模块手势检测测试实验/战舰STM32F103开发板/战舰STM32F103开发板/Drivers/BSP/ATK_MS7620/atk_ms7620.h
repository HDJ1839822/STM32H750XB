/**
 ****************************************************************************************************
 * @file        atk_ms7620.h
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2022-06-21
 * @brief       ATK-MS7620ģ����������
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

#ifndef __ATK_MS7620_H
#define __ATK_MS7620_H

#include "./SYSTEM/sys/sys.h"

/* ATK-MS7620ģ��ģʽö�� */
typedef enum
{
    ATK_MS7620_MODE_PS = 0x00,  /* �ӽ����ģʽ */
    ATK_MS7620_MODE_GESTURE,    /* ���Ƽ��ģʽ */
} atk_ms7620_mode_t;

/* ATK-MS7620ģ������ö�� */
typedef enum
{
    ATK_MS7620_GESTURE_UP = 0x00,
    ATK_MS7620_GESTURE_DOWN,
    ATK_MS7620_GESTURE_LEFT,
    ATK_MS7620_GESTURE_RIGHT,
    ATK_MS7620_GESTURE_FORWARD,
    ATK_MS7620_GESTURE_BACKWARD,
    ATK_MS7620_GESTURE_CLOCKWISE,
    ATK_MS7620_GESTURE_ANTICLOCKWISE,
    ATK_MS7620_GESTURE_WAVE,
} atk_ms7620_gesture_t;

/* ����������� */
#define ATK_MS7620_EOK      0   /* û�д��� */
#define ATK_MS7620_ERROR    1   /* ͨ�ô��� */
#define ATK_MS7620_EINVAL   2   /* �������� */
#define ATK_MS7620_EACK     3   /* IICͨѶACK���� */

/* �������� */
uint8_t atk_ms7620_init(void);                                  /* ATK-MS7620ģ���ʼ�� */
uint8_t atk_ms7620_mode_config(atk_ms7620_mode_t mode);         /* ����ATK-MS7620ģ���ģʽ */
uint8_t atk_ms7620_get_obj_brightness(uint8_t *brightness);     /* ATK-MS7620ģ���ȡ�������� */
uint8_t atk_ms7620_get_obj_size(uint16_t *size);                /* ATK-MS7620ģ���ȡ�����С */
uint8_t atk_ms7620_get_gesture(atk_ms7620_gesture_t *gesture);  /* ATK-MS7620ģ���ȡ���� */

#endif
