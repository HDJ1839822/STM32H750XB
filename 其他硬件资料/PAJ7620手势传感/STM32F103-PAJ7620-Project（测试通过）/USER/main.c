#include "sys.h"
#include "delay.h"
#include "led.h"				//LED0��LED1
#include "usart.h"			//����1������Ϣ��ӡ

#include "PAJ7620U2.h"	//PAJ7620
#include "timer.h"			//��ʱ���ж�

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//STM32F10x������  PAJ7620������ģ������	
//STM32ѧϰ�����⣬�����qȺ����: 643807576

void PAJ7620_Init(void);

int main(void)
{		
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	
	uart_init(115200);	    //���ڳ�ʼ��Ϊ115200
 	LED_Init();			    		//LED�˿ڳ�ʼ��
	PAJ7620_Init();					//LED1���𡢴��������PAJ7620U2 OK��--->˵��PAJ7620ģ���ʼ������
	 
	//TIM2_Int_Init(1299,7199); //130ms��ʱ--���ƴ������ݲɼ���130ms���������Ͳ���ʱ�䣬����ʱ��ԽС��ʶ��ӦԽ�����������Լ���һ�²���ʱ�䵽���ʡ�
	
	while(1)
	{
		delay_ms(200);
	

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
	
		
		
		;
	}
}

void  PAJ7620_Init(void)
{
	while(!paj7620u2_init())//PAJ7620U2��������ʼ��
	{
		printf("PAJ7620U2_B Error!!!\r\n");
		delay_ms(500);
		LED0=!LED0;//DS0��˸
	}
	LED1 =~LED1;	
	delay_ms(1000);
	Gesture_Init();
  printf("PAJ7620U2 OK\r\n");
}
