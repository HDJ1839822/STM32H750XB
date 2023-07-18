#include "paj7620u2_iic.h"
#include "paj7620u2.h"
#include "./SysTick/bsp_SysTick.h"

/**
  * @brief  PAJ7620U2��ʼ��
  * @param  ��
  * @retval ��
  */
void GS_i2c_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(GS_I2C_GPIO_CLK, ENABLE);	/* ��GPIOʱ�� */

	GPIO_InitStructure.GPIO_Pin =GS_I2C_SCL_PIN| GS_I2C_SDA_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;  	/* ��©��� */
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  //�������
	GPIO_Init(GS_I2C_GPIO_PORT, &GPIO_InitStructure);
	
  GPIO_SetBits(GS_I2C_GPIO_PORT,GS_I2C_SCL_PIN| GS_I2C_SDA_PIN);//PB10,PB11 �����	
}

/**
  * @brief  ����IIC��ʼ�ź�
  * @param  ��
  * @retval ��
  */
static void GS_IIC_Start(void)
{
	GS_SDA_OUT();   //sda�����
	GS_I2C_SDA_1()  ;	  	  
	GS_I2C_SCL_1();
	delay_us(4);
 	GS_I2C_SDA_0()  ;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	GS_I2C_SCL_0();  //ǯסI2C���ߣ�׼�����ͻ�������� 
}

/**
  * @brief  ����IICֹͣ�ź�
  * @param  ��
  * @retval ��
  */
static void GS_IIC_Stop(void)
{
	GS_SDA_OUT();    //sda�����
	GS_I2C_SCL_0();
	GS_I2C_SDA_0()  ;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	GS_I2C_SCL_1(); 
	GS_I2C_SDA_1()  ;//����I2C���߽����ź�
	delay_us(4);							   	
}
      
/**
  * @brief  �ȴ�Ӧ���źŵ���
  * @param  ��
  * @retval 1:����Ӧ��ʧ�ܣ�0:����Ӧ��ɹ�
  */
static uint8_t GS_IIC_Wait_Ack(void)
{
	uint8_t ucErrTime=0;
	GS_SDA_IN();  //SDA����Ϊ����  
	GS_I2C_SDA_1()  ;
  delay_us(3);	   
	GS_I2C_SCL_1()  ;
  delay_us(3);	 
	while(GS_I2C_SDA_READ())
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			GS_IIC_Stop();
			return 1;
		}
	}
	GS_I2C_SCL_0();//ʱ�����0 	   
	return 0;  
}

/**
  * @brief  ����ACKӦ��
  * @param  ��
  * @retval ��
  */
static void GS_IIC_Ack(void)
{
	GS_I2C_SCL_0();
	GS_SDA_OUT();
	GS_I2C_SDA_0()  ;
	delay_us(3);
	GS_I2C_SCL_1();
	delay_us(3);
	GS_I2C_SCL_0();
}

/**
  * @brief  ������ACKӦ��
  * @param  ��
  * @retval ��
  */
static void GS_IIC_NAck(void)
{
	GS_I2C_SCL_0();
	GS_SDA_OUT();
	GS_I2C_SDA_1()  ;
	delay_us(2);
	GS_I2C_SCL_1();
	delay_us(2);
	GS_I2C_SCL_0();
}

/**
  * @brief  IIC����һ���ֽ�
  * @param  ������ֽ�
  * @retval 1:��Ӧ��,0:��Ӧ��
  */
static void GS_IIC_Send_Byte(uint8_t txd)
{                        
    uint8_t t;   
	  GS_SDA_OUT(); 	    
    GS_I2C_SCL_0();//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
		 if((txd&0x80)>>7)
			 GS_I2C_SDA_1()  ;
		 else
			 GS_I2C_SDA_0()  ;
      txd<<=1; 	  
      delay_us(5);  
      GS_I2C_SCL_1();
      delay_us(5); 
      GS_I2C_SCL_0();	
      delay_us(5);
    }	 
} 

/**
  * @brief  ��1���ֽ�
  * @param  ack=1ʱ������ACK��ack=0������nACK
  * @retval ���յ�������
  */
static uint8_t GS_IIC_Read_Byte(uint8_t ack)
{
	uint8_t i,receive=0;
	GS_SDA_IN();//SDA����Ϊ����
	for(i=0;i<8;i++ )
	{
		GS_I2C_SCL_0(); 
		delay_us(4);
	  GS_I2C_SCL_1();
		receive<<=1;
		if(GS_I2C_SDA_READ()) receive++;   
	  delay_us(4); 
	}					 
	if (!ack)
		GS_IIC_NAck();//����nACK
	else
		GS_IIC_Ack(); //����ACK   
	return receive;
}

/**
  * @brief  PAJ7620U2дһ���ֽ�����
  * @param  �Ĵ�����ַ���Ĵ�������
  * @retval 1:�ɹ���0:ʧ��
  */
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

/**
  * @brief  PAJ7620U2��һ���ֽ�����
  * @param  �Ĵ�����ַ
  * @retval ���յ�����
  */
uint8_t GS_Read_Byte(uint8_t REG_Address)
{
	uint8_t REG_data;
	
	GS_IIC_Start();
	GS_IIC_Send_Byte(PAJ7620_ID);//��д����
	if(GS_IIC_Wait_Ack())
	{
		GS_IIC_Stop();//�ͷ�����
		return 0;     //ûӦ�����˳�
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

/**
  * @brief  PAJ7620U2��n���ֽ�����
  * @param  �Ĵ�����ַ�������ֽ���������ָ��
  * @retval ���յ�����
  */
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
	GS_IIC_Stop();               //�ͷ�����

	return 0;	
}

/**
  * @brief  PAJ7620U2����
  * @param  ��
  * @retval ��
  */
void GS_WakeUp(void)
{
	GS_IIC_Start();
	GS_IIC_Send_Byte(PAJ7620_ID);//��д����
	GS_IIC_Stop();               //�ͷ�����
}
