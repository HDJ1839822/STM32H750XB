#include "timer.h"
#include "USART.h"

#include "PAJ7620U2.h"

//ͨ�ö�ʱ��2�жϳ�ʼ��    //����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
void TIM2_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM2��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM2�ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�1��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���


	TIM_Cmd(TIM2, ENABLE);  //ʹ��TIMx					 
}

//��ʱ��2�жϷ������			/* ��������ʶ�� */
void TIM2_IRQHandler(void)   //TIM2�ж�
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //���TIM2�����жϷ������
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //���TIMx�����жϱ�־ 

		//����ʶ��
		gesture->detect = GS_Read_nByte(PAJ_GET_INT_FLAG1,2,&gesture->data[0]);//��ȡ����״̬			
		if(!gesture->detect)
		{   
			gesture->type =(u16)gesture->data[1]<<8 | gesture->data[0];
			if(gesture->type) 
			{
				switch(gesture->type)
				{
					case GES_UP:               printf("Up\r\n");            gesture->valid=1;      break; //����
					case GES_DOWN:             printf("Down\r\n");          gesture->valid=1;     break; //����
					case GES_LEFT:            printf("Left\r\n");          gesture->valid=1;    break; //����
					case GES_RIGHT:          	printf("Right\r\n");         gesture->valid=1;    break; //����
					case GES_FORWARD:       	printf("Forward\r\n");       gesture->valid=1;   break; //��ǰ
					case GES_BACKWARD:      		printf("Backward\r\n");      gesture->valid=1;   break; //���
					case GES_CLOCKWISE:     		printf("Clockwise\r\n");     gesture->valid=1;   break; //˳ʱ��
					case GES_ANTI_CLOCKWISE:  printf("AntiClockwise\r\n"); gesture->valid=1;    break; //��ʱ��
					case GES_WAVE:           printf("Wave\r\n");     gesture->valid=1;    break; //�Ӷ�
					default:  																				gesture->valid=0;   break;	
				}
			}
		}
	}
}
