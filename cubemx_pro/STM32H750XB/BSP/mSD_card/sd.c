#include "sd.h" 

#include "stdio.h" 
#include "sdmmc.h" 
#include "string.h" 


typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

/* ˽�к궨�� ----------------------------------------------------------------*/
#define BLOCK_SIZE            512         // SD�����С     
#define NUMBER_OF_BLOCKS      8           // ���Կ�����(С��15)
#define WRITE_READ_ADDRESS    0x00002000  // ���Զ�д��ַ

/* ˽�б��� ------------------------------------------------------------------*/
uint32_t Buffer_Block_Tx[BLOCK_SIZE*NUMBER_OF_BLOCKS]; // д���ݻ���
uint32_t Buffer_Block_Rx[BLOCK_SIZE*NUMBER_OF_BLOCKS] = {1,2}; // �����ݻ���
HAL_StatusTypeDef sd_status;    // HAL�⺯������SD����������ֵ���������
TestStatus test_status;           // ���ݲ��Խ��

/* ��չ���� ------------------------------------------------------------------*/
/* ˽�к���ԭ�� --------------------------------------------------------------*/
void Fill_Buffer(uint32_t *pBuffer, uint32_t BufferLength, uint32_t Offset);
TestStatus Buffercmp(uint32_t* pBuffer1, uint32_t* pBuffer2, uint32_t BufferLength);
TestStatus eBuffercmp(uint32_t* pBuffer, uint32_t BufferLength);

void SD_EraseTest(void);
void SD_Write_Read_Test(void);



void sd_test(void)
{
	
   HAL_SD_CardInfoTypeDef  SDCardInfo;         //SD����Ϣ�ṹ��
   printf("Micro SD Card Test...\r\n");
  /* ���SD���Ƿ��������������ݴ���ģʽ�Ĵ���״̬�� */
  if(HAL_SD_GetCardState(&hsd1) == HAL_SD_CARD_TRANSFER)
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
    HAL_SD_CardCIDTypeDef sdcard_cid;
    HAL_SD_GetCardCID(&hsd1,&sdcard_cid);
    printf(" ManufacturerID: %d \r\n",sdcard_cid.ManufacturerID); /*��ȡsd��������*/
  }
  else
  {
    printf("SD card init fail!\r\n" );
  }
 
 
			printf("Micro SD Card Test... \r\n");
		
		uint8_t read_buf[512];		// �����ݻ���
		uint8_t write_buf[512];		// д���ݻ���

		/* SD��״̬ */
		int sdcard_status = 0;
		
		
		
		/* ��ȡδ����֮ǰ������ */
		printf("------------------- Read SD card block data Test ------------------\r\n");
		/*
			��һ�����������ݣ�
			0: �ӵ�0��������ʼ��
			1����һ�����������ݡ�
			0xffff���ȴ�ʱ�䡣
			note��Ҳ����ֻ���˵�0��������
		*/
		
		
		/******************************������**************************************/
		sdcard_status = HAL_SD_Erase(&hsd1, 0, 2);/*����  0-2  ��*/
		// �ȴ��������
		if (sdcard_status == HAL_OK)
		{	
			printf("Erasing block. state = %d \r\n", HAL_SD_GetCardState(&hsd1));
			while (HAL_SD_GetCardState(&hsd1) == HAL_SD_CARD_PROGRAMMING);
			printf("Erase block ok state = %d \r\n", HAL_SD_GetCardState(&hsd1));
		}
		else
		{
			printf("Erase block fail! status = %d \r\n", sdcard_status);
		}
		
		
		
		
		
		/***************************************************************************/
		
		sdcard_status = HAL_SD_ReadBlocks(&hsd1, (uint8_t *)read_buf, 0, 1, 0xffff);
		if(sdcard_status == HAL_OK)
		{ 
			printf("Read block data ok! \r\n");
			for(int i = 0; i < 512; i++)
			{
				printf("0x%02x ", read_buf[i]);
				if((i+1)%16 == 0)
				{
					printf("\r\n");
				}
			}
		}
		else
		{
			printf("Read block data fail! status = %d \r\n", sdcard_status);
		}
		

		/* ��SD����д������ */
		printf("------------------- Write SD card block data Test ------------------\r\n");
		/* ��仺�������� */
		for(int i = 0; i < 512; i++)
		{
			write_buf[i] = i % 256;
		}
		// ��ʼд������
		/*
			дһ�����������ݣ�
			0: �ӵ�0��������ʼ��
			1��дһ�����������ݡ�
			0xffff���ȴ�ʱ�䡣
			note��Ҳ����ֻд�˵�0��������
		*/
		sdcard_status = HAL_SD_WriteBlocks(&hsd1, (uint8_t *)write_buf, 0, 1, 0xffff);
		if(sdcard_status == HAL_OK)
		{ 
			/* ������ɲ�����д����ɣ����Ҫ�ȴ�SD��״̬��Ϊ�ɴ���״̬����������Ҳ��һ���� */
			printf("Writing block data. state = %d \r\n", HAL_SD_GetCardState(&hsd1));
			while (HAL_SD_GetCardState(&hsd1) == HAL_SD_CARD_PROGRAMMING);
			printf("Write block data ok��state = %d \r\n", HAL_SD_GetCardState(&hsd1));
		}
		else
		{
			printf("Write block data fail! status = %d \r\n", sdcard_status);
		}
		
		/* ��ȡд��֮������� */
		printf("------------------- Read SD card block data after Write ------------------\r\n");
		sdcard_status = HAL_SD_ReadBlocks(&hsd1, (uint8_t *)read_buf, 0, 1, 0xffff);
		if(sdcard_status == HAL_OK)
		{ 
			printf("Read block data ok! \r\n");
			for(int i = 0; i < 512; i++)
			{
				printf("0x%02x ", read_buf[i]);
				if((i+1)%16 == 0)
				{
					printf("\r\n");
				}
			}
		}
		else
		{
			printf("Read block data fail! status = %d \r\n", sdcard_status);
		}
		
		/* ����SD���� */
		printf("------------------- Block Erase -------------------------------\r\n");
		/*
			����512�����������ݣ�
			0: �ӵ�0��������ʼ��
			1��һֱ������512������
			note��������0����512���������ݣ�Ҳ����0��512��Ҳ����һ��512����
		*/
		sdcard_status = HAL_SD_Erase(&hsd1, 0, 512);
		// �ȴ��������
		if (sdcard_status == HAL_OK)
		{	
			printf("Erasing block. state = %d \r\n", HAL_SD_GetCardState(&hsd1));
			while (HAL_SD_GetCardState(&hsd1) == HAL_SD_CARD_PROGRAMMING);
			printf("Erase block ok state = %d \r\n", HAL_SD_GetCardState(&hsd1));
		}
		else
		{
			printf("Erase block fail! status = %d \r\n", sdcard_status);
		}
		
		/* ��ȡ����֮������� */
		printf("------------------- Read SD card block data after Erase ------------------\r\n");
		sdcard_status = HAL_SD_ReadBlocks(&hsd1, (uint8_t *)read_buf, 0, 1, 0xffff);
		if(sdcard_status == HAL_OK)
		{ 
			printf("Read block data ok \r\n" );
			for(int i = 0; i < 512; i++)
			{
				printf("0x%02x ", read_buf[i]);
				if((i+1)%16 == 0)
				{
					printf("\r\n");
				}
			}
		}
		else
		{
			printf("Read block data fail! status = %d \r\n", sdcard_status);
		}

		printf("------------------- Over ------------------\r\n"); 

}







void get_sd_informatization(void)
{
	 
   printf("Micro SD Card Test...\r\n");
  /* ���SD���Ƿ��������������ݴ���ģʽ�Ĵ���״̬�� */
  if(HAL_SD_GetCardState(&hsd1) == HAL_SD_CARD_TRANSFER)
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
    HAL_SD_CardCIDTypeDef sdcard_cid;
    HAL_SD_GetCardCID(&hsd1,&sdcard_cid);
    printf(" ManufacturerID: %d \r\n",sdcard_cid.ManufacturerID); /*��ȡsd��������*/
  }
  else
  {
    printf("SD card init fail!\r\n" );
  }



}




void SD_EraseTest(void)
{
	/* ��1������ΪSD���������2������Ϊ������ʼ��ַ����3������Ϊ����������ַ */
   sd_status=HAL_SD_Erase(&hsd1,WRITE_READ_ADDRESS,WRITE_READ_ADDRESS+NUMBER_OF_BLOCKS*4);
   printf("erase status:%d\r\n",sd_status);
	 HAL_Delay(100);
  if (sd_status == HAL_OK)
  {	
    /* ��ȡ�ող��������� */
    sd_status = HAL_SD_ReadBlocks(&hsd1,(uint8_t *)Buffer_Block_Rx,WRITE_READ_ADDRESS,NUMBER_OF_BLOCKS,0xffff);
    printf("erase read status:%d\r\n",sd_status);
    /* �Ѳ�������������Ա� */
    test_status = eBuffercmp(Buffer_Block_Rx,BLOCK_SIZE*NUMBER_OF_BLOCKS);

    if(test_status == PASSED)
      printf("���������Գɹ���\r\n" ); 
    else	  
      printf("���������ɹ������ݳ���\r\n" );      
  }
  else
  {
    printf("����������ʧ�ܣ�����SD��֧�ֲ�����ֻҪ��д����ͨ������\r\n" );
  }
}

/**
  * ��������: SD����д����
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
void SD_Write_Read_Test(void)
{  
	int i,j = 0;
  /* ������ݵ�д���� */
  Fill_Buffer(Buffer_Block_Tx,BLOCK_SIZE*NUMBER_OF_BLOCKS, 0x32F1);
  
  /* ��SD��д������ */
  sd_status = HAL_SD_WriteBlocks(&hsd1,(uint8_t *)Buffer_Block_Tx,WRITE_READ_ADDRESS,NUMBER_OF_BLOCKS,0xffff);
  printf("write status:%d\r\n",sd_status);
  HAL_Delay(200);
 // HAL_Delay(500);
  /* ��SD����ȡ���� */
  sd_status = HAL_SD_ReadBlocks(&hsd1,(uint8_t *)Buffer_Block_Rx,WRITE_READ_ADDRESS,NUMBER_OF_BLOCKS,0xffff);
  printf("read status:%d\r\n",sd_status);
  
  /* �Ƚ����� */
  test_status = Buffercmp(Buffer_Block_Tx, Buffer_Block_Rx, BLOCK_SIZE*NUMBER_OF_BLOCKS/4);	//�Ƚ�
  if(test_status == PASSED)
	{
    printf("����д���Գɹ���\r\n" );
		
		for(i=0;i<BLOCK_SIZE*NUMBER_OF_BLOCKS/4;i++)
		{
			if(j==8)
			{
				printf("\r\n");
				j=0;
			}
			
			printf("%08x   ",Buffer_Block_Rx[i]);
			j++;
		}
		printf("\r\n");
	}
  else  
  	printf("����д����ʧ�ܣ�\r\n " );  
}


/**
  * ��������: �ڻ���������д����
  * �������: pBuffer��Ҫ���Ļ�����
  *           BufferLength��Ҫ���Ĵ�С
  *           Offset�����ڻ������ĵ�һ��ֵ 
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
void Fill_Buffer(uint32_t *pBuffer, uint32_t BufferLength, uint32_t Offset)
{
  uint32_t index = 0;
  /* ������� */
  for (index = 0; index < BufferLength; index++ )
  {
    pBuffer[index] = index + Offset;
  }
}

/**
  * ��������: �Ƚ������������е������Ƿ����
  * �������: pBuffer1��Ҫ�ȽϵĻ�����1��ָ��
  *           pBuffer2��Ҫ�ȽϵĻ�����2��ָ��
  *           BufferLength������������
  * �� �� ֵ: PASSED�����
  *           FAILED������
  * ˵    ��: ��
  */
TestStatus Buffercmp(uint32_t* pBuffer1, uint32_t* pBuffer2, uint32_t BufferLength)
{
  while (BufferLength--)
  {
    if(BufferLength%50==0)
    {
      printf("buf:0x%08X - 0x%08X\r\n",*pBuffer1,*pBuffer2);
    }
    if (*pBuffer1 != *pBuffer2)
    {
      return FAILED;
    }
    pBuffer1++;
    pBuffer2++;
  }
  return PASSED;
}

/**
  * ��������: ��黺�����������Ƿ�Ϊ0xff��0
  * �������: pBuffer��Ҫ�ȽϵĻ�������ָ��
  *           BufferLength������������
  * �� �� ֵ: PASSED��������������ȫΪ0xff��0
  *           FAILED��������������������һ����Ϊ0xff��0 
  * ˵    ��: ��
  */
TestStatus eBuffercmp(uint32_t* pBuffer, uint32_t BufferLength)
{
  while (BufferLength--)
  {
    /* SD��������Ŀ���ֵΪ0xff��0 */
    if ((*pBuffer != 0xFFFFFFFF) && (*pBuffer != 0))
    {
      return FAILED;
    }
    pBuffer++;
  }
  return PASSED;
}




