#ifndef __PAJ7620U2_IIC_H
#define __PAJ7620U2_IIC_H

#include "stdint.h"
#include "gpio.h"


extern uint16_t  tick_us_count;

void SDA_IN(void);
void SDA_OUT(void);
uint8_t READ_SDA(void);
void delay_us(uint16_t us);


///**
//PH11  SCL
//PH12  SDA
//*/


//GPIO�������ã������� PF7��ģ���SCL�š�PF8��ģ���SDA�ţ�	 
#define GS_IIC_SCL(n)    ((n)>0 ? HAL_GPIO_WritePin(GPIOH, SCL_PAJ7620_Pin, GPIO_PIN_SET)\
                             : HAL_GPIO_WritePin(GPIOH, SCL_PAJ7620_Pin, GPIO_PIN_RESET))		//SCL��SCL_OUT�� 


#define GS_IIC_SDA(n)    ((n)>0 ? HAL_GPIO_WritePin(GPIOH, SDA_PAJ7620_Pin, GPIO_PIN_SET)\
                                : HAL_GPIO_WritePin(GPIOH, SDA_PAJ7620_Pin, GPIO_PIN_RESET))		//SCL��SCL_OUT��  


//#define GS_IIC_SDA    GS_SDA_OUT() 		//SDA_OUT�����ڷ���SDA���ݸ�IIC������ģ��
//#define GS_READ_SDA    READ_SDA(); 		//SDA_IN�����ڶ�ȡIIC������ģ���SDA����

//I/O��������(�Ĵ�������).
#define GS_SDA_IN()  SDA_IN();	  //PH12  ��������
#define GS_SDA_OUT() SDA_OUT();	//PH12  ���������ͨ�ã�

//PS:  I/O���򲻻�����? :ɵ��ʽ���� ---> https://xinso.blog.csdn.net/article/details/115862486

uint8_t GS_Write_Byte(uint8_t REG_Address,uint8_t REG_data);
uint8_t GS_Read_Byte(uint8_t REG_Address);
uint8_t GS_Read_nByte(uint8_t REG_Address,uint16_t len,uint8_t *buf);
void GS_i2c_init(void);
void GS_WakeUp(void);

#endif


