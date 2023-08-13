/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32h7xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32h7xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include "bsp.h"
#include "app.h"
#include "stdio.h"
#include "usart.h"
#include "irda_nec.h"
#include "lcd.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */
 volatile uint32_t buttonPressTime = 0;
 volatile uint8_t buttonPressCount = 0;
 int buzzer_flag;
 
 uint16_t irda_count=0; /*0-65535*/
 uint16_t tick_us_count;
 
 bool isValueChanged = false;  // ��Ǳ����Ƿ�ı�
 bool nec_ValueChanged = false;  // ��Ǳ����Ƿ�ı�
 
bool GPIO_TS_IIC_INT_Pin_state = false;
uint8_t irda_code_flag;

uint32_t IC_TIMES;  // �����������λ1ms
uint8_t IC_START_FLAG;  // ����ʼ��־��1���Ѳ��񵽸ߵ�ƽ��0����û�в��񵽸ߵ�ƽ
uint8_t IC_DONE_FLAG;  // ������ɱ�־��1�������һ�θߵ�ƽ����
uint16_t IC_VALUE;  // ���벶��Ĳ���ֵ
/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern PCD_HandleTypeDef hpcd_USB_OTG_FS;
extern DMA_HandleTypeDef hdma_adc3;
extern LTDC_HandleTypeDef hltdc;
extern TIM_HandleTypeDef htim5;
extern DMA_HandleTypeDef hdma_uart4_tx;
extern TIM_HandleTypeDef htim7;

/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
  while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */

  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32H7xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32h7xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles EXTI line3 interrupt.
  */
void EXTI3_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI3_IRQn 0 */

  /* USER CODE END EXTI3_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(WL_HOST_WAKE_Pin);
  /* USER CODE BEGIN EXTI3_IRQn 1 */

  /* USER CODE END EXTI3_IRQn 1 */
}

/**
  * @brief This function handles EXTI line4 interrupt.
  */
void EXTI4_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI4_IRQn 0 */
	
  /* USER CODE END EXTI4_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(USER_KEY_Pin);
  /* USER CODE BEGIN EXTI4_IRQn 1 */

  /* USER CODE END EXTI4_IRQn 1 */
}

/**
  * @brief This function handles DMA1 stream0 global interrupt.
  */
void DMA1_Stream0_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Stream0_IRQn 0 */

  /* USER CODE END DMA1_Stream0_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_uart4_tx);
  /* USER CODE BEGIN DMA1_Stream0_IRQn 1 */

  /* USER CODE END DMA1_Stream0_IRQn 1 */
}

/**
  * @brief This function handles EXTI line[15:10] interrupts.
  */
void EXTI15_10_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI15_10_IRQn 0 */

  /* USER CODE END EXTI15_10_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_TS_IIC_INT_Pin);
  /* USER CODE BEGIN EXTI15_10_IRQn 1 */

  /* USER CODE END EXTI15_10_IRQn 1 */
}

/**
  * @brief This function handles TIM5 global interrupt.
  */
void TIM5_IRQHandler(void)
{
  /* USER CODE BEGIN TIM5_IRQn 0 */

  /* USER CODE END TIM5_IRQn 0 */
  HAL_TIM_IRQHandler(&htim5);
  /* USER CODE BEGIN TIM5_IRQn 1 */
	
  
	nec_ValueChanged=true;
	
	
	uint32_t code=IC_IRDA_NEC();
	if(code!=0x00)
	{
	  printf("irda_code:%08x\r\n",code);
		if(code==0x00ff22dd)
		{
		  lcd_clear(WHITE);
		}else if(code==0x00ff30cf)
		{
			irda_code_flag=1;
		}
	  else if(code==0x00ff18e7){
			
		  irda_code_flag=2;
		
	}
		
		
		
		
		
	}
  /* USER CODE END TIM5_IRQn 1 */
}

/**
  * @brief This function handles TIM7 global interrupt.
  */
void TIM7_IRQHandler(void)
{
  /* USER CODE BEGIN TIM7_IRQn 0 */
//  static int num;
	static uint16_t buzzer_count;
  

	
// int BUZZER_ON_TIME_MS =100;
// int BUZZER_OFF_TIME_MS= 2000;
  // ���ð���������
 //Button_Process();
  /* USER CODE END TIM7_IRQn 0 */
  HAL_TIM_IRQHandler(&htim7);
  /* USER CODE BEGIN TIM7_IRQn 1 */
 
	/************һ���жϷ�תһ��***********************/
//	 num++;
//	if(num)
//	HAL_GPIO_TogglePin(PB2_GPIO_Port, PB2_Pin);
	/***************************************************/
 
	/*********************BUZZER*************************/
	if(buzzer_flag==1)
	{
	     buzzer_count++;
    if (buzzer_count == 1) {
        BUZZER_ON; 
	    }
		else if (buzzer_count >= 100) {
        BUZZER_OFF;
			
    if (buzzer_count >= 2100) {
        buzzer_count = 0;
       }
    }
   	buzzer_flag=0;	
	}
  /***************************************************/
  /* USER CODE END TIM7_IRQn 1 */
}

/**
  * @brief This function handles LTDC global interrupt.
  */
void LTDC_IRQHandler(void)
{
  /* USER CODE BEGIN LTDC_IRQn 0 */

  /* USER CODE END LTDC_IRQn 0 */
  HAL_LTDC_IRQHandler(&hltdc);
  /* USER CODE BEGIN LTDC_IRQn 1 */

  /* USER CODE END LTDC_IRQn 1 */
}

/**
  * @brief This function handles USB On The Go FS global interrupt.
  */
void OTG_FS_IRQHandler(void)
{
  /* USER CODE BEGIN OTG_FS_IRQn 0 */

  /* USER CODE END OTG_FS_IRQn 0 */
  HAL_PCD_IRQHandler(&hpcd_USB_OTG_FS);
  /* USER CODE BEGIN OTG_FS_IRQn 1 */

  /* USER CODE END OTG_FS_IRQn 1 */
}

/**
  * @brief This function handles DMAMUX2 overrun interrupt.
  */
void DMAMUX2_OVR_IRQHandler(void)
{
  /* USER CODE BEGIN DMAMUX2_OVR_IRQn 0 */

  /* USER CODE END DMAMUX2_OVR_IRQn 0 */

  /* USER CODE BEGIN DMAMUX2_OVR_IRQn 1 */

  /* USER CODE END DMAMUX2_OVR_IRQn 1 */
}

/**
  * @brief This function handles BDMA channel0 global interrupt.
  */
void BDMA_Channel0_IRQHandler(void)
{
  /* USER CODE BEGIN BDMA_Channel0_IRQn 0 */

  /* USER CODE END BDMA_Channel0_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_adc3);
  /* USER CODE BEGIN BDMA_Channel0_IRQn 1 */

  /* USER CODE END BDMA_Channel0_IRQn 1 */
}

/* USER CODE BEGIN 1 */

void key_process(void)
{
 if(HAL_GPIO_ReadPin(USER_KEY_GPIO_Port,USER_KEY_Pin)==RESET)
   {
      // �����ͷ�ʱ�Ĵ���
      uint32_t currentTime = HAL_GetTick();
      uint32_t pressDuration = currentTime - buttonPressTime;

       if (pressDuration<800&&pressDuration>20) {
        // �̰�������
				 LED_R_ON;
				

      } else {
        // ����������
			   buttonPressCount = 0;
				// ִ�г������¼�����
				 LED_R_OFF;

        
      }
    }

}




uint16_t get_irda_tim(void)
{
  return irda_count;
}




void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	static uint16_t get_new_tim; /*��ȡ����ʱ��*/
	static uint16_t get_old_tim; /*��ȡ�ϴ�ʱ��*/
	static uint16_t think_tim;/*���ʱ��*/
	
  if(GPIO_Pin==GPIO_TS_IIC_INT_Pin)
	 {
		 get_new_tim=HAL_GetTick();
		 /**************�������������������**************************/	
		if(get_new_tim > get_old_tim) 
			think_tim = get_new_tim - get_old_tim;          /*�����*/
		else   
			think_tim = get_new_tim + 65535 - get_old_tim;    /*���*/
		
		get_old_tim = get_new_tim;
    /***************************************************************/
      /*����ж�ʱ��Ϊ10ms GPIO_TS_IIC_INT_Pin_state=true*/
		
		if(think_tim>=7&&think_tim<=13)
		{
			ctp_test();
		 //GPIO_TS_IIC_INT_Pin_state=true;
		 //HAL_GPIO_TogglePin(GPIOB, PB2_Pin);
		}
		
	}
	 
//	if(GPIO_Pin==USER_KEY_Pin)
//	{
//	  key_process();
//	}


}


/******************************NECЭ��******************************************************
������:  �͵�ƽ9ms      �ߵ�ƽ4.5ms      13.5ms
0:       �͵�ƽ0.56ms   �ߵ�ƽ0.56ms     1.12ms
1:       �͵�ƽ0.56ms   �ߵ�ƽ1.68ms     2.24ms
********************************************************************************************/
/**********************���ݶ�ʱ�������ɵ����ɵ�������***************************************/
#define   TIM_INTERRUPT_TIME        1                     /* ��һ����ʱ��(us)*/
#define   OVERFLOW                  65535                 /* ���ֵ (����������������޷���16λ) */
/******************NECЭ�����(������NECЭ�飬�Ǳ�Ҫ��ģ��ɶ�̬���ӷ�Χ��)***********************/
#define   START_MIN               13000/TIM_INTERRUPT_TIME    /* ��������С�������� */
#define   START_MAX               14000/TIM_INTERRUPT_TIME    /* ���������������� */
  
#define   SURE_0_MIN              1000/TIM_INTERRUPT_TIME     /* 0��С�������� */           
#define   SURE_0_MAX              1200/TIM_INTERRUPT_TIME     /* 0���������� */            
 
#define   SURE_1_MIN              2100/TIM_INTERRUPT_TIME      /* 1��С�������� */ 
#define   SURE_1_MAX              2350/TIM_INTERRUPT_TIME      /* 1���������� */            
/*******************************************************************************************/  

uint32_t scan_irda(void)
{
	static uint16_t get_new_tim; /*��ȡ����ʱ��*/
	static uint16_t get_old_tim; /*��ȡ�ϴ�ʱ��*/
	static uint16_t think_tim;/*���ʱ��*/
	static char irda_state;/*����״̬*/
	static uint32_t irda_decoder,get_ir_code;/*����ֵ*/
	static uint8_t code_flag; /*����ɹ���־λ*/
  static uint8_t irda_data_bit_count=32;/*�����32��bit*/

	if(isValueChanged)/*�����½����ж�*/
	{
	
		/*��ȡ����ֵ*/
		get_new_tim=get_irda_tim();
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
	
	 
	 isValueChanged=false;/*�ָ��жϱ�־λ*/
	
	}
	
	if(code_flag==1)
	{
	  
	  get_ir_code=irda_decoder;
    code_flag=0;	
		irda_decoder=0;	
		return get_ir_code;
		
	}
	
	 return 0x00;
		
			
}





/* USER CODE END 1 */
