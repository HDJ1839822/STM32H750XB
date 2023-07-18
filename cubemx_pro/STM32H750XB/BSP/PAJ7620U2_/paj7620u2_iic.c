#include "paj7620u2_iic.h"
#include "paj7620u2.h"
#include "stdio.h"

void delay_us(uint16_t us)
{
 static unsigned int i=0;
 for ( i=0;i<us*14;++i);
}


/**
PH11  SCL
PH12  SDA
*/

void SDA_IN(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
	__HAL_RCC_GPIOH_CLK_ENABLE();
	//HAL_GPIO_WritePin(GPIOH, SCL_PAJ7620_Pin|SDA_PAJ7620_Pin, GPIO_PIN_SET);
	/*Configure GPIO pins : PHPin PHPin */
  GPIO_InitStruct.Pin = SDA_PAJ7620_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);
}

void SDA_OUT(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
	__HAL_RCC_GPIOH_CLK_ENABLE();
	//HAL_GPIO_WritePin(GPIOH, SCL_PAJ7620_Pin|SDA_PAJ7620_Pin, GPIO_PIN_SET);
	/*Configure GPIO pins : PHPin PHPin */
  GPIO_InitStruct.Pin = SDA_PAJ7620_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);
}


uint8_t READ_SDA(void)
{
 return HAL_GPIO_ReadPin(GPIOH,SDA_PAJ7620_Pin);
}



//����IIC��ʼ�ź�
static void GS_IIC_Start()
{
	GS_SDA_OUT();//sda�����
	GS_IIC_SDA(1);	  	  
	GS_IIC_SCL(1);
	delay_us(4);
	GS_IIC_SDA(0);//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	GS_IIC_SCL(0);//ǯסI2C���ߣ�׼�����ͻ�������� 
}

//����IICֹͣ�ź�
static void GS_IIC_Stop()
{
	GS_SDA_OUT();//sda�����
	GS_IIC_SCL(0);
	GS_IIC_SDA(0);//STOP:when CLK is high DATA change form low to high
	delay_us(4);
	GS_IIC_SCL(1); 
	GS_IIC_SDA(1);//����I2C���߽����ź�
	delay_us(4);				   	
}

//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
static uint8_t GS_IIC_Wait_Ack()
{
	
	uint8_t ucErrTime=0;
	GS_SDA_IN();  //SDA����Ϊ����  
	GS_IIC_SDA(1);delay_us(3);	   
	GS_IIC_SCL(1);delay_us(3);	 
	while(READ_SDA())
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			GS_IIC_Stop();
			return 1;
		}
	}
	GS_IIC_SCL(0);//ʱ�����0 	
	return 0;  
}

//����ACKӦ��
static void GS_IIC_Ack()
{
	GS_IIC_SCL(0);
	GS_SDA_OUT();
	GS_IIC_SDA(0);
	delay_us(3);
	GS_IIC_SCL(1);
	delay_us(3);
	GS_IIC_SCL(0);
}

//������ACKӦ��		    
static void GS_IIC_NAck()
{
	GS_IIC_SCL(0);
	GS_SDA_OUT();
	GS_IIC_SDA(1);
	delay_us(2);
	GS_IIC_SCL(1);
	delay_us(2);
	GS_IIC_SCL(0);
}

//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
static void GS_IIC_Send_Byte(uint8_t txd)
{                        
	uint8_t t;   
	GS_SDA_OUT(); 	    
	GS_IIC_SCL(0);//����ʱ�ӿ�ʼ���ݴ���
	for(t=0;t<8;t++)
	{              
		if((txd&0x80)>>7)
			GS_IIC_SDA(1);
		else
			GS_IIC_SDA(0);
		txd<<=1; 	
    
		delay_us(5); 
   	
		GS_IIC_SCL(1);
		delay_us(5); 
		GS_IIC_SCL(0);	
		delay_us(5);
	}	 
} 

//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
static uint8_t GS_IIC_Read_Byte(uint8_t ack)
{
	
	uint8_t i,receive=0;

	GS_SDA_IN();//SDA����Ϊ����
	for(i=0;i<8;i++ )
	{
		GS_IIC_SCL(0); 
		delay_us(4);
		GS_IIC_SCL(1);
		receive<<=1;
		if(READ_SDA())receive++;   
		delay_us(4); 
	}					 
	if (!ack)
		GS_IIC_NAck();//����nACK
	else
		GS_IIC_Ack(); //����ACK   
	
	return receive;
}

//PAJ7620U2дһ���ֽ�����
uint8_t GS_Write_Byte(uint8_t REG_Address,uint8_t REG_data)
{
	GS_IIC_Start();
	GS_IIC_Send_Byte(PAJ7620_ID);
	if(GS_IIC_Wait_Ack())
	{
		GS_IIC_Stop();//�ͷ�����
		return 1;//ûӦ�����˳�
	}
	GS_IIC_Send_Byte(REG_Address);
	GS_IIC_Wait_Ack();	
	GS_IIC_Send_Byte(REG_data);
	GS_IIC_Wait_Ack();	
	GS_IIC_Stop();

	return 0;
}

//PAJ7620U2��һ���ֽ�����
uint8_t GS_Read_Byte(uint8_t REG_Address)
{
	uint8_t REG_data;
	
	GS_IIC_Start();
	GS_IIC_Send_Byte(PAJ7620_ID);//��д����
	if(GS_IIC_Wait_Ack())
	{
		 GS_IIC_Stop();//�ͷ�����
		 return 0;//ûӦ�����˳�
	}		
	GS_IIC_Send_Byte(REG_Address);
	GS_IIC_Wait_Ack();
	GS_IIC_Start(); 
	GS_IIC_Send_Byte(PAJ7620_ID|0x01);//��������
	GS_IIC_Wait_Ack();
	REG_data = GS_IIC_Read_Byte(0);
	GS_IIC_Stop();

	return REG_data;
}
//PAJ7620U2��n���ֽ�����
uint8_t GS_Read_nByte(uint8_t REG_Address,uint16_t len,uint8_t *buf)
{
	GS_IIC_Start();
	GS_IIC_Send_Byte(PAJ7620_ID);//��д����
	if(GS_IIC_Wait_Ack()) 
	{
		GS_IIC_Stop();//�ͷ�����
		return 1;//ûӦ�����˳�
	}
	GS_IIC_Send_Byte(REG_Address);
	GS_IIC_Wait_Ack();

	GS_IIC_Start();
	GS_IIC_Send_Byte(PAJ7620_ID|0x01);//��������
	GS_IIC_Wait_Ack();
	while(len)
	{
		if(len==1)
		{
			*buf = GS_IIC_Read_Byte(0);
		}
		else
		{
			*buf = GS_IIC_Read_Byte(1);
		}
		buf++;
		len--;
	}
	GS_IIC_Stop();//�ͷ�����

	return 0;
	
}
//PAJ7620����
void GS_WakeUp()
{
	//printf("test3\r\n");
	GS_IIC_Start();
	GS_IIC_Send_Byte(PAJ7620_ID);//��д����
	//printf("test4\r\n");
	GS_IIC_Stop();//�ͷ�����
}

//PAJ2670 I2C��ʼ��
void GS_i2c_init()
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	__HAL_RCC_GPIOH_CLK_ENABLE();
	HAL_GPIO_WritePin(GPIOH, SCL_PAJ7620_Pin|SDA_PAJ7620_Pin, GPIO_PIN_SET);
	/*Configure GPIO pins : PHPin PHPin */
  GPIO_InitStruct.Pin = SCL_PAJ7620_Pin|SDA_PAJ7620_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);
}
