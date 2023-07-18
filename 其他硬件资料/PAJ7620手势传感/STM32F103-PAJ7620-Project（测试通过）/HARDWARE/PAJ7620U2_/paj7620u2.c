#include "paj7620u2.h"
#include "paj7620u2_cfg.h"

static void paj7620u2_selectBank(bank_e bank);//ѡ��PAJ7620U2 BANK����
static u8 paj7620u2_wakeup(void);//PAJ7620U2����

//PAJ7620U2��ʼ��
//����ֵ��0:ʧ�� 1:�ɹ�
u8 paj7620u2_init()
{
	u8 i;
	u8 status;
	
	GS_i2c_init();//������I2C��ʼ��							
	status = paj7620u2_wakeup();		//����PAJ7620U2
	if(!status) 
		return 0;
	paj7620u2_selectBank(BANK0);	//����BANK0�Ĵ�������
	for(i=0;i<INIT_SIZE;i++)						//��ʼ��ģ��
	{
		GS_Write_Byte(init_Array[i][0], init_Array[i][1]);//��ʼ��PAJ7620U2
	}
  paj7620u2_selectBank(BANK0);//�л���BANK0�Ĵ�������
	return 1;
}

GestureData *gesture;
void Gesture_Init(void)
{
	u8 i;
	paj7620u2_selectBank(BANK0);//����BANK0�Ĵ�������
	for(i=0;i<GESTURE_SIZE;i++)
	{
		GS_Write_Byte(gesture_arry[i][0],gesture_arry[i][1]);//����ʶ��ģʽ��ʼ��
	}
	paj7620u2_selectBank(BANK0);//�л���BANK0�Ĵ�������

	gesture = (GestureData *)malloc(sizeof(GestureData));
	if(NULL == gesture){
		//
		printf("Error: struct \"GESTURE_DATA\" malloc failed\r\n");
	}
	memset(gesture, 0, sizeof(GestureData));
	
}

//ѡ��PAJ7620U2 BANK����
void paj7620u2_selectBank(bank_e bank)
{
	switch(bank)
	{
		case BANK0: GS_Write_Byte(PAJ_REGITER_BANK_SEL,PAJ_BANK0);break;//BANK0�Ĵ�������
		case BANK1: GS_Write_Byte(PAJ_REGITER_BANK_SEL,PAJ_BANK1);break;//BANK1�Ĵ�������
	}		
}

//PAJ7620U2����
u8 paj7620u2_wakeup()
{ 
	u8 data=0x0a;
	GS_WakeUp();//����PAJ7620U2
	delay_ms(5);//����ʱ��>400us
	GS_WakeUp();//����PAJ7620U2
	delay_ms(5);//����ʱ��>400us
	paj7620u2_selectBank(BANK0);//����BANK0�Ĵ�������
	data = GS_Read_Byte(0x00);//��ȡ״̬
	if(data!=0x20) return 0; //����ʧ��
	
	return 1;
}
