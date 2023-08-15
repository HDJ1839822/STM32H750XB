
#ifndef __APP_H
#define __APP_H


#include "stm32h7xx_hal.h"
#include "ff.h"


typedef struct {
    uint8_t header[2];    // ͷ���̶��ֽ�
    uint8_t command;      // �����ֽ�
    uint8_t data_high;    // ���ݸ��ֽ�
    uint8_t data_low;     // ���ݵ��ֽ�
    uint8_t checksum;     // У���ֽ�
    uint8_t tail;         // β���̶��ֽ�
} UartPacket;




 extern uint32_t rawValue;

void ctp_test(void);
void lcd_init(void);


void bsp_init(void);
void application(void);


/*****************SD����������*****************************/
void mount_sd(void);/*����sd��*/
FRESULT mkfs_sdcard(void);/*��ʽ��sd��  ��ʽ��ǰ���ܹ���sd��*/

uint8_t use_createDirectory(void);/*�����ļ���*/
uint8_t use_create_a_file(void);/*�����ļ�*/ 
uint8_t use_writeToFile(void);/*д�ļ�*/
uint8_t use_readFileContent(void);/*���ļ�*/



/**********************************************************/

#endif /* __APP_H */
