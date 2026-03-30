#ifndef __TASK_COMMON_H
#define __TASK_COMMON_H
#include "bsp_usart.h"
#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"
#include "events_init.h"
#define FIFO_INPUT_BUF_SIZE (size_t)(320*8*3)
#define MAX_CHUNK_SIZE       (size_t)(320*4*3)

extern TaskHandle_t xAppTaskCreate_Handle;
extern TaskHandle_t ImageDispaly_Handle;
extern TaskHandle_t FIFO_Read_Handle;
extern TaskHandle_t JPEG_Handle;
extern TaskHandle_t DMA_USART_Handle;
extern TaskHandle_t LVGL_Handle;

// 定义队列句柄
extern QueueHandle_t JPEG_FRAME_QueueHandle ;
extern QueueHandle_t JPEG_DMA_QueueHandle ;
extern QueueHandle_t Refresh_Button_QueueHandle;
extern QueueHandle_t Upload_Progress_QueueHandle ;

// 定义信号量句柄
extern SemaphoreHandle_t FIFO_DMA_SemaphoreHandle ;
extern SemaphoreHandle_t DMA_ISR_SemaphoreHandle ;
extern SemaphoreHandle_t DISPLAY_TRAN_MUTEX ;
extern SemaphoreHandle_t LCD_MUTEX ;
extern SemaphoreHandle_t Display_SemaphoreHandle ;
extern SemaphoreHandle_t Upload_SemaphoreHandle ;



//全局变量
extern uint8_t camera_data[FIFO_INPUT_BUF_SIZE];
extern uint8_t jpeg_chunk_buffer[MAX_CHUNK_SIZE];
extern size_t  jpeg_chunk_len;
extern volatile uint8_t Ov7725_vsync ;	 /* 帧同步信号标志，在中断函数和main函数里面使用 */
#endif /* __TASK_COMMON_H */