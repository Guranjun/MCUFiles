#ifndef __DMA_H
#define __DMA_H
#include "stm32f10x.h"
#include "Project_Tasks.h"
//#ifndef MAX_CHUNK_SIZE
//#define MAX_CHUNK_SIZE (uint32_t)320*4*3
//#endif
//#ifndef FIFO_INPUT_BUF_SIZE
//#define FIFO_INPUT_BUF_SIZE (uint32_t)320*8*3
//#endif
extern uint8_t jpeg_chunk_buffer[MAX_CHUNK_SIZE];
extern uint32_t  jpeg_chunk_len;
extern uint8_t camera_data[FIFO_INPUT_BUF_SIZE];
#define IMAGE_DMA_CHANNLE           DMA1_Channel4
#define IMAGE_DMA_PERIPHERABASEADDR (uint32_t)&USART1->DR
#define IMAGE_DMA_MEMORYBASEADDR    (uint32_t)jpeg_chunk_buffer
#define IMAGE_DMA_DIR               DMA_DIR_PeripheralDST
#define IMAGE_DMA_BUFFERSIZE        jpeg_chunk_len
#define IMAGE_DMA_PERIPHERAINC      DMA_PeripheralInc_Disable
#define IMAGE_DMA_MEMORYINC         DMA_MemoryInc_Enable
#define IMAGE_DMA_PERIPHEARDATASIZE DMA_PeripheralDataSize_Byte
#define IMAGE_DMA_MEMORYDATASIZE    DMA_MemoryDataSize_Byte
#define IMAGE_DMA_MODE              DMA_Mode_Normal
#define IMAGE_DMA_PRIORITY          DMA_Priority_High
#define IMAGE_DMA_M2M               DMA_M2M_Disable
void USART3_DMA_Config(void);
#endif

