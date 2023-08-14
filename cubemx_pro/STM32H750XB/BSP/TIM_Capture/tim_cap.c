#include "tim_cap.h"
#include "main.h"
#include "stdio.h"

/*���ַ�ʽδ����������ʺ��ض�Ƶ��*/

uint32_t InputCaptureValue1 = 0;  //���벶��ֵ1
uint32_t InputCaptureValue2 = 0;  //���벶��ֵ2
uint8_t  CaptureNumber = 0;       //�������
double_t TIM2CH1_Frequency = 0;   //Ƶ��ֵ
double_t TIM2CH1_DutyCycle = 0;   //ռ�ձ�
uint32_t TIM2_OverflowCount = 0;  //�������

/*���벶��ص�����*/
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	uint16_t value1=0,value2=0;
	static uint32_t count=0;
	double fre,duty;
	count++;
	 if (htim->Instance == TIM2) 
  {
	    if(htim->Channel==HAL_TIM_ACTIVE_CHANNEL_1)
		   {
		   	value1 = HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_1);	//ֱ��	
		   	value2 = HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_2);	//���
				HAL_TIM_IC_Stop(htim,TIM_CHANNEL_1);	//���¿�����ʱ��ͨ��
		   	HAL_TIM_IC_Stop(htim,TIM_CHANNEL_2);	//���¿�����ʱ��ͨ��
		   	__HAL_TIM_SetCounter(htim,0);			//����ֵ����
		   	fre = (240000000.0/240)/value1;		//����Ƶ��
		   	duty = (float)value2/value1 * 100;  //����ռ�ձ�
		   	HAL_TIM_IC_Start(htim,TIM_CHANNEL_1);	//���¿�����ʱ��ͨ��
		   	HAL_TIM_IC_Start(htim,TIM_CHANNEL_2);	//���¿�����ʱ��ͨ��
		   }

  }
	
	
   	if(count>=1000&&fre&&duty)
   	{
   	  printf("TIM2CH1_Freq=%lfHZ\n,TIM2CH1_Duty=%lf%%\r\n",fre,duty);
   	  count=0;
   	}
   		
	
		
}
