#ifndef W25Q64__H__
#define W25Q64__H__


#include "main.h"
/*���������*/
#define W25Q_SPI					hqspi

/*Ƭѡ���Ŷ����뺯������*/
#define W25Q_CS_Pin 			QSPI_CS_W25Q64_Pin
#define W25Q_CS_Port			QSPI_CS_W25Q64_GPIO_Port

#define W25Q_CS_Level(_CS_STATE__)		HAL_GPIO_WritePin(W25Q_CS_Port, W25Q_CS_Pin, (GPIO_PinState)_CS_STATE__)

//#define W25Q_CS_Level(_CS_STATE__) (*((volatile unsigned int *)(0x42000000+((uint32_t)&GPIOC->ODR-0x40000000)*32+0*4))) = _CS_STATE__

#define W25Q_W_ENA				0x06		//дʹ��
#define W25Q_W_DIS				0x04		//д��ֹ

#define W25Q_R_Dat				0x03		//������

#define W25Q_R_STA_REG1		0x05		//��״̬�Ĵ���1�������ŵ��ֽھ��ǵ�ǰ״̬
#define	W25Q_R_STA_REG2		0x35		//��״̬�Ĵ���2�������ŵ��ֽھ��ǵ�ǰ״̬

#define W25Q_W_STA_REG_		0x01		//д״̬�Ĵ���,д�������ֽ�,�ֱ𵽼Ĵ���1,�ͼĴ���2

#define W25Q_Page_Program 0x02		//ҳ��̣��ȸ�3����ַ�ֽڣ��ٸ�һ�������ֽ�

#define W25Q_Block_Erase	0xD8		//�����64k��������ַ�ֽ�

#define W25Q_Sector_Erase	0x20		//������������������ַ

#define W25Q_Full_Erase		0xC7		//ȫƬ����
												//0x60
												
#define W25Q_Susp_Erase		0x75		//��ͣ����

#define W25Q_Rest_Erase		0x7A		//�ָ�����

#define W25Q_PowDow_Mode	0xB9		//����ģʽ

#define W25Q_HPer_Mode		0xA3		//������ģʽ

#define W25Q_JEDEC_ID			0x9F		//��3���ֽڷֱ����������ҡ��洢�����͡�����




#endif /*W25Q64__H__*/



//#define QSPI_BK1_CLK_GPIO_PORT          GPIOB
//#define QSPI_BK1_CLK_GPIO_PIN           GPIO_PIN_2
//#define QSPI_BK1_CLK_GPIO_AF            GPIO_AF9_QUADSPI
//#define QSPI_BK1_CLK_GPIO_CLK_ENABLE()  do{ __HAL_RCC_GPIOB_CLK_ENABLE; }while(0)   /* PB???????? */

//#define QSPI_BK1_NCS_GPIO_PORT          GPIOB
//#define QSPI_BK1_NCS_GPIO_PIN           GPIO_PIN_6
//#define QSPI_BK1_NCS_GPIO_AF            GPIO_AF10_QUADSPI
//#define QSPI_BK1_NCS_GPIO_CLK_ENABLE()  do{ __HAL_RCC_GPIOB_CLK_ENABLE; }while(0)   /* PB???????? */

//#define QSPI_BK1_IO0_GPIO_PORT          GPIOD
//#define QSPI_BK1_IO0_GPIO_PIN           GPIO_PIN_11
//#define QSPI_BK1_IO0_GPIO_AF            GPIO_AF9_QUADSPI
//#define QSPI_BK1_IO0_GPIO_CLK_ENABLE()  do{ __HAL_RCC_GPIOD_CLK_ENABLE; }while(0)   /* PD???????? */

//#define QSPI_BK1_IO1_GPIO_PORT          GPIOD
//#define QSPI_BK1_IO1_GPIO_PIN           GPIO_PIN_12
//#define QSPI_BK1_IO1_GPIO_AF            GPIO_AF9_QUADSPI
//#define QSPI_BK1_IO1_GPIO_CLK_ENABLE()  do{ __HAL_RCC_GPIOD_CLK_ENABLE; }while(0)   /* PD???????? */

//#define QSPI_BK1_IO2_GPIO_PORT          GPIOD
//#define QSPI_BK1_IO2_GPIO_PIN           GPIO_PIN_13
//#define QSPI_BK1_IO2_GPIO_AF            GPIO_AF9_QUADSPI
//#define QSPI_BK1_IO2_GPIO_CLK_ENABLE()  do{ __HAL_RCC_GPIOD_CLK_ENABLE; }while(0)   /* PD???????? */

//#define QSPI_BK1_IO3_GPIO_PORT          GPIOE
//#define QSPI_BK1_IO3_GPIO_PIN           GPIO_PIN_2
//#define QSPI_BK1_IO3_GPIO_AF            GPIO_AF9_QUADSPI
//#define QSPI_BK1_IO3_GPIO_CLK_ENABLE()  do{ __HAL_RCC_GPIOE_CLK_ENABLE; }while(0)   /* PE???????? */

/******************************************************************************************/


uint8_t qspi_wait_flag(uint32_t flag, uint8_t sta, uint32_t wtime); /* QSPI???????? */
uint8_t qspi_init(void);    /* ?????QSPI */
void qspi_send_cmd(uint8_t cmd, uint32_t addr, uint8_t mode, uint8_t dmcycle);  /* QSPI???????? */
uint8_t qspi_receive(uint8_t *buf, uint32_t datalen);   /* QSPI???????? */
uint8_t qspi_transmit(uint8_t *buf, uint32_t datalen);  /* QSPI???????? */





















