#include "irda_nec.h"
#include "stm32h7xx_it.h"
#include "tim.h"




/**********************���ݶ�ʱ�������ɵ����ɵ�������***************************************/
#define   TIM_INTERRUPT_TIME        10                     /* ��һ����ʱ��(us)*/
#define   OVERFLOW                  65535                 /* ���ֵ (����������������޷���16λ) */
/******************NECЭ�����(������NECЭ�飬�Ǳ�Ҫ��ģ��ɶ�̬���ӷ�Χ��)***********************/
#define   START_MIN               13000/TIM_INTERRUPT_TIME    /* ��������С�������� */
#define   START_MAX               14000/TIM_INTERRUPT_TIME    /* ���������������� */
  
#define   SURE_0_MIN              1000/TIM_INTERRUPT_TIME     /* 0��С�������� */           
#define   SURE_0_MAX              1200/TIM_INTERRUPT_TIME     /* 0���������� */            
 
#define   SURE_1_MIN              2100/TIM_INTERRUPT_TIME      /* 1��С�������� */ 
#define   SURE_1_MAX              2350/TIM_INTERRUPT_TIME      /* 1���������� */            
/*******************************************************************************************/  



uint32_t IC_IRDA_NEC(void)
{
	static uint16_t get_new_tim; /*��ȡ����ʱ��*/
	static uint16_t get_old_tim; /*��ȡ�ϴ�ʱ��*/
	static uint16_t think_tim;/*���ʱ��*/
	static char irda_state;/*����״̬*/
	static uint32_t irda_decoder,get_ir_code;/*����ֵ*/
	static uint8_t code_flag; /*����ɹ���־λ*/
  static uint8_t irda_data_bit_count=32;/*�����32��bit*/

	if(nec_ValueChanged)/*�����½����ж�*/
	{
	
		/*��ȡ����ֵ*/
		get_new_tim=HAL_TIM_ReadCapturedValue(&htim5,TIM_CHANNEL_3);
		/**************�������������������**************************/	
		if(get_new_tim > get_old_tim) 
			think_tim = get_new_tim - get_old_tim;          /*�����*/
		else   
			think_tim = get_new_tim + OVERFLOW - get_old_tim;    /*���*/
		
		get_old_tim = get_new_tim;
    /***************************************************************/
		
		
	switch (irda_state)
	{
		case 0: 
			/*������*/
			if((think_tim>=START_MIN&&think_tim<=START_MAX))
					irda_state=1; /*��������ȷ������һ�׶�*/
			else 
				//HAL_TIM_Base_Stop(&htim6);
			 break; /*��������ֱ������*/
	  
		case 1:
			/*�ж�0��1*/
	   if((think_tim>=SURE_0_MIN&&think_tim<=SURE_0_MAX))   /*�ж�Ϊ0   1.12ms*/
			{
				irda_decoder|=0<<(irda_data_bit_count-1); 
			  irda_data_bit_count-=1; /*����32λ����*/
				irda_state=2;
				
			}
			else if(think_tim>=SURE_1_MIN&&think_tim<=SURE_1_MAX) /*�ж�Ϊ1   2.24ms*/
			{
			  irda_decoder|=1<<(irda_data_bit_count-1);
				irda_data_bit_count-=1;
				irda_state=2;
			}
			
			else /*��0��1*/
			{
			 irda_state=0; /*�����ж�������*/
			
			}
			
			
		/*�ж��Ƿ������32λ���ݣ�����������ɽ��룬δ������������ж�0��1*/	
		case 2:
			
		if(irda_data_bit_count==0)   /*�Ƿ�����32λ����*/
		{
		  code_flag=1; /*������ɽ����־λ��1*/
		  irda_data_bit_count=32; /*��������32bit����*/
		  irda_state=0; /*�����ж�������*/
	  }
	 else
			irda_state=1; /*�����ж�0��1*/
		  	
	
	}
	
	 
	 nec_ValueChanged=false;/*�ָ��жϱ�־λ*/
	
	}
	
	if(code_flag==1)
	{
	  
	  get_ir_code=irda_decoder;
    code_flag=0;	
		irda_decoder=0;	
		/***********************У��******************************/
		
		unsigned int address = (get_ir_code >> 24) & 0xFF;/*��ȡ��ַ��*/
    unsigned int address_complement = (get_ir_code >> 16) & 0xFF;// ��ȡ��ַ����
    unsigned int data = (get_ir_code >> 8) & 0xFF;  // ��ȡ������
    unsigned int data_complement = get_ir_code & 0xFF;// ��ȡ���ݷ���
		
		//    printf("��ַ��: 0x%02X\n", address);
    //    printf("��ַ����: 0x%02X\n", address_complement);
    //    printf("������: 0x%02X\n", data);
    //    printf("���ݷ���: 0x%02X\r\n", data_complement);
		
		
		if((address+address_complement==0xff)&&(data+data_complement==0xff))
		{
		 return get_ir_code;/*������*/
		 //return data;/*������*/
		}
/**************************************************************************/
//	return get_ir_code;
		
	}
	
	 return 0x00;
	
	
	
}





