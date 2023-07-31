#ifndef _LCD_H
#define _LCD_H
#include "sys.h"
#include "stdint.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32H7������
//LCD��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2017/8/12
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	
#define LCD_LED(n)              (n?HAL_GPIO_WritePin(GPIOD,GPIO_PIN_4,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOD,GPIO_PIN_4,GPIO_PIN_RESET))   //LCD����PD13  

//LCD LTDC��Ҫ������
typedef struct  
{							 
	u32 pwidth;			//LCD���Ŀ��,�̶�����,������ʾ����ı�,���Ϊ0,˵��û���κ�RGB������
	u32 pheight;		//LCD���ĸ߶�,�̶�����,������ʾ����ı�
	u16 hsw;			//ˮƽͬ�����
	u16 vsw;			//��ֱͬ�����
	u16 hbp;			//ˮƽ����
	u16 vbp;			//��ֱ����
	u16 hfp;			//ˮƽǰ��
	u16 vfp;			//��ֱǰ�� 
	uint8_t activelayer;		//��ǰ����:0/1	
	uint8_t dir;				//0,����;1,����;
	u16 width;			//LCD���
	u16 height;			//LCD�߶�
	u32 pixsize;		//ÿ��������ռ�ֽ���
}_ltdc_dev; 

extern _ltdc_dev lcdltdc;		            //����LCD LTDC����
extern LTDC_HandleTypeDef LTDC_Handler;	    //LTDC���
extern DMA2D_HandleTypeDef DMA2D_Handler;   //DMA2D���
extern u32 *ltdc_framebuf[2];					//LTDC LCD֡��������ָ��,����ָ���Ӧ��С���ڴ�����

#define LCD_PIXEL_FORMAT_ARGB8888       0X00    
#define LCD_PIXEL_FORMAT_RGB888         0X01    
#define LCD_PIXEL_FORMAT_RGB565         0X02       
#define LCD_PIXEL_FORMAT_ARGB1555       0X03      
#define LCD_PIXEL_FORMAT_ARGB4444       0X04     
#define LCD_PIXEL_FORMAT_L8             0X05     
#define LCD_PIXEL_FORMAT_AL44           0X06     
#define LCD_PIXEL_FORMAT_AL88           0X07      

///////////////////////////////////////////////////////////////////////
//�û��޸����ò���:

//������ɫ���ظ�ʽ,һ����RGB565
#define LCD_PIXFORMAT				LCD_PIXEL_FORMAT_RGB565	
//����Ĭ�ϱ�������ɫ
#define LTDC_BACKLAYERCOLOR			0X00000000	
//LCD֡�������׵�ַ,���ﶨ����SDRAM����.
#define LCD_FRAME_BUF_ADDR			0XC0000000  

void LTDC_Switch(uint8_t sw);					//LTDC����
void LTDC_Layer_Switch(uint8_t layerx,uint8_t sw);	//�㿪��
void LTDC_Select_Layer(uint8_t layerx);			//��ѡ��
void LTDC_Display_Dir(uint8_t dir);				//��ʾ�������
void LTDC_Draw_Point(u16 x,u16 y,u32 color);//���㺯��
u32 LTDC_Read_Point(u16 x,u16 y);			//���㺯��
void LTDC_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u32 color);			//���ε�ɫ��亯��
void LTDC_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color);	//���β�ɫ��亯��
void LTDC_Clear(u32 color);					//��������
uint8_t LTDC_Clk_Set(u32 pll3m,u32 pll3n,u32 pll3r);//LTDCʱ������
//uint8_t LTDC_Clk_Set(u32 pllsain,u32 pllsair,u32 pllsaidivr);//LTDCʱ������
void LTDC_Layer_Window_Config(uint8_t layerx,u16 sx,u16 sy,u16 width,u16 height);//LTDC�㴰������
void LTDC_Layer_Parameter_Config(uint8_t layerx,u32 bufaddr,uint8_t pixformat,uint8_t alpha,uint8_t alpha0,uint8_t bfac1,uint8_t bfac2,u32 bkcolor);//LTDC������������
u16 LTDC_PanelID_Read(void);				//LCD ID��ȡ����
void LTDC_Init(void);						//LTDC��ʼ������
#endif 
