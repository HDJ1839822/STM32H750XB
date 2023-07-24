#include "ap6212_wifi.h"

#include "stdio.h" 
#include "sdmmc.h" 
#include "string.h" 
#include "fatfs.h"













void get_ap6212_wifi_informatization(void)
{
	  HAL_SD_CardCIDTypeDef sdcard_cid;
    HAL_SD_GetCardCID(&hsd2,&sdcard_cid);
    printf("wifi weak Test...\r\n");
	  printf(" ManufacturerID: %d \r\n",sdcard_cid.ManufacturerID); /*��ȡsd��������*/
	 uint32_t state =HAL_SD_GetCardState(&hsd2);
	
	printf("%04d\r\n",state);
	
  /* ���SD���Ƿ��������������ݴ���ģʽ�Ĵ���״̬�� */
  if(HAL_SD_GetCardState(&hsd2) == HAL_SD_CARD_READY)
  {      
    printf("Initialize SD card successfully!\r\n");
    // ��ӡSD��������Ϣ
    printf(" SD card information! \r\n");
		/*ʵ�ʴ洢���� = LogBlockNbr * LogBlockSize*/
		printf(" LogBlockNbr   : %d \r\n", hsd1.SdCard.LogBlockNbr);	// �߼�������
	  printf(" LogBlockSize  : %d \r\n", hsd1.SdCard.LogBlockSize); // �߼����С(�ֽ�)
    printf(" Card Log Capacity  : %llu byte\r\n", (unsigned long long)hsd1.SdCard.BlockSize * hsd1.SdCard.BlockNbr);// ��ʾ����(�ֽ�)
		printf(" Card Log Capacity  : %llu M\r\n", ((unsigned long long)hsd1.SdCard.BlockSize * hsd1.SdCard.BlockNbr)/(1024*1024));// ��ʾ����(M)
		printf(" CardBlockNbr  : %d \r\n", hsd1.SdCard.BlockNbr);   // ���������
    printf(" CardBlockSize : %d \r\n", hsd1.SdCard.BlockSize);   // ������С
		printf(" Card physical Capacity  : %llu M\r\n", ((unsigned long long)hsd1.SdCard.BlockSize * hsd1.SdCard.BlockNbr)/(1024*1024));// ��ʾ����(�ֽ�)

    printf(" RCA           : %d \r\n", hsd1.SdCard.RelCardAdd);  // ����Ե�ַ
    printf(" CardType      : %d \r\n", hsd1.SdCard.CardType);    // ������
    // ��ȡ����ӡSD����CID��Ϣ
  
   
  }
  else
  {
    printf("wifi init fail!\r\n" );
  }



}



