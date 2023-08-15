#include "app.h" 
#include "bsp.h" 
#include "lcd.h" 
#include "touch.h"
#include "tim.h"
#include "stdio.h"
#include "sdram.h"
#include "stm32h7xx.h"
#include "paj7620u2.h"
#include "oled_test.h"
#include "stm32_u8g2.h"
#include "dht11.h"
#include "delay.h" 
#include "adc.h" 
#include "ltdc_.h" 
#include "stm32h7xx_it.h" 
#include "sd.h" 
#include "w25qxx.h"
#include "w25q64jv.h"
#include "display.h"
#include "fatfs.h"
#include "ff.h"
#include "config.h"

 uint32_t rawValue;
//  n  
/* 10�����ص����ɫ(���ݴ�������) */
const uint16_t POINT_COLOR_TBL[10] = {RED, GREEN, BLUE, BROWN, YELLOW, MAGENTA, CYAN, LIGHTBLUE, BRRED, GRAY};

void sys_qspi_enable_memmapmode(uint8_t ftype)
{
    uint32_t tempreg = 0; 
//    GPIO_InitTypeDef qspi_gpio;

//    __HAL_RCC_GPIOB_CLK_ENABLE();                            /* ʹ��PORTBʱ�� */
//    __HAL_RCC_GPIOD_CLK_ENABLE();                            /* ʹ��PORTDʱ�� */
//    __HAL_RCC_GPIOE_CLK_ENABLE();                            /* ʹ��PORTEʱ�� */
//    __HAL_RCC_QSPI_CLK_ENABLE();                             /* QSPIʱ��ʹ�� */

//    qspi_gpio.Pin = GPIO_PIN_6;                              /* PB6 AF10 */
//    qspi_gpio.Mode = GPIO_MODE_AF_PP;
//    qspi_gpio.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
//    qspi_gpio.Pull = GPIO_PULLUP;
//    qspi_gpio.Alternate = GPIO_AF10_QUADSPI;
//    HAL_GPIO_Init(GPIOB, &qspi_gpio);

//    qspi_gpio.Pin = GPIO_PIN_2;                              /* PB2 AF9 */
//    qspi_gpio.Alternate = GPIO_AF9_QUADSPI;
//    HAL_GPIO_Init(GPIOB, &qspi_gpio);

//    qspi_gpio.Pin = GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13; /* PD11,12,13 AF9 */
//    qspi_gpio.Alternate = GPIO_AF9_QUADSPI;
//    HAL_GPIO_Init(GPIOD, &qspi_gpio);

//    qspi_gpio.Pin = GPIO_PIN_2;                              /* PE2 AF9 */
//    qspi_gpio.Alternate = GPIO_AF9_QUADSPI;
//    HAL_GPIO_Init(GPIOE, &qspi_gpio);

    /* QSPI���ã��ο�QSPIʵ���QSPI_Init���� */
    RCC->AHB3RSTR |= 1 << 14;       /* ��λQSPI */
    RCC->AHB3RSTR &= ~(1 << 14);    /* ֹͣ��λQSPI */

    while (QUADSPI->SR & (1 << 5)); /* �ȴ�BUSYλ���� */

    /* QSPIʱ��Դ�Ѿ���sys_stm32_clock_init()���������� */
    QUADSPI->CR = 0X01000310;       /* ����CR�Ĵ���, ��Щֵ��ô���ģ���ο�QSPIʵ��/��H750�ο��ֲ�Ĵ����������� */
    QUADSPI->DCR = 0X00180201;      /* ����DCR�Ĵ���(FLASH����32M(�����������Ϊ32M, Ĭ����16M��), tSHSL = 3��ʱ��) */
    QUADSPI->CR |= 1 << 0;          /* ʹ��QSPI */
    //QUADSPI->CR |= 0 << 0;          /* ʧ��QSPI */
    /*
     *  ע��:QSPI QEλ��ʹ�ܣ���QSPI��д�㷨���棬���Ѿ�������
     *  ����, ������Բ�������QEλ��������Ҫ�����QEλ��1�Ĵ���
     *  ����, �������ͨ������������, ֱ����¼���ⲿQSPI FLASH, �ǲ����õ�
     *  �����ֱ����¼���ⲿQSPI FLASHҲ������, ����Ҫ���������QEλ��1�Ĵ���
     *
     *  ����, ����W25Q256,����Ҫʹ��4�ֽڵ�ַģʽ,��������S3��ADPλΪ1.
     *  ������QSPI��д�㷨�����Ѿ�������ADP=1(�ϵ缴32λ��ַģʽ),�������Ҳ
     *  ����Ҫ���ͽ���4�ֽڵ�ַģʽָ��/����ADP=1��, ������Ҫ����ADP=1
     */

    /* BY/W25QXX дʹ�ܣ�0X06ָ�*/
    while (QUADSPI->SR & (1 << 5)); /* �ȴ�BUSYλ���� */

    QUADSPI->CCR = 0X00000106;      /* ����0X06ָ�BY/W25QXXдʹ�� */

    while ((QUADSPI->SR & (1 << 1)) == 0);/* �ȴ�ָ������ */

    QUADSPI->FCR |= 1 << 1;         /* ���������ɱ�־λ */

    /* MemroyMap ģʽ���� */
    while (QUADSPI->SR & (1 << 5)); /* �ȴ�BUSYλ���� */

    QUADSPI->ABR = 0;               /* �����ֽ�����Ϊ0��ʵ���Ͼ���25QXX 0XEBָ���, M0~M7 = 0 */
    tempreg = 0XEB;                 /* INSTRUCTION[7:0] = 0XEB, ����0XEBָ�Fast Read QUAD I/O�� */
    tempreg |= 1 << 8;              /* IMODE[1:0] = 1, ���ߴ���ָ�� */
    tempreg |= 3 << 10;             /* ADDRESS[1:0] = 3, ���ߴ����ַ */
    tempreg |= (2 + ftype) << 12;   /* ADSIZE[1:0] = 2/3, 24λ(ftype = 0) / 32λ(ftype = 1)��ַ���� */
    tempreg |= 3 << 14;             /* ABMODE[1:0] = 3, ���ߴ��佻���ֽ� */
    tempreg |= 0 << 16;             /* ABSIZE[1:0] = 0, 8λ�����ֽ�(M0~M7) */
    tempreg |= 4 << 18;             /* DCYC[4:0] = 4, 4��dummy���� */
    tempreg |= 3 << 24;             /* DMODE[1:0] = 3, ���ߴ������� */
    tempreg |= 3 << 26;             /* FMODE[1:0] = 3, �ڴ�ӳ��ģʽ */
    QUADSPI->CCR = tempreg;         /* ����CCR�Ĵ��� */

    /* ����QSPI FLASH�ռ��MPU���� */
    SCB->SHCSR &= ~(1 << 16);       /* ��ֹMemManage */
    MPU->CTRL &= ~(1 << 0);         /* ��ֹMPU */
    MPU->RNR = 0;                   /* ���ñ���������Ϊ0(1~7���Ը������ڴ���) */
    MPU->RBAR = 0X90000000;         /* ����ַΪ0X9000 000, ��QSPI����ʼ��ַ */
    MPU->RASR = 0X0303002D;         /* ������ر�������(��ֹ����, ����cache, ������), ���MPUʵ��Ľ��� */
    MPU->CTRL = (1 << 2) | (1 << 0);/* ʹ��PRIVDEFENA, ʹ��MPU */
    SCB->SHCSR |= 1 << 16;          /* ʹ��MemManage */
}

/**
 * @brief       �����Ļ�������Ͻ���ʾ"RST"
 * @param       ��
 * @retval      ��
 */
void load_draw_dialog(void)
{
    lcd_clear(WHITE);                                                /* ���� */
    lcd_show_string(lcddev.width - 24, 0, 200, 16, 16, "RST", BLUE); /* ��ʾ�������� */
}


/**
 * @brief       ������
 * @param       x1,y1: �������
 * @param       x2,y2: �յ�����
 * @param       size : ������ϸ�̶�
 * @param       color: �ߵ���ɫ
 * @retval      ��
 */
void lcd_draw_bline(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t size, uint16_t color)
{
    uint16_t t;
    int xerr = 0, yerr = 0, delta_x, delta_y, distance;
    int incx, incy, row, col;

    if (x1 < size || x2 < size || y1 < size || y2 < size)
        return;

    delta_x = x2 - x1; /* ������������ */
    delta_y = y2 - y1;
    row = x1;
    col = y1;

    if (delta_x > 0)
    {
        incx = 1; /* ���õ������� */
    }
    else if (delta_x == 0)
    {
        incx = 0; /* ��ֱ�� */
    }
    else
    {
        incx = -1;
        delta_x = -delta_x;
    }

    if (delta_y > 0)
    {
        incy = 1;
    }
    else if (delta_y == 0)
    {
        incy = 0; /* ˮƽ�� */
    }
    else
    {
        incy = -1;
        delta_y = -delta_y;
    }

    if (delta_x > delta_y)
        distance = delta_x; /* ѡȡ�������������� */
    else
        distance = delta_y;

    for (t = 0; t <= distance + 1; t++) /* ������� */
    {
        lcd_fill_circle(row, col, size, color); /* ���� */
        xerr += delta_x;
        yerr += delta_y;

        if (xerr > distance)
        {
            xerr -= distance;
            row += incx;
        }

        if (yerr > distance)
        {
            yerr -= distance;
            col += incy;
        }
    }
}




void ctp_test(void)
{
//    uint8_t t = 0;
//  
//    uint16_t lastpos[10][2];        /* ���һ�ε����� */
//    uint8_t maxp = 5;

//    while (1)
//    {
//        tp_dev.scan(0);

//        for (t = 0; t < maxp; t++)
//        {
//            if ((tp_dev.sta) & (1 << t))
//            {
//                if (tp_dev.x[t] < lcddev.width && tp_dev.y[t] < lcddev.height)  /* ��������Ļ��Χ�� */
//                {
//                    if (lastpos[t][0] == 0XFFFF)
//                    {
//                        lastpos[t][0] = tp_dev.x[t];
//                        lastpos[t][1] = tp_dev.y[t];
//                    }

//                    lcd_draw_bline(lastpos[t][0], lastpos[t][1], tp_dev.x[t], tp_dev.y[t], 2, POINT_COLOR_TBL[t]); /* ���� */
//                    lastpos[t][0] = tp_dev.x[t];
//                    lastpos[t][1] = tp_dev.y[t];

//                    if (tp_dev.x[t] > (lcddev.width - 24) && tp_dev.y[t] < 20)
//                    {
//                        load_draw_dialog();/* ��� */
//                    }
//                }
//            }
//            else 
//            {
//                lastpos[t][0] = 0XFFFF;
//            }
//        }

//        delay_ms(5);
//   
//    }



  static  uint8_t t;
  static  uint8_t maxp = 5;
  static  uint16_t lastpos[10][2];        /* ���һ�ε����� */
    tp_dev.scan(0); // �������ɨ������Ƿ�������

    for (t = 0; t < maxp; t++)
    {
        if ((tp_dev.sta) & (1 << t))
        {
            if (tp_dev.x[t] < lcddev.width && tp_dev.y[t] < lcddev.height)
            {
                if (lastpos[t][0] == 0XFFFF)
                {
                    lastpos[t][0] = tp_dev.x[t];
                    lastpos[t][1] = tp_dev.y[t];
                }

                lcd_draw_bline(lastpos[t][0], lastpos[t][1], tp_dev.x[t], tp_dev.y[t], 2, POINT_COLOR_TBL[t]);
                lastpos[t][0] = tp_dev.x[t];
                lastpos[t][1] = tp_dev.y[t];

                if (tp_dev.x[t] > (lcddev.width - 24) && tp_dev.y[t] < 20)
                {
                    load_draw_dialog();
                }
            }
        }
        else 
        {
            lastpos[t][0] = 0XFFFF;
        }
    }
}


static void  PAJ7620_Init(void)
{
	while(!paj7620u2_init())//PAJ7620U2��������ʼ��
	{
		printf("PAJ7620U2_B Error!!!\r\n");
		
		HAL_Delay(500);
	}
	
	HAL_Delay(1000);
	Gesture_Init();
  printf("PAJ7620U2 OK\r\n");
}


static void paj7620_scan(void)
{
		gesture->detect = GS_Read_nByte(PAJ_GET_INT_FLAG1,2,&gesture->data[0]);//��ȡ����״̬			
		if(!gesture->detect)
		{   
			gesture->type =(uint16_t)gesture->data[1]<<8 | gesture->data[0];
			if(gesture->type) 
			{
				switch(gesture->type)
				{
					case GES_UP:               printf("%d:Up\r\n",gesture->type);            gesture->valid=1;      break; //����
					case GES_DOWN:             printf("%d:Down\r\n",gesture->type);          gesture->valid=1;      break; //����
					case GES_LEFT:             printf("%d:Left\r\n",gesture->type);          gesture->valid=1;      break; //����
					case GES_RIGHT:          	 printf("%d:Right\r\n",gesture->type);         gesture->valid=1;      break; //����
					case GES_FORWARD:       	 printf("%d:Forward\r\n",gesture->type);       gesture->valid=1;      break; //��ǰ
					case GES_BACKWARD:      	 printf("%d:Backward\r\n",gesture->type);      gesture->valid=1;      break; //���
					case GES_CLOCKWISE:     	 printf("%d:Clockwise\r\n",gesture->type);     gesture->valid=1;      break; //˳ʱ��
					case GES_ANTI_CLOCKWISE:   printf("%d:AntiClockwise\r\n",gesture->type); gesture->valid=1;      break; //��ʱ��
					case GES_WAVE:             printf("%d:Wave\r\n",gesture->type);          gesture->valid=1;      break; //�Ӷ�
					default:  																				      gesture->valid=0;      break;	
				}
				
//              printf("%d:\r\n",gesture->type);
			}
		}
}

void __lcd_init(void)
{
    
		 lcd_init();
	   g_point_color=RED; /*������ɫ*/ 
	   lcd_display_dir(1);		//��Ļ��ʾ����0������1����
	   lcd_clear(WHITE);
		
//    lcd_clear(BLACK);
//		lcd_clear(BLUE);
//		lcd_clear(RED);
//		lcd_clear(MAGENTA);
//		lcd_clear(GREEN);
//		lcd_clear(CYAN);
//		lcd_clear(YELLOW);
//		lcd_clear(BRRED);
//		lcd_clear(GRAY);
//		lcd_clear(LGRAY);
//		lcd_clear(BROWN);
	    lcd_fill(20 , 80,120 ,400,0xFFE0);
			lcd_fill(140, 80,240 ,400,0x8888);
			lcd_fill(260, 80,360 ,400,0x2222);
			lcd_fill(380, 80,480 ,400,0xbbbb); 
			lcd_fill(20,400,480,475,0x1111);
			
		 
			lcd_show_string(100,10,800,80,32,"Newbie:STM32H750XB Display LCD RGB888",RED); 
		

	    lcd_draw_rectangle(500,120,780,400,RED);
	   for(int i=137;i>=1;i-=3)
			{
	      lcd_draw_circle(640,260,i,RED);
			}
			
			

			
}


void delay_us_init()
{
  HAL_TIM_Base_Start_IT(&htim6);
}

void irda_tick_init()
{
  HAL_TIM_Base_Start_IT(&htim6);
}




void readInternalVoltage(void)
{
  static float Current_Temp;
  static	float voltage;
	uint16_t TS_CAL1;
	uint16_t TS_CAL2;
	uint16_t VREFINT_CAL;
	
  uint16_t value[3];
  TS_CAL1 = *(__IO uint16_t *)(0x1FF1E820); 
	TS_CAL2 = *(__IO uint16_t *)(0x1FF1E840); 
	VREFINT_CAL = *(__IO uint16_t *)(0x1FF1E860UL); 
 
   for(int i=0;i<=2;i++)  
	{
		  HAL_ADC_Start_DMA(&hadc3,(uint32_t*)value,3);
      value[i] = HAL_ADC_GetValue(&hadc3);
	    printf("ADC value[%d]=%d\r\n",i,value[i]);
		
		if(i==0)/*��λ����ѹ*/
		{
		  printf("ADC external voltage=%f\r\n",value[i]*3.3/65535);
			printf("\r\n");
		}
		
		if(i==1)/*�ڲ��ο���ѹ*/
		{
			printf("ADC inner voltage=%f\r\n",3.3*VREFINT_CAL/value[i]);
			printf("\r\n");
		}
		
		if(i==2)/*�ڲ��¶�*/
		{
		
			Current_Temp=(110.0 - 30.0)*(value[2] - TS_CAL1)/ (TS_CAL2 - TS_CAL1) + 30;
			printf("ADC temp:%f\r\n",Current_Temp);
			printf("\r\n");
		}
	  
	
	}



}









void qspi_test(void)
{
	static uint8_t test_buf[W25Q64JV_SECTOR_SIZE];
	static uint8_t read_buf[W25Q64JV_SECTOR_SIZE];

  for (int i = 0; i < W25Q64JV_SECTOR_SIZE; i++) {
    test_buf[i] = i;
		printf("test_buf[%d]=%d\r\n",i,test_buf[i]); /*��ֵ����Ϊ�޷��Ű�λ 255֮���ִ�0��ʼд*/
	}
  QSPI_W25Q64JV_Write(test_buf, 0x0, W25Q64JV_SECTOR_SIZE);
	
	
	
  QSPI_W25Q64JV_Read(read_buf, 0x0, W25Q64JV_SECTOR_SIZE);
	for(int i=0;i<W25Q64JV_SECTOR_SIZE;i++)
	 {
	  printf("read_buf[%d]=%d\r\n",i,test_buf[i]);
	
	 }
}

void adc_Calibration(void)
{
 HAL_ADCEx_Calibration_Start(&hadc3,ADC_CALIB_OFFSET,ADC_SINGLE_ENDED);//У׼ADC
}








/************************�״����ݽ���*******************************/

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == UART4) {  
        // �����ݷ�����Ϻ󣬴˲��ִ����ִ��
        // �������ڴ˴���Ӵ��룬�������һ��LED������һ����־
			
    }
		
		if (huart->Instance == USART1) {  
        // �����ݷ�����Ϻ󣬴˲��ִ����ִ��
        // �������ڴ˴���Ӵ��룬�������һ��LED������һ����־
			
    }
}


/************************�״﷢������*******************************/
static void initUartPacket(UartPacket* packet) {
    packet->header[0] = 0x55; /*����ͷ*/
    packet->header[1] = 0x5A;
    packet->tail = 0xFE;/*����β*/
}

static void setPacketContent(UartPacket* packet, uint8_t command, uint16_t data) {
    packet->command = command; /*����*/
	  packet->data_high = (data >> 8) & 0xFF; /*�������ݸ�λ*/
    packet->data_low = data & 0xFF;/*�������ݵ�λ*/
    packet->checksum = command ^ packet->data_high ^ packet->data_low; // ����У���
}

static void sendUartPacket(UART_HandleTypeDef* huart, UartPacket* packet) {
    HAL_UART_Transmit(huart, (uint8_t*)packet, sizeof(UartPacket), HAL_MAX_DELAY);
}



/*֧����������
0x09   �򿪹ر��״�       0x0001��  0x0000�ر�
0x89   ��ȡ�״￪��״̬
0x01   д�����ȼ�       0x0000  ---  0x000F(15) 0�������
0x81   ��ȡ����ȼ� 
0x02   д���ӳ�ʱ��       0x0000  ---  0xffff
0x82   ��ȡ�ӳ�ʱ��       
0x03   �򿪹رչ��       0x0001��   0x0000�ر�  
0x83   ��ȡ��п���
0x06   ���÷���ʱ��       0x0000  ---  0xffff    

����ʱ�䣬Ҳ�Ʊ���ʱ�䣬���״� OUT �����ɸ߱��֮�󣬼���Ӧ���
�����󣬽���������һ��ʱ��ֹͣ��⣬���ֹͣ����ʱ�䱻��Ϊ����ʱ�䣬
Ĭ�� 1S(��ֵ 0x03e8=1000ms),һ�㲻���޸ģ���������Ҫ�޸ģ�Ӧ���ò�С��
500ms
*/
UartPacket packet;
void usart_radar_process(void)
{
  static uint8_t commandValue = 0x09;
  static uint16_t dataValue = 0x0000;
	
  initUartPacket(&packet);
  setPacketContent(&packet, commandValue, dataValue);
  sendUartPacket(&huart1, &packet);
}






/*********************debug�������ݽ���***********************************/
uint8_t rx_data;
uint8_t rx_buffer[128];
uint32_t rx_index = 0;

//UartPacket txPacket;  // ���ڷ��͵����ݰ�
UartPacket rxPacket;  // ���ڽ��յ����ݰ�


//{"LED":"ON"} or {"LED":"OFF"}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == UART4) 
    {
        // Add received byte to the buffer
        rx_buffer[rx_index++] = rx_data;

        // Check if received a complete JSON message e.g., {"LED":"ON"} or {"LED":"OFF"}
        if (rx_data == '}' && rx_index < sizeof(rx_buffer)) 
        {
            rx_buffer[rx_index] = '\0';  // Null terminate the string
            if (strstr((char *)rx_buffer, "{\"LED\":\"ON\"}")) 
            {
               HAL_GPIO_WritePin(GPIOC, LED_R_Pin, GPIO_PIN_RESET);
            } 
            else if (strstr((char *)rx_buffer, "{\"LED\":\"OFF\"}")) 
            {
               HAL_GPIO_WritePin(GPIOC, LED_R_Pin, GPIO_PIN_SET);
            }
						HAL_UART_Transmit_IT(&huart4,(uint8_t*)rx_buffer,sizeof(rx_buffer));
						
            rx_index = 0; // Reset the buffer index
        }
				
				

        // Restart interrupt for next byte
        HAL_UART_Receive_IT(&huart4, &rx_data, 1);
     }
		
//					if (huart->Instance == USART1) 
//				{
//		      // ������ջ����������뷢�͵����ݰ���ƥ��
//            if(memcmp(&packet, &rxPacket, sizeof(UartPacket)) == 0) 
//            {
//							/*����ƥ�䣬д����ɹ�*/
//                printf("radar write command ok !!!\r\n");
//            }
//            else 
//            {
//                // ���ݲ�ƥ�䣬�ж��Ƿ�Ϊ��ѯ����
//							   printf("radar write command ERR !!!\r\n");
//            }
//						
//						HAL_UART_Receive_IT(&huart1, (uint8_t*)&rxPacket, sizeof(UartPacket));
//		    }
		
}
/*******************************************************************************************************************/
/*******************************************************************************************************************/
/**************************************************SD����������*****************************************************/
/*******************************************************************************************************************/
/*******************************************************************************************************************/
FIL file;      // �ļ�����
FRESULT fres;  // FatFs�����ķ�������
FILINFO fno;
FATFS fs;       // �ļ�ϵͳ����



/*************************����SD***************************************/
void mount_sd(void)
{
	retSD = f_mount(&fs, SDPath, 1);
	if(retSD)
	{
		   #ifdef  FATFS_DEBUG
			   printf(" mount error : %d \r\n",retSD);
			 #endif
			
			Error_Handler();
	}
	else
			
	     #ifdef  FATFS_DEBUG
			   printf(" mount sucess!!! \r\n");
			 #endif
	
}
/*************************��ʽ��SD��***************************************/
/*�ڵ���f_mkfs֮ǰ��ȷ��û�ж�Ŀ�����������й���
��ʽ��֮��ͨ����ж����������ȷ�����е�ϵͳ��Դ�õ���ȷ���ͷ�*/

//�����������������С
#define  FF_MAX_SS  512

FRESULT mkfs_sdcard(void) {

    BYTE work[FF_MAX_SS];  // Working buffer for f_mkfs

    fres = f_mount(&fs, "", 0);  // �����豸
    if (fres != FR_OK) {
			#ifdef  FATFS_DEBUG
			printf("ERR SD Mounting failed:%d\r\n",fres);
			#endif
			
        return fres;  // ���ش���
    }

    fres = f_mkfs("", FM_FAT32, 0, work, sizeof(work));  // Format as FAT32
    if (fres != FR_OK) {
			#ifdef  FATFS_DEBUG
			printf("ERR SD Formatting failed:%d\r\n",fres);
			#endif
        return fres;  //��ʽ��ʧ��
    }

    f_mount(0, "", 0);  // ��ʽ����Ĭ��ж������
		#ifdef  FATFS_DEBUG
		printf("SD Formatting ok\r\n");
		#endif
    return FR_OK;
}



/**********************���Ŀ¼*****************************/
static uint8_t Check_whether_the_directory_exists(const char* path)
{
    
    fres = f_stat(path, &fno);
    if (fres == FR_OK && (fno.fattrib & AM_DIR)) {
        return 1;  // Ŀ¼����
    }
    return 0;  // Ŀ¼������
}

/************************����Ŀ¼***********************************/
static uint8_t createDirectory(const char* path) {
    if (!Check_whether_the_directory_exists(path)) {
        fres = f_mkdir(path);
        if(fres != FR_OK) {
            //���������ʾ������Ϣ
					#ifdef  FATFS_DEBUG
					printf("ERR create Directory :%d\r\n",fres);
					#endif
					    return 1;/*���ش�����Ϣ*/
        } else {
            // Ŀ¼�ɹ�����
					#ifdef  FATFS_DEBUG
					    printf("create Directory ok\r\n");
					#endif
						  return 0;
        }
    } else {
        // Ŀ¼�Ѿ�����
			#ifdef  FATFS_DEBUG
		     printf("Directory already exists\r\n");
			#endif
		     return 0;
    }
}

/*****************************����Ŀ¼********************************/
uint8_t use_createDirectory(void)
{
  createDirectory("/system");
}





/************************����ļ��Ƿ����******************************/
static uint8_t createFileIfNotExist(const char* folderPath, const char* filename) {
    char fullPath[256];  // �����ļ�·�����Ȳ�����256�����Ը���ʵ����Ҫ������С

    // �ж��Ƿ�Ϊ��Ŀ¼
    if (folderPath && *folderPath) {
        sprintf(fullPath, "%s/%s", folderPath, filename);
    } else {
        sprintf(fullPath, "%s", filename); // ֱ��ʹ���ļ�������Ϊ���ڸ�Ŀ¼
    }

    // ����ļ��Ƿ����
    fres = f_stat(fullPath, &fno);
    if (fres == FR_OK) {
        // �ļ�����
        return 1;
    } else if (fres == FR_NO_FILE) {
        // �ļ������ڣ����Դ���
        fres = f_open(&file, fullPath, FA_WRITE | FA_CREATE_NEW);
        if (fres == FR_OK) {
            f_close(&file);
            return 0;  // �ļ��ѳɹ�����
        } else {
            return -1;  // �����ļ�ʱ��������
        }
    } else {
        // ��������
        return -1;
    }
}

/*************************�����ļ�*************************/
static uint8_t create_a_file(const char* folderPath, const char* filename) {
    uint8_t result = createFileIfNotExist(folderPath, filename);

    if (result == 1) {
        // �ļ��Ѵ���
        #ifdef FATFS_DEBUG
            printf("File already exists: %d\r\n", fres);
        #endif
    } else if (result == 0) {
        // �ļ��ѳɹ�����
        #ifdef FATFS_DEBUG
            printf("File created successfully: %d\r\n", fres);
        #endif
    } else {
        // ��������
        #ifdef FATFS_DEBUG
            printf("Error creating file: %d\r\n", fres);
        #endif
    }

    return 0;
}

/**************************�����ļ�************************************/
uint8_t use_create_a_file(void)
{
  // �ڸ�Ŀ¼����
  // create_a_file("", "myfile.txt");
  // �������ļ��д���
  // create_a_file("system", "myfile.txt");
    create_a_file("system", "file1.txt");
	  create_a_file("system", "file2.txt");
}

/*************************д�ļ�����*************************************/

uint8_t writeToFile(const char *filePath, const void *buffer, uint32_t length) {
  
    UINT bytesWritten;

    // ���ļ�
    fres = f_open(&file, filePath, FA_WRITE | FA_OPEN_ALWAYS);
    if (fres != FR_OK) {
        // �ļ���ʧ��
        return -1;
    }

    // ��дָ���Ƶ��ļ�ĩβ���Ա����ļ�ĩβ׷������
    f_lseek(&file, f_size(&file));

    // д������
    fres = f_write(&file, buffer, length, &bytesWritten);

    // �ر��ļ�
    f_close(&file);

    // ����Ƿ��������ݶ���д��
    if (fres == FR_OK && bytesWritten == length) {
        #ifdef FATFS_DEBUG
            printf("File write length ok: %d\r\n", fres);
        #endif
			 return 0; // �ɹ�
    } else {
			 #ifdef FATFS_DEBUG
            printf("File write length err: %d\r\n", fres);
        #endif
			return -1; // д�����
    }
}


/**************************д���ļ�************************************/
uint8_t use_writeToFile(void) {
   
	 uint8_t buffer[256];
	 for(int i=0;i<256;i++)
	{
	  buffer[i]=i;
	}

   writeToFile("system/file1.txt", buffer, sizeof(buffer));

    return 0;  
}



/**
 * ��ȡ�ļ������ݡ�
 * @param filePath Ҫ��ȡ���ļ���·����
 * @param buffer ��Ŷ�ȡ���ݵĻ�������
 * @param bufferSize �������Ĵ�С��
 * @param offset �����ַ���ʼ��ȡ��
 * @return ʵ�ʶ�ȡ���ֽ�������-1��ʾ����
 */
uint32_t readFileContent(const char* filePath, void* buffer, int bufferSize, DWORD offset) {

    UINT bytesRead;

    fres = f_open(&file, filePath, FA_READ);
    if (fres != FR_OK) {
        // �ļ���ʧ��
        return -1;
    }

    // ���ö�ȡ�Ŀ�ʼλ��
    if (f_lseek(&file, offset) != FR_OK) {
        // ������
        f_close(&file);
        return -1;
    }

    fres = f_read(&file, buffer, bufferSize - 1, &bytesRead);
    if (fres != FR_OK) {
        // ��ȡ�ļ�ʧ��
        f_close(&file);
        return -1;
    }

//    buffer[bytesRead] = '\0';  // ����ַ�����ֹ��

    f_close(&file);  // �ǵùر��ļ�
		#ifdef FATFS_DEBUG
	  printf("bytesRead num=%d\r\n",bytesRead);/*ʵ�ʶ�ȡ�ֽ���*/
    #endif
    return bytesRead;  // ����ʵ�ʶ�ȡ���ֽ���
}


/*ʹ�ö��ļ�����*/
uint8_t use_readFileContent(void)
{
  uint8_t buffer[512];
	memset(buffer, 0, sizeof(buffer));/*��ջ�����*/
	
  uint32_t bytesRead = readFileContent("system/file1.txt", buffer, sizeof(buffer), 0); // �ӵ�0���ֽڿ�ʼ��ȡ
	HAL_Delay(10);
	
	
	for(int i=0;i<512;i++)
	printf("read sd buffer[%d]:%x\r\n",i,buffer[i]);

}


/*sd��Ӧ��*/
void sd_application(void)
{
//	 mkfs_sdcard();//��ʽ��sd��
	 mount_sd();/*����sd��*/
   use_createDirectory();/*�����ļ���*/
   use_create_a_file();/*�����ļ�*/ 
   use_writeToFile();/*д�ļ�*/
   use_readFileContent();/*���ļ�*/
}





void bsp_init(void)
{

	
	uint8_t ID[2];
//	u8g2_t u8g2;
//  u8g2Init(&u8g2);
 //  sys_qspi_enable_memmapmode(0);  
	 delay_us_init();
	 irda_tick_init();

	 SDRAM_InitSequence(); /*��ʼ��SDRAM*/
	// HAL_GPIO_WritePin(GPIO_LCD_BL_GPIO_Port, GPIO_LCD_BL_Pin, GPIO_PIN_RESET);/*�ر���Ļ����*/
	 __lcd_init();
   PAJ7620_Init();
 	 DHT11_Init();
 	 HAL_Delay(1000);
	
	 adc_Calibration();
   HAL_ADC_Start(&hadc3);
   //BSP_W25Qx_Init();
	 lcd_clear(WHITE);
//   mount_sd();/*����SD��*/
	 QSPI_W25Q64JV_Reset();
   uint8_t id[2];
   if (QSPI_OK != QSPI_W25Q64JV_DeviceID(id)) {
    while (1);
  }
//	 printf("QSPI_W25Q64JV id=%x%X\r\n",id[0],id[1]);
    HAL_Delay(1000);

  for(int i=0;i<=800;i+=2)
	{
		for(int j=0;j<=480;j+=2)  
     lcd_draw_point(i,j,RED);
	}


	
   draw_grid(800, 480, 80, 80, 4,RED);

	//testDrawXBM(&u8g2);
  //u8g2DrawTest(&u8g2);
	
	HAL_UART_Receive_IT(&huart4, &rx_data, 1);
//  HAL_UART_Receive_IT(&huart1, (uint8_t*)&rxPacket, sizeof(UartPacket));
//   usart_radar_process();

	
	
	  tp_dev.init();
//	  printf("run here \r\n");   
	
   
		HAL_Delay(100);
    sd_application();
}





void application(void)
{
	
 static uint8_t temperature=10;  	    
 static uint8_t humidity=10; 
 static uint8_t write_qspi_displaynum; 
 static uint8_t read_qspi_displaynum; 
	
	static uint32_t  tick=0; 
	tick=HAL_GetTick(); 
//	if(tick%200==0)
//	{
//	 paj7620_scan();
//	}
//	
//	if(tick%1000==0)
//	{
//	    DHT11_Read_Data(&temperature,&humidity);	
//		
//      printf("Tem:%d\r\n",temperature);
//		  printf("Hum:%d\r\n",humidity);	
//		  printf("\r\n");
//		
//	}
	
//	if(tick%2000==0)
//	{
//	   
//		write_qspi_displaynum++;

//		lcd_show_num(600,200,write_qspi_displaynum,3,32,RED);
//	}
	
	
	
	
// if(irda_code_flag==1)
// {
//    		QSPI_W25Q64JV_Write(&write_qspi_displaynum, 0x0, 1);
//	      lcd_show_string(300,300,100,100,32,"qspi write",RED); 
//	      lcd_show_num(300,400,read_qspi_displaynum,3,32,RED);
//        printf("write qspi flash ok");
//	 
//     
//	           irda_code_flag=0;
// }
//  else if(irda_code_flag==2)
//	{	
//  	    QSPI_W25Q64JV_Read(&read_qspi_displaynum, 0x0, 1);
//     	  irda_code_flag=0;   
//		    lcd_show_string(300,300,100,100,32,"save value for qspi ok",RED); 
//		    lcd_show_num(300,500,read_qspi_displaynum,3,32,RED);
//		   
//	}
//	
	
	
//	if(tick%800==0)
//	{
//	   readInternalVoltage();
//	}

	
}

