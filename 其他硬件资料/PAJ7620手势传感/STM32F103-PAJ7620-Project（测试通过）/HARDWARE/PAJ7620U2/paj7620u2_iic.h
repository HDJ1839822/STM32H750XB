#ifndef __PAJ7620U2_IIC_H
#define __PAJ7620U2_IIC_H
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//STM32F10x������  PAJ7620������ģ��IIC����	��ģ��I2C��������ͷ�ļ���
//STM32ѧϰ�����⣬�����qȺ����: 643807576

#include "sys.h"

//GPIO�������ã������� PF7��ģ���SCL�š�PF8��ģ���SDA�ţ�	 
#define GS_IIC_SCL    PFout(7) 		//SCL��SCL_OUT��
#define GS_IIC_SDA    PFout(8) 		//SDA_OUT�����ڷ���SDA���ݸ�IIC������ģ��
#define GS_READ_SDA   PFin(8) 		//SDA_IN�����ڶ�ȡIIC������ģ���SDA����

//I/O��������(�Ĵ�������).
#define GS_SDA_IN()  {GPIOF->CRH&=0xFFFFFFF0;GPIOF->CRH|=4<<0;}	//PF8  ��������
#define GS_SDA_OUT() {GPIOF->CRH&=0xFFFFFFF0;GPIOF->CRH|=3<<0;}	//PF8  ���������ͨ�ã�

//PS:  I/O���򲻻�����? :ɵ��ʽ���� ---> https://xinso.blog.csdn.net/article/details/115862486

u8 GS_Write_Byte(u8 REG_Address,u8 REG_data);
u8 GS_Read_Byte(u8 REG_Address);
u8 GS_Read_nByte(u8 REG_Address,u16 len,u8 *buf);
void GS_i2c_init(void);
void GS_WakeUp(void);

#endif


