#ifndef __PAJ7620U2_IIC_H
#define __PAJ7620U2_IIC_H

#include "stm32f10x.h"


/* �����дSCL��SDA�ĺ꣬�����Ӵ���Ŀ���ֲ�ԺͿ��Ķ��� */
#if 0	/* �������룺 1 ѡ��GPIO�Ŀ⺯��ʵ��IO��д */
	#define GS_I2C_SCL_1()  GPIO_SetBits(GS_I2C_GPIO_PORT,  GS_I2C_SCL_PIN)		/* SCL = 1 */
	#define GS_I2C_SCL_0()  GPIO_ResetBits(GS_I2C_GPIO_PORT,  GS_I2C_SCL_PIN)		/* SCL = 0 */
	
	#define GS_I2C_SDA_1()  GPIO_SetBits(GS_I2C_GPIO_PORT, GS_I2C_SDA_PIN)		/* SDA = 1 */
	#define GS_I2C_SDA_0()  GPIO_ResetBits(GS_I2C_GPIO_PORT, GS_I2C_SDA_PIN)		/* SDA = 0 */
	
	#define GS_I2C_SDA_READ()  GPIO_ReadInputDataBit(GS_I2C_GPIO_PORT, GS_I2C_SDA_PIN)	/* ��SDA����״̬ */
#else	/* �����֧ѡ��ֱ�ӼĴ�������ʵ��IO��д */
    /*��ע�⣺����д������IAR��߼����Ż�ʱ���ᱻ�����������Ż� */
	#define GS_I2C_SCL_1()  GS_I2C_GPIO_PORT->BSRR =  GS_I2C_SCL_PIN				/* SCL = 1 */
	#define GS_I2C_SCL_0()  GS_I2C_GPIO_PORT->BRR =  GS_I2C_SCL_PIN				/* SCL = 0 */
	
	#define GS_I2C_SDA_1()  GS_I2C_GPIO_PORT->BSRR = GS_I2C_SDA_PIN				/* SDA = 1 */
	#define GS_I2C_SDA_0()  GS_I2C_GPIO_PORT->BRR = GS_I2C_SDA_PIN				/* SDA = 0 */
	
	#define GS_I2C_SDA_READ()  ((GS_I2C_GPIO_PORT->IDR & GS_I2C_SDA_PIN) != 0)	/* ��SDA����״̬ */
#endif

  #define GS_SDA_IN()  {GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=8<<12;}
  #define GS_SDA_OUT() {GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=3<<12;}

/* ����I2C�������ӵ�GPIO�˿�, �û�ֻ��Ҫ�޸�����4�д��뼴������ı�SCL��SDA������ */
#define GS_I2C_GPIO_PORT				GPIOB			/* GPIO�˿� */
#define GS_I2C_GPIO_CLK			  	RCC_APB2Periph_GPIOB		/* GPIO�˿�ʱ�� */
#define GS_I2C_SCL_PIN					GPIO_Pin_10			/* ���ӵ�SCLʱ���ߵ�GPIO */
#define GS_I2C_SDA_PIN					GPIO_Pin_11/* ���ӵ�SDA�����ߵ�GPIO */

uint8_t GS_Write_Byte(uint8_t REG_Address,uint8_t REG_data);
uint8_t GS_Read_Byte(uint8_t REG_Address);
uint8_t GS_Read_nByte(uint8_t REG_Address,uint16_t len,uint8_t *buf);
void GS_i2c_init(void);
void GS_WakeUp(void);

#endif


