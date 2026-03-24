#ifndef __PROJECT_TASKS_H
#define __PROJECT_TASKS_H
#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "Task.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"


#define FIFO_INPUT_BUF_SIZE (size_t)(320*8*3)
#define MAX_CHUNK_SIZE       (size_t)(320*4*3)
#define FIFO_READ_COMPLETED_EVENT (0x01<<0)
#define FIFO_FIRST_COMPLETED    (0x01<<1)
#define FIFO_ROW_COMPLETED  (0x01<<2)
#define FIFO_FRAME_COMPLETED (0x01<<3)
//uint8_t jpeg_chunk_buffer[MAX_CHUNK_SIZE];
//size_t  jpeg_chunk_len;
//void FIFO_Read_Task(void);
//void QVAG_JPEG_Task(void);
//void JPEG_ESP8266_Task(void);
void ReadAndUpgrate(void);
void xAppTaskCreate_Task(void);
void ImageDispaly_Task(void);

void FIFO_Read_Task(void);//
void JPEG_Task(void);
void DMA_USART_Task(void );
#endif
