#include "Task_appcreate.h"
#include "Task_common.h"
#include "Task_camera.h"
#include "Task_jpeg_encoder.h"
#include "Task_dma_usart.h"
#include "Task_LVGL.h"


void xAppTaskCreate_Task(void *pvParameters) {
    BaseType_t xReturn = pdPASS;

    taskENTER_CRITICAL();

    // 创建队列
    JPEG_FRAME_QueueHandle = xQueueCreate(2, sizeof(uint8_t));
    JPEG_DMA_QueueHandle = xQueueCreate(2, sizeof(uint8_t));
	Refresh_Button_QueueHandle = xQueueCreate(2,sizeof(uint8_t));
	
    // 创建信号量
    FIFO_DMA_SemaphoreHandle = xSemaphoreCreateBinary();
    DMA_ISR_SemaphoreHandle = xSemaphoreCreateBinary();
    DISPLAY_TRAN_MUTEX = xSemaphoreCreateMutex();
	LCD_MUTEX = xSemaphoreCreateMutex();
	Display_SemaphoreHandle = xSemaphoreCreateBinary();
	Upload_SemaphoreHandle = xSemaphoreCreateBinary();
	
    // 创建任务
    xReturn = xTaskCreate(ImageDispaly_Task, "ImageDisplay", 128, NULL, 2, &ImageDispaly_Handle);
    xReturn = xTaskCreate(FIFO_Read_Task, "FIFO_Read", 128, NULL, 3, &FIFO_Read_Handle);
    xReturn = xTaskCreate(JPEG_Task, "JPEG", 2048, NULL, 4, &JPEG_Handle);
    xReturn = xTaskCreate(DMA_USART_Task, "DMA_USART", 128, NULL, 4, &DMA_USART_Handle);
	xReturn = xTaskCreate(LVGL_Task, "LVGL" , 1024 , NULL , 3 , &LVGL_Handle);
    // 启动第一个信号量
    xSemaphoreGive(FIFO_DMA_SemaphoreHandle);

    taskEXIT_CRITICAL();

    vTaskDelete(NULL);
}