/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   ����ʶ��ģ��ʵ��
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� F103-�Ե� STM32 ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 
 
#include "stm32f10x.h"
#include "./usart/bsp_usart.h"
#include "./SysTick/bsp_SysTick.h"
#include "./paj7620u2/paj7620u2.h"
#include "bsp_exti.h"
/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{		
  /*��ʼ��USART ����ģʽΪ 115200 8-N-1���жϽ���*/
  USART_Config();
  EXTI_Key_Config(); 
  
  printf(" ��ӭʹ��Ұ�𿪷���\r\n");
  printf(" ����ʶ�𴫸���ʵ��\r\n");
  
  while(!paj7620u2_init())  //PAJ7620U2��������ʼ��
	{
	  printf("PAJ7620U2 ��ʼ��ʧ��!!!\r\n");
	  delay_ms(600);	
	}
    printf("PAJ7620U2 ��ʼ���ɹ�\r\n");
  while(1)
	{
		paj7620u2_sensor_test();//PAJ7620U2����������
	}	
 }
  
  
/*********************************************END OF FILE**********************/
