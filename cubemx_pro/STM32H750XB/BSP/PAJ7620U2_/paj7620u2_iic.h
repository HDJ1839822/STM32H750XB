#ifndef __PAJ7620U2_IIC_H
#define __PAJ7620U2_IIC_H
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//STM32F10x������  PAJ7620������ģ��IIC����	��ģ��I2C��������ͷ�ļ���
//STM32ѧϰ�����⣬�����qȺ����: 643807576


#include "stdint.h"
#include "gpio.h"


extern uint16_t  tick_us_count;


void SDA_IN(void);
void SDA_OUT(void);
uint8_t READ_SDA(void);
void delay_us(uint16_t us);
//void delay_us(uint32_t us);



//void delay_us(uint32_t us)
//{
//    uint32_t startTicks = tick_us_count;

//    // ��ʱ�ȴ�
//    while ((tick_us_count - startTicks) < us);
//}

///**
//PH11  SCL
//PH12  SDA
//*/

//void SDA_IN(void)
//{
//  GPIO_InitTypeDef GPIO_InitStruct = {0};
//	__HAL_RCC_GPIOH_CLK_ENABLE();
//	//HAL_GPIO_WritePin(GPIOH, SCL_PAJ7620_Pin|SDA_PAJ7620_Pin, GPIO_PIN_SET);
//	/*Configure GPIO pins : PHPin PHPin */
//  GPIO_InitStruct.Pin = SDA_PAJ7620_Pin;
//  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
//  GPIO_InitStruct.Pull = GPIO_NOPULL;
//  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
//  HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);
//}

//void SDA_OUT(void)
//{
//  GPIO_InitTypeDef GPIO_InitStruct = {0};
//	__HAL_RCC_GPIOH_CLK_ENABLE();
//	//HAL_GPIO_WritePin(GPIOH, SCL_PAJ7620_Pin|SDA_PAJ7620_Pin, GPIO_PIN_SET);
//	/*Configure GPIO pins : PHPin PHPin */
//  GPIO_InitStruct.Pin = SDA_PAJ7620_Pin;
//  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//  GPIO_InitStruct.Pull = GPIO_PULLUP;
//  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
//  HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);
//}


//uint8_t READ_SDA(void)
//{
// 
//return HAL_GPIO_ReadPin(GPIOH,SDA_PAJ7620_Pin);


//}








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


