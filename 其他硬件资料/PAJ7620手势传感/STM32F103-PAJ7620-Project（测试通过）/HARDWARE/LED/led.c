#include "led.h"

//STM32ѧϰ�����⣬�����qȺ����: 643807576
//��ʼ��PB5��PE5Ϊ�����.��ʹ���������ڵ�ʱ��		    
//LED GPIO��ʼ��
void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(LED0_GPIO_CLK|LED1_GPIO_CLK, ENABLE);	 //ʹ��LED0��LED1�˿ڵ�ʱ��
	
 GPIO_InitStructure.GPIO_Pin = LED0_GPIO_PIN;				 //LED0�˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 				//�����趨������ʼ��GPIO
 GPIO_SetBits(LED0_GPIO_PORT,LED0_GPIO_PIN);						 //LED0�����

 GPIO_InitStructure.GPIO_Pin = LED1_GPIO_PIN;	    		 //LED1�˿�����, �������
 GPIO_Init(GPIOE, &GPIO_InitStructure);	  				 			//������� ��IO���ٶ�Ϊ50MHz
 GPIO_SetBits(LED1_GPIO_PORT,LED1_GPIO_PIN); 						 //LED1�����
}
 
