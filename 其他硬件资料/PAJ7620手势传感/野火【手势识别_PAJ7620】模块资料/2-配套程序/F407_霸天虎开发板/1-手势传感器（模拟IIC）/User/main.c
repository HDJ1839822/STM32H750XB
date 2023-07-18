/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   ADC����
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  STM32 F429 ������
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "stm32f4xx.h"
#include "./usart/bsp_debug_usart.h"	
#include "./SysTick/bsp_SysTick.h"
#include "./paj7620u2/paj7620u2.h"
#include "./key/bsp_exti.h"
/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{		
  /*��ʼ��USART ����ģʽΪ 115200 8-N-1���жϽ���*/
  Debug_USART_Config();
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

