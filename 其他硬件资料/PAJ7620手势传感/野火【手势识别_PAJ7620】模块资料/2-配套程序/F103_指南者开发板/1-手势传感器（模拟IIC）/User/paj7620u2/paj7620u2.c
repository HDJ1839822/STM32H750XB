#include "paj7620u2.h"
#include "paj7620u2_cfg.h"
#include "./usart/bsp_usart.h"
#include "./SysTick/bsp_SysTick.h"
#include "paj7620u2_iic.h"

uint8_t  flag=0;

/**
  * @brief  ѡ��PAJ7620U2 BANK����
  * @param  ��
  * @retval ��
  */
void paj7620u2_selectBank(bank_e bank)
{
	switch(bank)
	{
		case BANK0: GS_Write_Byte(PAJ_REGITER_BANK_SEL,PAJ_BANK0);break;//BANK0�Ĵ�������
		case BANK1: GS_Write_Byte(PAJ_REGITER_BANK_SEL,PAJ_BANK1);break;//BANK1�Ĵ�������
	}			
}

/**
  * @brief  PAJ7620U2����
  * @param  ��
  * @retval 0:ʧ�� 1:�ɹ�
  */
uint8_t paj7620u2_wakeup(void)
{ 
	uint8_t data=0x0a;
	GS_WakeUp();//����PAJ7620U2
	delay_ms(5);//����ʱ��>700us
	GS_WakeUp();//����PAJ7620U2
	delay_ms(5);//����ʱ��>700us
	paj7620u2_selectBank(BANK0);//����BANK0�Ĵ�������
	data = GS_Read_Byte(0x00);//��ȡ״̬
	if(data!=0x20) return 0; //����ʧ��
	
	return 1;
}


/**
  * @brief  PAJ7620U2��ʼ��
  * @param  ��
  * @retval 0:ʧ�� 1:�ɹ�
  */
uint8_t paj7620u2_init(void)
{
	uint8_t i;
	uint8_t status;
	
	GS_i2c_init();//IIC��ʼ��
	status = paj7620u2_wakeup();//����PAJ7620U2
	if(!status) return 0;
    paj7620u2_selectBank(BANK0);//����BANK0�Ĵ�������
	for(i=0;i<INIT_SIZE;i++)
	{
		GS_Write_Byte(init_Array[i][0],init_Array[i][1]);//��ʼ��PAJ7620U2
	}
    paj7620u2_selectBank(BANK0);//�л���BANK0�Ĵ�������
	
	return 1;
}

/**
  * @brief  ����ʶ�����
  * @param  ��
  * @retval ��
  */
void Gesrure_test(void)
{
  uint8_t status,i;
	uint8_t data[2]={0x00};
	uint16_t gesture_data;

	paj7620u2_selectBank(BANK0);//����BANK0
	for(i=0;i<GESTURE_SIZE;i++)
	{
		GS_Write_Byte(gesture_arry[i][0],gesture_arry[i][1]);//����ʶ��ģʽ��ʼ��
	}
	paj7620u2_selectBank(BANK0);//�л���BANK0
  printf("��KEY1���˳���ǰģʽ\r\n");
	
	while(1)
	{       
		if(flag)
		{
      flag=0;
			GS_Write_Byte(PAJ_SET_INT_FLAG1,0X00);//�ر�����ʶ���ж����
			GS_Write_Byte(PAJ_SET_INT_FLAG2,0X00);
			break;
		}			
    status = GS_Read_nByte(PAJ_GET_INT_FLAG1,2,&data[0]);//��ȡ����״̬			
		if(!status)
		{   
			gesture_data =(uint16_t)data[1]<<8 | data[0];
			if(gesture_data) 
			{
				switch(gesture_data)
				{
					case GES_UP:              
					                           printf("Up\r\n");                break; //����
					case GES_DOWM:                 
               						           printf("Dowm\r\n");              break; //����
					case GES_LEFT:                     
  						                       printf("Left\r\n");              break; //����
					case GES_RIGHT:                  
                						         printf("Right\r\n");             break; //����
					case GES_FORWARD:                 
						                         printf("Forward\r\n");           break; //��ǰ
					case GES_BACKWARD:           
            						             printf("Backward\r\n");          break; //���
					case GES_CLOCKWISE:         
                						         printf("Clockwise\r\n");         break; //˳ʱ��
					case GES_COUNT_CLOCKWISE:  
                   						       printf("AntiClockwise\r\n");     break; //��ʱ��
					case GES_WAVE:               
						                         printf("Wave\r\n");              break; //�Ӷ�
					default:  break;	
				}	          
			}	
		}		   
	}
}

/**
  * @brief  �ӽ�������
  * @param  ��
  * @retval ��
  */
void Ps_test(void)
{
  
  uint8_t i;
	uint8_t data[2]={0x00};
	uint8_t obj_brightness=0;
	uint16_t obj_size=0;
	
	paj7620u2_selectBank(BANK0);//����BANK0
	for(i=0;i<PROXIM_SIZE;i++)
	{
		GS_Write_Byte(proximity_arry[i][0],proximity_arry[i][1]);//�ӽ����ģʽ��ʼ��
	}
	paj7620u2_selectBank(BANK0);//�л���BANK0
	printf("��KEY1���˳���ǰģʽ\r\n");
	
	while(1)
	{	
		if(flag) 
    {
      flag=0;
      break;
    }
		obj_brightness = GS_Read_Byte(PAJ_GET_OBJECT_BRIGHTNESS);//��ȡ��������
		data[0] = GS_Read_Byte(PAJ_GET_OBJECT_SIZE_1);//��ȡ�����С
		data[1] = GS_Read_Byte(PAJ_GET_OBJECT_SIZE_2);
		obj_size = ((uint16_t)data[1] & 0x0f)<<8 | data[0];
		printf("obj_brightness: %d\r\n",obj_brightness);
    printf("obj_size: %d\r\n",obj_size);
	
		delay_ms(500);	
	}	
}

/**
  * @brief  ָ����Ϣ
  * @param  ��
  * @retval ��
  */
void Show_Mode_Message(void)
{ 
  printf("   ָ��   ------      ���� \r\n");
  printf("    1     ------    ���Ƽ��ģʽ \r\n"); 
  printf("    2     ------    �ӽ����ģʽ \r\n");
}

/**
  * @brief  PAJ7620U2����������
  * @param  ��
  * @retval ��
  */
void paj7620u2_sensor_test(void)
{   
	uint32_t   ch;
  
	Show_Mode_Message();//��ʾ��ͨ����ģʽUI
	while(1)
	{
		scanf("%d",&ch);
    printf("���յ��ַ���%d\r\n",ch);

		switch(ch)
		{
			case 1:  Gesrure_test();                 break;//���Ƽ��ģʽ
			case 2:  Ps_test();                      break;//�ӽ����ģʽ 
      default: printf("������Ϸ�ָ�\r\n");  break;
		}
		Show_Mode_Message();
	} 
}

