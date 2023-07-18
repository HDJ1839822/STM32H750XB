/**
 ****************************************************************************************************
 * @file        atk_ms7620.c
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

#include "./BSP/ATK_MS7620/atk_ms7620.h"
#include "./BSP/ATK_MS7620/atk_ms7620_iic.h"
#include "./SYSTEM/delay/delay.h"
#include "./SYSTEM/usart/usart.h"

/* ATK-MS7620ģ��IICͨѶ��ַ */
#define ATK_MS7620_IIC_ADDR                 0x73

/* ATK-MS7620ģ��Ĵ�����ַ */
#define ATK_MS7620_REG_BANK_SEL             0xEF
#define ATK_MS7620_REG_00                   0x00
#define ATK_MS7620_REG_OBJ_BRIGHTNESS       0xB0
#define ATK_MS7620_REG_OBJ_SIZE_1           0xB1
#define ATK_MS7620_REG_OBJ_SIZE_2           0xB2
#define ATK_MS7620_REG_INT_FLAG_1           0x43
#define ATK_MS7620_REG_INT_FLAG_2           0x44

/* ATK-MS7620ģ�������жϱ�ʶ */
#define ATK_MS7620_GES_UP_FLAG              (1 << 0)
#define ATK_MS7620_GES_DOWN_FLAG            (1 << 1)
#define ATK_MS7620_GES_LEFT_FLAG            (1 << 2)
#define ATK_MS7620_GES_RIGHT_FLAG           (1 << 3)
#define ATK_MS7620_GES_FORWARD_FLAG         (1 << 4)
#define ATK_MS7620_GES_BACKWARD_FLAG        (1 << 5)
#define ATK_MS7620_GES_CLOCKWISE_FLAG       (1 << 6)
#define ATK_MS7620_GES_ANTICLOCKWISE_FLAG   (1 << 7)
#define ATK_MS7620_GES_WAVE_FLAG            (1 << 8)

/* ATK-MS7620ģ��Ĵ���Bankö�� */
typedef enum
{
    ATK_MS7620_BANK0 = 0x00,
    ATK_MS7620_BANK1 = 0x01,
} atk_ms7620_bank_t;

/* ��ʼ���������� */
static uint8_t initial_register_array[][2] = {
    {0xEF, 0x00},
    {0x37, 0x07},
    {0x38, 0x17},
    {0x39, 0x06},
    {0x41, 0x00},
    {0x42, 0x00},
    {0x46, 0x2D},
    {0x47, 0x0F},
    {0x48, 0x3C},
    {0x49, 0x00},
    {0x4A, 0x1E},
    {0x4C, 0x20},
    {0x51, 0x10},
    {0x5E, 0x10},
    {0x60, 0x27},
    {0x80, 0x42},
    {0x81, 0x44},
    {0x82, 0x04},
    {0x8B, 0x01},
    {0x90, 0x06},
    {0x95, 0x0A},
    {0x96, 0x0C},
    {0x97, 0x05},
    {0x9A, 0x14},
    {0x9C, 0x3F},
    {0xA5, 0x19},
    {0xCC, 0x19},
    {0xCD, 0x0B},
    {0xCE, 0x13},
    {0xCF, 0x64},
    {0xD0, 0x21},
    {0xEF, 0x01},
    {0x02, 0x0F},
    {0x03, 0x10},
    {0x04, 0x02},
    {0x25, 0x01},
    {0x27, 0x39},
    {0x28, 0x7F},
    {0x29, 0x08},
    {0x3E, 0xFF},
    {0x5E, 0x3D},
    {0x65, 0x96},
    {0x67, 0x97},
    {0x69, 0xCD},
    {0x6A, 0x01},
    {0x6D, 0x2C},
    {0x6E, 0x01},
    {0x72, 0x01},
    {0x73, 0x35},
    {0x74, 0x00},
    {0x77, 0x01}
};

/* PSģʽ�������� */
static uint8_t change_to_proximity_register_array[][2] = {
    {0xEF, 0x00},
    {0x41, 0x00},
    {0x42, 0x00},
    {0x48, 0x3C},
    {0x49, 0x00},
    {0x51, 0x13},
    {0x83, 0x20},
    {0x84, 0x20},
    {0x85, 0x00},
    {0x86, 0x10},
    {0x87, 0x00},
    {0x88, 0x05},
    {0x89, 0x18},
    {0x8A, 0x10},
    {0x9F, 0xF8},
    {0x69, 0x96},
    {0x6A, 0x02},
    {0xEF, 0x01},
    {0x01, 0x1E},
    {0x02, 0x0F},
    {0x03, 0x10},
    {0x04, 0x02},
    {0x41, 0x50},
    {0x43, 0x34},
    {0x65, 0xCE},
    {0x66, 0x0B},
    {0x67, 0xCE},
    {0x68, 0x0B},
    {0x69, 0xE9},
    {0x6A, 0x05},
    {0x6B, 0x50},
    {0x6C, 0xC3},
    {0x6D, 0x50},
    {0x6E, 0xC3},
    {0x74, 0x05}
};

/* Gestureģʽ�������� */
static uint8_t change_to_gesture_register_array[][2] = {
    {0xEF, 0x00},
    {0x41, 0x00},
    {0x42, 0x00},
    {0xEF, 0x00},
    {0x48, 0x3C},
    {0x49, 0x00},
    {0x51, 0x10},
    {0x83, 0x20},
    {0x9F, 0xF9},
    {0xEF, 0x01},
    {0x01, 0x1E},
    {0x02, 0x0F},
    {0x03, 0x10},
    {0x04, 0x02},
    {0x41, 0x40},
    {0x43, 0x30},
    {0x65, 0x96},
    {0x66, 0x00},
    {0x67, 0x97},
    {0x68, 0x01},
    {0x69, 0xCD},
    {0x6A, 0x01},
    {0x6B, 0xB0},
    {0x6C, 0x04},
    {0x6D, 0x2C},
    {0x6E, 0x01},
    {0x74, 0x00},
    {0xEF, 0x00},
    {0x41, 0xFF},
    {0x42, 0x01}
};

/**
 * @brief       ������ȡATK-MS7620ָ���Ĵ�����ֵ
 * @param       addr: ATK-MS7620��IICͨѶ��ַ
 *              reg : ATK-MS7620�Ĵ�����ַ
 *              len : ��ȡ�ĳ���
 *              dat : ��Ŷ�ȡ�������ݵĵ�ַ
 * @retval      ATK_MS7620_EOK : ����ִ�гɹ�
 *              ATK_MS7620_EACK: IICͨѶACK���󣬺���ִ��ʧ��
 */
static uint8_t atk_ms7620_read(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *dat)
{
    atk_ms7620_iic_start();
    atk_ms7620_iic_send_byte((addr << 1) | 0);
    if (atk_ms7620_iic_wait_ack() == 1)
    {
        atk_ms7620_iic_stop();
        return ATK_MS7620_EACK;
    }
    atk_ms7620_iic_send_byte(reg);
    if (atk_ms7620_iic_wait_ack() == 1)
    {
        atk_ms7620_iic_stop();
        return ATK_MS7620_EACK;
    }
    atk_ms7620_iic_start();
    atk_ms7620_iic_send_byte((addr << 1) | 1);
    if (atk_ms7620_iic_wait_ack() == 1)
    {
        atk_ms7620_iic_stop();
        return ATK_MS7620_EACK;
    }
    while (len)
    {
        *dat = atk_ms7620_iic_read_byte((len > 1) ? 1 : 0);
        len--;
        dat++;
    }
    atk_ms7620_iic_stop();
    return ATK_MS7620_EOK;
}

/**
 * @brief       ��ATK-MS7620��ָ���Ĵ�������д��ָ������
 * @param       addr: ATK-MS7620��IICͨѶ��ַ
 *              reg : ATK-MS7620�Ĵ�����ַ
 *              len : д��ĳ���
 *              dat : д�������
 * @retval      ATK_MS7620_EOK : ����ִ�гɹ�
 *              ATK_MS7620_EACK: IICͨѶACK���󣬺���ִ��ʧ��
 */
static uint8_t atk_ms7620_write(uint8_t addr,uint8_t reg, uint8_t len, uint8_t *dat)
{
    uint8_t i;
    
    atk_ms7620_iic_start();
    atk_ms7620_iic_send_byte((addr << 1) | 0);
    if (atk_ms7620_iic_wait_ack() == 1)
    {
        atk_ms7620_iic_stop();
        return ATK_MS7620_EACK;
    }
    atk_ms7620_iic_send_byte(reg);
    if (atk_ms7620_iic_wait_ack() == 1)
    {
        atk_ms7620_iic_stop();
        return ATK_MS7620_EACK;
    }
    for (i=0; i<len; i++)
    {
        atk_ms7620_iic_send_byte(dat[i]);
        if (atk_ms7620_iic_wait_ack() == 1)
        {
            atk_ms7620_iic_stop();
            return ATK_MS7620_EACK;
        }
    }
    atk_ms7620_iic_stop();
    return ATK_MS7620_EOK;
}

/**
 * @brief       ��ȡATK-MS7620ָ���Ĵ�����ֵ
 * @param       addr: ATK-MS7620��IICͨѶ��ַ
 *              reg : ATK-MS7620�Ĵ�����ַ
 *              dat : ��ȡ���ļĴ�����ֵ
 * @retval      ATK_MS7620_EOK : ����ִ�гɹ�
 *              ATK_MS7620_EACK: IICͨѶACK���󣬺���ִ��ʧ��
 */
static inline uint8_t atk_ms7620_read_byte(uint8_t addr, uint8_t reg, uint8_t *dat)
{
    return atk_ms7620_read(addr, reg, 1, dat);
}

/**
 * @brief       ��ATK-MS7620��ָ���Ĵ���д��һ�ֽ�����
 * @param       addr: ATK-MS7620��IICͨѶ��ַ
 *              reg : ATK-MS7620�Ĵ�����ַ
 *              dat : д�������
 * @retval      ATK_MS7620_EOK : ����ִ�гɹ�
 *              ATK_MS7620_EACK: IICͨѶACK���󣬺���ִ��ʧ��
 */
static inline uint8_t atk_ms7620_write_byte(uint8_t addr, uint8_t reg, uint8_t dat)
{
    return atk_ms7620_write(addr, reg, 1, &dat);
}

/**
 * @brief       �л�ATK-MS7620�Ĵ���Bank
 * @param       bank: ATK_MS7620_BANK0: �Ĵ���Bank0
 *                    ATK_MS7620_BANK1: �Ĵ���Bank1
 * @retval      ��
 */
static void atk_ms7620_switch_reg_bank(atk_ms7620_bank_t bank)
{
    switch (bank)
    {
        case ATK_MS7620_BANK0:
        case ATK_MS7620_BANK1:
        {
            break;
        }
        default:
        {
            return;
        }
    }
    
    atk_ms7620_write_byte(ATK_MS7620_IIC_ADDR, ATK_MS7620_REG_BANK_SEL, bank);
}

/**
 * @brief       ��ȡATK-MS7620ģ�黽��״̬
 * @param       ��
 * @retval      ATK_MS7620_EOK  : ATK-MS7620ģ���ѻ���
 *              ATK_MS7620_ERROR: ATK-MS7620ģ��δ����
 */
static uint8_t atk_ms7620_get_wakeup_status(void)
{
    uint8_t reg00;
    
    atk_ms7620_switch_reg_bank(ATK_MS7620_BANK0);
    atk_ms7620_read_byte(ATK_MS7620_IIC_ADDR, ATK_MS7620_REG_00, &reg00);
    if (reg00 != 0x20)
    {
        return ATK_MS7620_ERROR;
    }
    
    return ATK_MS7620_EOK;
}

/**
 * @brief       ATK-MS7620ģ���ʼ���Ĵ�������
 * @param       ��
 * @retval      ��
 */
static void atk_ms7620_initial_register(void)
{
    uint8_t array_index;
    
    for (array_index=0; array_index<(sizeof(initial_register_array)/sizeof(initial_register_array[0])); array_index++)
    {
        atk_ms7620_write_byte(ATK_MS7620_IIC_ADDR, initial_register_array[array_index][0], initial_register_array[array_index][1]);
    }
}

/**
 * @brief       ATK-MS7620ģ���ʼ��
 * @param       ��
 * @retval      ATK_MS7620_EOK  : ATK-MS7620ģ���ʼ���ɹ�
 *              ATK_MS7620_ERROR: ATK-MS7620ģ���ʼ��ʧ��
 */
uint8_t atk_ms7620_init(void)
{
    uint8_t ret;
    
    delay_ms(1);                            /* �ȴ�����700us */
    atk_ms7620_iic_init();                  /* ��ʼ��IIC�ӿ� */
    ret = atk_ms7620_get_wakeup_status();   /* ��ȡ����״̬ */
    if (ret != ATK_MS7620_EOK)
    {
        return ATK_MS7620_ERROR;
    }
    atk_ms7620_initial_register();          /* ��ʼ���Ĵ������� */
    
    return ATK_MS7620_EOK;
}

/**
 * @brief       ����ATK-MS7620ģ���ģʽ
 * @param       mode: ATK_MS7620_MODE_PS     : �ӽ����ģʽ
 *                    ATK_MS7620_MODE_GESTURE: ���Ƽ��ģʽ
 * @retval      ATK_MS7620_EOK   : ģʽ���óɹ�
 *              ATK_MS7620_EINVAL: ������������
 */
uint8_t atk_ms7620_mode_config(atk_ms7620_mode_t mode)
{
    uint8_t (*array)[2];
    uint8_t array_size;
    uint8_t array_index;
    
    switch (mode)
    {
        case ATK_MS7620_MODE_PS:
        {
            array = change_to_proximity_register_array;
            array_size = sizeof(change_to_proximity_register_array) / sizeof(change_to_proximity_register_array[0]);
            break;
        }
        case ATK_MS7620_MODE_GESTURE:
        {
            array = change_to_gesture_register_array;
            array_size = sizeof(change_to_gesture_register_array) / sizeof(change_to_gesture_register_array[0]);
            break;
        }
        default:
        {
            return ATK_MS7620_EINVAL;
        }
    }
    
    for (array_index=0; array_index<array_size; array_index++)
    {
        atk_ms7620_write_byte(ATK_MS7620_IIC_ADDR, array[array_index][0], array[array_index][1]);
    }
    
    return ATK_MS7620_EOK;
}

/**
 * @brief       ATK-MS7620ģ���ȡ��������
 * @param       brightness: �������ȣ���Χ0~255
 * @retval      ATK_MS7620_EOK   : ��ȡ�������ȳɹ�
 *              ATK_MS7620_ERROR : ��ȡ��������ʧ��
 *              ATK_MS7620_EINVAL: ������������
 */
uint8_t atk_ms7620_get_obj_brightness(uint8_t *brightness)
{
    uint8_t ret;
    uint8_t _brightness[1];
    
    if (brightness == NULL)
    {
        return ATK_MS7620_EINVAL;
    }
    
    atk_ms7620_switch_reg_bank(ATK_MS7620_BANK0);
    ret = atk_ms7620_read_byte(ATK_MS7620_IIC_ADDR, ATK_MS7620_REG_OBJ_BRIGHTNESS, _brightness);
    if (ret != ATK_MS7620_EOK)
    {
        return ATK_MS7620_ERROR;
    }
    
    *brightness = _brightness[0];
    
    return ATK_MS7620_EOK;
}

/**
 * @brief       ATK-MS7620ģ���ȡ�����С
 * @param       size: �����С����Χ0~900
 * @retval      ATK_MS7620_EOK   : ��ȡ�����С�ɹ�
 *              ATK_MS7620_ERROR : ��ȡ�����Сʧ��
 *              ATK_MS7620_EINVAL: ������������
 */
uint8_t atk_ms7620_get_obj_size(uint16_t *size)
{
    uint8_t ret;
    uint8_t _size[2];
    
    if (size == NULL)
    {
        return ATK_MS7620_EINVAL;
    }
    
    atk_ms7620_switch_reg_bank(ATK_MS7620_BANK0);
    ret  = atk_ms7620_read_byte(ATK_MS7620_IIC_ADDR, ATK_MS7620_REG_OBJ_SIZE_1, &_size[0]);
    ret += atk_ms7620_read_byte(ATK_MS7620_IIC_ADDR, ATK_MS7620_REG_OBJ_SIZE_2, &_size[1]);
    
    if (ret != ATK_MS7620_EOK)
    {
        return ATK_MS7620_ERROR;
    }
    
    *size = (((uint16_t)_size[1] << 8) & 0x0F00) | _size[0];
    
    return ATK_MS7620_EOK;
}

/**
 * @brief       ATK-MS7620ģ���ȡ����
 * @param       gesture: ����
 * @retval      ATK_MS7620_EOK   : ��ȡ���Ƴɹ�
 *              ATK_MS7620_ERROR : ��ȡ����ʧ��
 *              ATK_MS7620_EINVAL: ������������
 */
uint8_t atk_ms7620_get_gesture(atk_ms7620_gesture_t *gesture)
{
    uint8_t ret;
    union
    {
        uint8_t byte[2];
        uint16_t halfword;
    }_flag;
    
    if (gesture == NULL)
    {
        return ATK_MS7620_EINVAL;
    }
    
    atk_ms7620_switch_reg_bank(ATK_MS7620_BANK0);
    ret = atk_ms7620_read_byte(ATK_MS7620_IIC_ADDR, ATK_MS7620_REG_INT_FLAG_1, &_flag.byte[0]);
    ret += atk_ms7620_read_byte(ATK_MS7620_IIC_ADDR, ATK_MS7620_REG_INT_FLAG_2, &_flag.byte[1]);
    if (ret != ATK_MS7620_EOK)
    {
        return ATK_MS7620_ERROR;
    }
    
    switch (_flag.halfword)
    {
        case ATK_MS7620_GES_UP_FLAG:
        {
            *gesture = ATK_MS7620_GESTURE_UP;
            break;
        }
        case ATK_MS7620_GES_DOWN_FLAG:
        {
            *gesture = ATK_MS7620_GESTURE_DOWN;
            break;
        }
        case ATK_MS7620_GES_LEFT_FLAG:
        {
            *gesture = ATK_MS7620_GESTURE_LEFT;
            break;
        }
        case ATK_MS7620_GES_RIGHT_FLAG:
        {
            *gesture = ATK_MS7620_GESTURE_RIGHT;
            break;
        }
        case ATK_MS7620_GES_FORWARD_FLAG:
        {
            *gesture = ATK_MS7620_GESTURE_FORWARD;
            break;
        }
        case ATK_MS7620_GES_BACKWARD_FLAG:
        {
            *gesture = ATK_MS7620_GESTURE_BACKWARD;
            break;
        }
        case ATK_MS7620_GES_CLOCKWISE_FLAG:
        {
            *gesture = ATK_MS7620_GESTURE_CLOCKWISE;
            break;
        }
        case ATK_MS7620_GES_ANTICLOCKWISE_FLAG:
        {
            *gesture = ATK_MS7620_GESTURE_ANTICLOCKWISE;
            break;
        }
        case ATK_MS7620_GES_WAVE_FLAG:
        {
            *gesture = ATK_MS7620_GESTURE_WAVE;
            break;
        }
        default:
        {
            return ATK_MS7620_ERROR;
        }
    }
    
    return ATK_MS7620_EOK;
}
