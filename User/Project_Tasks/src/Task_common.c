#include "Task_common.h"

// 定义任务句柄
TaskHandle_t xAppTaskCreate_Handle = NULL;
TaskHandle_t ImageDispaly_Handle = NULL;
TaskHandle_t FIFO_Read_Handle = NULL;
TaskHandle_t JPEG_Handle = NULL;
TaskHandle_t DMA_USART_Handle = NULL;
TaskHandle_t LVGL_Handle=NULL;

// 定义队列句柄（如果尚未在 app_task_create.c 中定义，可移至此处）
QueueHandle_t JPEG_FRAME_QueueHandle = NULL;
QueueHandle_t JPEG_DMA_QueueHandle = NULL;
QueueHandle_t Refresh_Button_QueueHandle = NULL;

// 定义信号量句柄
SemaphoreHandle_t FIFO_DMA_SemaphoreHandle = NULL;
SemaphoreHandle_t DMA_ISR_SemaphoreHandle = NULL;
SemaphoreHandle_t DISPLAY_TRAN_MUTEX = NULL;
SemaphoreHandle_t LCD_MUTEX = NULL;
SemaphoreHandle_t Display_SemaphoreHandle = NULL;
SemaphoreHandle_t Upload_SemaphoreHandle = NULL;


//全局变量
uint8_t camera_data[FIFO_INPUT_BUF_SIZE];
uint8_t jpeg_chunk_buffer[MAX_CHUNK_SIZE];
size_t  jpeg_chunk_len=0;
volatile uint8_t Ov7725_vsync ;	 /* 帧同步信号标志，在中断函数和main函数里面使用 */
