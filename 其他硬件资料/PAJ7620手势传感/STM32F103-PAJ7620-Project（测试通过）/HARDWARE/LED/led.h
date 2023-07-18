#ifndef __LED_H
#define __LED_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
// ������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
// STM32F103������ LED��������	   
// STM32ѧϰ�����⣬�����qȺ����: 643807576

//�Ĵ�����������Լ�棩
#define LED0 PBout(5)// PB5
#define LED1 PEout(5)// PE5	

/* ����LED����GPIO�ĺ� */
#define LED0_GPIO_PORT    	GPIOB			              
#define LED0_GPIO_CLK 	    RCC_APB2Periph_GPIOB		
#define LED0_GPIO_PIN		  	GPIO_Pin_5  
 
#define LED1_GPIO_PORT    	GPIOE		              
#define LED1_GPIO_CLK 	    RCC_APB2Periph_GPIOE		
#define LED1_GPIO_PIN		  	GPIO_Pin_5  

void LED_Init(void);//��ʼ��
	    
#endif
